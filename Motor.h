#ifndef _MOTOR_H_
#define _MOTOR_H_

#define FORWARD 0
#define BACKWARD 1

class Motor
{
  public:
    struct MotorPins {
      int EA, EB, I1, I2, I3, I4;
    };

    Motor(MotorPins pins);
    void init(void);

    void setLeftSpeed(int speed);
    void setRightSpeed(int speed);
    void setSpeed(int speed);

    int getLeftDir(void);
    void setLeftDir(int d);

    int getRightDir(void);
    void setRightDir(int d);

    void setDir(int d);

    void forward(void);
    void backward(void);

    void resolve(int speed, int deflect);
    
  private:
    MotorPins _pins;
};

#endif