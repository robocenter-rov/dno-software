/*
 Name:		MATE_ROV_2017_DNO.ino
 Created:	16.11.2016 22:23:19
 Author:	JIukaviy
*/

#include "StdMain.h"
#include "StdCommunicator.h"
#include "UdpConnectionProvider.h"
#include <Wire.h>
#include "PCA96685Motor.h"
#include "MS5803SensorDepth.h"
#include "Debug.h"

Main_t* Main;

void setup() {
	ResourceLocker::Init();
	Exceptions::Init();
	Wire.begin();
	Serial1.begin(115200);
#ifdef _DEBUG
	Serial.begin(9600);
#endif
	auto pwm1 = new Adafruit_PWMServoDriver(0x40);
	auto pwm2 = new Adafruit_PWMServoDriver(0x41);

	pwm1->begin();
	pwm1->setPWMFreq(60);

	pwm2->begin();
	pwm2->setPWMFreq(60);
	
	Motors_t* motors = new Motors_t(6);
	motors->AddMotor(new PCA96685Motor_t(pwm1, 13, 14, 15));
	motors->AddMotor(new PCA96685Motor_t(pwm1, 12, 12, 1));
	motors->AddMotor(new PCA96685Motor_t(pwm1, 12, 12, 2));
	motors->AddMotor(new PCA96685Motor_t(pwm2, 12, 12, 3));
	motors->AddMotor(new PCA96685Motor_t(pwm2, 12, 12, 4));
	motors->AddMotor(new PCA96685Motor_t(pwm2, 12, 12, 5));

	Movement_t* movement = new Movement_t(nullptr, motors, nullptr);
	
	FlashlightPeriphery_t* flashlight_periphery = new FlashlightPeriphery_t(13);

	ManipulatorPeriphery_t* manipulator_periphery = new ManipulatorPeriphery_t(
		new PCA9685ServoMotor_t(pwm1, 7),
		new PCA9685ServoMotor_t(pwm2, 15)
	);

	PeripheryManager_t* periphery_manager = new PeripheryManager_t(flashlight_periphery, manipulator_periphery);

	SensorRotation_t* rotation_sensor = new SensorRotation_t();
	MS5803SensorDepth_t* depth_sensor = new MS5803SensorDepth_t(ADDRESS_HIGH, ADC_4096);

	SensorManager_t* sensor_manager = new SensorManager_t(rotation_sensor, depth_sensor);

	byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
	ConnectionProvider_t* connection_provider = new UdpConnectionProvider_t(20, mac, IPAddress(192, 168, 0, 50), 3000);
	Communicator_t* communicator = new StdCommunicator_t(connection_provider);

	Main = new StdMain_t(communicator, movement, sensor_manager, periphery_manager);

	if (Main->Begin()) {
#ifdef _DEBUG
		LOGLN("Fail at the begin");
		auto node = Exceptions::GetList().FrontNode();
		while (node) {
			LOGLN(node->GetData().GetFullMessage());
			node = node->GetNext();
		}
		Exceptions::Release();
#endif
		while (true) {}
	}
}

void loop() {
	Main->Loop();
	if(Exceptions::SmthngWrong()) {
#ifdef _DEBUG
		LOGLN("Some shit happens");
		auto node = Exceptions::GetList().FrontNode();
		while(node) {
			Serial.println(node->GetData().GetFullMessage());
			node = node->GetNext();
		}
		Exceptions::Release();
#endif
		while (true) {}
	}
}
