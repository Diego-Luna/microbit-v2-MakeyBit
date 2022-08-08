#include "MicroBit.h"

#include "Tests.h"
#include "NRF52PWM.h"
#include "MemorySource.h"

MicroBit uBit;

// accelerometer
int value_z;
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

    // uBit.serial.printf("\n en moveMotorForward ");
    if (str == 'A')
    {
        uBit.io.P14.setAnalogValue(0);
        uBit.io.P13.setAnalogValue(speed);

        uBit.io.P20.setAnalogValue(200);
        // uBit.serial.printf("\n str == 'A' ");
    }
    else if (str == 'B')
    {
        uBit.io.P15.setAnalogValue(0);
        uBit.io.P16.setAnalogValue(speed);

        uBit.io.P19.setAnalogValue(200);
        // uBit.serial.printf("\n str == 'B' ");
    }
}
void moveMotorForwardALL(int speed)
{

    // uBit.serial.printf("\n en ForwardALL ");

    uBit.io.P14.setAnalogValue(0);
    uBit.io.P15.setAnalogValue(0);

    uBit.io.P13.setAnalogValue(speed);
    uBit.io.P16.setAnalogValue(speed);

    uBit.io.P20.setAnalogValue(200);
    uBit.io.P19.setAnalogValue(200);
}

void moveMotorBackwardALL(int speed)
{
    // uBit.serial.printf("\n en BackwardALL ");

    uBit.io.P13.setAnalogValue(0);
    uBit.io.P16.setAnalogValue(0);

    uBit.io.P14.setAnalogValue(speed);
    uBit.io.P15.setAnalogValue(speed);

    uBit.io.P20.setAnalogValue(200);
    uBit.io.P19.setAnalogValue(200);
}

void moveMotorBackward(int speed, char str)
{
    // uBit.serial.printf("\n en moveMotorBackward ");
    if (str == 'A')
    {
        uBit.io.P14.setAnalogValue(speed);
        uBit.io.P13.setAnalogValue(0);

        uBit.io.P20.setAnalogValue(200);
        // uBit.serial.printf("\n str == 'A' ");
    }
    else if (str == 'B')
    {
        uBit.io.P15.setAnalogValue(speed);
        uBit.io.P16.setAnalogValue(0);

        uBit.io.P19.setAnalogValue(200);
        // uBit.serial.printf("\n str == 'B' ");
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
        // moveMotorBackward(speed, 'A');
        // moveMotorBackward(speed, 'B');
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

    while (1)
    {
        // uBit.serial.printf("\n value z : %d", uBit.accelerometer.getZ());
        value_z = uBit.accelerometer.getZ();

        // if (value_z > -100 && value_z < 100)
        if (value_z > -50 && value_z < 50)
        {
            // uBit.serial.printf("\n OK : %d", uBit.accelerometer.getZ());
            if (stado != 0)
            {
                disableMotors();
                fullStop();
                stado = 0;
            }
        }
        // else if (value_z >= 100)
        else if (value_z >= 50)
        {
            // uBit.serial.printf("\n Acia atras : %d", uBit.accelerometer.getZ());
            if (stado != 1)
            {
                enableMotors();
                // move(1, value_z * 3);
                move(1, 700 + value_z);
                stado = 1;
            }
        }
        // else if (value_z <= -100)
        else if (value_z <= -50)
        {
            // uBit.serial.printf("\n Acia delante : %d", uBit.accelerometer.getZ());
            if (stado != 2)
            {
                enableMotors();
                // move(2, value_z * 3);
                move(2, 700 + (value_z * -1));
                stado = 2;
            }
        }

        // moveMotorBackward(300,'B');
        // moveMotorBackward(300,'A');
        // moveMotorForward(300, 'B');
        // moveMotorForward(300,'A');
        // move(2, value_z);
        // uBit.io.P14.setAnalogValue(900);
        // uBit.io.P13.setAnalogValue(0);
        // uBit.io.P20.setAnalogValue(20);
        // move(forward, 180);
        // uBit.sleep(100);
        // uBit.sleep(40);
    }
}