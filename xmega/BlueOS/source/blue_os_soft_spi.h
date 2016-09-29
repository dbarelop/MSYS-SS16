
#ifndef BLUE_OS_SOFT_SPI_H
#define BLUE_OS_SOFT_SPI_H
	#include "blue_os.h"
	#ifdef BLUE_OS_SOFT_SPI	
		typedef struct _soft_spi_dev
		{
			uint32_t _port;
			uint32_t _pin;
			uint32_t _ddr;

			uint8_t _DI:3;
			uint8_t _DO:3;
			uint8_t _CL:3;
			uint8_t _CS:3;
			uint8_t active:1;
			
		} soft_spi_dev;

		int8_t blue_os_soft_spi_init(soft_spi_dev *myDev);
		int8_t blue_os_soft_spi_read(soft_spi_dev *myDev, uint8_t *data);
		int8_t blue_os_soft_spi_write(soft_spi_dev *myDev, uint8_t *data);
		int8_t blue_os_soft_spi_enable(soft_spi_dev *myDev);
		int8_t blue_os_soft_spi_disable(soft_spi_dev *myDev);


	#endif
#endif
