#include "TaskPool.h"
#include "Exceptions.h"

inline bool TaskPool_t::check_worker_id(int worker_id) const {
	return worker_id < _pool_size;
}

void TaskPool_t::UpdateTask(WorkerQueueNode_t* worker) {
	Task_t* task = worker->task;

#ifdef _DEBUG
	Serial.print("Update task_id: ");
	Serial.print(task->GetId());
	Serial.print(", worker_id: ");
	Serial.println(worker->id);
	delay(200);
#endif

	if (task->UpdateState(worker->saved_task_state)) {

#ifdef _DEBUG
		Serial.println("Task finished, removing task");
		delay(200);
#endif

		RemoveTask(worker);
	}
}

void TaskPool_t::Update() const {
	for (WorkerQueueNode_t* node = _busy_workers; node != nullptr; ) {
		WorkerQueueNode_t* next_node = node->next_node;

		UpdateTask(node);

		node = next_node;
	}
}

void TaskPool_t::MoveWorker(WorkerQueueNode_t*& from, WorkerQueueNode_t*& to, WorkerQueueNode_t* worker) {

#ifdef _DEBUG
	Serial.println("Moving worker");
	Serial.print("Previous worker: ");
#endif

	if (worker->prev_node) {

#ifdef _DEBUG
		Serial.println(worker->prev_node->id);
#endif

		worker->prev_node->next_node = worker->next_node;
	} else {

#ifdef _DEBUG
		Serial.println("null");
#endif

		from = worker->next_node;
	}

#ifdef _DEBUG
	Serial.print("Next worker: ");
#endif

	if (worker->next_node) {

#ifdef _DEBUG
		Serial.println(worker->next_node->id);
#endif

		worker->next_node->prev_node = worker->prev_node;
	}
#ifdef _DEBUG
	else {
		Serial.println("null");
	}
#endif

	worker->next_node = to;
	worker->prev_node = nullptr;

	to = worker;
}

void TaskPool_t::RemoveTask(WorkerQueueNode_t* worker) {

#ifdef _DEBUG
	Serial.print("Task removed, id: ");
	Serial.print(worker->task->GetId());
	Serial.print(", worker_id: ");
	Serial.println(worker->id);
#endif

	delete worker->task;
	worker->task = nullptr;
}

TaskPool_t::TaskPool_t(unsigned int pool_size) {
	_pool_size = pool_size;
	_worker_nodes = new WorkerQueueNode_t[pool_size];

	for (int i = 0; i < pool_size; i++) {
		_worker_nodes[i].task = nullptr;
		_worker_nodes[i].id = i;
		_worker_nodes[i].next_node = i < pool_size - 1 ? &_worker_nodes[i + 1] : nullptr;
		_worker_nodes[i].prev_node = i > 0 ? &_worker_nodes[i - 1] : nullptr;
	}

	_free_workers = _worker_nodes;
	_busy_workers = nullptr;
	_awaiting_workers = nullptr;
	_last_added_task_worker_id = 0;
}

int TaskPool_t::AddTask(Task_t* task, int worker_id = -1) {

#ifdef _DEBUG
	Serial.print("Adding task, id: ");
	Serial.println(task->GetId());
#endif

	WorkerQueueNode_t* current_worker = nullptr;

	if (worker_id < 0) {
		if (!_free_workers) {

#ifdef _DEBUG
			Serial.print("Failed add task: Pool is busy, task_id: ");
			Serial.println(task->GetId());
			delay(200);
#endif

			ThrowException(Exceptions::EC_TP_FULL);
			return 1;
		}

		current_worker = _free_workers;
	} else {
		if (!check_worker_id(worker_id)) {
			ThrowException(Exceptions::EC_TP_INVALID_WORKER_ID);
			return 1;
		}
		if (current_worker->status == WS_BUSY) {
			ThrowException(Exceptions::EC_TP_WORKER_IS_BUSY);
			return 1;
		}

		current_worker = &_worker_nodes[worker_id];
	}

	RESOURCE locked_resource;
	if (task->LockNeededResources(locked_resource, current_worker->id)) {
		if (current_worker->status == WS_FREE) {
			MoveWorker(_free_workers, _busy_workers, current_worker);
		} else {
			MoveWorker(_awaiting_workers, _busy_workers, current_worker);
		}
		current_worker->task = task;
		current_worker->status = WS_BUSY;

#ifdef _DEBUG
		Serial.print("Successfully add task to worker, id: ");
		Serial.println(current_worker->id);
		delay(200);
#endif

		UpdateTask(current_worker);
	} else {

#ifdef _DEBUG
		Serial.println("Failed add task, resource locked");
		Serial.print("State saved in worker: ");
		Serial.println(current_worker->id);
		delay(200);
#endif

		ThrowException(Exceptions::EC_TP_RESOURCE_LOCKED);

		return 1;
	}

	_last_added_task_worker_id = current_worker->id;

	return 0;
}

