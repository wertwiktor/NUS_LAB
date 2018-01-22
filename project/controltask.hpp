/****************************************************************************************************************************************************************************************************************
Control task: 
- connections between fundamental control modules
- input/output interfaces

(c) KSIS 2015, M. Kie³czewski, D. Pazderski
*****************************************************************************************************************************************************************************************************************/

#pragma once

#include "WinCom.h"
#include "MTrackerCommands.h"

#include "reftrajectory.hpp"
#include "controller.hpp"

class ControlTask 
{
private:
	FILE *stream;
	WinCom port;
	
	RefTrajectory refTr;
	Controller cntr;
	
public:
	bool running;
	int receivedStatus;

	MTrackerCommands mtracker;

	ControlTask() { running = false; receivedStatus = 0;}

	int OpenCom() 
	{
		return port.Open(3, 921600);
	}

	void CloseCom() 
	{
		if (!port.isClosed) port.Close();
	}

	bool Send() 
	{
		return port.Send(mtracker.bufOut, mtracker.sizeDataOut);
	}
 
	void StartRecording() 
	{
		stream = fopen("robot_state.txt",  "w");
	}

	void StopRecording() 
	{
		fclose(stream);
	}

	void Task() 
	{
		if (running)
			Run();
		else 
			Stop();
	}

	void Stop() {};

	void Run() 
	{

		Vector3d g;
		auto p = port.Receive();

		static LARGE_INTEGER fr,st,end;BOOL b;
		static double dtime,time = 0;
		b = QueryPerformanceFrequency(&fr);

		if (p) 
		{
			receivedStatus = mtracker.ReadData(port.buffer_in);

			g << mtracker.state.x, mtracker.state.y, mtracker.state.th;
		
			refTr.Update();
			cntr.time = time/1000.0;
			cntr.g = g;
			cntr.gr = refTr.gr;
			cntr.dpr = refTr.dpr;
			cntr.ddpr = refTr.ddpr;
			cntr.Update();

			b = QueryPerformanceCounter(&end);
			dtime =1000*(double(end.QuadPart)-double(st.QuadPart))/(double(fr.QuadPart)); st=end;
			if (dtime>10000) time = 0; 
			else time = time + dtime;

			if (receivedStatus > 0) 
			{
				fprintf(stream, "%3.6f, %3.6f, %3.6f, %3.6f, %3.6f, %3.6f, %3.6f, %3.6f", time, mtracker.state.x, mtracker.state.y, mtracker.state.th, mtracker.state.wr, mtracker.state.wl, mtracker.state.ur, mtracker.state.ul);
				fprintf(stream, "%3.6f, %3.6f, %3.6f\n", cntr.u(0), cntr.u(1), cntr.gr(2));
			}
		}
	
		mtracker.SetWheelsVelocitiesWithOdometry(0, 0x03, cntr.uw(1), cntr.uw(0), 0, 0, 0);
		Send();	
	}
};