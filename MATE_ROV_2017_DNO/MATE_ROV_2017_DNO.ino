/*
 Name:		MATE_ROV_2017_DNO.ino
 Created:	16.11.2016 22:23:19
 Author:	JIukaviy
*/

#include "Main.h"
Main_t* Main;

void setup() {
	Serial.begin(9600);

	FlashlightPeriphery_t* flashlight_periphery = new FlashlightPeriphery_t(13);
	PeripheryManager_t* periphery_manager = new PeripheryManager_t(flashlight_periphery);

	byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
	Communicator_t* communicator = new UDPCommunicator_t(mac, 3000);

	Main = new Main_t(communicator, nullptr, nullptr, periphery_manager);
}

void loop() {
	Main->Loop();
}
