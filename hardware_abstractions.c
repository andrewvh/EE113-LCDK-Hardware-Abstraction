#include "hardware_abstractions.h"
#include <stdio.h>
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

/* Initialization code for the J14 GPIO pins. This needs to be called
   at the beginning of the program if it uses any J14 GPIO pins. */
int GPIOinit()
{
    /* The PSC (Power Sleep Controller) needs to be set to "on" for the GPIO pins */
    return PSCModuleControl(SOC_PSC_1_REGS, HW_PSC_GPIO, PSC_POWERDOMAIN_ALWAYS_ON,
             PSC_MDCTL_NEXT_ENABLE);
}

/* Initialize a pin for use. Set as input or output. */
int GPIOsetPin(PinNumberOnBoard pinNumber, int mode)
{
    unsigned int savePinmux = 0;

    switch(pinNumber)
    {
         /* PIN MUXING
        ** - To save space on the package size of the processor, it multiplexes separate internal
        **   functionalities together on the same external pin using 4 bits to distingish the functions.
        ** 
        ** - There are 19 pin muxes, each with 32 bits addressable to enable different uses. We are using
        **   4 bits on each pin. To access the 4 bits that we need to, we appropriately shift the bits to
        **   the correct spot.
        **
        ** - The code below is setting the appropriate bits on each PINMUX register to enable the correct
        **   functionalities on those pins. This can be explicitly seen in the "Register" window in CCS by
        **   clicking the appropriate PINMUX register (0-19). Checking this value is a good way to verify
        **   that you have appropriately selected the correct pin functionality
        */
    case J14_3:
        /*
        ** Clearing the bit in context and retaining the other bit values
        ** in PINMUX9 register.
        */
        savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(9)) &
                     ~(SYSCFG_PINMUX9_PINMUX9_31_28));

        /* Setting the pins corresponding to GP4[8] in PINMUX9 register.*/
        HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(9)) =
             ((SYSCFG_PINMUX9_PINMUX9_31_28_GPIO4_8 << SYSCFG_PINMUX9_PINMUX9_31_28_SHIFT) | savePinmux);

        break;
    case J14_4:
        savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(9)) &
                             ~(SYSCFG_PINMUX9_PINMUX9_27_24));

        HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(9)) =
             ((SYSCFG_PINMUX9_PINMUX9_27_24_GPIO4_9 << SYSCFG_PINMUX9_PINMUX9_27_24_SHIFT) | savePinmux);
        break;
    case J14_5:
        savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(9)) &
                             ~(SYSCFG_PINMUX9_PINMUX9_23_20));

        HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(9)) =
             ((SYSCFG_PINMUX9_PINMUX9_23_20_GPIO4_10 << SYSCFG_PINMUX9_PINMUX9_23_20_SHIFT) | savePinmux);
        break;
    case J14_6:
        savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(9)) &
                             ~(SYSCFG_PINMUX9_PINMUX9_19_16));

        HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(9)) =
             ((SYSCFG_PINMUX9_PINMUX9_19_16_GPIO4_11 << SYSCFG_PINMUX9_PINMUX9_19_16_SHIFT) | savePinmux);
        break;
    case J14_7:
        savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(9)) &
                             ~(SYSCFG_PINMUX9_PINMUX9_15_12));

        HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(9)) =
             ((SYSCFG_PINMUX9_PINMUX9_15_12_GPIO4_12 << SYSCFG_PINMUX9_PINMUX9_15_12_SHIFT) | savePinmux);
        break;
    case J14_8:
        savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(9)) &
                             ~(SYSCFG_PINMUX9_PINMUX9_11_8));

        HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(9)) =
             ((SYSCFG_PINMUX9_PINMUX9_11_8_GPIO4_13 << SYSCFG_PINMUX9_PINMUX9_11_8_SHIFT) | savePinmux);
        break;
    case J14_9:
        savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(9)) &
                             ~(SYSCFG_PINMUX9_PINMUX9_7_4));

        HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(9)) =
             ((SYSCFG_PINMUX9_PINMUX9_7_4_GPIO4_14 << SYSCFG_PINMUX9_PINMUX9_7_4_SHIFT) | savePinmux);
        break;
    case J14_10:
        savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(9)) &
                             ~(SYSCFG_PINMUX9_PINMUX9_3_0));

        HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(9)) =
             ((SYSCFG_PINMUX9_PINMUX9_3_0_GPIO4_15 << SYSCFG_PINMUX9_PINMUX9_3_0_SHIFT) | savePinmux);
        break;

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    case J14_11:
        savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(8)) &
                             ~(SYSCFG_PINMUX8_PINMUX8_31_28));

        HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(8)) =
             ((SYSCFG_PINMUX8_PINMUX8_31_28_GPIO3_0 << SYSCFG_PINMUX8_PINMUX8_31_28_SHIFT) | savePinmux);
        break;
    case J14_12:
        savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(8)) &
                             ~(SYSCFG_PINMUX8_PINMUX8_27_24));

        HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(8)) =
             ((SYSCFG_PINMUX8_PINMUX8_27_24_GPIO3_1 << SYSCFG_PINMUX8_PINMUX8_27_24_SHIFT) | savePinmux);
        break;
    case J14_13:
        savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(8)) &
                             ~(SYSCFG_PINMUX8_PINMUX8_23_20));

        HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(8)) =
             ((SYSCFG_PINMUX8_PINMUX8_23_20_GPIO3_2 << SYSCFG_PINMUX8_PINMUX8_23_20_SHIFT) | savePinmux);
        break;
    case J14_14:
        savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(8)) &
                             ~(SYSCFG_PINMUX8_PINMUX8_19_16));

        HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(8)) =
             ((SYSCFG_PINMUX8_PINMUX8_19_16_GPIO3_3 << SYSCFG_PINMUX8_PINMUX8_19_16_SHIFT) | savePinmux);
        break;
    case J14_15:
        savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(8)) &
                             ~(SYSCFG_PINMUX8_PINMUX8_15_12));

        HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(8)) =
             ((SYSCFG_PINMUX8_PINMUX8_15_12_GPIO3_4 << SYSCFG_PINMUX8_PINMUX8_15_12_SHIFT) | savePinmux);
        break;
    case J14_16:
        savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(8)) &
                             ~(SYSCFG_PINMUX8_PINMUX8_11_8));

        HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(8)) =
             ((SYSCFG_PINMUX8_PINMUX8_11_8_GPIO3_5 << SYSCFG_PINMUX8_PINMUX8_11_8_SHIFT) | savePinmux);
        break;
    case J14_17:
        savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(8)) &
                             ~(SYSCFG_PINMUX8_PINMUX8_7_4));

        HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(8)) =
             ((SYSCFG_PINMUX8_PINMUX8_7_4_GPIO3_6 << SYSCFG_PINMUX8_PINMUX8_7_4_SHIFT) | savePinmux);
        break;
    case J14_18:
        savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(8)) &
                             ~(SYSCFG_PINMUX8_PINMUX8_3_0));

        HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(8)) =
             ((SYSCFG_PINMUX8_PINMUX8_3_0_GPIO3_7 << SYSCFG_PINMUX8_PINMUX8_3_0_SHIFT) | savePinmux);
        break;

    /////////////////////////////////////////////////////////////////////////////////////////////
    case J14_21:
        savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(12)) &
                             ~(SYSCFG_PINMUX12_PINMUX12_31_28));

        HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(12)) =
             ((SYSCFG_PINMUX12_PINMUX12_31_28_GPIO5_0 << SYSCFG_PINMUX12_PINMUX12_31_28_SHIFT) | savePinmux);
        break;
    case J14_22:
        savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(12)) &
                             ~(SYSCFG_PINMUX12_PINMUX12_27_24));

        HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(12)) =
             ((SYSCFG_PINMUX12_PINMUX12_27_24_GPIO5_1 << SYSCFG_PINMUX12_PINMUX12_27_24_SHIFT) | savePinmux);
        break;
    case J14_23:
        savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(12)) &
                             ~(SYSCFG_PINMUX12_PINMUX12_23_20));

        HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(12)) =
             ((SYSCFG_PINMUX12_PINMUX12_23_20_GPIO5_2 << SYSCFG_PINMUX12_PINMUX12_23_20_SHIFT) | savePinmux);
        break;
    case J14_24:
        savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(12)) &
                             ~(SYSCFG_PINMUX12_PINMUX12_19_16));

        HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(12)) =
             ((SYSCFG_PINMUX12_PINMUX12_19_16_GPIO5_3 << SYSCFG_PINMUX12_PINMUX12_19_16_SHIFT) | savePinmux);
        break;
    case J14_25:
        savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(12)) &
                             ~(SYSCFG_PINMUX12_PINMUX12_15_12));

        HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(12)) =
             ((SYSCFG_PINMUX12_PINMUX12_15_12_GPIO5_4 << SYSCFG_PINMUX12_PINMUX12_15_12_SHIFT) | savePinmux);
        break;
    case J14_26:
        savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(12)) &
                             ~(SYSCFG_PINMUX12_PINMUX12_11_8));

        HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(12)) =
             ((SYSCFG_PINMUX12_PINMUX12_11_8_GPIO5_5 << SYSCFG_PINMUX12_PINMUX12_11_8_SHIFT) | savePinmux);
        break;
    case J14_27:
        savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(12)) &
                             ~(SYSCFG_PINMUX12_PINMUX12_7_4));

        HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(12)) =
             ((SYSCFG_PINMUX12_PINMUX12_7_4_GPIO5_6 << SYSCFG_PINMUX12_PINMUX12_7_4_SHIFT) | savePinmux);
        break;
    case J14_28:
        savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(12)) &
                             ~(SYSCFG_PINMUX12_PINMUX12_3_0));

        HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(12)) =
             ((SYSCFG_PINMUX12_PINMUX12_3_0_GPIO5_7 << SYSCFG_PINMUX12_PINMUX12_3_0_SHIFT) | savePinmux);
        break;
    case J14_29:
        savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(11)) &
                             ~(SYSCFG_PINMUX11_PINMUX11_31_28));

        HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(11)) =
             ((SYSCFG_PINMUX11_PINMUX11_31_28_GPIO5_8 << SYSCFG_PINMUX11_PINMUX11_31_28_SHIFT) | savePinmux);
        break;
    case J14_30:
        savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(11)) &
                             ~(SYSCFG_PINMUX11_PINMUX11_27_24));

        HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(11)) =
             ((SYSCFG_PINMUX11_PINMUX11_27_24_GPIO5_9 << SYSCFG_PINMUX11_PINMUX11_27_24_SHIFT) | savePinmux);
        break;
    case J14_31:
        savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(11)) &
                             ~(SYSCFG_PINMUX11_PINMUX11_23_20));

        HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(11)) =
             ((SYSCFG_PINMUX11_PINMUX11_23_20_GPIO5_10 << SYSCFG_PINMUX11_PINMUX11_23_20_SHIFT) | savePinmux);
        break;
    case J14_32:
        savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(11)) &
                             ~(SYSCFG_PINMUX11_PINMUX11_19_16));

        HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(11)) =
             ((SYSCFG_PINMUX11_PINMUX11_19_16_GPIO5_11 << SYSCFG_PINMUX11_PINMUX11_19_16_SHIFT) | savePinmux);
        break;
    case J14_33:
        savePinmux = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(11)) &
                             ~(SYSCFG_PINMUX11_PINMUX11_15_12));

        HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(11)) =
             ((SYSCFG_PINMUX11_PINMUX11_15_12_GPIO5_12 << SYSCFG_PINMUX11_PINMUX11_15_12_SHIFT) | savePinmux);
        break;
    default:
        break;
    }

    /* Set the Pin mode to input or output.*/
    if(mode == GPIO_OUTPUT)
    {
        GPIODirModeSet(SOC_GPIO_0_REGS, pinNumber, GPIO_DIR_OUTPUT);
    }
    else
    {
        GPIODirModeSet(SOC_GPIO_0_REGS, pinNumber, GPIO_DIR_INPUT);
    }

    return 0;
}

