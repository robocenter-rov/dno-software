#include "TaskPool.h"

inline bool TaskPool_t::check_worker_id(unsigned int worker_id) const {
	return worker_id < _pool_size;
}

void TaskPool_t::UpdateTask(WorkerQueryNode_t* worker) {
	Task_t* task = worker->task;

#ifdef _DEBUG
	Serial.print("Update task_id: ");
	Serial.print(task->GetId());
	Serial.print(", worker_id: ");
	Serial.println(worker->id);
	delay(200);
#endif

	if (task->UpdateState(reinterpret_cast<TaskState_t*>(&worker->state))) {

#ifdef _DEBUG
		Serial.println("Task finished, removing task");
		delay(200);
#endif

		RemoveTask(worker);
	}
}

void TaskPool_t::Update() {
	for (WorkerQueryNode_t* node = _busy_workers; node != nullptr; ) {
		WorkerQueryNode_t* next_node = node->next_node;

		UpdateTask(node);

		node = next_node;
	}
}

void TaskPool_t::MoveWorker(WorkerQueryNode_t*& from, WorkerQueryNode_t*& to, WorkerQueryNode_t* worker) {

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

void TaskPool_t::RemoveTask(WorkerQueryNode_t* worker) {

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
	_worker_nodes = new WorkerQueryNode_t[pool_size];

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

bool TaskPool_t::AddTask(Task_t* task) {

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
		return false;
	}

	WorkerQueryNode_t* current_worker = _free_workers;

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

		current_worker->state.resource_locked_task_state
			= ResourceLockedTaskState_t(task->GetId(), current_worker->id, locked_resource, ResourceLocker::GetOwner(locked_resource));
	}

	_last_added_task_worker_id = current_worker->id;

	return true;
}

bool TaskPool_t::FreeWorker(unsigned int worker_id) {
	if (!check_worker_id(worker_id) || _worker_nodes[worker_id].task == nullptr) {
		return false;
	}

	WorkerQueryNode_t* current_worker = &_worker_nodes[worker_id];

	current_worker->state.cancelled_task_state =
		CancelledTaskState_t(current_worker->task->GetId(), worker_id);

	RemoveTask(current_worker);

	return true;
}

void TaskPool_t::GetTaskState(unsigned int worker_id, TaskStateUnion_t& out_state) const {
	if (!check_worker_id(worker_id)) {
		return;
	}
	out_state = _worker_nodes[worker_id].state;
}

void TaskPool_t::GetLastAddedTaskState(TaskStateUnion_t& out_state) const {
#ifdef _DEBUG
	Serial.println("Getting last added task state, last worker_id: ");
	Serial.println(_last_added_task_worker_id);
	delay(200);
#endif
	out_state = _worker_nodes[_last_added_task_worker_id].state;
}

unsigned int TaskPool_t::GetLastAddedTaskWorkerId() const {
	return _last_added_task_worker_id;
}
