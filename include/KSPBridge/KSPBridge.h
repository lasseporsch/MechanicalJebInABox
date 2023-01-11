//
// Project: MechanicalJebInABox
// File: /include/KSPBridge/Simpit/SimpitManager.h
//
// Created by Lasse Porsch on 5th, January 2023
// Copyright 2023 - 2023 Lasse Porsch
//

#pragma once

#include "KerbalSimpit.h"
#include "AutoPilot/VesselSituation.h"

class KSPBridge {

    public:

        VesselSituation vessel_situation = VesselSituation::no_active_vessel;

        float vessel_altitude_above_sealevel = 0;
        float vessel_heading = 0;
        float vessel_pitch = 0;
        float vessel_roll = 0;
        float vessel_orbital_velocity = 0;
        float vessel_orbital_heading = 0;
        float vessel_orbital_pitch = 0;
        float vessel_surface_velocity = 0;
        float vessel_surface_heading = 0;
        float vessel_surface_pitch = 0;
        float vessel_vertical_velocity = 0;
        byte vessel_current_stage = 0;
        float vessel_stage_deltav = 0;
        float vessel_stage_fuel = 0;
        float vessel_stage_oxidizer = 0;
        float vessel_stage_burntime = 0;

        float vessel_total_deltav = 0;
        float vessel_total_fuel = 0;
        float vessel_total_oxidizer = 0;
        float vessel_total_burntime = 0;

        float apoapsis = 0;
        float periapsis = 0;
        int32_t time_to_apoapsis = 0;
        int32_t time_to_periapsis = 0;

        float orbit_inclination = 0;

        void connect();
        void loop();

        void stage_vessel();
        void throttle_vessel(int16_t thrust);
        void rotate_vessel(int16_t rotation);
        void pitch_vessel(int16_t pitch);
        void yaw_vessel(int16_t pitch);

        void activate_sas();
        void deactivate_sas();
        void set_sas_mode(AutopilotMode mode);

        void ui_message(String message);

        KerbalSimpit simpit = KerbalSimpit(Serial);

    private:
        bool connected = false;
};