/* Write to GPIO pin. Logic level constants are GPIO_PIN_HIGH and GPIO_PIN_LOW */
void GPIOwritePin(PinNumberOnBoard pinNumber, unsigned int logicLevel)
{

    GPIOPinWrite(SOC_GPIO_0_REGS, pinNumber, logicLevel);
}

/* Read GPIO pin. Function returns GPIO_PIN_HIGH or GPIO_PIN_LOW */
int GPIOreadPin(PinNumberOnBoard pinNumber)
{
    int value = GPIOPinRead(SOC_GPIO_0_REGS, pinNumber);
    return value;
}

/* Just for testing. */
void Delay(unsigned int delay)
{
    while(delay--);
}
//--------------------------------- End of J14 -------------------------------------//


/************************************* J15 *****************************************/
/*************** Serial Communication over USB and Direct Hardware *****************/

//char txArray[] = "StarterWare UART echo application\n\r";
char txArray[] = "Initializing Serial with Interrupts\n\r";

///////////////////////////////////////////////////
//==================== UART2 ====================//
///////////////////////////////////////////////////
void UART2init()
{
    unsigned int config = 0;

    /* Enabling the PSC for UART2.*/
    PSCModuleControl(SOC_PSC_1_REGS, HW_PSC_UART2, PSC_POWERDOMAIN_ALWAYS_ON,
             PSC_MDCTL_NEXT_ENABLE);

    /* Setup PINMUX */
    UARTPinMuxSetup(2, FALSE);

    /* Enabling the transmitter and receiver*/
    UARTEnable(SOC_UART_2_REGS);

    /* 1 stopbit, 8-bit character, no parity */
    config = UART_WORDL_8BITS;

    /* Configuring the UART parameters*/
    UARTConfigSetExpClk(SOC_UART_2_REGS, SOC_UART_2_MODULE_FREQ,
                        BAUD_115200, config,
                        UART_OVER_SAMP_RATE_16);

    /* Enabling the FIFO and flushing the Tx and Rx FIFOs.*/
    UARTFIFOEnable(SOC_UART_2_REGS);

    /* Setting the UART Receiver Trigger Level*/
    UARTFIFOLevelSet(SOC_UART_2_REGS, UART_RX_TRIG_LEVEL_1);

    return;
}

void UART2enableInterrupt()
{
    unsigned int intFlags = 0;

    /*
    ** Enable AINTC to handle interrupts. Also enable IRQ interrupt in ARM
    ** processor.
    */
    SetupInt2();

    /* Configure AINTC to receive and handle UART interrupts. */
    ConfigureIntUART2();

    /* Preparing the 'intFlags' variable to be passed as an argument.*/
    intFlags |= (UART_INT_LINE_STAT  |  \
                 UART_INT_TX_EMPTY |    \
                 UART_INT_RXDATA_CTI);

    /* Enable the Interrupts in UART.*/
    UARTIntEnable(SOC_UART_2_REGS, intFlags);

    return;
}

unsigned int UART2putCharNonBlocking(unsigned char byteToWrite)
{
    return UARTCharPutNonBlocking(SOC_UART_2_REGS, byteToWrite);
}

int UART2getCharNonBlocking()
{
    return UARTCharGetNonBlocking(SOC_UART_2_REGS);
}

int UART2getChar()
{
    return UARTCharGet(SOC_UART_2_REGS);
}

void UART2putChar(unsigned char byteToWrite)
{
    UARTCharPut(SOC_UART_2_REGS, byteToWrite);
    return;
}

