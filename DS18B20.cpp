/*
 * DS18B20.cpp
 *
 *  Created on: Jan 31, 2016
 *      Author: roland
 */

#include "DS18B20.h"

#include "Delay.h"

DS18B20::DS18B20(GPIO_TypeDef* gpio, uint16_t pin)
	: _gpio(gpio), _pin(pin)
{
	if(_gpio == GPIOA)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	else if(_gpio == GPIOB)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	else if(_gpio == GPIOC)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	else if(_gpio == GPIOD)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	else if(_gpio == GPIOE)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	else if(_gpio == GPIOF)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	else if(_gpio == GPIOG)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);

	GPIO_InitTypeDef init;
	init.GPIO_Mode = GPIO_Mode_Out_OD;
	init.GPIO_Pin = _pin;
	init.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(_gpio, &init);
}

float DS18B20::getTemperature()
{
	  reset();

	  send(0xcc);
	  send(0x44);
	  strongPullUp(750000);

	  reset();
	  send(0xcc);
	  send(0xbe);

	  uint8_t data[9];
	  for(int i = 0; i < 9; ++i)
		  data[i] = readByte();

	  uint16_t tempWord = data[1] << 8 | data[0];

	  return tempWord * 0.0625;
}

bool DS18B20::reset()
{
	GPIO_ResetBits(_gpio, _pin);
	delay(480);		//according do datasheet min time is 480us
	GPIO_SetBits(_gpio, _pin);

	delay(60);

	bool ret = !GPIO_ReadInputDataBit(_gpio, _pin);

	delay(480);

	return ret;
}

void DS18B20::writeZero()
{
	GPIO_ResetBits(_gpio, _pin);
	delay(60);
	GPIO_SetBits(_gpio, _pin);
	delay(1);
}

void DS18B20::writeOne()
{
	GPIO_ResetBits(_gpio, _pin);
	delay(5);
	GPIO_SetBits(_gpio, _pin);
	delay(56);
}

void DS18B20::send(uint8_t data)
{
	for(int i = 0; i < 8; ++i)
	{
		if(data & 0x01)
			writeOne();
		else
			writeZero();

		data >>= 1;
	}
}

bool DS18B20::readSlot()
{
	GPIO_ResetBits(_gpio, _pin);
	delay(5);
	GPIO_SetBits(_gpio, _pin);
	delay(10);

	bool ret = GPIO_ReadInputDataBit(_gpio, _pin);

	delay(46);

	return ret;
}

uint8_t DS18B20::readByte()
{
	uint8_t data = 0;

	for(int i = 0; i < 8; ++i)
	{
		if(readSlot())
			data |= 0x01 << i;
	}

	return data;
}

void DS18B20::strongPullUp(uint32_t us)
{
	GPIO_InitTypeDef init;
	init.GPIO_Mode = GPIO_Mode_Out_PP;
	init.GPIO_Pin = _pin;
	init.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(_gpio, &init);

	delay(us);

	init.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(_gpio, &init);
}
