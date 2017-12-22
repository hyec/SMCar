#ifndef _SPEED_H_
#define _SPEED_H_

#define SENSOR_L 3
#define SENSOR_R 2

class Displacement
{
public:
    float left, right;

    Displacement(unsigned long *);
    void begin(void);
    void update(void);
private:
    unsigned long *_times;
};

extern Displacement Displace;

#endif