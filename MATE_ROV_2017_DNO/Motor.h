#pragma once

class Motor_t {
protected:
	float _thrust;
	float _mul;
	
	virtual void _SetThrust() = 0;
public:
	Motor_t();
	
	void SetThrust(float thrust);
	float GetThrust() const;
	void SetMultiplier(float mul);

	virtual ~Motor_t();
};

