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
#include "Circuit/CircuitRotaryEncoder.h"

class CircuitManager {

    public:
        // The Launch/Staging button on pin 2
        CircuitButton stagingButton = CircuitButton(2);

        // The 16x2 LCD display on pins 9 - 12
        CircuitLCD16x2 lcd16x2 = CircuitLCD16x2(7, 8, 9, 10, 11, 12);

        // The Altitude Rotary Encoder on pins 4, 5 and 6
        CircuitRotaryEncoder altitudeRotaryEncoder = CircuitRotaryEncoder(6, 5, 4);

        // The 7-digit display on pins...

        void setup();
        void loop();


    private:
        // The Launch/Staging button pin
        static const uint8_t pinStagingButton = 2;


        static const uint8_t pinAltitudeRotaryEncoderA = 9; // CLK
        static const uint8_t pinAltitudeRotaryEncoderB = 8; // DT

};
