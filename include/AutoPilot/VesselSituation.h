//
// Project: MechanicalJebInABox
// File: /include/AutoPilot/VesselSituation.h
//
// Created by Lasse Porsch on 7th, January 2023
// Copyright 2023 - 2023 Lasse Porsch
//

#pragma once


enum class VesselSituation: unsigned char {
    no_active_vessel = 0,
    landed = 1,
    splashed = 2,
    prelaunch = 4,
    flying = 8,
    sub_orbital = 16,
    orbiting = 32,
    escaping = 64,
    docked = 128
};
