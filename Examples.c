#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/family/c64p/Hwi.h>

#include <stdio.h>
#include "psc.h"
#include "vpif.h"
#include "raster.h"
#include "interrupt.h"
#include "lcdkC6748.h"
#include "soc_C6748.h"
#include "hw_psc_C6748.h"
#include "adv7343.h"
#include "tvp5147.h"
#include "cdce913.h"
#include "codecif.h"
#include "i2cgpio.h"
#include "cv.h"
#include "cxtypes.h"
#include "string.h"
#include "facedetect.h"
#include "highgui.h"
#include "hardware_abstractions.h"

/* Call one of the following tests in the main function at the bottom */

/* tests all GPIO pins at once by switching them HIGH/LOW to check voltages easily */
void GPIO_test_output()
{
    GPIOinit();

    GPIOsetPin(J14_3, GPIO_OUTPUT);
    GPIOsetPin(J14_4, GPIO_OUTPUT);
    GPIOsetPin(J14_5, GPIO_OUTPUT);
    GPIOsetPin(J14_6, GPIO_OUTPUT);
    GPIOsetPin(J14_7, GPIO_OUTPUT);
    GPIOsetPin(J14_8, GPIO_OUTPUT);
    GPIOsetPin(J14_9, GPIO_OUTPUT);
    GPIOsetPin(J14_10, GPIO_OUTPUT);
    GPIOsetPin(J14_11, GPIO_OUTPUT);
    GPIOsetPin(J14_12, GPIO_OUTPUT);
    GPIOsetPin(J14_13, GPIO_OUTPUT);
    GPIOsetPin(J14_14, GPIO_OUTPUT);
    GPIOsetPin(J14_15, GPIO_OUTPUT);
    GPIOsetPin(J14_16, GPIO_OUTPUT);
    GPIOsetPin(J14_17, GPIO_OUTPUT);
    GPIOsetPin(J14_18, GPIO_OUTPUT);
    GPIOsetPin(J14_21, GPIO_OUTPUT);
    GPIOsetPin(J14_22, GPIO_OUTPUT);
    GPIOsetPin(J14_23, GPIO_OUTPUT);
    GPIOsetPin(J14_24, GPIO_OUTPUT);
    GPIOsetPin(J14_25, GPIO_OUTPUT);
    GPIOsetPin(J14_26, GPIO_OUTPUT);
    GPIOsetPin(J14_27, GPIO_OUTPUT);
    GPIOsetPin(J14_28, GPIO_OUTPUT);
    GPIOsetPin(J14_29, GPIO_OUTPUT);
    GPIOsetPin(J14_30, GPIO_OUTPUT);
    GPIOsetPin(J14_31, GPIO_OUTPUT);
    GPIOsetPin(J14_32, GPIO_OUTPUT);
    GPIOsetPin(J14_33, GPIO_OUTPUT);
    GPIOsetPin(J15_16, GPIO_OUTPUT);
    GPIOsetPin(J15_20, GPIO_OUTPUT);

    while(1)
    {
        GPIOwritePin(J14_3, GPIO_PIN_LOW);
        GPIOwritePin(J14_4, GPIO_PIN_LOW);
        GPIOwritePin(J14_5, GPIO_PIN_LOW);
        GPIOwritePin(J14_6, GPIO_PIN_LOW);
        GPIOwritePin(J14_7, GPIO_PIN_LOW);
        GPIOwritePin(J14_8, GPIO_PIN_LOW);
        GPIOwritePin(J14_9, GPIO_PIN_LOW);
        GPIOwritePin(J14_10, GPIO_PIN_LOW);
        GPIOwritePin(J14_11, GPIO_PIN_LOW);
        GPIOwritePin(J14_12, GPIO_PIN_LOW);
        GPIOwritePin(J14_13, GPIO_PIN_LOW);
        GPIOwritePin(J14_14, GPIO_PIN_LOW);
        GPIOwritePin(J14_15, GPIO_PIN_LOW);
        GPIOwritePin(J14_16, GPIO_PIN_LOW);
        GPIOwritePin(J14_17, GPIO_PIN_LOW);
        GPIOwritePin(J14_18, GPIO_PIN_LOW);
        GPIOwritePin(J14_21, GPIO_PIN_LOW);
        GPIOwritePin(J14_22, GPIO_PIN_LOW);
        GPIOwritePin(J14_23, GPIO_PIN_LOW);
        GPIOwritePin(J14_24, GPIO_PIN_LOW);
        GPIOwritePin(J14_25, GPIO_PIN_LOW);
        GPIOwritePin(J14_26, GPIO_PIN_LOW);
        GPIOwritePin(J14_27, GPIO_PIN_LOW);
        GPIOwritePin(J14_28, GPIO_PIN_LOW);
        GPIOwritePin(J14_29, GPIO_PIN_LOW);
        GPIOwritePin(J14_30, GPIO_PIN_LOW);
        GPIOwritePin(J14_31, GPIO_PIN_LOW);
        GPIOwritePin(J14_32, GPIO_PIN_LOW);
        GPIOwritePin(J14_33, GPIO_PIN_LOW);
        GPIOwritePin(J15_16, GPIO_PIN_LOW);
        GPIOwritePin(J15_20, GPIO_PIN_LOW);

        Delay(5000000);

        GPIOwritePin(J14_3, GPIO_PIN_HIGH);
        GPIOwritePin(J14_4, GPIO_PIN_HIGH);
        GPIOwritePin(J14_5, GPIO_PIN_HIGH);
        GPIOwritePin(J14_6, GPIO_PIN_HIGH);
        GPIOwritePin(J14_7, GPIO_PIN_HIGH);
        GPIOwritePin(J14_8, GPIO_PIN_HIGH);
        GPIOwritePin(J14_9, GPIO_PIN_HIGH);
        GPIOwritePin(J14_10, GPIO_PIN_HIGH);
        GPIOwritePin(J14_11, GPIO_PIN_HIGH);
        GPIOwritePin(J14_12, GPIO_PIN_HIGH);
        GPIOwritePin(J14_13, GPIO_PIN_HIGH);
        GPIOwritePin(J14_14, GPIO_PIN_HIGH);
        GPIOwritePin(J14_15, GPIO_PIN_HIGH);
        GPIOwritePin(J14_16, GPIO_PIN_HIGH);
        GPIOwritePin(J14_17, GPIO_PIN_HIGH);
        GPIOwritePin(J14_18, GPIO_PIN_HIGH);
        GPIOwritePin(J14_21, GPIO_PIN_HIGH);
        GPIOwritePin(J14_22, GPIO_PIN_HIGH);
        GPIOwritePin(J14_23, GPIO_PIN_HIGH);
        GPIOwritePin(J14_24, GPIO_PIN_HIGH);
        GPIOwritePin(J14_25, GPIO_PIN_HIGH);
        GPIOwritePin(J14_26, GPIO_PIN_HIGH);
        GPIOwritePin(J14_27, GPIO_PIN_HIGH);
        GPIOwritePin(J14_28, GPIO_PIN_HIGH);
        GPIOwritePin(J14_29, GPIO_PIN_HIGH);
        GPIOwritePin(J14_30, GPIO_PIN_HIGH);
        GPIOwritePin(J14_31, GPIO_PIN_HIGH);
        GPIOwritePin(J14_32, GPIO_PIN_HIGH);
        GPIOwritePin(J14_33, GPIO_PIN_HIGH);
        GPIOwritePin(J15_16, GPIO_PIN_HIGH);
        GPIOwritePin(J15_20, GPIO_PIN_HIGH);

        Delay(5000000);
    }
}