/*
** \brief   Interrupt Service Routine(ISR) to be executed on UART interrupts.
**          Depending on the source of interrupt, this
**          1> writes to the serial communication console, or
**          2> reads from the serial communication console, or
**          3> reads the byte in RBR if receiver line error has occured.
*/
static void UART2Isr()
    {
    static unsigned int length = sizeof(txArray);
    static unsigned int count = 0;
    unsigned char rxData = 0;
    unsigned int int_id = 0;

    /* This determines the cause of UART2 interrupt.*/
    int_id = UARTIntStatus(SOC_UART_2_REGS);

    #ifdef _TMS320C6X
    // Clear UART2 system interrupt in DSPINTC
    IntEventClear(SYS_INT_UART2_INT);
    #else
    /* Clears the system interupt status of UART2 in AINTC. */
    IntSystemStatusClear(SYS_INT_UARTINT2);
    #endif

    /* Checked if the cause is transmitter empty condition.*/
    if(UART_INTID_TX_EMPTY == int_id)
    {
        if(0 < length)
        {
            /* Write a byte into the THR if THR is free. */
            UARTCharPutNonBlocking(SOC_UART_2_REGS, txArray[count]);
            length--;
            count++;
        }
        if(0 == length)
        {
            /* Disable the Transmitter interrupt in UART.*/
            UARTIntDisable(SOC_UART_2_REGS, UART_INT_TX_EMPTY);
        }
     }

    /* Check if the cause is receiver data condition.*/
    if(UART_INTID_RX_DATA == int_id)
    {
        rxData = UARTCharGetNonBlocking(SOC_UART_2_REGS);
        UARTCharPutNonBlocking(SOC_UART_2_REGS, rxData);
    }


    /* Check if the cause is receiver line error condition.*/
    if(UART_INTID_RX_LINE_STAT == int_id)
    {
        while(UARTRxErrorGet(SOC_UART_2_REGS))
        {
            /* Read a byte from the RBR if RBR has data.*/
            UARTCharGetNonBlocking(SOC_UART_2_REGS);
        }
    }

    return;
}

/*
** \brief   This function invokes necessary functions to configure the ARM
**          processor and ARM Interrupt Controller(AINTC) to receive and
**          handle interrupts.
*/
static void SetupInt2(void)
{
    #ifdef _TMS320C6X
    // Initialize the DSP INTC
    IntDSPINTCInit();

    // Enable DSP interrupts globally
    IntGlobalEnable();
    #else
    /* Initialize the ARM Interrupt Controller(AINTC). */
    IntAINTCInit();

    /* Enable IRQ in CPSR.*/
    IntMasterIRQEnable();

    /* Enable the interrupts in GER of AINTC.*/
    IntGlobalEnable();

    /* Enable the interrupts in HIER of AINTC.*/
    IntIRQEnable();
    #endif
}

/*
** \brief  This function configures the AINTC to receive UART interrupts.
*/
static void ConfigureIntUART2(void)
{
    #ifdef _TMS320C6X
    IntRegister(C674X_MASK_INT4, UART2Isr);
    IntEventMap(C674X_MASK_INT4, SYS_INT_UART2_INT);
    IntEnable(C674X_MASK_INT4);
    #else
    /* Registers the UARTIsr in the Interrupt Vector Table of AINTC. */
    IntRegister(SYS_INT_UARTINT2, UART2Isr);

    /* Map the channel number 2 of AINTC to UART2 system interrupt. */
    IntChannelSet(SYS_INT_UARTINT2, 2);

    IntSystemEnable(SYS_INT_UARTINT2);
    #endif
}

///////////////////////////////////////////////////
//==================== UART1 ====================//
///////////////////////////////////////////////////
void UART1init()
{
    unsigned int config = 0;

    /* Enabling the PSC for UART1.*/
    PSCModuleControl(SOC_PSC_1_REGS, HW_PSC_UART1, PSC_POWERDOMAIN_ALWAYS_ON,
             PSC_MDCTL_NEXT_ENABLE);

    /* Setup PINMUX */
    UARTPinMuxSetup(1, FALSE);

    /* Enabling the transmitter and receiver*/
    UARTEnable(SOC_UART_1_REGS);

    /* 1 stopbit, 8-bit character, no parity */
    config = UART_WORDL_8BITS;

    /* Configuring the UART parameters*/
    UARTConfigSetExpClk(SOC_UART_1_REGS, SOC_UART_2_MODULE_FREQ,
                        BAUD_115200, config,
                        UART_OVER_SAMP_RATE_16);

    /* Enabling the FIFO and flushing the Tx and Rx FIFOs.*/
    UARTFIFOEnable(SOC_UART_1_REGS);

    /* Setting the UART Receiver Trigger Level*/
    UARTFIFOLevelSet(SOC_UART_1_REGS, UART_RX_TRIG_LEVEL_1);

    return;
}

void UART1enableInterrupt()
{
    unsigned int intFlags = 0;

    /*
    ** Enable AINTC to handle interrupts. Also enable IRQ interrupt in ARM
    ** processor.
    */
    SetupInt1();

    /* Configure AINTC to receive and handle UART interrupts. */
    ConfigureIntUART1();

    /* Preparing the 'intFlags' variable to be passed as an argument.*/
    intFlags |= (UART_INT_LINE_STAT  |  \
                 UART_INT_TX_EMPTY |    \
                 UART_INT_RXDATA_CTI);

    /* Enable the Interrupts in UART.*/
    UARTIntEnable(SOC_UART_1_REGS, intFlags);

    return;
}

unsigned int UART1putCharNonBlocking(unsigned char byteToWrite)
{
    return UARTCharPutNonBlocking(SOC_UART_1_REGS, byteToWrite);
}

int UART1getCharNonBlocking()
{
    return UARTCharGetNonBlocking(SOC_UART_1_REGS);
}

int UART1getChar()
{
    return UARTCharGet(SOC_UART_1_REGS);
}

void UART1putChar(unsigned char byteToWrite)
{
    UARTCharPut(SOC_UART_1_REGS, byteToWrite);
    return;
}

/*
** \brief   Interrupt Service Routine(ISR) to be executed on UART interrupts.
**          Depending on the source of interrupt, this
**          1> writes to the serial communication console, or
**          2> reads from the serial communication console, or
**          3> reads the byte in RBR if receiver line error has occurred.
*/
static void UART1Isr()
    {
    static unsigned int length = sizeof(txArray);
    static unsigned int count = 0;
    unsigned char rxData = 0;
    unsigned int int_id = 0;

    /* This determines the cause of UART1 interrupt.*/
    int_id = UARTIntStatus(SOC_UART_1_REGS);

    #ifdef _TMS320C6X
    // Clear UART1 system interrupt in DSPINTC
    IntEventClear(SYS_INT_UART1_INT);
    #else
    /* Clears the system interrupt status of UART1 in AINTC. */
    IntSystemStatusClear(SYS_INT_UARTINT1);
    #endif

    /* Checked if the cause is transmitter empty condition.*/
    if(UART_INTID_TX_EMPTY == int_id)
    {
        if(0 < length)
        {
            /* Write a byte into the THR if THR is free. */
            UARTCharPutNonBlocking(SOC_UART_1_REGS, txArray[count]);
            length--;
            count++;
        }
        if(0 == length)
        {
            /* Disable the Transmitter interrupt in UART.*/
            UARTIntDisable(SOC_UART_1_REGS, UART_INT_TX_EMPTY);
        }
     }

    /* Check if the cause is receiver data condition.*/
    if(UART_INTID_RX_DATA == int_id)
    {
        rxData = UARTCharGetNonBlocking(SOC_UART_1_REGS);
        UARTCharPutNonBlocking(SOC_UART_1_REGS, rxData);
    }


    /* Check if the cause is receiver line error condition.*/
    if(UART_INTID_RX_LINE_STAT == int_id)
    {
        while(UARTRxErrorGet(SOC_UART_1_REGS))
        {
            /* Read a byte from the RBR if RBR has data.*/
            UARTCharGetNonBlocking(SOC_UART_1_REGS);
        }
    }

    return;
}

/*
** \brief   This function invokes necessary functions to configure the ARM
**          processor and ARM Interrupt Controller(AINTC) to receive and
**          handle interrupts.
*/
static void SetupInt1(void)
{
    #ifdef _TMS320C6X
    // Initialize the DSP INTC
    IntDSPINTCInit();

    // Enable DSP interrupts globally
    IntGlobalEnable();
    #else
    /* Initialize the ARM Interrupt Controller(AINTC). */
    IntAINTCInit();

    /* Enable IRQ in CPSR.*/
    IntMasterIRQEnable();

    /* Enable the interrupts in GER of AINTC.*/
    IntGlobalEnable();

    /* Enable the interrupts in HIER of AINTC.*/
    IntIRQEnable();
    #endif
}

