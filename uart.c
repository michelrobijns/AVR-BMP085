/*
   This code is based on the BMP085 library by Stefan Sicklinger published on
   http://www.sicklinger.com
   
   I am redistributing this modified version under the GNU General Public
   License version 3
*/

#ifndef F_CPU
#define F_CPU 16000000L
#endif

#define BAUD 19200UL
#define BRC ((F_CPU / 16 / BAUD) - 1)

#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include "uart.h"

static FILE stdoutUart = FDEV_SETUP_STREAM(uartPutChar, NULL, _FDEV_SETUP_WRITE);

void uartSetup(void)
{
    // Set baud rate
    UBRR0H = (BRC >> 8);
    UBRR0L = BRC;
    
    // Enable the USART Receiver and Transmitter
    UCSR0B |= (1 << RXEN0);
    UCSR0B |= (1 << TXEN0);
    
    // Enable the RX Complete Interrupt and the TX Complete Interrupt
    //UCSR0B |= (1 << RXCIE0);
    //UCSR0B |= (1 << TXCIE0);
    
    // Set the Character Size to 8 bits. There is one stop bit, which is
    // the default setting.
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
    
    stdout = &stdoutUart;
}

void uartSendChar(unsigned char c)
{
    // Wait until able to send
    while (!(UCSR0A & (1 << UDRE0)))
    {
        ;
    }
    
    // Send character
    UDR0 = c;
}

void uartSendString (char *c)
{
    // While *s != '\0'
    while (*c)
    {
        uartSendChar(*c);
        c++;
    }
}

int uartPutChar(char c, FILE *stream)
{
    if (c == '\n')
    {
        uartPutChar('\r', stream);
    }
    
    // Wait until able to send
    while (!(UCSR0A & (1 << UDRE0)))
    {
        ;
    }
    
    UDR0 = c;
    
    return 0;
}
