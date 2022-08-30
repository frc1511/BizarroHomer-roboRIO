#pragma once

#include <Wrappers/GameController/GameController.h>
#include <Wrappers/GameController/PS4Controller.h>
#include <Wrappers/GameController/XboxController.h>

#include <Wrappers/MotorController/CANMotorController.h>
#include <Wrappers/MotorController/Rev/CANSparkMax.h>
#include <Wrappers/MotorController/CTRE/CANTalonFX.h>

#include <Hardware/IOMap.h>

class HardwareManager {
public:

// Test board hardware.
#ifdef TEST_BOARD
    using DriveMotor = ThunderCANMotorController;

// Regular robot hardware.
#else
    using DriveMotor = ThunderCANSparkMax;
#endif

    using DriveGameController = ThunderPS4Controller;
    using AuxGameController = ThunderPS4Controller;
};
