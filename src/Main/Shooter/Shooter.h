#pragma once

#include <Basic/Mechanism.h>
#include <Hardware/HardwareManager.h>
#include <Basic/Feedback.h>
#include <units/angle.h>
#include "Hardware/IOMap.h"
#include <frc/DigitalInput.h>
#include <frc/Solenoid.h>

class Shooter: public Mechanism {
public:
    units::degree_t desiredShooterPivotAngle;
    int desiredShooterBarrelPosition;
    bool shooterIsAligned;
    void process() override;
    void pivotShooter(units::degree_t angle);
    void getShooterAngle();
    void rotateBarrel();
    void Preshoot();
    void Shoot();
    void homeShooter();

private:
    units::degree_t maxAngle = 65_deg;
    units::degree_t minAngle = 0_deg;
    double maxPosition = 4.01;
    double minPosition = 0.0;
    int shooterPivotAngle;
    int targetPivotAngle;
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
    frc::DigitalInput shooterBanner { DIO_SHOOTER_BANNER_ZEROING };
    frc::Solenoid fillVolume {frc::PneumaticsModuleType::CTREPCM, FILL_VOLUME_TANK};
    frc::Solenoid shooterValve {frc::PneumaticsModuleType::CTREPCM, OPEN_SHOOTER};

};