/*
 * DS18B20.h
 *
 *  Created on: Jan 31, 2016
 *      Author: roland
 */

#ifndef DS18B20_H_
#define DS18B20_H_

#include <stm32f10x.h>

#include <inttypes.h>

class DS18B20
{
public:
	DS18B20(GPIO_TypeDef* gpio, uint16_t pin);

	float getTemperature();

	bool reset();

	void send(uint8_t data);
	uint8_t readByte();
	void strongPullUp(uint32_t us);

private:
	void writeZero();
	void writeOne();

	bool readSlot();


	GPIO_TypeDef* _gpio;
	uint16_t _pin;
};


#endif /* DS18B20_H_ */
