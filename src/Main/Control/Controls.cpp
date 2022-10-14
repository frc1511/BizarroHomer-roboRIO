#include <Control/Controls.h>
#include <wpi/numbers>
#include <cmath>

#define AXIS_DEADZONE 0.1

Controls::Controls(Drive* drive, Shooter* shooter)
: drive(drive) { }

Controls::~Controls() = default;

void Controls::process() {
    doDrive();
    doAux();
    doSwitchPanel();
}

void Controls::processInDisabled() {
    doSwitchPanel();

    using DriveButton = HardwareManager::DriveGameController::Button;

    bool toggleCamera = driveController.getButtonPressed(DriveButton::SQUARE);
    bool resetOdometry = driveController.getButtonPressed(DriveButton::OPTIONS);
    // bool calGyro = driveController.getButtonPressed(DriveButton::SHARE);

    if (toggleCamera) {
        whichCamera = !whichCamera;
    }

    if (resetOdometry) {
        drive->resetOdometry();
    }

    // if (calGyro) {
    //     drive->calibrateGyro();
    // }
}

void Controls::doDrive() {
    using DriveButton = HardwareManager::DriveGameController::Button;
    using DriveAxis = HardwareManager::DriveGameController::Axis;

    bool toggleCamera = driveController.getButtonPressed(DriveButton::SQUARE);

    double arcadeForwardBack = -driveController.getAxis(DriveAxis::LEFT_Y);
    double arcadeTurn = driveController.getAxis(DriveAxis::RIGHT_X);

    double tankLeft = -driveController.getAxis(DriveAxis::LEFT_Y);
    double tankRight = -driveController.getAxis(DriveAxis::RIGHT_Y);

    bool resetOdometry = driveController.getButtonPressed(DriveButton::OPTIONS);

    //fun shooter preset thing
    //30 degrees
    if (driveController.getButton(DriveButton::CROSS)){
        shooter->pivotShooter(30_deg);
    }
    //45 degrees
    if (driveController.getButton(DriveButton::TRIANGLE)){
        shooter->pivotShooter(45_deg);
    }
    //60 degrees
    if (driveController.getButton(DriveButton::CIRCLE)){
        shooter->pivotShooter(60_deg);
    }

    //manual angle adjustment
    if ((driveController.getDPad() == 0)){
        shooter->pivotShooter(shooter->getShooterAngle()+.5_deg);
    } 

    if ((driveController.getDPad() == 180)){
        shooter->pivotShooter(shooter->getShooterAngle()-.5_deg);
    }

    //SHOOTING
    if (driveController.getAxis(DriveAxis::LEFT_TRIGGER)){
        shooter->Shoot();
    }
    if (driveController.getButton(DriveButton::LEFT_BUMPER)){
        shooter->Preshoot();
    }

    // bool calGryo = driveController.getButtonPressed(DriveButton::SHARE);

    if (toggleCamera) {
        whichCamera = !whichCamera;
    }

    if (resetOdometry) {
        drive->resetOdometry();
    }

    // if (calGryo) {
    //     drive->calibrateGryo();
    // }

    // Improves the joystick axis to be smoother and easier to control.
    auto improveAxis = [](double axis) -> double {
        return std::sin(axis * (wpi::numbers::pi / 2.0));
    };

    if (driveTank) {
        double finalLeft = 0.0,
               finalRight = 0.0;

        if (std::fabs(tankLeft) > AXIS_DEADZONE) {
            finalLeft = improveAxis(tankLeft);
        }
        if (std::fabs(tankRight) > AXIS_DEADZONE) {
            finalRight = improveAxis(tankRight);
        }

        // Control the drivetrain.
        drive->manualControlTank(finalLeft, finalRight);
    }
    else { // Arcade.
        double finalForwardBack = 0.0,
               finalTurn = 0.0;

        if (std::fabs(arcadeForwardBack) > AXIS_DEADZONE) {
            finalForwardBack = improveAxis(arcadeForwardBack);
        }
        if (std::fabs(arcadeTurn) > AXIS_DEADZONE) {
            finalTurn = improveAxis(arcadeTurn);
        }

        // Control the drivetrain.
        drive->manualControlArcade(finalForwardBack, -finalTurn);
    }
}

bool Controls::getShouldPersistConfig() {
    doSwitchPanel();

    using DriveButton = HardwareManager::DriveGameController::Button;
    using AuxButton = HardwareManager::AuxGameController::Button;

    return settings.isCraterMode
        && driveController.getButton(DriveButton::TRIANGLE) && driveController.getDPad() == 180
        && auxController.getButton(AuxButton::CROSS) && auxController.getDPad() == 0;
}

void Controls::doAux() {
    // using AuxButton = HardwareManager::AuxGameController::Button;
    // using AuxAxis = HardwareManager::AuxGameController::Axis;

    // D:
}

void Controls::doSwitchPanel() {
    settings.isCraterMode = switchPanel.getButton(1);
    driveTank = switchPanel.getButton(2);
}

void Controls::sendFeedback() { }
