#pragma once

#include <Eigen/Dense>

using namespace Eigen;

class Controller 
{
	double Ts;
	double Wmax;
	Matrix2d J;

	void CalculateWheelsVel();
public:
	double time;

	Vector3d g, gr;
	Vector2d dpr;
	Vector2d ddpr;

	Vector2d u, uw;
	
	Matrix<double, 2, 3> K;

	void Init()
	{
		Ts = 0.02;	
		Wmax = 30.0;

		const double r = 0.025;
		const double b = 0.145;
		J << r/2.0, r/2.0, r/b, - r/b;

		u(0) = 0.05;
		uw << 0, 0;
	}

	void Update();
	Controller() 
	{
		Init();
	};



};