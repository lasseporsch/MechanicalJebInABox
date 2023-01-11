//
// Project: MechanicalJebInABox
// File: /src/KSPBridge/Simpit/KSPSimpitBridge.cpp
//
// Created by Lasse Porsch on 5th, January 2023
// Copyright 2023 - 2023 Lasse Porsch
//

#include <Arduino.h>
#include "KSPBridge/KSPBridge.h"
#include "KerbalSimpit.h"



void KSPBridge::connect() {
    // Open the serial connection.
    Serial.begin(115200);

    // This loop continually attempts to handshake with the plugin.
    // It will keep retrying until it gets a successful handshake.
    while (!simpit.init()) {
        delay(100);
    }
    connected = true;
}

void KSPBridge::loop() {
    if( connected ) {
        //
    }
}


void KSPBridge::stage_vessel() {
    simpit.activateAction(STAGE_ACTION);
}

void KSPBridge::activate_sas() {
    simpit.activateAction(SAS_ACTION);
}
void KSPBridge::deactivate_sas() {
    simpit.deactivateAction(SAS_ACTION);
}
void KSPBridge::set_sas_mode(AutopilotMode mode) {
    simpit.setSASMode(mode);
}
void KSPBridge::throttle_vessel(int16_t thrust) {
    throttleMessage msg;
    msg.throttle = thrust;
    simpit.send(THROTTLE_MESSAGE, msg);
}

void KSPBridge::rotate_vessel(int16_t rotation) {
    rotationMessage msg;
    msg.setRoll(rotation);

    simpit.send(ROTATION_MESSAGE, msg);
}
void KSPBridge::pitch_vessel(int16_t pitch) {
    rotationMessage msg;
    msg.setPitch(pitch);

    simpit.send(ROTATION_MESSAGE, msg);
}
void KSPBridge::yaw_vessel(int16_t yaw) {
    rotationMessage msg;
    msg.setYaw(yaw);

    simpit.send(ROTATION_MESSAGE, msg);
}


void KSPBridge::ui_message(String message) {
    simpit.printToKSP(message, PRINT_TO_SCREEN);
}


