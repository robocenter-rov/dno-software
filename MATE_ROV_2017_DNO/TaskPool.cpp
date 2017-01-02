#include "TaskPool.h"
#include "Exceptions.h"

inline bool TaskPool_t::check_worker_id(unsigned int worker_id) const {
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

	if (task->UpdateState(worker->state)) {

#ifdef _DEBUG
		Serial.println("Task finished, removing task");
		delay(200);
#endif

		RemoveTask(worker);
	}
}

void TaskPool_t::Update() {
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

	MoveWorker(_busy_workers, _free_workers, worker);
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
	_last_added_task_worker_id = 0;
}

int TaskPool_t::AddTask(Task_t* task) {

#ifdef _DEBUG
	Serial.print("Adding task, id: ");
	Serial.println(task->GetId());
#endif

	if (!_free_workers) {

#ifdef _DEBUG
		Serial.print("Failed add task: Pool is busy, task_id: ");
		Serial.println(task->GetId());
		delay(200);
#endif

		ThrowException(Exceptions::EC_TP_FULL);
		return 1;
	}

	WorkerQueueNode_t* current_worker = _free_workers;

	RESOURCE locked_resource;
	if (task->LockNeededResources(locked_resource)) {
		MoveWorker(_free_workers, _busy_workers, _free_workers);
		current_worker->task = task;

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

int TaskPool_t::FreeWorker(unsigned int worker_id) {
	if (!check_worker_id(worker_id)) {
		ThrowException(Exceptions::EC_TP_INVALID_WORKER_ID);
		return 1;
	}

	if (_worker_nodes[worker_id].task == nullptr) {
		return 0;
	}

	WorkerQueueNode_t* current_worker = &_worker_nodes[worker_id];

	current_worker->state =
		CancelledTaskState_t(current_worker->task->GetId(), worker_id);

	RemoveTask(current_worker);

	return 0;
}

void TaskPool_t::GetTaskState(unsigned int worker_id, SelfExpandoContainer_t<TaskState_t>& out_state) const {
	if (!check_worker_id(worker_id)) {
		return;
	}
	out_state = _worker_nodes[worker_id].state;
}

TaskState_t* TaskPool_t::GetTaskStatePtr(unsigned int worker_id) const {
	if (!check_worker_id(worker_id)) {
		return nullptr;
	}
	return _worker_nodes[worker_id].state.Get();
}


void TaskPool_t::GetLastAddedTaskState(SelfExpandoContainer_t<TaskState_t>& out_state) const {
#ifdef _DEBUG
	Serial.print("Getting last added task state, last worker_id: ");
	Serial.println(_last_added_task_worker_id);
	delay(200);
#endif
	out_state = _worker_nodes[_last_added_task_worker_id].state;
}

TaskState_t* TaskPool_t::GetLastAddedTaskStatePtr() const {
#ifdef _DEBUG
	Serial.print("Getting last added task state, last worker_id: ");
	Serial.println(_last_added_task_worker_id);
	delay(200);
#endif
	return _worker_nodes[_last_added_task_worker_id].state.Get();
}

unsigned int TaskPool_t::GetLastAddedTaskWorkerId() const {
	return _last_added_task_worker_id;
}
