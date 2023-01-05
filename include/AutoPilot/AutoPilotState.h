//
// Project: MechanicalJebInABox
// File: /src/AutoPilot/AutoPilotState.h
//
// Created by Lasse Porsch on 5th, January 2023
// Copyright 2023 - 2023 Lasse Porsch
//

#pragma once


enum class AutoPilotState {
  inactive,
  countingDown,
  launching,
  flyingVertical,
  flyingRollprogram,
  flyingPitchingDownrange,
  flyingGravityTurn,
  orbitAchieved
};