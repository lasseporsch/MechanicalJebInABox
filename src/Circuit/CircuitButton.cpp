//
// Project: MechanicalJebInABox
// File: /src/Circut/CircutButton.cpp
//
// Created by Lasse Porsch on 4th, January 2023
// Copyright 2023 - 2023 Lasse Porsch
//


#include <Arduino.h>
#include "Circuit/CircuitButton.h"


CircuitButtonState CircuitButton::state() {
    return _state;
}

CircuitButton::CircuitButton(uint8_t pin) {
    this->_state = CircuitButtonState::OFF;
    this->pin = pin;
    this->debounceDelay = 50;
    this->internalCurrentState = LOW;
    this->internalPreviousState = LOW;
    this->lastDebounceTime = 0;
}

void CircuitButton::setup() {
    pinMode(this->pin, INPUT_PULLUP);
    for (uint8_t i = 0; i < this->pin; i++)
    {
        digitalWrite(12, HIGH);
        delay(250);
        digitalWrite(12, LOW);
        delay(250);
    }
}

void CircuitButton::loop() {
    // check to see if you just pressed the button (i.e. the input went from LOW to HIGH),  and you've waited
    // long enough since the last press to ignore any noise:
    int reading = digitalRead(pin);
    _state = reading == HIGH ? CircuitButtonState::ON : CircuitButtonState::OFF;

//     // If the switch changed, due to noise or pressing:
//     if (reading != internalPreviousState) {
//         internalPreviousState = reading;
//         lastDebounceTime = millis();
//         return;
//     }

//     if ((millis() - lastDebounceTime) > debounceDelay) {
//         // whatever the reading is at, it's been there for longer
//         // than the debounce delay, so take it as the actual current state:
//         _state = reading == HIGH ? CircuitButtonState::ON : CircuitButtonState::OFF;
//         digitalWrite(12, reading);
//    }
}