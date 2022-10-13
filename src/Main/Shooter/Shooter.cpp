#include <Shooter/Shooter.h>
#include <Rev/CANSparkMax.h>
#include <Wrappers/MotorController/CANMotorController.h>

void Shooter::Preshoot(){

}

void Shooter::pivotShooter(units::degree_t desiredShooterPivotAngle){
    shooterMode = ShooterMode::PIVOTING;
    targetPivotAngle = (((desiredShooterPivotAngle-minAngle)/(maxAngle-minAngle))*(maxPosition-minPosition))+minPosition;
    pivotBarrel0.set(ThunderCANMotorController::ControlMode::POSITION, targetPivotAngle);
    pivotBarrel1.set(ThunderCANMotorController::ControlMode::POSITION, targetPivotAngle);
}

units::degree_t Shooter::getShooterAngle(){
    return ((pivotBarrel0.getEncoderPosition())/(maxPosition-minPosition))*(maxAngle-minAngle)+minAngle;
}

void Shooter::rotateBarrel(void){
    shooterIsAligned = shooterBanner.Get();
    if (shooterIsAligned == true) {
        shooterMode = ShooterMode::WANT_TO_SHOOT;
    }
    if (shooterIsAligned == false) {
        shooterMode = ShooterMode::ROTATING;
        rotateBarrel0.set(ThunderCANMotorController::ControlMode::PERCENT_OUTPUT, .05);
    }
}

void Shooter::Shoot(void){
    shooterMode = ShooterMode::SHOOTING;
    //close volume tank if open
    fillVolume.Set(false);
    //setup a pulse duration for the shot
    shooterValve.SetPulseDuration(.5_s);
    shooterValve.StartPulse();
}

//zeroes the shooter on bootup
void Shooter::homeShooter(void){
    //invert one motor because of they way they're opposite 
    pivotBarrel0.setInverted(true);
    //sets percent output to bring them down slowly 
    pivotBarrel0.set(ThunderCANMotorController::ControlMode::PERCENT_OUTPUT, -.05);
    pivotBarrel1.set(ThunderCANMotorController::ControlMode::PERCENT_OUTPUT, -.05);
    //looks for current spike, if the current spikes, set encoders to 0 and then stop the motors
    if ((pivotBarrel0.getOutputCurrent() > 30_A) && (pivotBarrel1.getOutputCurrent() > 30_A)){
        pivotBarrel0.setEncoderPosition(0);
        pivotBarrel1.setEncoderPosition(0);
        pivotBarrel0.set(ThunderCANMotorController::ControlMode::PERCENT_OUTPUT, 0);
        pivotBarrel1.set(ThunderCANMotorController::ControlMode::PERCENT_OUTPUT, 0);
        //sets shooter mode to idle once the zero has completed
        shooterMode = ShooterMode::IDLE;
    }
    
}

void Shooter::process(){
    switch (shooterMode) {
        case ShooterMode::LOCALIZATION:
            homeShooter();
            break;  
        case ShooterMode::IDLE:
            return;
        case ShooterMode::WANT_TO_PIVOT:
            //placeholder value for now
            pivotShooter(desiredShooterPivotAngle);
            break;
        case ShooterMode::WANT_TO_ROTATE:
            rotateBarrel();
            break;
        case ShooterMode::WANT_TO_SHOOT:
            Preshoot();
            break;
        case ShooterMode::ROTATING:
            break;
        case ShooterMode::SHOOTING:
            break;
        case ShooterMode::PIVOTING:
            break;
    }
}