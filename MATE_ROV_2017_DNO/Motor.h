#pragma once

class Motor_t {
public:
	Motor_t();
	
	virtual void SetThrust(float thrust);

	virtual ~Motor_t();
};

