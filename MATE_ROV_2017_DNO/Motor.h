#pragma once

class Motor_t {
	float _thrust;
public:
	Motor_t();
	
	virtual void SetThrust(float thrust);
	float GetThrust() const;

	virtual ~Motor_t();
};

