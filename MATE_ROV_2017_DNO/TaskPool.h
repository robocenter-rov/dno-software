#pragma once
#include "Task.h"

class TaskPool_t {
private:
	struct WorkerQueryNode_t {
		unsigned int id;
		Task_t* task;
		TaskStateUnion_t state;
		WorkerQueryNode_t* next_node;
		WorkerQueryNode_t* prev_node;
	};

	unsigned int _pool_size;

	WorkerQueryNode_t* _worker_nodes;
	WorkerQueryNode_t* _free_workers;
	WorkerQueryNode_t* _busy_workers;

	int _last_added_task_worker_id;

	inline bool check_worker_id(unsigned int worker_id) const;
	void UpdateTask(WorkerQueryNode_t* worker);

	static void MoveWorker(WorkerQueryNode_t*& from, WorkerQueryNode_t*& to, WorkerQueryNode_t* worker);
	void RemoveTask(WorkerQueryNode_t* worker);
public:
	TaskPool_t(unsigned int worker_id);

	void Update();

	bool AddTask(Task_t* task);
	bool FreeWorker(unsigned int worker_id);
	void GetTaskState(unsigned int worker_id, TaskStateUnion_t& out_state) const;
	void GetLastAddedTaskState(TaskStateUnion_t& out_state) const;
	unsigned int GetLastAddedTaskWorkerId() const;
};
