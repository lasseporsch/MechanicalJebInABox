//
// Project: MechanicalJebInABox
// File: /src/Circut/CircutButton.cpp
//
// Created by Lasse Porsch on 4th, January 2023
// Copyright 2023 - 2023 Lasse Porsch
//


#include <Arduino.h>
#include "Circuit/CircuitRotaryEncoder.h"


const uint8_t clockwise4 = 0b0010;
const uint8_t clockwise1 = 0b1011;
const uint8_t clockwise2 = 0b1101;
const uint8_t clockwise3 = 0b0100;
const uint8_t anticlkws1 = 0b0001;
const uint8_t anticlkws2 = 0b0111;
const uint8_t anticlkws3 = 0b1110;
const uint8_t anticlkws4 = 0b1000;


CircuitRotaryEncoder::CircuitRotaryEncoder(uint8_t pin_clk, uint8_t pin_dt, uint8_t pin_sw) {
    this->pin_clk = pin_clk;
    this->pin_dt = pin_dt;
    this->pin_sw = pin_sw;
    this->step = 0;
}

void CircuitRotaryEncoder::setup() {
    pinMode(this->pin_clk, INPUT);
    pinMode(this->pin_dt, INPUT);
    pinMode(this->pin_sw, INPUT);

    this->last_state_clk = digitalRead(this->pin_clk);
    this->last_state_dt = digitalRead(this->pin_dt);
}

void CircuitRotaryEncoder::loop() {
    // delay(20);
    int state_clk = digitalRead(this->pin_clk);
    int state_dt = digitalRead(this->pin_dt);

    uint8_t state_change = 0;
    state_change ^= (-this->last_state_clk ^ state_change) & (1UL << 3);
    state_change ^= (-this->last_state_dt ^ state_change) & (1UL << 2);
    state_change ^= (-state_clk ^ state_change) & (1UL << 1);
    state_change ^= (-state_dt ^ state_change) & (1UL << 0);

    // if( state_change == clockwise1 || state_change == clockwise2 || state_change == clockwise3 || state_change == clockwise4 ) {
    if( state_change == clockwise3) {
        this->step = 1;
    // } else if( state_change == anticlkws1 || state_change == anticlkws2 || state_change == anticlkws3 || state_change == anticlkws4 ) {
    } else if( state_change == anticlkws4 ) {
        this->step = -1;
    } else {
        this->step = 0;
    }

    this->last_state_clk = state_clk;
    this->last_state_dt = state_dt;
}