/*
** \brief  This function configures the AINTC to receive UART interrupts.
*/
static void ConfigureIntUART1(void)
{
    #ifdef _TMS320C6X
    IntRegister(C674X_MASK_INT5, UART1Isr);
    IntEventMap(C674X_MASK_INT5, SYS_INT_UART1_INT);
    IntEnable(C674X_MASK_INT5);
    #else
    /* Registers the UARTIsr in the Interrupt Vector Table of AINTC. */
    IntRegister(SYS_INT_UARTINT1, UART1Isr);

    /* Map the channel number 1 of AINTC to UART1 system interrupt. */
    IntChannelSet(SYS_INT_UARTINT1, 1);

    IntSystemEnable(SYS_INT_UARTINT1);
    #endif
}

///////////////////////////////////////////////////
//==================== UART0 ====================//
///////////////////////////////////////////////////
void UART0init()
{
    unsigned int config = 0;

    /* Enabling the PSC for UART0.*/
    PSCModuleControl(SOC_PSC_0_REGS, HW_PSC_UART0, PSC_POWERDOMAIN_ALWAYS_ON,
             PSC_MDCTL_NEXT_ENABLE);

    /* Setup PINMUX */
    UARTPinMuxSetup(0, FALSE);

    /* Enabling the transmitter and receiver*/
    UARTEnable(SOC_UART_0_REGS);

    /* 1 stopbit, 8-bit character, no parity */
    config = UART_WORDL_8BITS;

    /* Configuring the UART parameters*/
    UARTConfigSetExpClk(SOC_UART_0_REGS, SOC_UART_2_MODULE_FREQ,
                        BAUD_115200, config,
                        UART_OVER_SAMP_RATE_16);

    /* Enabling the FIFO and flushing the Tx and Rx FIFOs.*/
    UARTFIFOEnable(SOC_UART_0_REGS);

    /* Setting the UART Receiver Trigger Level*/
    UARTFIFOLevelSet(SOC_UART_0_REGS, UART_RX_TRIG_LEVEL_1);

    return;
}

void UART0enableInterrupt()
{
    unsigned int intFlags = 0;

    /*
    ** Enable AINTC to handle interrupts. Also enable IRQ interrupt in ARM
    ** processor.
    */
    SetupInt0();

    /* Configure AINTC to receive and handle UART interrupts. */
    ConfigureIntUART0();

    /* Preparing the 'intFlags' variable to be passed as an argument.*/
    intFlags |= (UART_INT_LINE_STAT  |  \
                 UART_INT_TX_EMPTY |    \
                 UART_INT_RXDATA_CTI);

    /* Enable the Interrupts in UART.*/
    UARTIntEnable(SOC_UART_0_REGS, intFlags);

    return;
}

unsigned int UART0putCharNonBlocking(unsigned char byteToWrite)
{
    return UARTCharPutNonBlocking(SOC_UART_0_REGS, byteToWrite);
}

int UART0getCharNonBlocking()
{
    return UARTCharGetNonBlocking(SOC_UART_0_REGS);
}

int UART0getChar()
{
    return UARTCharGet(SOC_UART_0_REGS);
}

void UART0putChar(unsigned char byteToWrite)
{
    UARTCharPut(SOC_UART_0_REGS, byteToWrite);
    return;
}

/*
** \brief   Interrupt Service Routine(ISR) to be executed on UART interrupts.
**          Depending on the source of interrupt, this
**          1> writes to the serial communication console, or
**          2> reads from the serial communication console, or
**          3> reads the byte in RBR if receiver line error has occured.
*/
static void UART0Isr()
    {
    static unsigned int length = sizeof(txArray);
    static unsigned int count = 0;
    unsigned char rxData = 0;
    unsigned int int_id = 0;

    /* This determines the cause of UART1 interrupt.*/
    int_id = UARTIntStatus(SOC_UART_0_REGS);

    #ifdef _TMS320C6X
    // Clear UART1 system interrupt in DSPINTC
    IntEventClear(SYS_INT_UART0_INT);
    #else
    /* Clears the system interupt status of UART1 in AINTC. */
    IntSystemStatusClear(SYS_INT_UARTINT0);
    #endif

    /* Checked if the cause is transmitter empty condition.*/
    if(UART_INTID_TX_EMPTY == int_id)
    {
        if(0 < length)
        {
            /* Write a byte into the THR if THR is free. */
            UARTCharPutNonBlocking(SOC_UART_0_REGS, txArray[count]);
            length--;
            count++;
        }
        if(0 == length)
        {
            /* Disable the Transmitter interrupt in UART.*/
            UARTIntDisable(SOC_UART_0_REGS, UART_INT_TX_EMPTY);
        }
     }

    /* Check if the cause is receiver data condition.*/
    if(UART_INTID_RX_DATA == int_id)
    {
        rxData = UARTCharGetNonBlocking(SOC_UART_0_REGS);
        UARTCharPutNonBlocking(SOC_UART_0_REGS, rxData);
    }


    /* Check if the cause is receiver line error condition.*/
    if(UART_INTID_RX_LINE_STAT == int_id)
    {
        while(UARTRxErrorGet(SOC_UART_0_REGS))
        {
            /* Read a byte from the RBR if RBR has data.*/
            UARTCharGetNonBlocking(SOC_UART_0_REGS);
        }
    }

    return;
}

/*
** \brief   This function invokes necessary functions to configure the ARM
**          processor and ARM Interrupt Controller(AINTC) to receive and
**          handle interrupts.
*/
static void SetupInt0(void)
{
    #ifdef _TMS320C6X
    // Initialize the DSP INTC
    IntDSPINTCInit();

    // Enable DSP interrupts globally
    IntGlobalEnable();
    #else
    /* Initialize the ARM Interrupt Controller(AINTC). */
    IntAINTCInit();

    /* Enable IRQ in CPSR.*/
    IntMasterIRQEnable();

    /* Enable the interrupts in GER of AINTC.*/
    IntGlobalEnable();

    /* Enable the interrupts in HIER of AINTC.*/
    IntIRQEnable();
    #endif
}

/*
** \brief  This function configures the AINTC to receive UART interrupts.
*/
static void ConfigureIntUART0(void)
{
    #ifdef _TMS320C6X
    IntRegister(C674X_MASK_INT6, UART0Isr);
    IntEventMap(C674X_MASK_INT6, SYS_INT_UART0_INT);
    IntEnable(C674X_MASK_INT6);
    #else
    /* Registers the UARTIsr in the Interrupt Vector Table of AINTC. */
    IntRegister(SYS_INT_UARTINT0, UART0Isr);

    /* Map the channel number 0 of AINTC to UART0 system interrupt. */
    IntChannelSet(SYS_INT_UARTINT0, 0);

    IntSystemEnable(SYS_INT_UARTINT0);
    #endif
}

//--------------------------------- End of J15 -------------------------------------//



/*********************************** J15: PWM **************************************/
/**************************** PWM and Servo Control ********************************/

#define CLOCK_DIV_VAL       10

#define PULSE_MIN 0.65
#define PULSE_MAX 2.4

/* Pin Multiplexing bit mask to select EPWM0A pin */
#define PINMUX3_EPWM0A_ENABLE    (SYSCFG_PINMUX3_PINMUX3_3_0_EPWM0A << \
                    SYSCFG_PINMUX3_PINMUX3_3_0_SHIFT)

/* Pin Multiplexing bit mask to select EPWM0B pin. */
#define PINMUX3_EPWM0B_ENABLE    (SYSCFG_PINMUX3_PINMUX3_7_4_EPWM0B << \
                    SYSCFG_PINMUX3_PINMUX3_7_4_SHIFT)

/* Pin Multiplexing bit mask to select EPWM0TZ0 pin. */
#define PINMUX1_EPWM0TZ0_ENABLE (SYSCFG_PINMUX1_PINMUX1_3_0_EPWM0TZ0 << \
                    SYSCFG_PINMUX1_PINMUX1_3_0_SHIFT)

