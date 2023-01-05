//
// Project: MechanicalJebInABox
// File: /src/GlobalState.h
//
// Created by Lasse Porsch on 5th, January 2023
// Copyright 2023 - 2023 Lasse Porsch
//

#pragma once

enum class GlobalState {
  uninitialized,
  notConnectedToKSP,
  connectedToKSP,
  unableToLaunch,
  readyToLaunch,
  autoPilotActive
};