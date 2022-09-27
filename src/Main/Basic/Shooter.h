#include <Basic/Mechanism.h>

class Shooter: public Mechanism {
    public:
        int shooterPivotPosition;
        int shooterBarrelPosition;
        void pivotShooter();
        void rotateBarrel();

};