#pragma once

#include <Basic/Mechanism.h>
#include <Hardware/HardwareManager.h>
#include <Basic/Feedback.h>

class Shooter: public Mechanism {
public:
    int desiredShooterPivotPosition;
    int desiredShooterBarrelPosition;
    void pivotShooter();
    void rotateBarrel();

private:
    int shooterPivotAngle;
    int shooterBarrelPosition;

    enum class ShooterMode {
        IDLE, 
        PIVOTING,
        ROTATING,
        WANT_TO_SHOOT,
        SHOOTING
    };

    HardwareManager::ShooterRotationMotor rotateBarrel0 { CAN_SHOOTER_ROTATE_MOTOR };
    HardwareManager::ShooterPivotMotor pivotBarrel0 { CAN_SHOOTER_PIVOT_MOTOR0 };
    HardwareManager::ShooterPivotMotor pivotBarrel1 { CAN_SHOOTER_PIVOT_MOTOR1 };

};