void GPIO_test_input()
{
    GPIOinit();
    GPIOsetPin(J14_3, GPIO_OUTPUT);
    GPIOsetPin(J14_4, GPIO_INPUT);
    GPIOsetPin(J14_5, GPIO_OUTPUT);
    int val = 0;
    while(1)
    {
        GPIOwritePin(J14_3, GPIO_PIN_HIGH);
        val = GPIOreadPin(J14_4);
        GPIOwritePin(J14_5, val);
        Delay(5000000);

        GPIOwritePin(J14_3, GPIO_PIN_LOW);
        val = GPIOreadPin(J14_4);
        GPIOwritePin(J14_5, val);

        Delay(5000000);
    }
}

void UART1_and_UART2_test()
{
    char transmit1[] = "How much wood can a woodchuck chuck if a woodchuck could chuck wood.\n\r";
    char transmit2[] = "She sells seashells by the seashore is a tongue twister. Try it out.\n\r";
    UART1init();
    UART2init();
    int i;

    for(i = 0; i < strlen(transmit1); i++)
    {
        UART1putChar(transmit1[i]);
        UART2putChar(transmit2[i]);
    }
    for(i = 0; i < strlen(transmit2); i++)
    {
        UART1putChar(transmit2[i]);
        UART2putChar(transmit1[i]);
    }

    while(1)
    {
        i = UART1getCharNonBlocking();
        if (i >= 0)
        {
            UART2putChar((char)i);
        }

        i = UART2getCharNonBlocking();
        if (i >= 0)
        {
            UART2putChar((char)i);
            UART1putChar((char)i);
        }
    }
}

/* Sweeps a servo on J14_40 from 0 to 180 degrees */
void Servo1_test()
{
    PWMandServoInit();
    PWMset(true);
    int i;
    while(1)
    {
        for(i = 0; i <= 180; i=i+10)
        {
            Servo1write(i);
            Delay(10000000);
        }
        for(i = 180; i >= 0; i=i-10)
        {
            Servo1write(i);
            Delay(10000000);
        }
    }
}

void PWM1_and_PWM2_test()
{
    PWMandServoInit();
    PWMset(false);
    int i;
    while(1)
    {
        for(i = 0; i <= 255; i=i+5)
        {
            PWM1write(i);
            PWM2write(255-i);
            Delay(10000000);
        }
        for(i = 255; i >= 0; i=i-5)
        {
            PWM1write(i);
            PWM2write(255-i);
            Delay(10000000);
        }
    }
}

void Servo_1_and_2_test()
{
    PWMandServoInit();
    PWMset(true);
    Servo1write(0);
    Servo2write(180);
    while(1)
    {;}
}

void PWM1_and_Servo2_test()
{
    PWMandServoInit();
    PWMset(true);
    int i;
    while(1)
    {
        for(i = 0; i <= 180; i=i+10)
        {
            PWM1write(1.0*i/180*255);
            Servo2write(i);
            Delay(10000000);
        }
        for(i = 180; i >= 0; i=i-10)
        {
            PWM1write(1.0*i/180*255);
            Servo2write(i);
            Delay(10000000);
        }
    }
}

void PWM2_and_Servo1_test()
{
    PWMandServoInit();
    PWMset(true);
    int i;
    while(1)
    {
        for(i = 0; i <= 180; i=i+10)
        {
            PWM2write(1.0*i/180*255);
            Servo1write(i);
            Delay(10000000);
        }
        for(i = 180; i >= 0; i=i-10)
        {
            PWM2write(1.0*i/180*255);
            Servo1write(i);
            Delay(10000000);
        }
    }
}

void main()
{
	//call one of the example tests here
}