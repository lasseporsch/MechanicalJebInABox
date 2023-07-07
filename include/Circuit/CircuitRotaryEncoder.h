//
// Project: MechanicalJebInABox
// File: /include/Circuit/CircuitButton.h
//
// Created by Lasse Porsch on 4th, January 2023
// Copyright 2023 - 2023 Lasse Porsch
//

#pragma once

#include <Arduino.h>


class CircuitRotaryEncoder {
    public:

        int8_t step;

        CircuitRotaryEncoder(uint8_t pin_clk, uint8_t pin_dt, uint8_t pin_sw);
        void setup();
        void loop();

    private:
        uint8_t pin_clk; // Pin A
        uint8_t pin_dt;  // Pin B
        uint8_t pin_sw;  // Pin Button

        int8_t last_state_clk;
        int8_t last_state_dt;
};
