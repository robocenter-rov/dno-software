#include "CameraPeriphery.h"

CameraPeriphery_t::CameraPeriphery_t(PCA9685ServoMotor_t* servo1, PCA9685ServoMotor_t* servo2)
{
	_servo1 = servo1;
	_servo2 = servo2;
}

void CameraPeriphery_t::SetAngleServo1(float angle_radians)
{
	_servo1->SetAngle(angle_radians);
}

void CameraPeriphery_t::SetAngleServo2(float angle_radians)
{
	_servo2->SetAngle(angle_radians);
}