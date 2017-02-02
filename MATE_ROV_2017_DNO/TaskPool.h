#pragma once
#include "Task.h"

class TaskPool_t {
private:
	struct WorkerQueueNode_t {
		unsigned int id;
		Task_t* task;
		SelfExpandoContainer_t<TaskState_t> state;
		WorkerQueueNode_t* next_node;
		WorkerQueueNode_t* prev_node;
	};

	unsigned int _pool_size;

	WorkerQueueNode_t* _worker_nodes;
	WorkerQueueNode_t* _free_workers;
	WorkerQueueNode_t* _busy_workers;

	int _last_added_task_worker_id;

	inline bool check_worker_id(int worker_id) const;
	void UpdateTask(WorkerQueueNode_t* worker);

	static void MoveWorker(WorkerQueueNode_t*& from, WorkerQueueNode_t*& to, WorkerQueueNode_t* worker);
	void RemoveTask(WorkerQueueNode_t* worker);
public:
	TaskPool_t(unsigned int pool_size);

	void Update();

	int AddTask(Task_t* task, int worker_id = -1);
	int FreeWorker(int worker_id);
	void GetTaskState(int worker_id, SelfExpandoContainer_t<TaskState_t>& out_state) const;
	TaskState_t* GetTaskStatePtr(int worker_id) const;
	void GetLastAddedTaskState(SelfExpandoContainer_t<TaskState_t>& out_state) const;
	TaskState_t* GetLastAddedTaskStatePtr() const;
	unsigned int GetLastAddedTaskWorkerId() const;
};
