#include "MicroBit.h"

#include "Tests.h"
#include "NRF52PWM.h"
#include "MemorySource.h"

MicroBit uBit;

// accelerometer
int value_z;
int old_value_z;
int stado = 0;

// motor
Pin pinPWMA = uBit.io.P20;
Pin pinAIN2 = uBit.io.P14;
Pin pinAIN1 = uBit.io.P13;

Pin pinBIN1 = uBit.io.P16;
Pin pinBIN2 = uBit.io.P15;
Pin pinPWMB = uBit.io.P19;

Pin pinSTBY = uBit.io.P0;

int waitTime = 2000; // espera entre fases
int speed = 200;     // velocidad de giro

Pin pinMotorA[3] = {pinPWMA, pinAIN2, pinAIN1};
Pin pinMotorB[3] = {pinPWMB, pinBIN1, pinBIN2};

enum moveDirection
{
    forward,
    backward
};

enum turnDirection
{
    clockwise,
    counterClockwise
};

void stopMotor(char str)
{

    if (str == 'A')
    {
        uBit.io.P14.setAnalogValue(0);
        uBit.io.P13.setAnalogValue(0);

        uBit.io.P20.setAnalogValue(0);
    }
    else
    {
        uBit.io.P15.setAnalogValue(0);
        uBit.io.P16.setAnalogValue(0);

        uBit.io.P19.setAnalogValue(0);
    }
}

void moveMotorForward(int speed, char str)
{

    if (str == 'A')
    {
        uBit.io.P14.setAnalogValue(0);
        uBit.io.P13.setAnalogValue(speed);

        uBit.io.P20.setAnalogValue(200);
    }
    else if (str == 'B')
    {
        uBit.io.P15.setAnalogValue(0);
        uBit.io.P16.setAnalogValue(speed);

        uBit.io.P19.setAnalogValue(200);
    }
}
void moveMotorForwardALL(int speed)
{

    uBit.io.P14.setAnalogValue(0);
    uBit.io.P15.setAnalogValue(0);

    uBit.io.P13.setAnalogValue(speed);
    uBit.io.P16.setAnalogValue(speed);

    uBit.io.P20.setAnalogValue(200);
    uBit.io.P19.setAnalogValue(200);
}

void moveMotorBackwardALL(int speed)
{

    uBit.io.P13.setAnalogValue(0);
    uBit.io.P16.setAnalogValue(0);

    uBit.io.P14.setAnalogValue(speed);
    uBit.io.P15.setAnalogValue(speed);

    uBit.io.P20.setAnalogValue(200);
    uBit.io.P19.setAnalogValue(200);
}

void moveMotorBackward(int speed, char str)
{
    if (str == 'A')
    {
        uBit.io.P14.setAnalogValue(speed);
        uBit.io.P13.setAnalogValue(0);

        uBit.io.P20.setAnalogValue(200);
    }
    else if (str == 'B')
    {
        uBit.io.P15.setAnalogValue(speed);
        uBit.io.P16.setAnalogValue(0);

        uBit.io.P19.setAnalogValue(200);
    }
}

void enableMotors()
{
    uBit.io.P0.setDigitalValue(1);
}

void disableMotors()
{
    uBit.io.P0.setDigitalValue(0);
}

// Funciones que controlan el vehiculo
void move(int direction, int speed)
{
    if (speed < 0)
    {
        speed *= -1;
    }

    if (speed < 200)
    {
        speed = 200;
    }
    if (speed > 1000)
    {
        speed = 1000;
    }

    if (direction == 1)
    {
        moveMotorForwardALL(speed);
    }
    if (direction == 2)
    {
        moveMotorBackwardALL(speed);
    }
}

void fullStop()
{
    disableMotors();
    stopMotor('A');
    stopMotor('B');
}

int main()
{
    uBit.init();

    enableMotors();
    stopMotor('A');
    stopMotor('B');

    int min = 70;
    int max = 100;

    while (1)
    {
        // uBit.serial.printf("\n value z : %d", uBit.accelerometer.getZ());
        value_z = uBit.accelerometer.getZ();

        if (value_z >= min && value_z <= max)
        {
            if (stado != 0)
            {
                disableMotors();
                fullStop();
                stado = 0;
            }
        }
        else if (value_z >= max)
        {
            if (stado != 1)
            {
                enableMotors();
                move(1, 600);
                stado = 1;
                old_value_z = value_z;
            }
        }
        else if (value_z <= min)
        {
            if (stado != 2)
            {
                enableMotors();
                move(2, 600);
                stado = 2;
                old_value_z = value_z;
            }

            if (value_z < -350)
            {
                disableMotors();
                fullStop();
                stado = 2;
            }
        }
    }
}
