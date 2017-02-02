#pragma once
#include "Task.h"

enum WORKER_STATUS {
	WS_FREE,
	WS_BUSY,
	WS_AWAIT
};

class TaskPool_t {
private:
	struct WorkerQueueNode_t {
		unsigned int id;
		WORKER_STATUS status;
		Task_t* task;
		SelfExpandoContainer_t<TaskState_t> saved_task_state;
		WorkerQueueNode_t* next_node;
		WorkerQueueNode_t* prev_node;
	};

	unsigned int _pool_size;

	WorkerQueueNode_t* _worker_nodes;
	WorkerQueueNode_t* _free_workers;
	WorkerQueueNode_t* _busy_workers;
	WorkerQueueNode_t* _awaiting_workers;

	int _last_added_task_worker_id;

	inline bool check_worker_id(int worker_id) const;
	void UpdateTask(WorkerQueueNode_t* worker);

	static void MoveWorker(WorkerQueueNode_t*& from, WorkerQueueNode_t*& to, WorkerQueueNode_t* worker);
	static void RemoveTask(WorkerQueueNode_t* worker);
	void MakeWorkerFree(WorkerQueueNode_t* worker);
	void MakeWorkerAwait(WorkerQueueNode_t* worker);
public:
	TaskPool_t(unsigned int pool_size);

	void Update();

	int AddTask(Task_t* task, int worker_id = -1);
	int FreeWorker(int worker_id);
	int CancelWorker(int worker_id);
	int GetWorkerState(int worker_id, SelfExpandoContainer_t<TaskState_t>& out_task_state, WORKER_STATUS& out_worker_status) const;
	int GetWorkerState(int worker_id, TaskState_t*& out_task_state_ptr, WORKER_STATUS& out_worker_status) const;
	int GetLastAddedWorkerState(SelfExpandoContainer_t<TaskState_t>& out_state, int& worker_id, WORKER_STATUS& out_worker_status) const;
	int GetLastAddedWorkerState(TaskState_t*& out_state_ptr, int& worker_id, WORKER_STATUS& out_worker_status) const;
	int GetLastAddedTaskWorkerId() const;
};
