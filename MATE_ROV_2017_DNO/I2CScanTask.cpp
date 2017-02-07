#include "I2CScanTask.h"
#include "Wire.h"

I2CScanTask_t::I2CScanTask_t(int tag) : Task_t(tag) {
	_current_adress = 1;
	_device_count = 0;
}

bool I2CScanTask_t::LockNeededResources(RESOURCE& out_locked_resource, int worker_id) {
	return true;
}

bool I2CScanTask_t::UpdateState(SelfExpandoContainer_t<TaskState_t>& out_state) {
	if (_current_adress < 127) {
		Wire.beginTransmission(_current_adress);
		int error = Wire.endTransmission();

		if (error == 0) {
#ifdef _DEBUG
			LOG("I2C device found at address 0x");
			if (_current_adress<16)
				LOG("0");
			LOGHEX(_current_adress);
			LOGLN("  !");
#endif
			_adresses[_device_count] = _current_adress;
			_device_count++;
		}
#ifdef _DEBUG
		else if (error == 4) {
			LOG("Unknow error at address 0x");
			if (_current_adress<16)
				LOG("0");
			LOGHEX(_current_adress);
			LOGLN();
		}
#endif
		_current_adress++;
		out_state = I2CScanningTaskState_t(_tag);
		return false;
	} else {
		out_state = I2CScanningDoneTaskState_t(_tag, _adresses, _device_count);
		return true;
	}
}
