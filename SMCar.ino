
#include "Wire.h"

#include "TimerOne.h"
#include "Command.h"
#include "Motor.h"
#include "Displace.h"
#include "MPU9250.h"

#define CALLBACK_INTERVAL (300000L)
#define CALIBRATE_INTERVAL (100000L)

struct _Status
{
  uint8_t connected = 0;
  uint8_t calibrating = 0;
} state;

MPU9250 mpu9250;
Motor motor({11, 6, 9, 10, 8, 7});

void setup()
{
  Wire.begin();
  Serial.begin(115200);

  Timer1.initialize(CALLBACK_INTERVAL);
  Timer1.attachInterrupt(period);

  motor.init();
  motor.setLeftDir(FORWARD);
  motor.setRightDir(BACKWARD);

  Displace.begin();

  mpu9250.MPU9250SelfTest(mpu9250.SelfTest);
  mpu9250.calibrateMPU9250(mpu9250.gyroBias, mpu9250.accelBias);
  mpu9250.initMPU9250();
  mpu9250.initAK8963(mpu9250.magCalibration);

  mpu9250.getMres();

  mpu9250.magbias[0] = -636.5306;
  mpu9250.magbias[1] = 488.6518;
  mpu9250.magbias[2] = -231.4022;

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{

  mpu9250.readMagData(mpu9250.magCount); // Read the x/y/z adc values

  mpu9250.mx = (float)mpu9250.magCount[0] * mpu9250.mRes * mpu9250.magCalibration[0] - mpu9250.magbias[0];
  mpu9250.my = (float)mpu9250.magCount[1] * mpu9250.mRes * mpu9250.magCalibration[1] - mpu9250.magbias[1];
  mpu9250.mz = (float)mpu9250.magCount[2] * mpu9250.mRes * mpu9250.magCalibration[2] - mpu9250.magbias[2];

  // Serial.print(mpu9250.mx);
  // Serial.print(",");
  // Serial.print(mpu9250.my);
  // Serial.print(",");
  // Serial.print(mpu9250.mz);
  // Serial.println();

  if (Serial.available())
  {
    char cmd = Serial.read();

    switch (cmd)
    {
    case 'C':
      mpu9250.magbias[0] = 0.;
      mpu9250.magbias[1] = 0.;
      mpu9250.magbias[2] = 0.;
      state.calibrating = 1;
      Timer1.setPeriod(CALIBRATE_INTERVAL);
      break;
    case 'R':
      mpu9250.magbias[0] = Serial.parseFloat();
      mpu9250.magbias[1] = Serial.parseFloat();
      mpu9250.magbias[2] = Serial.parseFloat();
      Timer1.setPeriod(CALLBACK_INTERVAL);
      break;
    case 'M':
    {
      int s, d;
      s = Serial.parseInt();
      d = Serial.parseInt();
      motor.resolve(s, d);
      break;
    }
    case 'S':
      Displace.update();
      state.connected = Serial.parseInt();
      break;
    default:
      break;
    }

    while (Serial.read() != '\n')
      ;
  }
}

float getDisplace(void)
{
  float dl, dr;
  Displace.update();

  dl = Displace.left;
  dr = Displace.right;

  if (motor.getLeftDir() == BACKWARD)
    dl = -dl;

  if (motor.getRightDir() == BACKWARD)
    dr = -dr;

  return dr + dr;
}

float getHeading(void)
{

  if (mpu9250.mx == 0)
  {
    if (mpu9250.my < 0)
      return HALF_PI;
    else
      return PI + HALF_PI;
  }

  float d = atan2(mpu9250.my, mpu9250.mx);

  if (d < 0)
  {
    d = TWO_PI + d;
  }

  return d;
}

void period()
{
  if (!state.connected)
  {
    return;
  }

  Serial.print("M ");
  Serial.print(getDisplace());
  Serial.print(" ");
  Serial.print(getHeading());
  Serial.println();
}