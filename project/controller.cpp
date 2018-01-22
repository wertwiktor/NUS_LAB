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
	Vector2d v, w;
	Matrix2d Kp, Kd, P;
	
	// gain matrices
	Kp << 1.0, 0, 0, 1.0;
	Kd << 1.0, 0, 0, 1.0;

	// for test purpose
	u << 0.3*sin(4.0*time), 0.0;

	CalculateWheelsVel();

}

