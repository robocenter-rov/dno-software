/*
 Name:		MATE_ROV_2017_DNO.ino
 Created:	16.11.2016 22:23:19
 Author:	JIukaviy
*/

#include "Main.h"
Main_t* Main;

void setup() {
	SensorManager_t* sensor_manager = new SensorManager_t();
}

void loop() {
	Main->Loop();
}
