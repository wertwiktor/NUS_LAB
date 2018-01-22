/****************************************************************************************************************************************************************************************************************
RefTrajectory: 
- the class for implementation of an analitycal reference trajekcotory for a unicycle-like robot

(c) KSIS 2014, D. Pazderski
*****************************************************************************************************************************************************************************************************************/

#pragma once

#include <Eigen/Dense>

using namespace Eigen;

class RefTrajectory {
private:
	double Ts, t;

	Vector2d GetPos(double tau) 
	{
		Vector2d pr;
		
		double xr = 0.2*sin(0.1*tau);			
		double yr = 0.2*cos(0.1*tau);

		pr << xr, yr;
		return pr;
	}

	Vector2d GetLinearVel(double tau) 
	{
		Vector2d dp;
		dp = (GetPos(tau)-GetPos(tau-Ts))/Ts;
		return dp;
	}

	Vector2d GetLinearAcc(double tau) 
	{
		Vector2d ddp;
		ddp = (GetLinearVel(tau)-GetLinearVel(tau-Ts))/Ts;
		return ddp;
	}
		
	double GetOrientation() 
	{
		return atan2(dpr(1), dpr(0));
	}

public:
	Vector3d gr;
	Vector2d dpr;
	Vector2d ddpr;

	RefTrajectory() 
	{
		Ts = 0.02;
		Init();	
	}

	void Init() 
	{
		t = 0;
	}

	void Update() 
	{	  
		Vector2d pr;
		pr = GetPos(t);
		dpr = GetLinearVel(t);
		ddpr = GetLinearAcc(t);
		auto thr = GetOrientation();

		gr << pr(0), pr(1), thr;

		t += Ts;
	}
};
