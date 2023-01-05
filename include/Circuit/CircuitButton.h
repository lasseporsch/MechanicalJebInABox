//
// Project: MechanicalJebInABox
// File: /include/Circuit/CircuitButton.h
//
// Created by Lasse Porsch on 4th, January 2023
// Copyright 2023 - 2023 Lasse Porsch
//

#pragma once

#include <Arduino.h>


enum class CircuitButtonState {
    ON, OFF
};


class CircuitButton {
    public:
        CircuitButtonState state();

        CircuitButton(uint8_t pin);
        void setup();
        void loop();

    private:
        CircuitButtonState _state;

        uint8_t pin;

        int internalCurrentState;
        int internalPreviousState;
        unsigned long lastDebounceTime;
        unsigned long debounceDelay;
};
