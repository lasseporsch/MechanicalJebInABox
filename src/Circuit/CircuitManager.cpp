//
// Project: MechanicalJebInABox
// File: /src/circut.cpp
//
// Created by Lasse Porsch on 4th, January 2023
// Copyright 2023 - 2023 Lasse Porsch
//

#include <Arduino.h>
#include "Circuit/CircuitManager.h"


void CircuitManager::setup() {
    // Set initial pin states, and turn on the LED
    pinMode(3, OUTPUT);

    stagingButton.setup();
    lcd16x2.setup();
};


void CircuitManager::loop() {
    stagingButton.loop();
};


