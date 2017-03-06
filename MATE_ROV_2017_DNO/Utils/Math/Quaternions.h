#pragma once

inline float quatLength(float q0, float q1, float q2, float q3) {
	return sqrtf(q0*q0 + q1*q1 + q2*q2 + q3*q3);
}

inline void quatNormalize(
	float q0, float q1, float q2, float q3,
	float& out_q0, float& out_q1, float& out_q2, float& out_q3) 
{
	float len = quatLength(q0, q1, q2, q3);
	out_q0 = q0 / len;
	out_q1 = -q1 / len;
	out_q2 = -q2 / len;
	out_q3 = -q3 / len;
}

inline void quatInvert(
	float q0, float q1, float q2, float q3,
	float& out_q0, float& out_q1, float& out_q2, float& out_q3) 
{
	out_q0 = q0;
	out_q1 = -q1;
	out_q2 = -q2;
	out_q3 = -q3;
	quatNormalize(q0, q1, q2, q3, q0, q1, q2, q3);
}

inline void quatMulQuat(
	float q0_0, float q0_1, float q0_2, float q0_3,
	float q1_0, float q1_1, float q1_2, float q1_3,
	float& out_q0, float& out_q1, float& out_q2, float& out_q3) 
{
	out_q0 = q0_0 * q1_0 - q0_1 * q1_1 - q0_2 * q1_2 - q0_3 * q1_3;
	out_q1 = q0_0 * q1_1 + q0_1 * q1_0 + q0_2 * q1_3 - q0_3 * q1_2;
	out_q2 = q0_0 * q1_2 - q0_1 * q1_3 + q0_2 * q1_0 + q0_3 * q1_1;
	out_q3 = q0_0 * q1_3 + q0_1 * q1_2 - q0_2 * q1_1 + q0_3 * q1_0;
}

inline void quatMulVector(
	float q0, float q1, float q2, float q3, 
	float x, float y, float z, 
	float& out_q0, float& out_q1, float& out_q2, float& out_q3) 
{
	out_q0 = -q1 * x - q2 * y - q3 * z;
	out_q1 = q0 * x + q2 * z - q3 * y;
	out_q2 = q0 * y - q1 * z + q3 * x;
	out_q3 = q0 * z + q1 * y - q2 * x;
}

inline void quatTransformVector(
	float q0, float q1, float q2, float q3,
	float x, float y, float z,
	float& out_x, float& out_y, float& out_z) 
{
	float t0, t1, t2, t3;
	quatMulVector(q0, q1, q2, q3, x, y, z, t0, t1, t2, t3);
	quatInvert(q0, q1, q2, q3, q0, q1, q2, q3);
	float t;
	quatMulQuat(t0, t1, t2, t3, q0, q1, q2, q3, t, out_x, out_y, out_z);
}

inline void quatToYpr(
	float q0, float q1, float q2, float q3,
	float& y, float& p, float& r) 
{
	float qx2 = q1 * q1;
	float qy2 = q2 * q2;
	float qz2 = q3 * q3;
	r = atan2f(2 * (q1 * q0 + q2 * q3), 1 - 2 * (qx2 + qy2));
	p = asinf(2 * (q2 * q0 - q3 * q1));
	y = atan2f(2 * (q3 * q0 + q1 * q2), 1 - 2 * (qy2 + qz2));
}