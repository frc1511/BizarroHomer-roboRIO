#include <Basic/BizarroHomer.h>
#include <Basic/Mechanism.h>

using namespace frc;

void BizarroHomer::RobotInit() { }

void BizarroHomer::RobotPeriodic() {
    for (Mechanism* mech : allMechanisms) {
        mech->sendFeedback();
    }
}

void BizarroHomer::AutonomousInit() {
    reset(Mechanism::MatchMode::AUTO);
}

void BizarroHomer::AutonomousPeriodic() {
    drive.process();
}

void BizarroHomer::TeleopInit() {
    reset(Mechanism::MatchMode::TELEOP);
}

void BizarroHomer::TeleopPeriodic() {
    drive.process();
    shooter.process();
    controls.process();
}

void BizarroHomer::DisabledInit() {
    reset(Mechanism::MatchMode::DISABLED);
}

void BizarroHomer::DisabledPeriodic() {
    controls.processInDisabled();
}

void BizarroHomer::TestInit() {
    if (controls.getShouldPersistConfig()) {
        puts("*** Persistent configuration activating...");
        for (Mechanism* mech : allMechanisms) {
          mech->doPersistentConfiguration();
        }
        puts("*** Persistent configuration complete!");
    }
    reset(Mechanism::MatchMode::TEST);
}

void BizarroHomer::TestPeriodic() { }

void BizarroHomer::reset(Mechanism::MatchMode mode) {
    for (Mechanism* mech : allMechanisms) {
        mech->resetToMode(mode);
    }
}

int main() {
    return frc::StartRobot<BizarroHomer>();
}
