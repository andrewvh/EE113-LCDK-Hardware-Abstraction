/*
 * \file    hardware_abstractions.h
 *
 *  Created on: Feb 17, 2017
 *      Author: Andrew Van Ho
 *
 * \brief   This file contains code to abstract the use of the LCDK.
 *          It was made mostly by copying and expanding the code in the
 *          starterware examples. The functions in this file are hopefully
 *          well documented enough and new-user friendly. TI is soooo crappy
 *          with documentation. This code is split into two sections: J14 and J15.
 *          These correspond to the two header pin groups on the LCDK.
 *
 *                                      J14
 *          The J14 pins have be implemented as digital pins, even if any of the
 *          pins have specialized skills. I have only gathered the functions for
 *          basic input/output. So if you want to do fancy interrupt stuff, you'll
 *          need to refer back to the starterware examples and figure out those
 *          functions. For some reason, J14_1 and J14_2 are connected to 5V, but the
 *          digital logic is 3.3V (or at least that was what I measured, not sure if
 *          this can be changed).
 *
 *                                      J15
 *          The J15 headers have special purposes (UART, I2C, SPI). I only tried to
 *          implement the UART serial communication. Go to the section to read more about
 *          this.
 *          Update: It seems I also tried to implement PWM. Anything to avoid doing my
 *          GE homework. I added it as a new sections "J15: PWM" at the bottom.
 *          Update: I have made it so that the PWM pins can also control servos. I am really
 *          avoiding my GE homework...
 */

#ifndef HARDWARE_ABSTRACTIONS_H_
#define HARDWARE_ABSTRACTIONS_H_

#include "gpio.h"
#include "uart.h"
#include "i2c.h"
#include "psc.h"

#include "soc_OMAPL138.h"
#include "lcdkOMAPL138.h"
#include "hw_types.h"
#include "hw_syscfg0_OMAPL138.h"

#include "interrupt.h"
#include "uart.h"

#include "ehrpwm.h"
#include "hw_pwmss.h"

/************************************* J14 *****************************************/
/****************************** Digital GPIO Pins **********************************/

/**
 * The J14 headers contain digital GPIO pins. The following functions and preprocessor directives
 * are supposed to make it easier to use the GPIO by abstracting away the register setup
 * and all the register constants that need to be referenced.
 *
 * Not all of the J14 pins are GPIO. Those that are available for use are in the enumerated type
 * 'PinNumberOnBoard' where they are given names corresponding to their actual location on the
 * headers. The function documentation will be written here instead of the .c file so
 * that you don't have to keep switching back in forth (any feedback on this decision).
 * I have not implemented any GPIO interrupt capabilities. When I measured the J14 voltages,
 * the GPIOs used 3.3V for digital high.
 */

/**
 * Pin modes
 * These will be used as a parameter to the GPIOsetPin function so that you can tell whether you
 * want an pin to be an input or output.
 */
#define GPIO_OUTPUT 0
#define GPIO_INPUT 1

/**
 * Logic Levels.
 * These are the constants used as a parameter to tell the GPIOwritePin function what voltage you want
 * to write to the pin. These have already been declared for us elsewhere, which is why this is commented.
 */
//      GPIO_PIN_LOW
//      GPIO_PIN_HIGH

/**
 * These are the available GPIO pins on the J14 headers. Their names correspond to the location
 * on the headers. These are used as a parameter to nearly all of the functions to specify which
 * pin you are reading/writing/initializing/etc.
 */
typedef enum
{
    J14_3 = 73,
    J14_4 = 74,
    J14_5 = 75,
    J14_6 = 76,
    J14_7 = 77,
    J14_8 = 78,
    J14_9 = 79,
    J14_10 = 80,
    J14_11 = 49,
    J14_12 = 50,
    J14_13 = 51,
    J14_14 = 52,
    J14_15 = 53,
    J14_16 = 54,
    J14_17 = 55,
    J14_18 = 56,
    J14_21 = 81,
    J14_22 = 82,
    J14_23 = 83,
    J14_24 = 84,
    J14_25 = 85,
    J14_26 = 86,
    J14_27 = 87,
    J14_28 = 88,
    J14_29 = 89,
    J14_30 = 90,
    J14_31 = 91,
    J14_32 = 92,
    J14_33 = 93
} PinNumberOnBoard;

/**
 * \brief   Initialization code for the J14 GPIO pins. This needs to be called at the beginning of the program
 *          if the program uses any of the J14 GPIO pins. It sets some registers and stuff so that the pins
 *          can be used.
 *
 * \return  Returns the whatever PSCModuleControl returned.
 */
int GPIOinit();

/**
 * \brief   Initialize a pin for use and set the pin as input or output.
 *
 * \param   pinNumber   The pin designation used by the PinNumberOnBoard enum.
 *          mode        The mode of the pin: either GPIO_INPUT or GPIO_OUTPUT.
 *
 * \return  Always returns 0.
 */
int GPIOsetPin(PinNumberOnBoard pinNumber, int mode);

/**
 * \brief   Write digital logic to a GPIO output pin.
 *
 * \param   pinNumber   The pin designation used by the PinNumberOnBoard enum.
 *          logiclevel  Voltage logic level to write to output pin. This is either
 *                      GPIO_PIN_HIGH or GPIO_PIN_LOW.
 */