/* Pin Multiplexing bit mask to select EPWM1A pin. */
#define PINMUX5_EPWM1A_ENABLE   (SYSCFG_PINMUX5_PINMUX5_3_0_EPWM1A << \
                    SYSCFG_PINMUX5_PINMUX5_3_0_SHIFT)

/* Pin Multiplexing bit mask to select EPWM1B pin. */
#define PINMUX5_EPWM1B_ENABLE   (SYSCFG_PINMUX5_PINMUX5_7_4_EPWM1B << \
                    SYSCFG_PINMUX5_PINMUX5_7_4_SHIFT)

/* Pin Multiplexing bit mask to select EPWM1TZ0 pin. */
#define PINMUX2_EPWM1TZ0_ENABLE (SYSCFG_PINMUX2_PINMUX2_3_0_EPWM1TZ0 << \
                    SYSCFG_PINMUX2_PINMUX2_3_0_SHIFT)

/* Pin Multiplexing bit mask to select EPWMSYNCO pin. */
#define PINMUX3_EPWMSYNCO_ENABLE (SYSCFG_PINMUX3_PINMUX3_15_12_EPWMSYNCO << \
                    SYSCFG_PINMUX3_PINMUX3_15_12_SHIFT)

/* Pin Multiplexing bit mask to select EPWMSYNCI pin. */
#define PINMUX3_EPWMSYNCI_ENABLE (SYSCFG_PINMUX3_PINMUX3_11_8_EPWMSYNCI << \
                    SYSCFG_PINMUX3_PINMUX3_11_8_SHIFT)

static void SetupIntc(void);
static void PWMEventIsr(void);
static void PWMTZIsr(void);

volatile bool PWM1_Servo_Status;
volatile int PWM1_Duty_Cycle_Or_Angle;

volatile bool PWM2_Servo_Status;
volatile int PWM2_Duty_Cycle_Or_Angle;


void PWMandServoInit()
{
    //SetupIntc();

    PSCModuleControl(SOC_PSC_1_REGS, HW_PSC_EHRPWM, PSC_POWERDOMAIN_ALWAYS_ON,
             PSC_MDCTL_NEXT_ENABLE);

    /* EPWM1A */
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(5)) =
            (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(5)) & (~(SYSCFG_PINMUX5_PINMUX5_3_0))) |
            (PINMUX5_EPWM1A_ENABLE);

    /* EPWM1B */
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(5)) =
            (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(5)) & (~(SYSCFG_PINMUX5_PINMUX5_7_4))) |
            (PINMUX5_EPWM1B_ENABLE);

    /* EPWM1TZ[0] */
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(2)) =
            (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(2)) & (~(SYSCFG_PINMUX2_PINMUX2_3_0))) |
            (PINMUX2_EPWM1TZ0_ENABLE);


    /* EPWMSYNCO */
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(3)) =
            (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(3)) & (~(SYSCFG_PINMUX3_PINMUX3_15_12))) |
            (PINMUX3_EPWMSYNCO_ENABLE);

    /* EPWMSYNCI */
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(3)) =
            (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(3)) & (~(SYSCFG_PINMUX3_PINMUX3_11_8))) |
            (PINMUX3_EPWMSYNCI_ENABLE);

    /* Enable PWM Clock in chip config reg 1 */
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_CFGCHIP1) |= SYSCFG_CFGCHIP1_TBCLKSYNC;

    return;
}


