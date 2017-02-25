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
 * \return  Returns whatever PSCModuleControl returned.
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
/**************************** PWM and Servo Control ********************************/

/**
 * This section contains functions to abstract the use of the PWM pins. There are two PWM pins:
 * J15_40 and J15_44. In the following functions, PWM1 refers to J15_40 and PWM2 refers to J15_44.
 * The PWM pins can also act as servo control pins. The two PWM pins are part of the same module, which means
 * that they will be forced to function at the same frequency. This means that if you want one of the pins to
 * operate compatibly with servos, both pins will be forced to operate at 50Hz. PWM can still be used if the pins are 
 * set compatible with servos, but it will be operating at 50Hz, which might still be good enough for some 
 * PWM applications. If the PWM pins are not set to control servos, they operate at 58.8kHz.
 *
 * Summary: PWM1 refers to J15_40
 *          PWM2 refers to J15_44
 */

/**
 * \brief   Initialization code for the PWM pins. This needs to be called at the beginning of the program
 *          if the program uses any PWM pins. It sets some registers and stuff so that the PWM pins can be used.
 */
void PWMandServoInit();

/**
 * \brief   Sets up the PWM pins.
 *
 * \param   isServo     A boolean to indicate whether the PWM pins should be set so that
 *                      the can be used with servos. 
 *                          true:   PWM pins will be initialized for servo control.
 *                                  The PWM frequency is set to 50Hz which is compatible with 
 *                                  servos, and the Servo1write and Servo2write functions can 
 *                                  be used to set the angle. PWM1write and PWM2write can still
 *                                  be used for duty cycle, but the frequency of the PWM is still
 *                                  set to 50Hz.
 *                          false:  PWM pins will be initialized for generic high frequency PWM.
 *                                  The PWM is set at 58.8 kHz and the PWM1write and PWM2write 
 *                                  functions are used to write the duty cycle. Servos cannot be
 *                                  controlled at this frequnency and therefore none of the Servo
 *                                  functions should be called in this state.
 */
void PWMset(bool isServos);

/**
 * \brief   This function is used to control the duty cycle of the J15_40 PWM pin. If the PWM pins were set as
 *          servo pins, then the PWM will operate 50Hz. Otherwise, they operate at 58.8kHz.
 *          
 * \param   value       The duty cycle of the PWM. This number can be from 0 to 255.
 */
void PWM1write(int value);

/**
 * \brief   This function gets the duty cycle of the J15_40 PWM pin. This should only be called if the user intends
 *          to use this pin for full PWM. This function is actually identical to Servo1getAngle. Therefore, if the user
 *          calls this function after using Servo1write, the return value will not be guaranteed to correspond to duty
 *          cycle and will probably not have any significant meaning.
 *
 * \return  This function returns a number between 0 and 255.
 */
int PWM1getValue();

/**
 * \brief   This function is used to control the angle of the Servo connected to the J15_40 PWM pin. 
 *          This should be used only if the PWMs were declared as servos in the PWMset function. 
 *          Calling this function when the PWM pins were not declared as servo pins will have undefined behavior.
 */
void Servo1write(int degrees);

/**
 * \brief   This function gets the angle of the servo connected to the J15_40 PWM pin. This should only be called
 *          if the user intends to use PWM1 as a servo control. This function is actually identical to the PWM1getValue
 *          function. Therefore, if the user tries to call this function after using PWM1write, the return value is not
 *          guaranteed to be within [0,180] and will probably not have any significant meaning with regard to servo angle.
 *
 * \return  This function should return a number between 0 and 180 degrees.
 */
int Servo1getAngle();

//void PWM2set(bool isServo);

/**
 * \brief   This function is used to control the duty cycle of the J15_44 PWM pin. If the PWM pins were set as
 *          servo pins, then the PWM will operate 50Hz. Otherwise, they operate at 58.8kHz.
 *          
 * \param   value       The duty cycle of the PWM. This number can be from 0 to 255.
 */
void PWM2write(int value);

/**
 * \brief   This function gets the duty cycle of the J15_44 PWM pin. This should only be called if the user intends
 *          to use this pin for full PWM. This function is actually identical to Servo2getAngle. Therefore, if the user
 *          calls this function after using Servo2write, the return value will not be guaranteed to correspond to duty
 *          cycle and will probably not have any significant meaning.
 *
 * \return  This function returns a number between 0 and 255.
 */
int PWM2getValue();

/**
 * \brief   This function is used to control the angle of the Servo connected to the J15_44 PWM pin. 
 *          This should be used only if the PWMs were declared as servos in the PWMset function. 
 *          Calling this function when the PWM pins were not declared as servo pins will have undefined behavior.
 */
void Servo2write(int degrees);

/**
 * \brief   This function gets the angle of the servo connected to the J15_44 PWM pin. This should only be called
 *          if the user intends to use PWM1 as a servo control. This function is actually identical to the PWM2getValue
 *          function. Therefore, if the user tries to call this function after using PWM2write, the return value is not
 *          guaranteed to be within [0,180] and will probably not have any significant meaning with regard to servo angle.
 *
 * \return  This function returns a number between 0 and 180 degrees.
 */
int Servo2getAngle();



//------------------------------ End of J15: PWM -----------------------------------//

#endif /* HARDWARE_ABSTRACTIONS_H_ */
