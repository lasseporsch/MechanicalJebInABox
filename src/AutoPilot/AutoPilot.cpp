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

void AutoPilot::set_target_altitude(uint32_t altitude_meters) {
    target_altitude = (float) altitude_meters;
    target_orbital_velocity = kerbin_radius * sqrt( kerbin_gravity / (kerbin_radius + target_altitude) );
    target_orbital_period = 2 * PI * (kerbin_radius + altitude_meters) / target_orbital_velocity;
        char line1[100] = "";
        snprintf(line1, sizeof(line1), "Target Orbit: %lu m", (unsigned long)target_altitude);
        kspBridge->ui_message(String(line1));

        char line2[100] = "";
        snprintf(line1, sizeof(line2), "Orbital Velocity: %lu m/s", (unsigned long)target_orbital_velocity);
        kspBridge->ui_message(String(line1));

        char line3[100] = "";
        snprintf(line1, sizeof(line3), "Orbital Period: %lu s", (unsigned long)target_orbital_period);
        kspBridge->ui_message(String(line1));
}
void AutoPilot::set_target_inclination(int8_t inclination) {
    target_inclination = inclination;
}
void AutoPilot::engage(uint8_t countdown) {
    time = millis();
    state = AutoPilotState::countingDown;
}

void AutoPilot::loop() {

    AutoPilotState stateBeforeLoop = state;
    bool stateChanged = false;
    if( state != previousState ) {
        stateChanged = true;
    }
    switch( state ) {
        case AutoPilotState::inactive:
            break;

        case AutoPilotState::countingDown:
            loop_countingDown(stateChanged);
            break;

        case AutoPilotState::launching:
            loop_launching(stateChanged);
            break;

        case AutoPilotState::flyingVertical:
            loop_flyingVertical(stateChanged);
            loop_autostaging();
            break;

        case AutoPilotState::flyingRollprogram:
            loop_flyingRollProgram(stateChanged);
            loop_autostaging();
            break;

        case AutoPilotState::flyingPitchingDownrange:
            loop_flyingPitchingDownrange(stateChanged);
            loop_autostaging();
            break;

        case AutoPilotState::flyingGravityTurn:
            loop_flyingGravityTurn(stateChanged);
            loop_autostaging();
            break;

        case AutoPilotState::flyingCoastToApoapsis:
            loop_flyingCoastToApoapsis(stateChanged);
            loop_autostaging();
            break;

        case AutoPilotState::flyingCircularizationBurn:
            loop_flyingCircularizationBurn(stateChanged);
            loop_autostaging();
            break;

        case AutoPilotState::orbitAchieved:
            loop_flyingOrbitAchieved(stateChanged);
            break;

        default:
            break;
    }
    previousState = stateBeforeLoop;

}

