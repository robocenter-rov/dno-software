#include "Main\StdMain.h"
#include "Communication\Communicators\StdCommunicator.h"
#include "UdpConnectionProvider.h"
#include <Wire.h>
#include "PCA96685Motor.h"
#include <SparkFun_MS5803_I2C.h>
#include "Debug.h"

Main_t* Main;

void setup() {
	ResourceLocker::Init();
	Exceptions::Init();
	Wire.begin();
	Serial2.begin(9600);
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
	motors->AddMotor(new PCA96685Motor_t(pwm1, 6, 5, 7));
	motors->AddMotor(new PCA96685Motor_t(pwm1, 4, 3, 2));
	motors->AddMotor(new PCA96685Motor_t(pwm1, 14, 13, 15));
	motors->AddMotor(new PCA96685Motor_t(pwm1, 10, 11, 12));
	motors->AddMotor(new PCA96685Motor_t(pwm2, 2, 1, 0));
	motors->AddMotor(new PCA96685Motor_t(pwm2, 4, 5, 3));

	SensorRotation_t* rotation_sensor = new SensorRotation_t();
	
	//MS5803SensorDepth_t* depth_sensor = new MS5803SensorDepth_t(ADDRESS_HIGH, ADC_4096);
	
	SensorManager_t* sensor_manager = new SensorManager_t(rotation_sensor, depth_sensor);

	MotorsForceDistributor_t* motors_force_distributor = new MotorsForceDistributor_t(sensor_manager, motors);

	Movement_t* movement = new Movement_t(motors, motors_force_distributor, nullptr);
	
	FlashlightPeriphery_t* flashlight_periphery = new FlashlightPeriphery_t(13);

	ManipulatorPeriphery_t* manipulator_periphery = new ManipulatorPeriphery_t(
		new PCA9685ServoMotor_t(pwm1, 0),
		new PCA9685ServoMotor_t(pwm1, 1)
		//new PCA96685Motor_t(pwm2, 12, 11, 13)
		//new PCA96685Motor_t(pwm2, 10, 9, 8)
	);

	PeripheryManager_t* periphery_manager = new PeripheryManager_t(flashlight_periphery, manipulator_periphery);

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
