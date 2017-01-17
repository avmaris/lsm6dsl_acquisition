//-----------------------------------------------------------------------------
// spi.h - common function declarations for different SPI implementations
//-----------------------------------------------------------------------------
#ifndef SPI_H_
#define SPI_H_

void spi_initialize(void);
uint8_t spi_send(const uint8_t);
uint8_t spi_receive(void);
uint8_t spi_waitRX();
uint8_t spi_waitTX();
uint8_t spi_wait();
#endif /*SPI_H_*/
