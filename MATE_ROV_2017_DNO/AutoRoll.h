#pragma once

#pragma once

#include "MovementRegulator.h"
#include "MotorsForceDistributor.h"

class AutoRoll_t : public MovementRegulator_t {
public:
	using MovementRegulator_t::MovementRegulator_t;
	void Update(MotorsForceDistributor_t* motors_force_distributor);
};
