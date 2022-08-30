#include <Drive/Drive.h>

#define DRIVE_MAX_AMPERAGE 40_A

#define DRIVE_RAMP_TIME 0.5_s

#define DRIVE_P 0.00001
#define DRIVE_I 0
#define DRIVE_D 0
#define DRIVE_I_ZONE 0
#define DRIVE_FF 0.000187

#define DRIVE_FOOT_TO_ENDODER_FACTOR 6.51

#define DRIVE_METER_TO_ENCODER_FACTOR (DRIVE_FOOT_TO_ENDODER_FACTOR * 3.28084)

#define DRIVE_ENCODER_TO_METER_FACTOR (1 / (DRIVE_METER_TO_ENCODER_FACTOR))

#define DRIVE_MAX_MANUAL_VEL 1_mps
#define DRIVE_MAX_MANUAL_ANG_VEL 180_deg_per_s

Drive::Drive() {
    configureMotors();
}

Drive::~Drive() = default;

void Drive::process() {
    switch (driveMode) {
        case DriveMode::STOPPED:
            execStopped();
            break;
        case DriveMode::MANUAL:
            execManual();
            break;
    }
}

void Drive::configureMotors() {
    for (HardwareManager::DriveMotor* motor : driveMotors) {
        motor->configFactoryDefault();

        motor->setIdleMode(ThunderCANMotorController::IdleMode::COAST);

        // Amperage limiting.
        motor->configSmartCurrentLimit(DRIVE_MAX_AMPERAGE);

        motor->setInverted(false);

        motor->configOpenLoopRamp(DRIVE_RAMP_TIME);
        motor->configClosedLoopRamp(DRIVE_RAMP_TIME);

        motor->configP(DRIVE_P);
        motor->configI(DRIVE_I);
        motor->configD(DRIVE_D);
        motor->configIZone(DRIVE_I_ZONE);
        motor->configFF(DRIVE_FF);
    }
}

void Drive::doPersistentConfiguration() {
    configureMotors();

    for (HardwareManager::DriveMotor* motor : driveMotors) {
        motor->burnFlash();
    }
}

void Drive::resetToMode(MatchMode mode) {
    manualVelocities = { 0_mps, 0_mps, 0_deg_per_s };
    driveMode = DriveMode::STOPPED;
}

void Drive::manualControlTank(double leftPct, double rightPct) {
    units::meters_per_second_t leftVel(leftPct * DRIVE_MAX_MANUAL_VEL),
                               rightVel(rightPct * DRIVE_MAX_MANUAL_VEL);

    frc::DifferentialDriveWheelSpeeds wheelSpeeds { leftVel, rightVel };

    manualVelocities = kinematics.ToChassisSpeeds(wheelSpeeds);
}

void Drive::manualControlArcade(double forwardBackPct, double turnPct) {
    units::meters_per_second_t xVel(forwardBackPct * DRIVE_MAX_MANUAL_VEL);
    units::degrees_per_second_t angVel(turnPct * DRIVE_MAX_MANUAL_ANG_VEL);

    manualVelocities = { xVel, 0_mps, angVel };
}

void Drive::resetOdometry(frc::Pose2d pose) { }

void Drive::updateOdometry() { }

void Drive::execStopped() {
    setChassisVelocities(frc::ChassisSpeeds{ 0_mps, 0_mps, 0_rad_per_s });
}

void Drive::execManual() {
    setChassisVelocities(manualVelocities);
}

void Drive::setIdleMode(ThunderCANMotorController::IdleMode mode) {
    for (HardwareManager::DriveMotor* motor : driveMotors) {
        motor->setIdleMode(mode);
    }
}

void Drive::setChassisVelocities(frc::ChassisSpeeds velocities) {
    auto [leftMPS, rightMPS] = kinematics.ToWheelSpeeds(velocities);

    auto toRPM = [&](units::meters_per_second_t mps) -> double {
        return mps.value() * 60 * DRIVE_METER_TO_ENCODER_FACTOR;
    };

    double leftRPM(toRPM(leftMPS)),
           rightRPM(toRPM(rightMPS));

    leftDrive0.set(ThunderCANMotorController::ControlMode::VELOCITY, leftRPM);
    leftDrive1.set(ThunderCANMotorController::ControlMode::VELOCITY, leftRPM);
    rightDrive0.set(ThunderCANMotorController::ControlMode::VELOCITY, rightRPM);
    rightDrive1.set(ThunderCANMotorController::ControlMode::VELOCITY, rightRPM);
}

void Drive::sendFeedback() {
    for (std::size_t i = 0; i < driveMotors.size(); ++i) {
        HardwareManager::DriveMotor* motor(driveMotors.at(i));
        Feedback::sendDouble("Drive Motor " + i, "encoder rotations", motor->getEncoderPosition());
        Feedback::sendDouble("Drive Motor " + i, "velocity (rpm)", motor->getEncoderVelocity());
        Feedback::sendDouble("Drive Motor " + i, "percent output", motor->getPercentOutput());
        Feedback::sendDouble("Drive Motor " + i, "temperature (F)", motor->getTemperature().value());
    }
}
