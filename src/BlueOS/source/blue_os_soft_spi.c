#include "blue_os_soft_spi.h"

#ifdef BLUE_OS_SOFT_SPI

#include <util/delay.h>
/*
		typedef struct _soft_spi_dev
		{
			uint8_t _port;
			uint8_t _pin;
			uint8_t _ddr;

			uint8_t _DI:3;
			uint8_t _DO:3;
			uint8_t _CL:3;
			uint8_t _CS:3;
			uint8_t active:1;
			
		} soft_spi_dev;

*/

int8_t blue_os_soft_spi_init(soft_spi_dev *myDev)
{
	uint8_t crit = blueOsEnterCriticalSection();
	DDRF &= ~(1<<myDev->_DI);
	DDRF |= (1<<myDev->_CL);
	DDRF |= (1<<myDev->_DO);
	DDRF |= (1<<myDev->_CS);
	
	PORTF |= (1<<myDev->_CS); //ChipSelect disable	
	PORTF &= ~(1<<myDev->_CL);
	PORTF &= ~(1<<myDev->_DO);
	myDev->active = 0;
	blueOsLeaveCriticalSection(crit);
	return 0;
}

int8_t blue_os_soft_spi_read(soft_spi_dev *myDev, uint8_t *data)
{
	volatile uint8_t crit;
	volatile uint8_t i;

	if(!myDev->active)
		return -1;

	crit = blueOsEnterCriticalSection();
	for(i=0;i<8;i++)
	{
		_delay_us(10);
		PORTF |= (1<<myDev->_CL);
		_delay_us(2);
		if(bit_is_set(PINF,myDev->_DI))
			*data |= (1 << (7-i));
		else
			*data &= ~(1 << (7-i));
		_delay_us(8);
		PORTF &= ~(1<<myDev->_CL);
		
	}
	blueOsLeaveCriticalSection(crit);
	return 0;
}

int8_t blue_os_soft_spi_write(soft_spi_dev *myDev, uint8_t *data)
{
	volatile uint8_t crit;
	volatile uint8_t i;

	if(!myDev->active)
		return -1;

	crit = blueOsEnterCriticalSection();
	for(i=0;i<8;i++)
	{
		if(bit_is_set(*data,(7-i)))
			PORTF |= (1<<myDev->_DO);
		else
			PORTF &= ~(1<<myDev->_DO);

		_delay_us(10);
		PORTF |= (1<<myDev->_CL);
		_delay_us(10);
		PORTF &= ~(1<<myDev->_CL);
	}
	myDev->_port &= ~(1<<myDev->_DO);
	blueOsLeaveCriticalSection(crit);
	
	return 0;
}

int8_t blue_os_soft_spi_enable(soft_spi_dev *myDev)
{
	PORTF &= ~(1<<myDev->_CS);
	myDev->active = 1;
	return 0;
}
int8_t blue_os_soft_spi_disable(soft_spi_dev *myDev)
{
	PORTF |= (1<<myDev->_CS);
	myDev->active = 0;
	return 0;
}

#endif
