//
// Project: MechanicalJebInABox
// File: /include/Circuit/CircuitLCD16x2.h
//
// Created by Lasse Porsch on 5th, January 2023
// Copyright 2023 - 2023 Lasse Porsch
//


#pragma once

#include <Arduino.h>
#include "LiquidCrystal.h"


class CircuitLCD16x2 {
    public:

        CircuitLCD16x2(uint8_t rs, uint8_t enable, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);
        void setup();

        void println1(const char *text);
        void println2(const char *text);

    private:

        LiquidCrystal lcd = LiquidCrystal(7, 8, 9, 10, 11, 12);

        uint8_t pin_rs;
        uint8_t pin_enable;
        uint8_t pin_d0;
        uint8_t pin_d1;
        uint8_t pin_d2;
        uint8_t pin_d3;
};