void PWMset(bool isServo)
{
    ///////////// For Pin A //////////////
    PWM1_Duty_Cycle_Or_Angle = 1;
    PWM1_Servo_Status = isServo;

    if(!PWM1_Servo_Status)
    {

        /* TimeBase configuration */
        /* Configure the clock frequency */
        EHRPWMTimebaseClkConfig(SOC_EHRPWM_1_REGS, SOC_EHRPWM_1_MODULE_FREQ/CLOCK_DIV_VAL , SOC_EHRPWM_1_MODULE_FREQ);

        /* Configure the period of the output waveform */
        EHRPWMPWMOpFreqSet(SOC_EHRPWM_1_REGS, SOC_EHRPWM_1_MODULE_FREQ/CLOCK_DIV_VAL,
                (SOC_EHRPWM_1_MODULE_FREQ/CLOCK_DIV_VAL)/0xFF, EHRPWM_COUNT_UP, EHRPWM_SHADOW_WRITE_DISABLE);

        /* Disable synchronization*/
        EHRPWMTimebaseSyncDisable(SOC_EHRPWM_1_REGS);

        /* Disable syncout*/
        EHRPWMSyncOutModeSet(SOC_EHRPWM_1_REGS, EHRPWM_SYNCOUT_DISABLE);

        /* Configure the emulation behaviour*/
        EHRPWMTBEmulationModeSet(SOC_EHRPWM_1_REGS, EHRPWM_STOP_AFTER_NEXT_TB_INCREMENT);

        /* Configure Counter compare cub-module */
        /* Load Compare A value */
        EHRPWMLoadCMPA(SOC_EHRPWM_1_REGS, PWM1_Duty_Cycle_Or_Angle, EHRPWM_SHADOW_WRITE_DISABLE,
               EHRPWM_COMPA_NO_LOAD, EHRPWM_CMPCTL_OVERWR_SH_FL);

        /* Load Compare B value */
        EHRPWMLoadCMPB(SOC_EHRPWM_1_REGS, 0, EHRPWM_SHADOW_WRITE_DISABLE,
               EHRPWM_COMPB_NO_LOAD, EHRPWM_CMPCTL_OVERWR_SH_FL);

        /* Configure Action qualifier */
        /* Toggle when CTR = CMPA OR CMPB */
        EHRPWMConfigureAQActionOnA(SOC_EHRPWM_1_REGS, EHRPWM_AQCTLA_ZRO_EPWMXAHIGH, EHRPWM_AQCTLA_PRD_DONOTHING,
            EHRPWM_AQCTLA_CAU_EPWMXALOW,  EHRPWM_AQCTLA_CAD_DONOTHING,  EHRPWM_AQCTLA_CBU_DONOTHING,
            EHRPWM_AQCTLA_CBD_DONOTHING, EHRPWM_AQSFRC_ACTSFA_DONOTHING);

        /* Bypass dead band sub-module */
        EHRPWMDBOutput(SOC_EHRPWM_1_REGS, EHRPWM_DBCTL_OUT_MODE_BYPASS);

        /* Disable Chopper sub-module */
        EHRPWMChopperDisable(SOC_EHRPWM_1_REGS);

        /* Disable trip events */
        EHRPWMTZTripEventDisable(SOC_EHRPWM_1_REGS, EHRPWM_TZ_ONESHOT);
        EHRPWMTZTripEventDisable(SOC_EHRPWM_1_REGS, EHRPWM_TZ_CYCLEBYCYCLE);

        //    /* Event trigger */
        //    /* Generate interrupt every 3rd occurance of the event */
        //    EHRPWMETIntPrescale(SOC_EHRPWM_1_REGS, EHRPWM_ETPS_INTPRD_THIRDEVENT);
        //    /* Generate event when CTR = CMPB */
        //    EHRPWMETIntSourceSelect(SOC_EHRPWM_1_REGS, EHRPWM_ETSEL_INTSEL_TBCTREQUCMPBINC);
        //    /* Enable interrupt */
        //    EHRPWMETIntEnable(SOC_EHRPWM_1_REGS);

        //    /* Disable High resolution capability */
        //    EHRPWMHRDisable(SOC_EHRPWM_1_REGS);
    }
    else
    {
        /* TimeBase configuration */
        /* Configure the clock frequency */
        EHRPWMTimebaseClkConfig(SOC_EHRPWM_1_REGS, SOC_EHRPWM_1_MODULE_FREQ/1792, SOC_EHRPWM_1_MODULE_FREQ);

        /* Configure the period of the output waveform */
        EHRPWMPWMOpFreqSet(SOC_EHRPWM_1_REGS, SOC_EHRPWM_1_MODULE_FREQ/1792,
                (SOC_EHRPWM_1_MODULE_FREQ/1792)/1674, EHRPWM_COUNT_UP, EHRPWM_SHADOW_WRITE_DISABLE);

        /* Disable synchronization*/
        EHRPWMTimebaseSyncDisable(SOC_EHRPWM_1_REGS);

        /* Disable syncout*/
        EHRPWMSyncOutModeSet(SOC_EHRPWM_1_REGS, EHRPWM_SYNCOUT_DISABLE);

        /* Configure the emulation behaviour*/
        EHRPWMTBEmulationModeSet(SOC_EHRPWM_1_REGS, EHRPWM_STOP_AFTER_NEXT_TB_INCREMENT);

        /* Configure Counter compare cub-module */
        /* Load Compare A value */
        EHRPWMLoadCMPA(SOC_EHRPWM_1_REGS, PWM1_Duty_Cycle_Or_Angle, EHRPWM_SHADOW_WRITE_DISABLE,
               EHRPWM_COMPA_NO_LOAD, EHRPWM_CMPCTL_OVERWR_SH_FL);

        /* Load Compare B value */
        EHRPWMLoadCMPB(SOC_EHRPWM_1_REGS, 0, EHRPWM_SHADOW_WRITE_DISABLE,
               EHRPWM_COMPB_NO_LOAD, EHRPWM_CMPCTL_OVERWR_SH_FL);

        /* Configure Action qualifier */
        EHRPWMConfigureAQActionOnA(SOC_EHRPWM_1_REGS, EHRPWM_AQCTLA_ZRO_EPWMXAHIGH, EHRPWM_AQCTLA_PRD_DONOTHING,
            EHRPWM_AQCTLA_CAU_EPWMXALOW,  EHRPWM_AQCTLA_CAD_DONOTHING,  EHRPWM_AQCTLA_CBU_DONOTHING,
            EHRPWM_AQCTLA_CBD_DONOTHING, EHRPWM_AQSFRC_ACTSFA_DONOTHING);

        /* Bypass dead band sub-module */
        EHRPWMDBOutput(SOC_EHRPWM_1_REGS, EHRPWM_DBCTL_OUT_MODE_BYPASS);

        /* Disable Chopper sub-module */
        EHRPWMChopperDisable(SOC_EHRPWM_1_REGS);

        /* Disable trip events */
        EHRPWMTZTripEventDisable(SOC_EHRPWM_1_REGS, EHRPWM_TZ_ONESHOT);
        EHRPWMTZTripEventDisable(SOC_EHRPWM_1_REGS, EHRPWM_TZ_CYCLEBYCYCLE);

        //    /* Event trigger */
        //    /* Generate interrupt every 3rd occurance of the event */
        //    EHRPWMETIntPrescale(SOC_EHRPWM_1_REGS, EHRPWM_ETPS_INTPRD_THIRDEVENT);
        //    /* Generate event when CTR = CMPB */
        //    EHRPWMETIntSourceSelect(SOC_EHRPWM_1_REGS, EHRPWM_ETSEL_INTSEL_TBCTREQUCMPBINC);
        //    /* Enable interrupt */
        //    EHRPWMETIntEnable(SOC_EHRPWM_1_REGS);

        //    /* Disable High resolution capability */
        //    EHRPWMHRDisable(SOC_EHRPWM_1_REGS);
    }

    //////// For Pin B //////////
    PWM2_Servo_Status = isServo;
    PWM2_Duty_Cycle_Or_Angle = 1;

    if(!PWM1_Servo_Status)
    {

        /* Load Compare B value */
        EHRPWMLoadCMPB(SOC_EHRPWM_1_REGS, PWM2_Duty_Cycle_Or_Angle, EHRPWM_SHADOW_WRITE_DISABLE,
               EHRPWM_COMPB_NO_LOAD, EHRPWM_CMPCTL_OVERWR_SH_FL);

        /* Configure Action qualifier */
        EHRPWMConfigureAQActionOnB(SOC_EHRPWM_1_REGS, EHRPWM_AQCTLB_ZRO_EPWMXBHIGH, EHRPWM_AQCTLB_PRD_DONOTHING,
            EHRPWM_AQCTLB_CAU_DONOTHING,  EHRPWM_AQCTLB_CAD_DONOTHING,  EHRPWM_AQCTLB_CBU_EPWMXBLOW,
            EHRPWM_AQCTLB_CBD_DONOTHING, EHRPWM_AQSFRC_ACTSFB_DONOTHING);

    }
    else
    {
        /* Load Compare B value */
        EHRPWMLoadCMPB(SOC_EHRPWM_1_REGS, PWM2_Duty_Cycle_Or_Angle, EHRPWM_SHADOW_WRITE_DISABLE,
               EHRPWM_COMPB_NO_LOAD, EHRPWM_CMPCTL_OVERWR_SH_FL);

        /* Configure Action qualifier */
        EHRPWMConfigureAQActionOnB(SOC_EHRPWM_1_REGS, EHRPWM_AQCTLB_ZRO_EPWMXBHIGH, EHRPWM_AQCTLB_PRD_DONOTHING,
            EHRPWM_AQCTLB_CAU_DONOTHING,  EHRPWM_AQCTLB_CAD_DONOTHING,  EHRPWM_AQCTLB_CBU_EPWMXBLOW,
            EHRPWM_AQCTLB_CBD_DONOTHING, EHRPWM_AQSFRC_ACTSFB_DONOTHING);

    }

    return;
}