void AutoPilot::loop_countingDown(bool stateChanged) {
    state = AutoPilotState::launching;
}
void AutoPilot::loop_launching(bool stateChanged) {
    delay(10);
    kspBridge->activate_sas();
    delay(10);
    kspBridge->set_sas_mode(AP_STABILITYASSIST);
    delay(10);
    current_throttle = INT16_MAX;
    kspBridge->throttle_vessel(current_throttle);
    delay(10);
    kspBridge->stage_vessel();
    delay(10);
    kspBridge->ui_message("Liftoff and launch!");
    state = AutoPilotState::flyingVertical;
}
void AutoPilot::loop_flyingVertical(bool stateChanged) {
    // We fly straight up until 200m above sea level; then we initiate the Roll Program
    if(kspBridge->vessel_altitude_above_sealevel > 200) {
        state = AutoPilotState::flyingRollprogram;
    }
}
void AutoPilot::loop_flyingRollProgram(bool stateChanged) {
    if( stateChanged ) {
        kspBridge->ui_message("Starting Roll Program");
        time = millis();
    }

    kspBridge->rotate_vessel(INT16_MAX / 4);

    if(millis() - time > 2000) {
        kspBridge->rotate_vessel(0);
        delay(10);
        kspBridge->ui_message("Stop rolling");
    }
    // We initiate the pitch downrange at 500m
    if( kspBridge->vessel_altitude_above_sealevel > 500) {
        state = AutoPilotState::flyingPitchingDownrange;
    }
}
void AutoPilot::loop_flyingPitchingDownrange(bool stateChanged) {
    if( stateChanged ) {
        time = millis();
        kspBridge->ui_message("Pitching downrange");
        delay(10);
    }
    kspBridge->yaw_vessel(INT16_MAX);
    if(millis() - time > 15000) {
        kspBridge->yaw_vessel(0);
        state = AutoPilotState::flyingGravityTurn;
    }
}
void AutoPilot::loop_flyingGravityTurn(bool stateChanged) {
    if( stateChanged ) {
        time = millis();
        kspBridge->ui_message("Gravity turn initiated");
        kspBridge->set_sas_mode(AP_PROGRADE);
    }

    if( kspBridge->apoapsis > target_altitude ) {
        kspBridge->throttle_vessel(0);
        state = AutoPilotState::flyingCoastToApoapsis;
    }
}
void AutoPilot::loop_flyingCoastToApoapsis(bool stateChanged) {
    if( stateChanged ) {
        time = millis();
        delay(10);
        kspBridge->deactivate_sas();
        delay(10);
        kspBridge->ui_message("Coasting to apoapsis");
        // Calcluate circularization burn duration here:


        float radius = kerbin_radius + kspBridge->apoapsis;
        float semi_major_axis = (2.0f * kerbin_radius + kspBridge->periapsis + kspBridge->apoapsis) / 2.0f;
        float dv_apoapsis = sqrt(kerbin_standard_gravitational_parameter * (2.0f / radius - 1.0f / semi_major_axis));
        float burn_dv = target_orbital_velocity - dv_apoapsis;
        float ship_mass = second_stage_dry_mass + 5.0f * (kspBridge->vessel_total_fuel + kspBridge->vessel_total_oxidizer);
        float burn_time = ship_mass * (1.0f - exp(-burn_dv / second_stage_isp / kerbin_gravity)) * second_stage_isp * kerbin_gravity / second_stage_thrust;
        circularization_burn_duration = burn_time;

        starting_pitch = kspBridge->vessel_pitch;
        pitch_correction = kspBridge->vessel_surface_pitch + kspBridge->vessel_surface_pitch - kspBridge->vessel_pitch;
        char line[100] = "";
        delay(10);
        // snprintf(line, sizeof(line), "Pitch: %lu", (unsigned long)kspBridge->vessel_pitch);
        // kspBridge->ui_message(String(line));
        // delay(10);
        // snprintf(line, sizeof(line), "Orb. Pitch: %lu", (unsigned long)kspBridge->vessel_orbital_pitch);
        // kspBridge->ui_message(String(line));
        // delay(10);
        // snprintf(line, sizeof(line), "Surf. Pitch: %lu", (unsigned long)kspBridge->vessel_surface_pitch);
        // kspBridge->ui_message(String(line));
        // delay(10);
        snprintf(line, sizeof(line), "Burn dv: %lu", (unsigned long)burn_dv);
        kspBridge->ui_message(String(line));
        delay(10);
        snprintf(line, sizeof(line), "Burn time: %lu", (unsigned long)burn_time);
        kspBridge->ui_message(String(line));
        delay(10);
        snprintf(line, sizeof(line), "Ship mass: %lu", (unsigned long)ship_mass);
        kspBridge->ui_message(String(line));

    }

    if( (starting_pitch - kspBridge->vessel_pitch) < (pitch_correction / 2) ) {
        kspBridge->yaw_vessel(INT16_MAX / 32);
    } else {
        if( pitch_correction_time == 0) {
            pitch_correction_time = (millis() - time) * 2;
        }
        if( (millis() - time) < pitch_correction_time) {
            kspBridge->yaw_vessel(INT16_MIN / 32);
        } else {
            kspBridge->yaw_vessel(0);
            delay(10);
            kspBridge->activate_sas();
            delay(10);
            kspBridge->set_sas_mode(AP_STABILITYASSIST);
        }
    }

    if( kspBridge->time_to_apoapsis < circularization_burn_duration / 2 ) {
        state = AutoPilotState::flyingCircularizationBurn;
    }
}
void AutoPilot::loop_flyingCircularizationBurn(bool stateChanged) {
    if( stateChanged ) {
        time = millis();
        kspBridge->throttle_vessel(INT16_MAX);
        delay(10);
        kspBridge->ui_message("Initiating circularization burn");
    }
    if( (millis() - time) / 1000 > circularization_burn_duration) {
        state = AutoPilotState::orbitAchieved;
    }
    // We burn until we raised the OTHER side of the orbit to the target altitude.
    // Figuring out this value by comparing time to apoapsis and time to periapsis
    // Our orbits will have a periodicy of about 30 min or more.
    // float opposite_orbit_altitude;
    // if( kspBridge->time_to_apoapsis  < 600 || kspBridge->time_to_periapsis < kspBridge->time_to_apoapsis ) {
    //     // The periapsis is on the other side
    //     opposite_orbit_altitude = kspBridge->periapsis;
    // }  else {
    //     // The apoapsis is on the other side
    //     opposite_orbit_altitude = kspBridge->apoapsis;
    // }

    // if( opposite_orbit_altitude > target_altitude * 0.9 ) {
    //     kspBridge->throttle_vessel(INT16_MAX / 4);
    // }

    // if( opposite_orbit_altitude > target_altitude ) {
    //     state = AutoPilotState::orbitAchieved;
    // }
}
void AutoPilot::loop_flyingOrbitAchieved(bool stateChanged) {
    if( stateChanged ) {
        time = millis();
        current_throttle = 0;
        kspBridge->throttle_vessel(0);
        delay(10);
        kspBridge->ui_message("Orbit achieved");

        // We should release control of the throttle back to the user... if we would know how?!?
    }
}


void AutoPilot::loop_autostaging() {
    float remaining_burntime = kspBridge->vessel_stage_burntime;
    if( remaining_burntime < 1) {
        staging_imminent = true;
    }
    if( staging_imminent && (remaining_burntime == 0 || remaining_burntime > 1) ) {
        kspBridge->ui_message("Staging");
        delay(100);
        kspBridge->stage_vessel();
        staging_imminent = false;
    }
}