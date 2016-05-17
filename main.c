/*
   This code is based on the BMP085 library by Stefan Sicklinger published on
   http://www.sicklinger.com
   
   I am redistributing this modified version under the GNU General Public
   License version 3
*/

#define F_CPU 16000000L

#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include "uart.h"
#include "i2c.h"
#include "bmp.h"

int main(void)
{
    int32_t temperature = 0;
    int32_t pressure = 0;
    uint8_t errorCode = 0;
    
    // Call setup functions
    uartSetup();
    i2cSetup();
    bmpCalibrate(&errorCode);
        
    while (1)
    {
        bmpComputePressureAndTemperature(&temperature, &pressure, &errorCode);
        
        printf("Temperature: %ld (in 0.1 degress Celcius)\n", temperature);
        printf("Pressure: %ld Pa\n", pressure);
        printf("Altitude: %ld dm\n", bmpComputeAltitude(pressure));
        printf("Global error code: %d\n\n", errorCode);
        
        _delay_ms(2000);
    }
    
    return 0;
}
