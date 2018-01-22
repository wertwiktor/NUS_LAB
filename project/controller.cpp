/****************************************************************************************************************************************************************************************************************
Controller: 
- the class for implementation of a control algorithm

(c) KSIS 2015, D. Pazderski
*****************************************************************************************************************************************************************************************************************/

#include "controller.hpp"
#include <algorithm> 

inline float sign(float f)
{
    return (f < 0) ? -1.0f : 1.0f;
}

void Controller::CalculateWheelsVel() 
{
	uw = J.inverse() * u;
	// compute scale function
	auto mu = std::max(1.0, (double) std::max(fabs((double)uw(0))/Wmax, fabs((double)uw(1))/Wmax));
	// scale control input
	uw /= mu;
}

void Controller::Update() 
{

	Vector2d p, pr, dp, e, de;
	Vector2d omega;
	Vector2d v, w;
	Matrix2d Kp, Kd, P, Pinv;
	Vector2d tempOut;
	double th, exp, eyp,u2;

	// theta do lokalnej
	th = g(2);
	
	// po³o¿enie do lokalnej
	p << g(0), g(1);

	//referencyjne
	pr << gr(0), gr(1);

	

	//oszukany uchyb
	exp = cos(th)*u(0) - dpr(0);
	eyp =3*( sin(th)*u(0) - dpr(1));
	de << exp, eyp;

	// gain matrices
	Kp << 1.0, 0, 0, 1.0;
	Kd << 2.0, 0, 0, 2.0;
	
	// uchyb sterowania
	e = p - pr;


	// macierz P
	P << cos(th), -sin(th)*u(0), sin(th), cos(th)*u(0);
	Pinv = P.inverse();
	// for test purpose
	tempOut = Pinv*(-Kp*e - Kd*de + ddpr);
	integral += tempOut(0)*0.02;
	u << integral, tempOut(1);
	
	

	CalculateWheelsVel();

}

