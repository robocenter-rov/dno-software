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

Main_t* Main;

void setup() {
	Exceptions::Init();
	Wire.begin();
	Serial.begin(9600);

	auto pwm1 = new Adafruit_PWMServoDriver(0x40);
	auto pwm2 = new Adafruit_PWMServoDriver(0x41);

	pwm1->begin();
	pwm1->setPWMFreq(1600);

	pwm2->begin();
	pwm2->setPWMFreq(1600);

	Motors_t* motors = new Motors_t(6);
	motors->AddMotor(new PCA96685Motor_t(pwm1, 2, 3, 0));
	motors->AddMotor(new PCA96685Motor_t(pwm1, 4, 5, 1));
	motors->AddMotor(new PCA96685Motor_t(pwm1, 6, 7, 2));
	motors->AddMotor(new PCA96685Motor_t(pwm1, 8, 9, 3));
	motors->AddMotor(new PCA96685Motor_t(pwm1, 10, 11, 4));
	motors->AddMotor(new PCA96685Motor_t(pwm1, 12, 13, 5));

	Movement_t* movement = new Movement_t(nullptr, motors, nullptr);

	FlashlightPeriphery_t* flashlight_periphery = new FlashlightPeriphery_t(13);
	PeripheryManager_t* periphery_manager = new PeripheryManager_t(flashlight_periphery);

	byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
	ConnectionProvider_t* connection_provider = new UdpConnectionProvider_t(20, mac, 3000);
	Communicator_t* communicator = new StdCommunicator_t(connection_provider);

	Main = new StdMain_t(communicator, movement, nullptr, periphery_manager);

	Main->Begin();
}

void loop() {
	Main->Loop();
	if(Exceptions::SmthngWrong()) {
#ifdef _DEBUG
		Serial.println("Some shit happens");
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
