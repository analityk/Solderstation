#ifndef pid_h__
#define pid_h__

#include <avr/io.h>

class PID{
private:
	float error;
	float prev_error;
	float deriv;
	float intergal;
	
	float output;
	float kt;
	float kp;
	float ki;
	float kd;
	float measure;
public:

	float setpoint;
	PID(float _kt, float _kp, float _ki, float _kd, float _val)
	:kt(_kt), kp(_kp), ki(_ki), kd(_kd), measure(_val) {};
		
	PID(){};
	
	void Compute(void);
	void SetParam(float kp, float ki, float kd, float dt);
	void SetSetpoint(float setpoint);
	void Feed(float val);
	float Output(void);
};

#endif // pid_h__