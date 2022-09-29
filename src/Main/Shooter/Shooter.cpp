#include <Shooter/Shooter.h>
#include <Rev/CANSparkMax.h>
#include <Wrappers/MotorController/CANMotorController.h>


void Shooter::pivotShooter(void){
    ShooterMode shooterMode = ShooterMode::PIVOTING;
}

void Shooter::rotateBarrel(void){
    ShooterMode shooterMode = ShooterMode::ROTATING;
}

void Shooter::process(){
    switch (shooterMode) {
        
    }
}