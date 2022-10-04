#pragma once

#include <Basic/Mechanism.h>
#include <Hardware/HardwareManager.h>
#include <Basic/Feedback.h>
#include <units/angle.h>

class Shooter: public Mechanism {
public:
    int desiredShooterPivotPosition;
    int desiredShooterBarrelPosition;
    void process() override;
    void pivotShooter(units::degree_t angle);
    void rotateBarrel();
    void Shoot();
    void homeShooter();

private:
    units::degree_t maxAngle = 65_deg;
    units::degree_t minAngle = 0_deg;
    double maxPosition = 4.01;
    double minPositon = 0;
    int shooterPivotPosition;
    int shooterBarrelPosition;

    enum class ShooterMode {
        LOCALIZATION,
        IDLE, 
        WANT_TO_PIVOT,
        PIVOTING,
        WANT_TO_ROTATE,
        ROTATING,
        WANT_TO_SHOOT,
        SHOOTING
    };

    ShooterMode shooterMode = ShooterMode::LOCALIZATION;

    HardwareManager::ShooterRotationMotor rotateBarrel0 { CAN_SHOOTER_ROTATE_MOTOR };
    HardwareManager::ShooterPivotMotor pivotBarrel0 { CAN_SHOOTER_PIVOT_MOTOR0 };
    HardwareManager::ShooterPivotMotor pivotBarrel1 { CAN_SHOOTER_PIVOT_MOTOR1 };

};