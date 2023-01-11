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

        void set_target_altitude(uint32_t altitude_meters);
        void set_target_inclination(int8_t inclination);

        void engage(uint8_t countdown);
        void loop();

    private:
        KSPBridge *kspBridge;
        AutoPilotState state = AutoPilotState::inactive;
        AutoPilotState previousState = AutoPilotState::inactive;
        float target_altitude = 0;
        int8_t target_inclination = 0;
        float target_orbital_velocity = 0;
        float target_orbital_period = 0;
        float circularization_burn_duration = 0;

        const float kerbin_radius = 600000;                                 // 600,000 m
        const float kerbin_gravity = 9.807;                                 // 9.807 m/s2 m/s2
        const float kerbin_standard_gravitational_parameter = 3.5316000e12; // 3.5316000×1012 m3/s2
        const float second_stage_dry_mass = 2005;                           // 2,005 kg
        const float second_stage_thrust = 60000;                            // 60 kN
        const float second_stage_isp = 345;                                 // 345 s in vacuum
        const float fuel_density = 5;                                       // 1 unit fuel weighs 5 kg
        const float oxidozer_density = 5;                                   // 1 unit oxidizer weighs 5 kg

        bool staging_imminent = false;
        float current_throttle = 0;
        unsigned long time;

        float pitch_correction = 0;
        float starting_pitch = 0;
        unsigned long pitch_correction_time = 0;
        bool isPitching = false;

        void loop_countingDown(bool stateChanged);
        void loop_launching(bool stateChanged);
        void loop_flyingVertical(bool stateChanged);
        void loop_flyingRollProgram(bool stateChanged);
        void loop_flyingPitchingDownrange(bool stateChanged);
        void loop_flyingGravityTurn(bool stateChanged);
        void loop_flyingCoastToApoapsis(bool stateChanged);
        void loop_flyingCircularizationBurn(bool stateChanged);
        void loop_flyingOrbitAchieved(bool stateChanged);

        void loop_autostaging();
};
