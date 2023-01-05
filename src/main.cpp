//
// Project: MechanicalJebInABox
// File: /src/main.cpp
//
// Created by Lasse Porsch on 3rd, January 2023
// Copyright 2023 - 2023 Lasse Porsch
//



#include "MechanicalJebArduino.h"

GlobalState globalState = GlobalState::uninitialized;
CircuitManager circuit = CircuitManager();
KSPBridge kspBridge = KSPBridge();
AutoPilot autoPilot = AutoPilot(&kspBridge);
unsigned long launchTime;


// For some reason this callback function needs to reside right here in main.cpp
// Would be nicer somewhere close to the KSPBridge definition, though...
// We just write the values back to theshared KSPBridge instance.
void messageHandler(byte messageType, byte msg[], byte msgSize) {
  switch(messageType) {
    case ALTITUDE_MESSAGE:
        if (msgSize == sizeof(altitudeMessage)) {
            altitudeMessage altitudeMsg;
            altitudeMsg = parseMessage<altitudeMessage>(msg);
            kspBridge.vessel_altitude_above_sealevel = altitudeMsg.sealevel;
        }
        break;
     case ROTATION_DATA_MESSAGE:
        if (msgSize == sizeof(vesselPointingMessage)) {
            vesselPointingMessage vesselPointingMsg;
            vesselPointingMsg = parseMessage<vesselPointingMessage>(msg);

            kspBridge.vessel_heading = vesselPointingMsg.heading;
            kspBridge.vessel_pitch = vesselPointingMsg.pitch;
            kspBridge.vessel_roll = vesselPointingMsg.roll;
        }
        break;

    default:
        break;
  }
}


void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    circuit.setup();

    kspBridge.connect(); // This method only ever returns if successfully connected
    kspBridge.simpit.inboundHandler(messageHandler);
    kspBridge.ui_message("Connected to MechanicalJeb");
    globalState = GlobalState::connectedToKSP;

    digitalWrite(LED_BUILTIN, LOW);
}


void print_vessel_orientation() {
    // Print current rotation on LCD
    char line1[17] = "";
    int roll = kspBridge.vessel_roll;
    snprintf(line1, sizeof(line1), "Roll: %10d", roll);
    circuit.lcd16x2.println1(line1);

    // Print current heading on LCD
    char line2[17] = "";
    int heading = kspBridge.vessel_heading;
    //                              1234567890123456
    snprintf(line2, sizeof(line2), "Heading: %7d", heading);
    circuit.lcd16x2.println2(line2);
}


void loop() {
    circuit.loop(); // Provides fresh reads from all circut elements
    kspBridge.simpit.update(); // Retrieves all messages from KSP from the queue and processes them with messageHandler

    print_vessel_orientation();

    switch( globalState ) {
        case GlobalState::connectedToKSP:
            kspBridge.simpit.registerChannel(ALTITUDE_MESSAGE);
            kspBridge.simpit.registerChannel(ROTATION_DATA_MESSAGE);
            globalState = GlobalState::readyToLaunch;
            break;

        case GlobalState::unableToLaunch:
            break;

        case GlobalState::readyToLaunch:
            if( circuit.stagingButton.state() == CircuitButtonState::ON ){
                autoPilot.engage(3);
                delay(10);
                kspBridge.activate_sas();
                delay(10);
                kspBridge.set_sas_mode(AP_STABILITYASSIST);
                delay(10);
                kspBridge.launch_vessel();
                kspBridge.ui_message("Liftoff and launch!");
                globalState = GlobalState::autoPilotActive;
            }
            break;

        case GlobalState::autoPilotActive:
            autoPilot.loop();

        default:
            break;
    }
}