/* Writes a PWM value to PWM pin 1A that ranges between 0 and 255 */
void PWM1write(int value)
{
    if(!PWM1_Servo_Status) // If the PWM was set to a generic PWM (58.8kHz)
    {
        if(value <= 0)
        {
            PWM1_Duty_Cycle_Or_Angle = 0;

            EHRPWMConfigureAQActionOnA(SOC_EHRPWM_1_REGS, EHRPWM_AQCTLA_ZRO_EPWMXALOW, EHRPWM_AQCTLA_PRD_DONOTHING,
                        EHRPWM_AQCTLA_CAU_EPWMXALOW,  EHRPWM_AQCTLA_CAD_DONOTHING,  EHRPWM_AQCTLA_CBU_DONOTHING,
                        EHRPWM_AQCTLA_CBD_DONOTHING, EHRPWM_AQSFRC_ACTSFA_DONOTHING);

            EHRPWMLoadCMPA(SOC_EHRPWM_1_REGS, PWM1_Duty_Cycle_Or_Angle, EHRPWM_SHADOW_WRITE_DISABLE,
                           EHRPWM_COMPA_NO_LOAD, EHRPWM_CMPCTL_OVERWR_SH_FL);
        }
        else if (value >= 255)
        {
            PWM1_Duty_Cycle_Or_Angle = 255;

            EHRPWMConfigureAQActionOnA(SOC_EHRPWM_1_REGS, EHRPWM_AQCTLA_ZRO_EPWMXAHIGH, EHRPWM_AQCTLA_PRD_DONOTHING,
                        EHRPWM_AQCTLA_CAU_EPWMXAHIGH,  EHRPWM_AQCTLA_CAD_DONOTHING,  EHRPWM_AQCTLA_CBU_DONOTHING,
                        EHRPWM_AQCTLA_CBD_DONOTHING, EHRPWM_AQSFRC_ACTSFA_DONOTHING);

            EHRPWMLoadCMPA(SOC_EHRPWM_1_REGS, PWM1_Duty_Cycle_Or_Angle, EHRPWM_SHADOW_WRITE_DISABLE,
                           EHRPWM_COMPA_NO_LOAD, EHRPWM_CMPCTL_OVERWR_SH_FL);


        }
        else
        {
            PWM1_Duty_Cycle_Or_Angle = value;

            EHRPWMConfigureAQActionOnA(SOC_EHRPWM_1_REGS, EHRPWM_AQCTLA_ZRO_EPWMXAHIGH, EHRPWM_AQCTLA_PRD_DONOTHING,
                        EHRPWM_AQCTLA_CAU_EPWMXALOW,  EHRPWM_AQCTLA_CAD_DONOTHING,  EHRPWM_AQCTLA_CBU_DONOTHING,
                        EHRPWM_AQCTLA_CBD_DONOTHING, EHRPWM_AQSFRC_ACTSFA_DONOTHING);

            EHRPWMLoadCMPA(SOC_EHRPWM_1_REGS, PWM1_Duty_Cycle_Or_Angle, EHRPWM_SHADOW_WRITE_DISABLE,
                           EHRPWM_COMPA_NO_LOAD, EHRPWM_CMPCTL_OVERWR_SH_FL);

        }
    }
    else // If the PWM was set to use the servo frequency of 50Hz
    {
        float countValue = 0;
        
        if(value <= 0)
        {
            PWM1_Duty_Cycle_Or_Angle = 0;
            countValue = 0;

            EHRPWMConfigureAQActionOnA(SOC_EHRPWM_1_REGS, EHRPWM_AQCTLA_ZRO_EPWMXALOW, EHRPWM_AQCTLA_PRD_DONOTHING,
                        EHRPWM_AQCTLA_CAU_EPWMXALOW,  EHRPWM_AQCTLA_CAD_DONOTHING,  EHRPWM_AQCTLA_CBU_DONOTHING,
                        EHRPWM_AQCTLA_CBD_DONOTHING, EHRPWM_AQSFRC_ACTSFA_DONOTHING);

            EHRPWMLoadCMPA(SOC_EHRPWM_1_REGS, countValue, EHRPWM_SHADOW_WRITE_DISABLE,
                           EHRPWM_COMPA_NO_LOAD, EHRPWM_CMPCTL_OVERWR_SH_FL);
        }
        else if (value >= 255)
        {
            PWM1_Duty_Cycle_Or_Angle = 255;
            countValue = 1674;

            EHRPWMConfigureAQActionOnA(SOC_EHRPWM_1_REGS, EHRPWM_AQCTLA_ZRO_EPWMXAHIGH, EHRPWM_AQCTLA_PRD_DONOTHING,
                        EHRPWM_AQCTLA_CAU_EPWMXAHIGH,  EHRPWM_AQCTLA_CAD_DONOTHING,  EHRPWM_AQCTLA_CBU_DONOTHING,
                        EHRPWM_AQCTLA_CBD_DONOTHING, EHRPWM_AQSFRC_ACTSFA_DONOTHING);

            EHRPWMLoadCMPA(SOC_EHRPWM_1_REGS, countValue, EHRPWM_SHADOW_WRITE_DISABLE,
                           EHRPWM_COMPA_NO_LOAD, EHRPWM_CMPCTL_OVERWR_SH_FL);


        }
        else
        {
            PWM1_Duty_Cycle_Or_Angle = value;
            countValue = 1.0*PWM1_Duty_Cycle_Or_Angle/255*1674;

            EHRPWMConfigureAQActionOnA(SOC_EHRPWM_1_REGS, EHRPWM_AQCTLA_ZRO_EPWMXAHIGH, EHRPWM_AQCTLA_PRD_DONOTHING,
                        EHRPWM_AQCTLA_CAU_EPWMXALOW,  EHRPWM_AQCTLA_CAD_DONOTHING,  EHRPWM_AQCTLA_CBU_DONOTHING,
                        EHRPWM_AQCTLA_CBD_DONOTHING, EHRPWM_AQSFRC_ACTSFA_DONOTHING);

            EHRPWMLoadCMPA(SOC_EHRPWM_1_REGS, countValue, EHRPWM_SHADOW_WRITE_DISABLE,
                           EHRPWM_COMPA_NO_LOAD, EHRPWM_CMPCTL_OVERWR_SH_FL);

        }
    }

    return;
}

int PWM1getValue()
{
    return PWM1_Duty_Cycle_Or_Angle;
}

/* Writes a value to a servo on PWM pin 1A between 0 and 180 degrees */
void Servo1write(int degrees)
{
    if(degrees < 0)
    {
        PWM1_Duty_Cycle_Or_Angle = 0;
    }
    else if(degrees > 180)
    {
        PWM1_Duty_Cycle_Or_Angle = 180;
    }
    else
    {
        PWM1_Duty_Cycle_Or_Angle = degrees;
    }
    float countValue = PULSE_MIN + PWM1_Duty_Cycle_Or_Angle*(PULSE_MAX-PULSE_MIN)/(180-0);

    EHRPWMLoadCMPA(SOC_EHRPWM_1_REGS, countValue/20*1674, EHRPWM_SHADOW_WRITE_DISABLE,
               EHRPWM_COMPA_NO_LOAD, EHRPWM_CMPCTL_OVERWR_SH_FL);

    return;
}

int Servo1getAngle()
{
    return PWM1_Duty_Cycle_Or_Angle;
}

/* Writes a PWM value to PWM pin 1B that ranges between 0 and 255 */
void PWM2write(int value)
{
    if (!PWM2_Servo_Status)
    {
        if(value <= 0)
        {
            PWM2_Duty_Cycle_Or_Angle = 0;

            EHRPWMConfigureAQActionOnB(SOC_EHRPWM_1_REGS, EHRPWM_AQCTLB_ZRO_EPWMXBLOW, EHRPWM_AQCTLB_PRD_DONOTHING,
                        EHRPWM_AQCTLB_CAU_DONOTHING,  EHRPWM_AQCTLB_CAD_DONOTHING,  EHRPWM_AQCTLB_CBU_EPWMXBLOW,
                        EHRPWM_AQCTLB_CBD_DONOTHING, EHRPWM_AQSFRC_ACTSFB_DONOTHING);

            EHRPWMLoadCMPB(SOC_EHRPWM_1_REGS, PWM2_Duty_Cycle_Or_Angle, EHRPWM_SHADOW_WRITE_DISABLE,
                           EHRPWM_COMPB_NO_LOAD, EHRPWM_CMPCTL_OVERWR_SH_FL);
        }
        else if (value >= 255)
        {
            PWM2_Duty_Cycle_Or_Angle = 255;

            EHRPWMConfigureAQActionOnB(SOC_EHRPWM_1_REGS, EHRPWM_AQCTLB_ZRO_EPWMXBHIGH, EHRPWM_AQCTLB_PRD_DONOTHING,
                        EHRPWM_AQCTLB_CAU_DONOTHING,  EHRPWM_AQCTLB_CAD_DONOTHING,  EHRPWM_AQCTLB_CBU_EPWMXBHIGH,
                        EHRPWM_AQCTLB_CBD_DONOTHING, EHRPWM_AQSFRC_ACTSFB_DONOTHING);

            EHRPWMLoadCMPB(SOC_EHRPWM_1_REGS, PWM2_Duty_Cycle_Or_Angle, EHRPWM_SHADOW_WRITE_DISABLE,
                           EHRPWM_COMPB_NO_LOAD, EHRPWM_CMPCTL_OVERWR_SH_FL);


        }
        else
        {
            PWM2_Duty_Cycle_Or_Angle = value;

            EHRPWMConfigureAQActionOnB(SOC_EHRPWM_1_REGS, EHRPWM_AQCTLB_ZRO_EPWMXBHIGH, EHRPWM_AQCTLB_PRD_DONOTHING,
                        EHRPWM_AQCTLB_CAU_DONOTHING,  EHRPWM_AQCTLB_CAD_DONOTHING,  EHRPWM_AQCTLB_CBU_EPWMXBLOW,
                        EHRPWM_AQCTLB_CBD_DONOTHING, EHRPWM_AQSFRC_ACTSFB_DONOTHING);

            EHRPWMLoadCMPB(SOC_EHRPWM_1_REGS, PWM2_Duty_Cycle_Or_Angle, EHRPWM_SHADOW_WRITE_DISABLE,
                           EHRPWM_COMPB_NO_LOAD, EHRPWM_CMPCTL_OVERWR_SH_FL);

        }
    }
    else
    {
        float countValue;
        
        if(value <= 0)
        {
            PWM2_Duty_Cycle_Or_Angle = 0;
            countValue = 0;

            EHRPWMConfigureAQActionOnB(SOC_EHRPWM_1_REGS, EHRPWM_AQCTLB_ZRO_EPWMXBLOW, EHRPWM_AQCTLB_PRD_DONOTHING,
                        EHRPWM_AQCTLB_CAU_DONOTHING,  EHRPWM_AQCTLB_CAD_DONOTHING,  EHRPWM_AQCTLB_CBU_EPWMXBLOW,
                        EHRPWM_AQCTLB_CBD_DONOTHING, EHRPWM_AQSFRC_ACTSFB_DONOTHING);

            EHRPWMLoadCMPB(SOC_EHRPWM_1_REGS, countValue, EHRPWM_SHADOW_WRITE_DISABLE,
                           EHRPWM_COMPB_NO_LOAD, EHRPWM_CMPCTL_OVERWR_SH_FL);
        }
        else if (value >= 255)
        {
            PWM2_Duty_Cycle_Or_Angle = 255;
            countValue = 1679;

            EHRPWMConfigureAQActionOnB(SOC_EHRPWM_1_REGS, EHRPWM_AQCTLB_ZRO_EPWMXBHIGH, EHRPWM_AQCTLB_PRD_DONOTHING,
                        EHRPWM_AQCTLB_CAU_DONOTHING,  EHRPWM_AQCTLB_CAD_DONOTHING,  EHRPWM_AQCTLB_CBU_EPWMXBHIGH,
                        EHRPWM_AQCTLB_CBD_DONOTHING, EHRPWM_AQSFRC_ACTSFB_DONOTHING);

            EHRPWMLoadCMPB(SOC_EHRPWM_1_REGS, countValue, EHRPWM_SHADOW_WRITE_DISABLE,
                           EHRPWM_COMPB_NO_LOAD, EHRPWM_CMPCTL_OVERWR_SH_FL);


        }
        else
        {
            PWM2_Duty_Cycle_Or_Angle = value;
            countValue = 1.0*PWM2_Duty_Cycle_Or_Angle/255*1679;

            EHRPWMConfigureAQActionOnB(SOC_EHRPWM_1_REGS, EHRPWM_AQCTLB_ZRO_EPWMXBHIGH, EHRPWM_AQCTLB_PRD_DONOTHING,
                        EHRPWM_AQCTLB_CAU_DONOTHING,  EHRPWM_AQCTLB_CAD_DONOTHING,  EHRPWM_AQCTLB_CBU_EPWMXBLOW,
                        EHRPWM_AQCTLB_CBD_DONOTHING, EHRPWM_AQSFRC_ACTSFB_DONOTHING);

            EHRPWMLoadCMPB(SOC_EHRPWM_1_REGS, countValue, EHRPWM_SHADOW_WRITE_DISABLE,
                           EHRPWM_COMPB_NO_LOAD, EHRPWM_CMPCTL_OVERWR_SH_FL);

        }
    }
        

    return;
}

