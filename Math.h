#pragma once


class Math
{
public:
	void VecAng(const Vector& forward, Vector& angles);
	void AngVec(const Vector& angles, Vector* forward = nullptr, Vector* right = nullptr, Vector* up = nullptr);
	Vector CalcAng(const Vector& a, const Vector& b);
	float NormEuler(float angle);
	void MatrixAngles(const matrix3x4_t& matrix, Vector& angles, Vector& position);
};

extern Math g_Math;