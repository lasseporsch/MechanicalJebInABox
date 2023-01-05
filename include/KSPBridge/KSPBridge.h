//
// Project: MechanicalJebInABox
// File: /include/KSPBridge/Simpit/SimpitManager.h
//
// Created by Lasse Porsch on 5th, January 2023
// Copyright 2023 - 2023 Lasse Porsch
//

#pragma once

#include "KerbalSimpit.h"

class KSPBridge {

    public:

        float vessel_altitude_above_sealevel = 0;
        float vessel_heading = 0;
        float vessel_pitch = 0;
        float vessel_roll = 0;

        void connect();
        void loop();

        void launch_vessel();
        void rotate_vessel(int16_t rotation);
        void pitch_vessel(int16_t pitch);
        void activate_sas();
        void set_sas_mode(AutopilotMode mode);

        void ui_message(String message);

        KerbalSimpit simpit = KerbalSimpit(Serial);

    private:
        bool connected = false;
};