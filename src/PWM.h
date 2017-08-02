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

#ifndef PROTEAN_PWM_LOGGER
#define PROTEAN_PWM_LOGGER

#include <inttypes.h>

#define PWM_DEF_ADDR    0x69
#define PWM_VERSION_REG 0x0A
#define PWM_REBOOT_REG  0x0B

#define PWM_FIRMWARE_VERSION 1


namespace Protean {

class PWM {
public:
	PWM();

	/**
	 * Constructs a new PWM logger object.
	 * @param addr 7-bit I2C address for a PWM Logger with alternative firmware.
	 */
	PWM(int addr);

	/**
	 * Reads timings in microseconds from the logger for 'count' channels
	 * beginning with 'start_channel'. Results are stored in array pointed
	 * to by 'timings'.
	 * @param  start_channel Logger channel to start reading from (1 indexed).
	 * @param  count         Number of consecutive timings to read.
	 * @param  timings       Pointer to an array of timings
	 * @return               true on success, false otherwise.
	 */
	bool readTimings(uint8_t start_channel, int count, unsigned int timings[]);

	/**
	 * Reads timing of channel in microseconds from the logger.
	 * @param  channel Logger channel to read from (1 indexed)/
	 * @return         -1 on error, non-zero otherwise.
	 */
	int  readTiming(uint8_t channel);

	/**
	 * Reads raw timings from the logger for 'count' channels
	 * beginning with 'start_channel'. Results are stored in array pointed
	 * to by 'timings'.
	 * @param  start_channel Logger channel to start reading from (1 indexed).
	 * @param  count         Number of consecutive timings to read.
	 * @param  timings       Pointer to an array of timings
	 * @return               true on success, false otherwise.
	 */
	bool readTimingsRaw(uint8_t start_channel, int count, uint8_t timings[]);

	/**
	 * Reads raw timing of channel from the logger.
	 * @param  channel Logger channel to read from (1 indexed)/
	 * @return         -1 on error, non-zero otherwise.
	 */
	int  readTimingRaw(uint8_t channel);

	/**
	 * Writes timings in microseconds to the logger for 'count' channels
	 * beginning with 'start_channel'. Timings are stored in array pointed
	 * to by 'timings'. The logger must have 'echo mode' disabled.
	 * @param  start_channel Logger channel to start writing to (1 indexed).
	 * @param  count         Number of consecutive timings to write.
	 * @param  timings       Pointer to an array of timings
	 * @return               true on success, false otherwise.
	 */
	bool writeTimings(uint8_t start_channel, int count, unsigned int timings[]);

	/**
	 * Writes a 'timing' in microseconds to 'channel'. The logger must have
	 * 'echo mode' disabled.
	 * @param  channel logger channel to write to (1 indexed).
	 * @param  timing  timing in microseconds.
	 * @return         true on success, false otherwise.
	 */
	bool writeTiming(uint8_t channel, int timing);

	/**
	 * Writes raw timings to the logger for 'count' channels
	 * beginning with 'start_channel'. Timings are stored in array pointed
	 * to by 'timings'. The logger must have 'echo mode' disabled.
	 * @param  start_channel Logger channel to start writing to (1 indexed).
	 * @param  count         Number of consecutive timings to write.
	 * @param  timings       Pointer to an array of timings
	 * @return               true on success, false otherwise.
	 */
	bool writeTimingsRaw(uint8_t start_channel, int count, uint8_t timings[]);

	/**
	 * Writes a raw 'timing' to 'channel'. The logger must have
	 * 'echo mode' disabled.
	 * @param  channel logger channel to write to (1 indexed).
	 * @param  timing  timing.
	 * @return         true on success, false otherwise.
	 */
	bool writeTimingRaw(uint8_t channel, uint8_t timing);

	/**
	 * Changes the 'echo mode' of the logger.
	 * @param  on true for ON false for OFF
	 * @return    true on success, false otherwise.
	 */
	bool setEcho(bool on);

	/**
	 * Causes the logger to perform a hardware reset.
	 * @return true on success, false otherwise.
	 */
	bool reset();

	/**
	 * Checks this API's potential compatibility with the logger's firmware.
	 * @return true on success, false otherwise.
	 */
	bool checkVersion();

private:
	int     addr;
	uint8_t lastTimings;
};

};

#endif
