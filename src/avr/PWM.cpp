#include "PWM.h"

#include <Arduino.h>
#include <Wire.h>

#define PROP_CLKSPEED_MHZ 80
#define PROP_CYCLES_USEC 80

Protean::PWM::PWM()
{
	this->addr = PWM_DEF_ADDR;
}


Protean::PWM::PWM(int addr)
{
	this->addr = addr;
}


bool Protean::PWM::readTimings(uint8_t start_channel, int count, int timings[])
{
	Wire.beginTransmission(addr);
	Wire.write(start_channel);
	if(!Wire.endTransmission())
	{
		return false;
	}

	if(!Wire.requestFrom(addr, count))
	{
		return false;
	}

	for(int i = 0; i < count; ++i)
	{
		// wait for the next byte
		while(Wire.available() <= 0);
		timings[i] = (Wire.read() << 10) / PROP_CYCLES_USEC;
	}

	return true;
}


int Protean::PWM::readTiming(uint8_t channel)
{
	int timing = 0;
	if(readTimings(channel, 1, &timing))
	{
		return timing;
	}

	return -1;
}


bool Protean::PWM::writeTiming(uint8_t channel, int timing)
{
	writeTimings(channel, 1, &timing);
}


bool Protean::PWM::writeTimings(uint8_t start_channel, int count, int timings[])
{

	Wire.beginTransmission(addr);
	Wire.write(start_channel);
	if(!Wire.endTransmission())
	{
		return false;
	}

	Wire.beginTransmission(addr);

	for(int i = 0; i < count; ++i)
	{
		// Convert from microseconds to cycles with reduced fidelity
		uint8_t b = (PROP_CYCLES_USEC * timings[i]) >> 10;
		Wire.write(b);
	}

	if(!Wire.endTransmission())
	{
		return false;
	}

	return true;
}


bool Protean::PWM::setEcho(bool on)
{
	Wire.beginTransmission(addr);
	Wire.write(0x0);
	if(!Wire.endTransmission())
	{
		return false;
	}

	Wire.beginTransmission(addr);
	Wire.write(on ? 1 : 0);
	if(!Wire.endTransmission())
	{
		return false;
	}

	return true;
}


bool Protean::PWM::reset()
{
	Wire.beginTransmission(addr);
	Wire.write(PWM_REBOOT_REG);
	if(!Wire.endTransmission())
	{
		return false;
	}

	return true;
}


bool Protean::PWM::checkVersion()
{
	Wire.beginTransmission(addr);
	Wire.write(PWM_VERSION_REG);
	if(!Wire.endTransmission())
	{
		return false;
	}

	return true;

	if(!Wire.requestFrom(addr, 1))
	{
		return false;
	}

	// wait for the data
	while(Wire.available() <= 0);
	return Wire.read() == PWM_FIRMWARE_VERSION;
}
