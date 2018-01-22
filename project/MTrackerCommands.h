#pragma once

#include "robot.h"
#include <stdint.h>

// definicje numerów rozkazów
#define	MAX_COMMAND_NUMBER					127

#define CMD_BUF_SIZE	128

#pragma pack(1)	
struct CmdFrame
{
	uint8_t header;
	uint8_t size;
	uint8_t groupAddress;
	uint8_t cmd;
	uint16_t number;
	uint8_t data[CMD_BUF_SIZE-6];
};

#pragma pack(1)	 
struct CmdSetVelocity
{
	int16_t vl;
	int16_t vr;
	static const uint8_t cmd = 1;
};

#pragma pack(1)	 
struct CmdSetVelocityAndOdometry
{
	uint16_t status;
	int16_t wl;
	int16_t wr;
	float x;
	float y;
	float th;
	static const uint8_t cmd = 3;
};

#pragma pack(1)	 
struct CmdAnsSetVelocityAndOdometry
{
	uint16_t status;
	int16_t wl;
	int16_t wr;
	float x;
	float y;
	float th;
	int16_t ul;
	int16_t ur;
};


class MTrackerCommands  
{
	CmdFrame * outFrame;

	uint8_t groupAddress;		// adres robota
	uint8_t request;			// informacja okreœlaj¹ca rodzaj zwracanych danych

	void MakeFrameToSend();					// przygotuj ramkê zapytania

public:
	MTrackerState state;

	int	sizeDataOut, cmdSent, cmdRead;

	uint8_t bufOut[CMD_BUF_SIZE];					// bufor pomocniczy przechowuj¹cy dane wysy³ane
	
	MTrackerCommands();
	virtual ~MTrackerCommands();
	
	int ReadData(uint8_t * s);			// czytaj dane wejœciowe z bufora

	void SetWheelsVelocities(uint8_t robot_nr, double vl, double vr);
	void SetWheelsVelocitiesWithOdometry(uint8_t robot_nr, uint16_t status, double wl, double wr, double x, double y, double th);
};

