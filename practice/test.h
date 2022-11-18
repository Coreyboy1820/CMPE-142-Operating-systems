            
    struct leg {
        int steer_motor_ = 0;
    }; 

    float CalcEncoderPositions(int leg) {}

    void SetAngle(int angle){}

    void HomeLegs() {
        leg left;
        leg right;
        leg back;
        while(!LegIsZeroed(left) && !LegIsZeroed(right) && !LegIsZeroed(back));
    }

    bool LegIsZeroed(leg leg_){
        if((CalcEncoderPositions(leg_.steer_motor_) >= 0.01f) ||  (CalcEncoderPositions(leg_.steer_motor_) <= -0.01f)) {
            SetAngle(0);
            return true;
        }
        else 
            return false;
    }