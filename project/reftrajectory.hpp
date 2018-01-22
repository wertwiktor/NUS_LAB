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
	//	Vector2d prp;
	//	Vector2d prpp;
		
		//oryginalne
		//double xr = A*sin(omega*tau);			
		//double yr = A*cos(omega*tau);

		//double xr = Xd + Ax*cos(omega*tau + psi_x);
		//double yr = Yd + Ay*sin(omega*tau/2 + psi_y);

		double xr = 0.5;
		double yr = 0.1;


/*
		double xrp = A*omega*cos(omega*tau);
		double yrp = -A*omega*sin(omega*tau);

		double xrpp = -A*omega*omega*sin(omega*tau);
		double yrpp = -A*omega*omega*cos(omega*tau);
*/

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

	double A;
	double omega;
	double psi_x, psi_y, Ax, Ay, Xd, Yd;

	RefTrajectory() 
	{
		Ts = 0.02;
		A = 0.3;
		omega = 0.5;
		Xd = 0;
		Yd = 0;
		Ax = 0.3;
		Ay = 0.3;
		psi_x = -3.1415/2;
		psi_y = 0;
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