int TaskPool_t::FreeWorker(int worker_id) {
	if (!check_worker_id(worker_id)) {
		ThrowException(Exceptions::EC_TP_INVALID_WORKER_ID);
		return 1;
	}

	WorkerQueueNode_t* current_worker = &_worker_nodes[worker_id];

	if (current_worker->status == WS_FREE) {
		return 0;
	}

	RemoveTask(current_worker);

	if (current_worker->status == WS_BUSY) {
		MoveWorker(_busy_workers, _free_workers, current_worker);
	} else {
		MoveWorker(_awaiting_workers, _free_workers, current_worker);
	}

	current_worker->status = WS_FREE;

	return 0;
}

int TaskPool_t::CancelWorker(int worker_id) {
	if (!check_worker_id(worker_id)) {
		ThrowException(Exceptions::EC_TP_INVALID_WORKER_ID);
		return 1;
	}

	WorkerQueueNode_t* current_worker = &_worker_nodes[worker_id];

	if (current_worker->status == WS_FREE || current_worker->status == WS_AWAIT) {
		return 0;
	}

	RemoveTask(current_worker);

	MoveWorker(_busy_workers, _awaiting_workers, current_worker);

	return 0;
}

int TaskPool_t::GetWorkerState(int worker_id, SelfExpandoContainer_t<TaskState_t>& out_task_state, WORKER_STATUS& out_worker_status) const {
	if (!check_worker_id(worker_id)) {
		ThrowException(Exceptions::EC_TP_INVALID_WORKER_ID);
		return 1;
	}
	out_task_state = _worker_nodes[worker_id].saved_task_state;
	out_worker_status = _worker_nodes[worker_id].status;
	return 0;
}

int TaskPool_t::GetWorkerState(int worker_id, TaskState_t*& out_task_state_ptr, WORKER_STATUS& out_worker_status) const {
	if (!check_worker_id(worker_id)) {
		ThrowException(Exceptions::EC_TP_INVALID_WORKER_ID);
		return 1;
	}
	out_task_state_ptr = _worker_nodes[worker_id].saved_task_state.Get();
	out_worker_status = _worker_nodes[worker_id].status;
	return 0;
}

int TaskPool_t::GetLastAddedWorkerState(SelfExpandoContainer_t<TaskState_t>& out_state, int& worker_id, WORKER_STATUS& out_worker_status) const {
	if (_last_added_task_worker_id < 0) {
		ThrowException(Exceptions::EC_TP_NO_ONE_WORKER_IS_USED);
		return 1;
	}
	WorkerQueueNode_t* current_worker = &_worker_nodes[_last_added_task_worker_id];
	out_state = current_worker->saved_task_state;
	worker_id = current_worker->id;
	out_worker_status = current_worker->status;
	return 0;
}

int TaskPool_t::GetLastAddedWorkerState(TaskState_t*& out_state_ptr, int& worker_id, WORKER_STATUS& out_worker_status) const {
	if (_last_added_task_worker_id < 0) {
		ThrowException(Exceptions::EC_TP_NO_ONE_WORKER_IS_USED);
		return 1;
	}
	WorkerQueueNode_t* current_worker = &_worker_nodes[_last_added_task_worker_id];
	out_state_ptr = current_worker->saved_task_state.Get();
	worker_id = current_worker->id;
	out_worker_status = current_worker->status;
	return 0;
}

int TaskPool_t::GetLastAddedTaskWorkerId() const {
	return _last_added_task_worker_id;
}