int PWM2getValue()
{
    return PWM2_Duty_Cycle_Or_Angle;
}

/* Writes a value to a servo on PWM pin 1B between 0 and 180 degrees */
void Servo2write(int degrees)
{
    if(degrees < 0)
    {
        PWM2_Duty_Cycle_Or_Angle = 0;
    }
    else if(degrees > 180)
    {
        PWM2_Duty_Cycle_Or_Angle = 180;
    }
    else
    {
        PWM2_Duty_Cycle_Or_Angle = degrees;
    }
    float countValue = PULSE_MIN + PWM2_Duty_Cycle_Or_Angle*(PULSE_MAX-PULSE_MIN)/(180-0);

    EHRPWMLoadCMPB(SOC_EHRPWM_1_REGS, countValue/20*1674, EHRPWM_SHADOW_WRITE_DISABLE,
           EHRPWM_COMPB_NO_LOAD, EHRPWM_CMPCTL_OVERWR_SH_FL);

    return;
}

int Servo2getAngle()
{
    return PWM2_Duty_Cycle_Or_Angle;
}

/*
** configures arm interrupt controller to generate PWM interrupts
*/
static void SetupIntc(void)
{
#ifdef _TMS320C6X
    // Initialize the DSP interrupt controller
    IntDSPINTCInit();

    // Register the ISRs to the vector table
    IntRegister(C674X_MASK_INT7, PWMEventIsr);
    IntRegister(C674X_MASK_INT8, PWMTZIsr);

    // Map system events to the DSP maskable interrupts
    IntEventMap(C674X_MASK_INT7, SYS_INT_EHRPWM1);
    IntEventMap(C674X_MASK_INT8, SYS_INT_EHRPWM1TZ);

    // Enable the DSP maskable interrupts
    IntEnable(C674X_MASK_INT7);
    IntEnable(C674X_MASK_INT8);

    // Enable DSP interrupts globally
    IntGlobalEnable();
#else
    /* Initialize the ARM Interrupt Controller.*/
    IntAINTCInit();

    IntSystemStatusClear(SYS_INT_EHRPWM1);
    EHRPWMETIntClear(SOC_EHRPWM_1_REGS);

    /************************PWM1****************************************/
    IntRegister(SYS_INT_EHRPWM1, PWMEventIsr);
    IntChannelSet(SYS_INT_EHRPWM1, 2);
    IntSystemEnable(SYS_INT_EHRPWM1);
    /********************************************************************/
    IntRegister(SYS_INT_EHRPWM1TZ, PWMTZIsr);
    IntChannelSet(SYS_INT_EHRPWM1TZ, 2);
    IntSystemEnable(SYS_INT_EHRPWM1TZ);
    /********************************************************************/

    /* Enable IRQ in CPSR.*/
    IntMasterIRQEnable();

    /* Enable the interrupts in GER of AINTC.*/
    IntGlobalEnable();

    /* Enable the interrupts in HIER of AINTC.*/
    IntIRQEnable();
#endif
}

static void PWMEventIsr(void)
{
#ifdef _TMS320C6X
    IntEventClear(SYS_INT_EHRPWM1);
#else
    IntSystemStatusClear(SYS_INT_EHRPWM1);
#endif

    EHRPWMETIntClear(SOC_EHRPWM_1_REGS);

#ifdef SYNC_EN
    EHRPWMTriggerSWSync(SOC_EHRPWM_1_REGS);
#endif

}

static void PWMTZIsr(void)
{
#ifdef _TMS320C6X
    IntEventClear(SYS_INT_EHRPWM1TZ);
#else
    IntSystemStatusClear(SYS_INT_EHRPWM1TZ);
#endif

    EHRPWMTZFlagClear(SOC_EHRPWM_1_REGS, EHRPWM_TZ_CYCLEBYCYCLE_CLEAR);
}

//------------------------------ End of J15: PWM -----------------------------------//


//int main(void)
//{
//
//    GenPWM1A_Basic();
//
//    int blah = 0;
//    while(1)
//    {
//    blah++;
//    if (blah == 20000000)
//        break;
//    }
//    EHRPWMLoadCMPB(SOC_EHRPWM_1_REGS, 63, EHRPWM_SHADOW_WRITE_DISABLE,
//           EHRPWM_COMPB_NO_LOAD, EHRPWM_CMPCTL_OVERWR_SH_FL);
//    blah = 0;
//    while(1)
//    {
//    blah++;
//    if (blah == 20000000)
//        break;
//    }
//    /* Load Compare B value */
//    EHRPWMLoadCMPB(SOC_EHRPWM_1_REGS, 127, EHRPWM_SHADOW_WRITE_DISABLE,
//               EHRPWM_COMPB_NO_LOAD, EHRPWM_CMPCTL_OVERWR_SH_FL);
//    blah = 0;
//    while(1)
//    {
//    blah++;
//    if (blah == 20000000)
//        break;
//    }
//
//    EHRPWMLoadCMPB(SOC_EHRPWM_1_REGS, 190, EHRPWM_SHADOW_WRITE_DISABLE,
//                   EHRPWM_COMPB_NO_LOAD, EHRPWM_CMPCTL_OVERWR_SH_FL);
//    blah = 0;
//    while(1)
//    {
//    blah++;
//    if (blah == 20000000)
//        break;
//    }
//
//    EHRPWMLoadCMPB(SOC_EHRPWM_1_REGS, 255, EHRPWM_SHADOW_WRITE_DISABLE,
//                   EHRPWM_COMPB_NO_LOAD, EHRPWM_CMPCTL_OVERWR_SH_FL);
//    blah = 0;
//    while(1)
//    {
//    blah++;
//    if (blah == 20000000)
//        break;
//    }
//
//    while(1)
//    {;}
//}



