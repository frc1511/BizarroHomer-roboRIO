#pragma once

#include <Basic/Mechanism.h>
#include <Basic/Feedback.h>
#include <Hardware/HardwareManager.h>

#include <frc/kinematics/ChassisSpeeds.h>
#include <frc/kinematics/DifferentialDriveWheelSpeeds.h>
#include <frc/kinematics/DifferentialDriveKinematics.h>
#include <frc/kinematics/DifferentialDriveOdometry.h>
#include <units/angle.h>
#include <units/velocity.h>
#include <units/length.h>
#include <array>

#define DRIVE_WIDTH 0.7_m

class Drive : public Mechanism {
public:
    Drive();
    ~Drive();

    void process() override;
    void sendFeedback() override;
    void doPersistentConfiguration() override;
    void resetToMode(MatchMode mode) override;

    void manualControlTank(double leftPct, double rightPct);
    void manualControlArcade(double forwardBackPct, double turnPct);

    void resetOdometry(frc::Pose2d pose = frc::Pose2d());

private:
    void updateOdometry();
    void execStopped();
    void execManual();

    void setIdleMode(ThunderCANMotorController::IdleMode mode);
    void setChassisVelocities(frc::ChassisSpeeds velocities);

    void configureMotors();

    enum class DriveMode {
        STOPPED,
        MANUAL,
    };

    DriveMode driveMode = DriveMode::STOPPED;

    frc::ChassisSpeeds manualVelocities;

    frc::DifferentialDriveKinematics kinematics { DRIVE_WIDTH };

    HardwareManager::DriveMotor leftDrive0  { CAN_DRIVE_MOTOR_L0 };
    HardwareManager::DriveMotor leftDrive1  { CAN_DRIVE_MOTOR_L1 };
    HardwareManager::DriveMotor rightDrive0 { CAN_DRIVE_MOTOR_R0 };
    HardwareManager::DriveMotor rightDrive1 { CAN_DRIVE_MOTOR_R1 };

    std::vector<HardwareManager::DriveMotor*> driveMotors {
        &leftDrive0, &leftDrive1, &rightDrive0, &rightDrive1
    };
};
