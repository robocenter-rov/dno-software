#pragma once

class Main_t {
public:
	virtual void Loop() = 0;
	virtual ~Main_t() {};
	virtual int Begin() = 0;
};
