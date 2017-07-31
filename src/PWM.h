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
	PWM(int addr);

	bool readTimings(uint8_t start_channel, int count, int timings[]);
	int  readTiming(uint8_t channel);

	bool writeTimings(uint8_t start_channel, int count, int timings[]);
	bool writeTiming(uint8_t channel, int timing);
	bool setEcho(bool on);
	bool reset();
	bool checkVersion();

private:
	int     addr;
	uint8_t lastTimings;
};

};

#endif
