#include "SetFlashlightStateTask.h"

SetFlashlightStateTask_t::SetFlashlightStateTask_t(int id, bool state, PeripheryManager_t* periphery_manager)
	: Task_t(id, TI_SET_FLASH_LIGHT_STATE), _locker(R_FLASHLIGHT) {
	_periphery_manager = periphery_manager;
	_flashlight_state = state;
}

bool SetFlashlightStateTask_t::LockNeededResources(RESOURCE& out_locked_resource, int worker_id) {
	if (_locker.Lock(worker_id)) {
		return true;
	} else {
		out_locked_resource = R_FLASHLIGHT;
		return false;
	}
}

bool SetFlashlightStateTask_t::UpdateState(SelfExpandoContainer_t<TaskState_t>& out_state) {
	_periphery_manager->SetFlashlightState(_flashlight_state);
	out_state = OkTaskState_t(TI_SET_FLASH_LIGHT_STATE, _tag);
	return true;
}
