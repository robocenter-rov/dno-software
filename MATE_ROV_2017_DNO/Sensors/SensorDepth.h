#pragma once

class SensorDepth_t {
private:
public:
	virtual ~SensorDepth_t() {}
	SensorDepth_t();

	virtual float GetDepth() = 0;
	virtual float GetRawData() = 0;
};
