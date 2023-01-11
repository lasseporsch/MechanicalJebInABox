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


// For some reason this callback function doesn't work unless it resides right here in main.cpp
// Would be nicer if it were somewhere close to the KSPBridge definition, though.
// We just write the values back to the shared KSPBridge instance.
void messageHandler(byte messageType, byte msg[], byte msgSize) {
  switch(messageType) {
    case ALTITUDE_MESSAGE:
        if (msgSize == sizeof(altitudeMessage)) {
            altitudeMessage altitudeMsg;
            altitudeMsg = parseMessage<altitudeMessage>(msg);
            kspBridge.vessel_altitude_above_sealevel = altitudeMsg.sealevel;
        }
        break;
     case VELOCITY_MESSAGE:
        if (msgSize == sizeof(velocityMessage)) {
            velocityMessage velocityMsg;
            velocityMsg = parseMessage<velocityMessage>(msg);

            kspBridge.vessel_orbital_velocity = velocityMsg.orbital;
            kspBridge.vessel_surface_velocity = velocityMsg.surface;
            kspBridge.vessel_vertical_velocity = velocityMsg.vertical;
        }
        break;     case ROTATION_DATA_MESSAGE:
        if (msgSize == sizeof(vesselPointingMessage)) {
            vesselPointingMessage vesselPointingMsg;
            vesselPointingMsg = parseMessage<vesselPointingMessage>(msg);

            kspBridge.vessel_heading = vesselPointingMsg.heading;
            kspBridge.vessel_pitch = vesselPointingMsg.pitch;
            kspBridge.vessel_roll = vesselPointingMsg.roll;
            kspBridge.vessel_orbital_heading = vesselPointingMsg.orbitalVelocityHeading;
            kspBridge.vessel_orbital_pitch = vesselPointingMsg.orbitalVelocityPitch;
            kspBridge.vessel_surface_heading = vesselPointingMsg.surfaceVelocityHeading;
            kspBridge.vessel_surface_pitch = vesselPointingMsg.surfaceVelocityPitch;
        }
        break;
    case FLIGHT_STATUS_MESSAGE:
        if (msgSize == sizeof(flightStatusMessage)) {
            flightStatusMessage flightStatusMsg;
            flightStatusMsg = parseMessage<flightStatusMessage>(msg);
            kspBridge.vessel_situation = static_cast<VesselSituation>(flightStatusMsg.vesselSituation);
            kspBridge.vessel_current_stage = flightStatusMsg.currentStage;
        }
        break;
    case DELTAV_MESSAGE:
        if (msgSize == sizeof(deltaVMessage)) {
            deltaVMessage deltaVMsg;
            deltaVMsg = parseMessage<deltaVMessage>(msg);
            kspBridge.vessel_stage_deltav = deltaVMsg.stageDeltaV;
            kspBridge.vessel_total_deltav = deltaVMsg.totalDeltaV;
        }
        break;
    case BURNTIME_MESSAGE:
        if (msgSize == sizeof(burnTimeMessage)) {
            burnTimeMessage burntimeMsg;
            burntimeMsg = parseMessage<burnTimeMessage>(msg);
            kspBridge.vessel_stage_burntime = burntimeMsg.stageBurnTime;
            kspBridge.vessel_total_burntime = burntimeMsg.totalBurnTime;
        }
        break;
    case APSIDES_MESSAGE:
        if (msgSize == sizeof(apsidesMessage)) {
            apsidesMessage apsidesMsg;
            apsidesMsg = parseMessage<apsidesMessage>(msg);
            kspBridge.apoapsis = apsidesMsg.apoapsis;
            kspBridge.periapsis = apsidesMsg.periapsis;
        }
        break;
    case APSIDESTIME_MESSAGE:
        if (msgSize == sizeof(apsidesTimeMessage)) {
            apsidesTimeMessage apsidesTimeMsg;
            apsidesTimeMsg = parseMessage<apsidesTimeMessage>(msg);
            kspBridge.time_to_apoapsis = apsidesTimeMsg.apoapsis;
            kspBridge.time_to_periapsis = apsidesTimeMsg.periapsis;
        }
        break;
    case ORBIT_MESSAGE:
        if (msgSize == sizeof(orbitInfoMessage)) {
            orbitInfoMessage orbitInfoMsg;
            orbitInfoMsg = parseMessage<orbitInfoMessage>(msg);
            kspBridge.orbit_inclination = orbitInfoMsg.inclination;
        }
        break;
    case LF_MESSAGE:
        if (msgSize == sizeof(resourceMessage)) {
            resourceMessage resourceMsg;
            resourceMsg = parseMessage<resourceMessage>(msg);
            kspBridge.vessel_total_fuel = resourceMsg.available;
        }
        break;
    case OX_MESSAGE:
        if (msgSize == sizeof(resourceMessage)) {
            resourceMessage resourceMsg;
            resourceMsg = parseMessage<resourceMessage>(msg);
            kspBridge.vessel_total_oxidizer = resourceMsg.available;
        }
        break;
    case LF_STAGE_MESSAGE:
        if (msgSize == sizeof(resourceMessage)) {
            resourceMessage resourceMsg;
            resourceMsg = parseMessage<resourceMessage>(msg);
            kspBridge.vessel_stage_fuel = resourceMsg.available;
        }
        break;
    case OX_STAGE_MESSAGE:
        if (msgSize == sizeof(resourceMessage)) {
            resourceMessage resourceMsg;
            resourceMsg = parseMessage<resourceMessage>(msg);
            kspBridge.vessel_stage_oxidizer = resourceMsg.available;
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
    int pitch_dec = kspBridge.vessel_pitch;
    int pitch_frac = abs(trunc((kspBridge.vessel_pitch - pitch_dec) * 100));
    //                              1234567890123456
    snprintf(line1, sizeof(line1), "Pitch: %6d.%02d", pitch_dec,  pitch_frac);
    circuit.lcd16x2.println1(line1);

    // Print current heading on LCD
    char line2[17] = "";
    int spitch_dec = kspBridge.vessel_surface_pitch;
    int spitch_frac = abs(trunc((kspBridge.vessel_surface_pitch - pitch_dec) * 100));
    //                              1234567890123456
    snprintf(line2, sizeof(line2), "S.Pitch: %4d.%02d", spitch_dec,  spitch_frac);
    circuit.lcd16x2.println2(line2);
}


void loop() {
    circuit.loop(); // Provides fresh reads from all circut elements
    kspBridge.simpit.update(); // Retrieves all messages from KSP from the queue and processes them with messageHandler

    switch( globalState ) {
        case GlobalState::connectedToKSP:
            kspBridge.simpit.registerChannel(ALTITUDE_MESSAGE);
            kspBridge.simpit.registerChannel(ROTATION_DATA_MESSAGE);
            kspBridge.simpit.registerChannel(FLIGHT_STATUS_MESSAGE);
            kspBridge.simpit.registerChannel(DELTAV_MESSAGE);
            kspBridge.simpit.registerChannel(BURNTIME_MESSAGE);
            kspBridge.simpit.registerChannel(APSIDES_MESSAGE);
            kspBridge.simpit.registerChannel(APSIDESTIME_MESSAGE);
            kspBridge.simpit.registerChannel(ORBIT_MESSAGE);
            kspBridge.simpit.registerChannel(VELOCITY_MESSAGE);
            kspBridge.simpit.registerChannel(LF_MESSAGE);
            kspBridge.simpit.registerChannel(OX_MESSAGE);

            globalState = GlobalState::unableToLaunch;
            break;

        case GlobalState::unableToLaunch:
            circuit.lcd16x2.println1("Unable to launch");
            circuit.lcd16x2.println2("Go to Launchpad");
            if( kspBridge.vessel_situation == VesselSituation::prelaunch) {
                globalState = GlobalState::readyToLaunch;
            }
            break;

        case GlobalState::readyToLaunch:
            circuit.lcd16x2.println1("Ready to launch ");
            circuit.lcd16x2.println2("Configure orbit");
            if( kspBridge.vessel_situation != VesselSituation::prelaunch) {
                globalState = GlobalState::unableToLaunch;
            }

            if( circuit.stagingButton.state() == CircuitButtonState::ON ){
                autoPilot.set_target_altitude(100000);
                autoPilot.set_target_inclination(59);
                autoPilot.engage(3);
                globalState = GlobalState::autoPilotActive;
            }
            break;

        case GlobalState::autoPilotActive:
            print_vessel_orientation();
            autoPilot.loop();

        default:
            break;
    }
}