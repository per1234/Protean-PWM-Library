//
// This file is part of the Protean_PWM_Library
//
// Protean_PWM_Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU  Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU  Lesser General Public License for more details.
//
// You should have received a copy of the GNU  Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// Author: Kirk Roerig <mr.possoms@gmail.com>
// Date: Aug 2, 2017

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


bool Protean::PWM::readTimings(uint8_t start_channel, int count, unsigned int timings[])
{
	Wire.beginTransmission(addr);
	Wire.write(start_channel);
	if(Wire.endTransmission())
	{
		return false;
	}

	if(!Wire.requestFrom(addr, count))
	{
		return false;
	}

	for(int i = 0; i < count && Wire.available(); ++i)
	{
		uint32_t t = (Wire.read() << 10) / PROP_CYCLES_USEC;
		timings[i] = (unsigned int)t;
	}

	return true;
}


bool Protean::PWM::readTimingsRaw(uint8_t start_channel, int count, uint8_t timings[])
{
	Wire.beginTransmission(addr);
	Wire.write(start_channel);
	if(Wire.endTransmission())
	{
		return false;
	}

	if(!Wire.requestFrom(addr, count))
	{
		return false;
	}

	for(int i = 0; i < count && Wire.available(); ++i)
	{
		timings[i] = Wire.read();
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


int Protean::PWM::readTimingRaw(uint8_t channel)
{
	uint8_t timing = 0;
	if(readTimingsRaw(channel, 1, &timing))
	{
		return timing;
	}

	return -1;
}


bool Protean::PWM::writeTiming(uint8_t channel, int timing)
{
	return writeTimings(channel, 1, &timing);
}


bool Protean::PWM::writeTimingRaw(uint8_t channel, uint8_t timing)
{
	return writeTimingsRaw(channel, 1, &timing);
}


bool Protean::PWM::writeTimings(uint8_t start_channel, int count, unsigned int timings[])
{

	Wire.beginTransmission(addr);
	Wire.write(start_channel);
	for(int i = 0; i < count; ++i)
	{
		// Convert from microseconds to cycles with reduced fidelity
		uint32_t t = timings[i] * PROP_CYCLES_USEC;
		uint8_t b = t >> 10;
		Wire.write(b);
	}

	if(Wire.endTransmission())
	{
		return false;
	}

	return true;
}


bool Protean::PWM::writeTimingsRaw(uint8_t start_channel, int count, uint8_t timings[])
{

	Wire.beginTransmission(addr);
	Wire.write(start_channel);
	for(int i = 0; i < count; ++i)
	{
		Wire.write(timings[i]);
	}

	if(Wire.endTransmission())
	{
		return false;
	}

	return true;
}


bool Protean::PWM::setEcho(bool on)
{
	Wire.beginTransmission(addr);
	Wire.write(0x0);
	Wire.write(on ? 1 : 0);
	if(Wire.endTransmission())
	{
		return false;
	}

	return true;
}


bool Protean::PWM::reset()
{
	Wire.beginTransmission(addr);
	Wire.write(PWM_REBOOT_REG);
	Wire.write(0);
	if(Wire.endTransmission())
	{
		return false;
	}

	return true;
}


bool Protean::PWM::checkVersion()
{
	Wire.beginTransmission(addr);
	Wire.write(PWM_VERSION_REG);
	if(Wire.endTransmission())
	{
		return true;
	}

	if(!Wire.requestFrom(addr, 1))
	{
		return false;
	}

	// wait for the data
	while(Wire.available() <= 0);
	int ver = Wire.read();
	return ver == PWM_FIRMWARE_VERSION;
}
