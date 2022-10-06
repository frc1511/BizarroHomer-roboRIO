#include <Shooter/Shooter.h>
#include <Rev/CANSparkMax.h>
#include <Wrappers/MotorController/CANMotorController.h>



void Shooter::pivotShooter(units::degree_t desiredShooterPivotAngle){
    ShooterMode shooterMode = ShooterMode::PIVOTING;
    targetPivotAngle = (((desiredShooterPivotAngle-minAngle)/(maxAngle-minAngle))*(maxPosition-minPosition))+minPosition;
    pivotBarrel0.set(ThunderCANMotorController::ControlMode::POSITION, targetPivotAngle);
    pivotBarrel1.set(ThunderCANMotorController::ControlMode::POSITION, targetPivotAngle);
}

void Shooter::rotateBarrel(void){
    ShooterMode shooterMode = ShooterMode::ROTATING;
    
    //rotateBarrel0.set(ThunderCANMotorController::ControlMode::POSITION, 5);
}

void Shooter::Shoot(void){

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
        case ShooterMode::IDLE:
            return;
        case ShooterMode::WANT_TO_PIVOT:
            //placeholder value for now
            pivotShooter(10_deg);
            break;
        case ShooterMode::WANT_TO_ROTATE:
            rotateBarrel();
            break;
        case ShooterMode::WANT_TO_SHOOT:
            Shoot();
            break;
    }
}