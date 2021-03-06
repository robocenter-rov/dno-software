#include "UdpConnectionProvider.h"
#include <Wire.h>
#include "PCA96685Motor.h"
#include "MS5803SensorDepth.h"
#include "Debug.h"
#include "UARTConnectionProvider.h"
#include "TestPCAMain.h"
#include "SimpleMain.h"
#include "PCA9685BrushlessMotor.h"

Main_t* Main;

void setup() {
	Exceptions::Init();
	Wire.begin();

	pinMode(13, OUTPUT);

	Serial.begin(9600);
	auto pwm1 = new Adafruit_PWMServoDriver(0x40);
	auto pwm2 = new Adafruit_PWMServoDriver(0x41);
	auto pwm3 = new Adafruit_PWMServoDriver(0x43);

	pwm1->begin();
	pwm1->setPWMFreq(60);
	pwm2->begin();
	pwm2->setPWMFreq(60);
	pwm3->begin();
	pwm3->setPWMFreq(500);

	//motors->AddMotor(new PCA96685Motor_t(pwm1, 6, 5, 7)); // 0
	Motors_t* motors = new Motors_t(/*
		new PCA96685Motor_t(pwm1, 6, 5, 7),
		new PCA96685Motor_t(pwm1, 4, 3, 2), //1
		new PCA96685Motor_t(pwm1, 14, 13, 15),
		new PCA96685Motor_t(pwm1, 10, 11, 12), //2
		new PCA96685Motor_t(pwm2, 2, 1, 0),
		new PCA96685Motor_t(pwm2, 4, 5, 3),
		new PCA96685Motor_t(pwm3, 4, 3, 2),
		new PCA96685Motor_t(pwm3, 4, 3, 2)*/
		new PCA9685BrushlessMotor(pwm3, 0, 2120, 3430),
		new PCA9685BrushlessMotor(pwm3, 13, 2120, 3430),
		new PCA9685BrushlessMotor(pwm3, 1, 2120, 3430),
		new PCA9685BrushlessMotor(pwm3, 12, 2120, 3430),
		new PCA9685BrushlessMotor(pwm3, 2, 2120, 3430),
		new PCA9685BrushlessMotor(pwm3, 15, 2120, 3430),
		new PCA9685BrushlessMotor(pwm3, 3, 2120, 3430),
		new PCA9685BrushlessMotor(pwm3, 14, 2120, 3430)
	); // 5
	motors->StopMotors();
	/*
	motors->AddMotor(new PCA96685Motor_t(pwm1, 6, 5, 7)); // 0
	motors->AddMotor(new PCA96685Motor_t(pwm1, 4, 3, 2)); // 1 
	motors->AddMotor(new PCA96685Motor_t(pwm1, 14, 13, 15)); // 2
	motors->AddMotor(new PCA96685Motor_t(pwm1, 10, 11, 12)); // 3
	motors->AddMotor(new PCA96685Motor_t(pwm2, 2, 1, 0)); // 4
	motors->AddMotor(new PCA96685Motor_t(pwm2, 4, 5, 3)); // 5
	*/
	SensorRotation_t* rotation_sensor = new SensorRotation_t();
	MS5803SensorDepth_t* depth_sensor = new MS5803SensorDepth_t(ADDRESS_HIGH, ADC_4096);
	/*
	float acc_bias[] = { 2.01769, -2.51933, -5.07049 };
	float acc_scale[] = { 257.626, 257.65, 247.943 };
	float gyro_bias[] = { 5.60526, 30.625, -17.5811 };
	float gyro_scale = 1000;
	*/
	
	float acc_bias[] = { -6.4322, -6.7379, -28.5567 };
	float acc_scale[] = { 253.999, 254.304, 249.271 };
	float gyro_bias[] = { 23.750677, 24.255642, -5.145686 };
	float gyro_scale = 950;

	float magn_cal_matrix[3][3] =
	{
		{ 2.815472, -0.021771, 0.057076 },
		{ -0.021771, 2.746060, -0.079616 },
		{ 0.057076, -0.079616, 3.345993 }
	};
	float magn_bias[3] = { 97.428119, -39.218888, -21.747817 };

	rotation_sensor->SetCalibrationValues(
		gyro_bias, gyro_scale,
		acc_bias, acc_scale,
		magn_cal_matrix, magn_bias);

	rotation_sensor->GyroZeroCalibrate(250, 100);

	SensorManager_t* sensor_manager = new SensorManager_t(rotation_sensor, depth_sensor);

	MotorsForceDistributor_t* motors_force_distributor = new MotorsForceDistributor_t(sensor_manager);

	AutoYaw_t* auto_yaw = new AutoYaw_t(sensor_manager);
	AutoPitch_t* auto_pitch = new AutoPitch_t(sensor_manager);
	AutoRoll_t* auto_roll = new AutoRoll_t(sensor_manager);
	AutoDepth_t* auto_depth = new AutoDepth_t(sensor_manager);

	Movement_t* movement = new Movement_t(motors, motors_force_distributor, auto_yaw, auto_pitch, auto_roll, auto_depth);
	
	FlashlightPeriphery_t* flashlight_periphery = new FlashlightPeriphery_t(8);
	
	Serial1.begin(38400);
	BluetoothPeriphery_t* bluetooth_periphery = new BluetoothPeriphery_t(&Serial1);

	ServoPeriphery_t* camera1_periphery = new ArduinoServoMotor_t(rotation_sensor, 5);
	ServoPeriphery_t* camera2_periphery = new ArduinoServoMotor_t(rotation_sensor, 7);

	ServoPeriphery_t* servo1_periphery = new PCA9685ServoMotor_t(rotation_sensor, pwm1, 0);
	ServoPeriphery_t* servo2_periphery = new PCA9685ServoMotor_t(rotation_sensor, pwm1, 1);

	ManipulatorPeriphery_t* manipulator_periphery = new ManipulatorPeriphery_t(
		new PCA96685Motor_t(pwm2, 9, 10, 8, 2000),
		new PCA96685Motor_t(pwm2, 12, 11, 13, 2000)
	);

	PeripheryManager_t* periphery_manager = new PeripheryManager_t(flashlight_periphery, manipulator_periphery, bluetooth_periphery, 
		camera1_periphery, camera2_periphery, servo1_periphery, servo2_periphery);

	byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
	ConnectionProvider_t* connection_provider = new UdpConnectionProvider_t(300, mac, IPAddress(192, 168, 1, 50), 3000);
	// Serial3.begin(19200);
	// ConnectionProvider_t* connection_provider = new UARTConnectionProvider(&Serial3, 300);
	auto communicator = new SimpleCommunicator_t(connection_provider);

	Main = new SimpleMain_t(communicator, movement, sensor_manager, periphery_manager);

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

bool g = false;

void loop() {
	//digitalWrite(13, g = !g);
	Main->Loop();
	if(Exceptions::SmthngWrong()) {
		Serial.println("Some shit happens");
		auto node = Exceptions::GetList().FrontNode();
		while(node) {
			Serial.println(node->GetData().GetFullMessage());
			node = node->GetNext();
		}
		Exceptions::Release();
		/*while (true) {
			digitalWrite(13, HIGH);
			delay(1000);
			digitalWrite(13, LOW);
			delay(1000);
		}*/
	}
}
