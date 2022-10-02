#include <Shooter/Shooter.h>
#include <Rev/CANSparkMax.h>
#include <Wrappers/MotorController/CANMotorController.h>


void Shooter::pivotShooter(void){
    ShooterMode shooterMode = ShooterMode::PIVOTING;
}

void Shooter::rotateBarrel(void){
    ShooterMode shooterMode = ShooterMode::ROTATING;
    shooterBarrelPosition =rotateBarrel0.getEncoderPosition();
    rotateBarrel0.set(ThunderCANMotorController::ControlMode::POSITION, 5);
}

void Shooter::Shoot(void){

}

void Shooter::process(){
    switch (shooterMode) {
        case ShooterMode::IDLE:
            return;
        case ShooterMode::WANT_TO_PIVOT:
            pivotShooter();
            break;
        case ShooterMode::WANT_TO_ROTATE:
            rotateBarrel();
            break;
        case ShooterMode::WANT_TO_SHOOT:
            Shoot();
            break;
    }
}