void GPIOwritePin(PinNumberOnBoard pinNumber, unsigned int logicLevel);

/**
 * \brief   Read digital logic level on a GPIO input pin.
 *
 * \param   pinNumber   The pin designation used by the PinNumberOnBoard enum.
 *
 * \return  This function returns the digital logic level read on the GPIO pin
 *          specified by pinNumber. It returns one of the following constants:
 *          GPIO_PIN_HIGH or GPIO_PIN_LOW.
 */
int GPIOreadPin(PinNumberOnBoard pinNumber);

// Delay function. Not really for anything. I was just testing something.
void Delay(unsigned int delay);

//--------------------------------- End of J14 -------------------------------------//



/********************************** J15: UART **************************************/
/*************** Serial Communication over USB and Direct Hardware *****************/

/**
 * Disclaimer:  This next section is still under development. UART1 and UART2 seem to
 *              be stable (I have lightly tested these). UART0 has not been tested, but
 *              the code for it should work. However, I have not bothered to find the pin
 *              locations for UART0. Use this code with caution.
 *
 * TODO:        Find the resistors for UART0 and figure out which side of the resistors are
 *              needed for serial communication. Clean up enable interrupt code (I think there
 *              are redundancies with the SetupInt functions, the other two functions can also
 *              be combined). Also, add the possibility of attaching a custom function to the
 *              interrupt when there is available data. For my own personal notes:
 *                  To enable UART1:
 *                      Remove R205, R208
 *                      Short R206, R209 (both right side important)
 *                  To enable UART0:
 *                      Remove R172, R173, R277
 *                      Short R218, R219
 *
 * This section contains code to abstract the use of the UART for serial communication.
 * UART2 is associated with the USB-UART port on the LCDK. So, UART2 can only be used to
 * communicate to the computer. UART1 and UART0 are hardware serial, so they can be used
 * to communicate with other hardware (arduino, teensy, etc). I have lightly tested UART2 and
 * UART1. Both seem to be working. I have not tested UART0. I think the code for UART0 should
 * work, but I have not bothered to find the resistors and test this. You will have to find the
 * resistors if you want to use UART0. Go to each of the subsections for the details on the pins
 * and resistors for each UART. In its current state, the code does not offer any way to attach
 * a custom function to the 'available data' interrupt. So your programs should make sure to check the
 * UART for data periodically and be cautious about buffer size limits, else data might be thrown away.
 */


///////////////////////////////////////////////////
//==================== UART2 ====================//
///////////////////////////////////////////////////
/**
 * UART2 is connected to the mini-USB port labeled 'UART-USB'. UART2 can only be used
 * to communicate with the computer. You just connect the computer to the mini-USB port.
 */

/**
 *
 */
void UART2init();

/**
 * Let's just not call this function...
 */
void UART2enableInterrupt();

/**
 *
 */
unsigned int UART2putCharNonBlocking(unsigned char byteToWrite);

/**
 *
 */
int UART2getCharNonBlocking();

/**
 *
 */
int UART2getChar();

/**
 *
 */
void UART2putChar(unsigned char byteToWrite);

/**
 * These three functions are called by the function UART2enableInterrupt. I'm not sure
 * that UART2enableInterrupt is necessary, which is why I said not to call it. You won't
 * really ever call these directly, so you can hopefully disregard them.
 */
static void ConfigureIntUART2(void);
static void SetupInt2(void);
static void UART2Isr(void);

///////////////////////////////////////////////////
//==================== UART1 ====================//
///////////////////////////////////////////////////
/**
 * UART1 is for hardware serial. So it can be used to communicate with other
 * serial devices (arduino, teensy, etc).
 *
 * These functions have the same descriptions as the UART2 functions. So, I decided
 * not to write the descriptions again.
 */

void UART1init();
void UART1enableInterrupt();
unsigned int UART1putCharNonBlocking(unsigned char byteToWrite);
int UART1getCharNonBlocking();
int UART1getChar();
void UART1putChar(unsigned char byteToWrite);

static void ConfigureIntUART1(void);
static void SetupInt1(void);
static void UART1Isr(void);

///////////////////////////////////////////////////
//==================== UART0 ====================//
///////////////////////////////////////////////////
/**
 *
 *
 * These functions have the same descriptions as the UART2 and UART1 functions. So, I
 * decided not to write the descriptions again.
 */
void UART0init();
void UART0enableInterrupt();
unsigned int UART0putCharNonBlocking(unsigned char byteToWrite);
int UART0getCharNonBlocking();
int UART0getChar();
void UART0putChar(unsigned char byteToWrite);

static void ConfigureIntUART0(void);
static void SetupInt0(void);
static void UART0Isr(void);

//------------------------------ End of J15: UART ----------------------------------//



/*********************************** J15: PWM **************************************/
/*************** Serial Communication over USB and Direct Hardware *****************/

/**
 *
 */

void PWMandServoInit();

void PWMset(bool isServos);

void PWM1write(int value);

int PWM1getValue();

void Servo1write(int degrees);

int Servo1getAngle();

//void PWM2set(bool isServo);

void PWM2write(int value);

int PWM2getValue();

void Servo2write(int degrees);

int Servo2getAngle();



//------------------------------ End of J15: PWM -----------------------------------//

#endif /* HARDWARE_ABSTRACTIONS_H_ */
