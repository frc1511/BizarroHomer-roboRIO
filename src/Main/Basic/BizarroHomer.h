#pragma once

#include <frc/TimedRobot.h>

#include <Basic/Mechanism.h>
#include <Control/Controls.h>
#include <Drive/Drive.h>
#include <Shooter/Shooter.h>
#include <Hardware/IOMap.h>
#include <vector>
#include <iostream>

class BizarroHomer : public frc::TimedRobot {
public:
    void RobotInit() override;
    void RobotPeriodic() override;

    void AutonomousInit() override;
    void AutonomousPeriodic() override;

    void TeleopInit() override;
    void TeleopPeriodic() override;

    void DisabledInit() override;
    void DisabledPeriodic() override;

    void TestInit() override;
    void TestPeriodic() override;

private:
    void reset(Mechanism::MatchMode mode);

    Drive drive;
    Shooter shooter;

    Controls controls { &drive };

    std::vector<Mechanism*> allMechanisms {
        &drive, &controls, &shooter
    };
};
