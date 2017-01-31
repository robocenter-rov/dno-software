#pragma once

#include "SensorDepth.h"
#include <SparkFun_MS5803_I2C.h>

class MS5803SensorDepth_t : public SensorDepth_t {
private:
	MS5803 _sensor;
	precision _precision;
public:
	MS5803SensorDepth_t(ms5803_addr adress, precision precision);

	float GetRawData() override;
	float GetDepth() override;
};
