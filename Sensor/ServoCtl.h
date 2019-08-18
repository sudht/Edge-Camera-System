#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <stdlib.h>
#include <unistd.h>

int initServo(int PIN) {

        if (wiringPiSetupPhys() == -1)  return -1;

        pinMode(PIN, PWM_OUTPUT);

        pwmSetMode(PWM_MODE_MS);

        pwmSetClock(384);
        pwmSetRange(1000);

        return 0;
}

int changePWM(int PIN, float PWM) {
        if (PWM >= 30 && PWM <= 120) {
                pwmWrite(PIN, PWM);
                sleep(1);
                return 0;
        }
        return -1;
}
