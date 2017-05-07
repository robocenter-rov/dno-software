#pragma once

class Motor_t {
protected:
	float _thrust;
	float _mul;
public:
	Motor_t();
	
	virtual void SetThrust(float thrust);
	float GetThrust() const;
	void SetMultiplier(float mul);

	virtual ~Motor_t();
};

