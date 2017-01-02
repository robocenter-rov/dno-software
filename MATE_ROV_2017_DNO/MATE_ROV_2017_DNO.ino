/*
 Name:		MATE_ROV_2017_DNO.ino
 Created:	16.11.2016 22:23:19
 Author:	JIukaviy
*/

#include "StdMain.h"
#include "StdCommunicator.h"
#include "UdpConnectionProvider.h"
Main_t* Main;

void setup() {
	Exceptions::Init();

	Serial.begin(9600);

	Motor_t* m1 = new PololuMotor_t(3, 4);

	Motors_t* motors = new Motors_t(2);
	motors->AddMotor(m1);

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
