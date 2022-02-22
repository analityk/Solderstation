#include <pid.h>

void PID::Compute(void)
{
	error = setpoint - measure;
	intergal = intergal + error * kt;
	deriv = (error - prev_error) / kt;

	output = (kp * error) + (ki * intergal) + (kd * deriv);
	prev_error = error;
};

void PID::SetParam(float kp, float ki, float kd, float dt)
{
	this->kp = kp;
	this->ki = ki;
	this->kd = kd;
	this->kt = dt;
};

void PID::SetSetpoint(float setpoint)
{
	this->setpoint = setpoint;
};

void PID::Feed(float val)
{
	this->measure = val;
};

float PID::Output(void)
{
	return output;
};

void PID::Reset(void)
{
	error = 0;
	intergal = 0;
	deriv = 0;
	prev_error = 0;
	output = 0;
}
