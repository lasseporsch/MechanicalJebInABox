//
// Project: MechanicalJebInABox
// File: /src/AutoPilot/AutoPilot.cpp
//
// Created by Lasse Porsch on 5th, January 2023
// Copyright 2023 - 2023 Lasse Porsch
//


#include "AutoPilot/AutoPilot.h"


AutoPilot::AutoPilot(KSPBridge *kspBridge) {
    this->kspBridge = kspBridge;
}

void AutoPilot::engage(uint8_t countdown) {
    time = millis();
    state = AutoPilotState::countingDown;
}

void AutoPilot::loop() {
    switch( state ) {
        case AutoPilotState::inactive:
            break;

        case AutoPilotState::countingDown:
            loop_countingDown();
            break;

        case AutoPilotState::launching:
            loop_launching();
            break;

        case AutoPilotState::flyingVertical:
            loop_flyingVertical();
            break;

        case AutoPilotState::flyingRollprogram:
            loop_flyingRollProgram();
            break;

        case AutoPilotState::flyingPitchingDownrange:
            loop_flyingPitchingDownrange();
            break;

        case AutoPilotState::flyingGravityTurn:
            loop_flyingGravityTurn();
            break;

        case AutoPilotState::orbitAchieved:
            break;

        default:
            break;
    }
}

void AutoPilot::loop_countingDown() {
    state = AutoPilotState::launching;
}

void AutoPilot::loop_launching() {
    delay(10);
    kspBridge->activate_sas();
    delay(10);
    kspBridge->set_sas_mode(AP_STABILITYASSIST);
    delay(10);
    kspBridge->launch_vessel();
    kspBridge->ui_message("Liftoff and launch!");
    state = AutoPilotState::flyingVertical;
}
void AutoPilot::loop_flyingVertical() {
    // We fly straight up until 200m above sea level; then we initiate the Roll Program
    if(kspBridge->vessel_altitude_above_sealevel > 200) {
        state = AutoPilotState::flyingRollprogram;
        stateChanged = true;
    }
}
void AutoPilot::loop_flyingRollProgram() {
    if( stateChanged ) {
        stateChanged = false;
        kspBridge->ui_message("Start rolling");
        time = millis();
    }

    kspBridge->rotate_vessel(INT16_MAX / 4);


    if(millis() - time > 2000) {
        kspBridge->rotate_vessel(0);
        kspBridge->ui_message("Stop rolling");

        kspBridge->ui_message("Pitching downrange");
        delay(10);
        kspBridge->set_sas_mode(AP_PROGRADE);
        delay(1000);
        kspBridge->pitch_vessel(INT16_MAX);

        time = millis();
        state = AutoPilotState::flyingPitchingDownrange;
    }
}
void AutoPilot::loop_flyingPitchingDownrange() {
    if(millis() - time > 5000) {
        kspBridge->pitch_vessel(0);
        kspBridge->ui_message("Gravity turn initiated");

        time = millis();
        state = AutoPilotState::flyingGravityTurn;
    }
}
void AutoPilot::loop_flyingGravityTurn() {

}