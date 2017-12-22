#include "Arduino.h"

#include "Motor.h"

Motor::Motor(MotorPins pins)
{
    _pins = pins;
}

void Motor::init(void)
{
    pinMode(_pins.EA, OUTPUT);
    pinMode(_pins.EB, OUTPUT);
    pinMode(_pins.I1, OUTPUT);
    pinMode(_pins.I2, OUTPUT);
    pinMode(_pins.I3, OUTPUT);
    pinMode(_pins.I4, OUTPUT);
}

void Motor::setLeftSpeed(int speed)
{
    speed = constrain(speed, 0, 255);
    analogWrite(_pins.EA, speed);
}

void Motor::setRightSpeed(int speed)
{
    speed = constrain(speed, 0, 255);
    analogWrite(_pins.EB, speed);
}

void Motor::setSpeed(int speed)
{
    setLeftSpeed(speed);
    setRightSpeed(speed);
}

int Motor::getLeftDir(void)
{
    return digitalRead(_pins.I1);
}

void Motor::setLeftDir(int d)
{
    uint8_t val = (d == 0) ? LOW : HIGH;

    digitalWrite(_pins.I1, val);
    digitalWrite(_pins.I2, !val);
}

int Motor::getRightDir(void)
{
    return digitalRead(_pins.I4);
}

void Motor::setRightDir(int d)
{
    uint8_t val = (d == 0) ? LOW : HIGH;

    digitalWrite(_pins.I3, !val);
    digitalWrite(_pins.I4, val);
}

void Motor::setDir(int d)
{
    setLeftDir(d);
    setRightDir(d);
}

void Motor::forward(void)
{
    digitalWrite(_pins.I1, LOW);
    digitalWrite(_pins.I2, HIGH);
    digitalWrite(_pins.I3, HIGH);
    digitalWrite(_pins.I4, LOW);
}

void Motor::backward(void)
{
    digitalWrite(_pins.I1, HIGH);
    digitalWrite(_pins.I2, LOW);
    digitalWrite(_pins.I3, LOW);
    digitalWrite(_pins.I4, HIGH);
}

void Motor::resolve(int speed, int deflect)
{
    int dir, slow;

    if (speed == 0)
    {
        setSpeed(0);
        return;
    }

    dir = speed > 0 ? FORWARD : BACKWARD;
    speed = abs(speed);

    if(deflect == 0)
    {
        setSpeed(speed);
        setDir(dir);
        return;
    }

    slow = speed - speed * abs(deflect) / 80;

    if (deflect > 0)
    {
        setLeftDir(dir);
        setRightDir(slow > 0 ? dir : !dir);
        setLeftSpeed(speed);
        setRightSpeed(abs(slow));
    } else {
        setRightDir(dir);
        setLeftDir(slow > 0 ? dir : !dir);
        setRightSpeed(speed);
        setLeftSpeed(abs(slow));
    }
}
