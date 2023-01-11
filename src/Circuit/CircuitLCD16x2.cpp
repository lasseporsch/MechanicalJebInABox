//
// Project: MechanicalJebInABox
// File: /src/Circuit/CircuitLCD16x2.cpp
//
// Created by Lasse Porsch on 5th, January 2023
// Copyright 2023 - 2023 Lasse Porsch
//



#include <Arduino.h>
#include "Circuit/CircuitLCD16x2.h"
#include "LiquidCrystal.h"

CircuitLCD16x2::CircuitLCD16x2(uint8_t rs, uint8_t enable, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3) {
    this->pin_rs = rs;
    this->pin_enable = enable;
    this->pin_d0 = d0;
    this->pin_d1 = d1;
    this->pin_d2 = d2;
    this->pin_d3 = d3;
}

void CircuitLCD16x2::setup() {
    lcd.begin(16, 2);
    this->println1("");
    this->println2("");
}

void CircuitLCD16x2::println1(const char *text) {
    lcd.setCursor(0, 0);
    lcd.print(text);
}
void CircuitLCD16x2::println2(const char *text) {
    lcd.setCursor(0, 1);
    lcd.print(text);
}