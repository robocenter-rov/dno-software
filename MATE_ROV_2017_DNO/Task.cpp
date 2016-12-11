#include "Task.h"

void Task_t::SetWorkerId(unsigned int worker_id) {
	_worker_id = worker_id;
}

SetFlashlightStateTask_t::SetFlashlightStateTask_t(int id, bool state, PeripheryManager_t* periphery_manager)
	: Task_t(id), _locker(R_FLASHLIGHT, id)
{
	_periphery_manager = periphery_manager;
	_flashlight_state = state;
}

bool SetFlashlightStateTask_t::LockNeededResources(RESOURCE& out_locked_resource) {
	if (_locker.Lock()) {
		return true;
	} else {
		out_locked_resource = R_FLASHLIGHT;
		return false;
	}
}

bool SetFlashlightStateTask_t::UpdateState(TaskState_t* out_state) {
	_periphery_manager->SetFlashlightState(_flashlight_state);
	*static_cast<OkTaskState_t*>(out_state) = OkTaskState_t(_id, _worker_id);
	return true;
}

BlinkFlashlightTask_t::BlinkFlashlightTask_t(int id, int blink_count, PeripheryManager_t* periphery_manager)
	: Task_t(id), _locker(R_FLASHLIGHT, id)
{
	_periphery_manager = periphery_manager;
	_blink_count = blink_count;
	_change_count = 0;
	_last_change_time = 0;
}

bool BlinkFlashlightTask_t::LockNeededResources(RESOURCE& out_locked_resource) {
	if (_locker.Lock()) {
		return true;
	} else {
		out_locked_resource = R_FLASHLIGHT;
		return false;
	}
}

bool BlinkFlashlightTask_t::UpdateState(TaskState_t* out_state) {
	unsigned long  current_time = millis();
	if (current_time - _last_change_time > 500) {
		_periphery_manager->SetFlashlightState(!_periphery_manager->GetFlashlightState());
		_last_change_time = current_time;
		_change_count++;
		if (_change_count / 2 >= _blink_count) {
			*static_cast<OkTaskState_t*>(out_state) = OkTaskState_t(_id, _worker_id);
			return true;
		}
	}

	*static_cast<BlinkFlashlightTaskState_t*>(out_state) = 
		BlinkFlashlightTaskState_t(_id, _worker_id, _change_count / 2);

	return false;
}

BlinkFlashlightTask_t::~BlinkFlashlightTask_t() {
	_periphery_manager->TurnOffFlashlight();
}
