//
// Project: MechanicalJebInABox
// File: /include/Circuit/CircuitManager.h
//
// Created by Lasse Porsch on 4th, January 2023
// Copyright 2023 - 2023 Lasse Porsch
//

#pragma once

#include "Arduino.h"
#include "Circuit/CircuitButton.h"
#include "Circuit/CircuitLCD16x2.h"

class CircuitManager {

    public:
        CircuitButton stagingButton = CircuitButton(2);
        CircuitLCD16x2 lcd16x2 = CircuitLCD16x2(7, 8, 9, 10, 11, 12);
        void setup();
        void loop();


    private:
        // The Launch/Staging button pin
        static const uint8_t pinStagingButton = 2;

        // The Altitude Rotary Encoder
        static const uint8_t pinAltitudeRotaryEncoderA = 9; // CLK
        static const uint8_t pinAltitudeRotaryEncoderB = 8; // DT

};
