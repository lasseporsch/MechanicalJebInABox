//
// Project: MechanicalJebInABox
// File: /include/AutoPilot/AutoPilot.h
//
// Created by Lasse Porsch on 5th, January 2023
// Copyright 2023 - 2023 Lasse Porsch
//

#pragma once

#include <Arduino.h>
#include "AutoPilot/AutoPilotState.h"
#include "KSPBridge/KSPBridge.h"

class AutoPilot {
    public:

        AutoPilot(KSPBridge *kspBridge);

        void set_orbit_altitude(uint32_t altitude_meters);
        void set_orbit_inclination(int8_t inclination);

        void engage(uint8_t countdown);
        void loop();

    private:
        KSPBridge *kspBridge;
        AutoPilotState state = AutoPilotState::inactive;
        bool stateChanged = false;
        uint32_t altitude_meters = 100000;
        int8_t inclination = 59;

        unsigned long time;

        void loop_launching();
        void loop_countingDown();
        void loop_flyingVertical();
        void loop_flyingRollProgram();
        void loop_flyingPitchingDownrange();
        void loop_flyingGravityTurn();

};
