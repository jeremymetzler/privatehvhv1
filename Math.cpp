#include "Main.h"

Math g_Math = Math();


void Math::VecAng(const Vector& forward, Vector& angles)
{
	float tmp, yaw, pitch;

	if (forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		if (forward[2] > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
		if (yaw < 0)
			yaw += 360;

		tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = (atan2(-forward[2], tmp) * 180 / M_PI);
		if (pitch < 0)
			pitch += 360;
	}

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}

void Math::AngVec(const Vector& angles, Vector* forward, Vector* right, Vector* up) {
	float sp, sy, sr, cp, cy, cr;

	auto sin_cos = [](float r, float* s, float* c) {
		*s = sin(r);
		*c = cos(r);
	};

	sin_cos(DEG2RAD(angles.x), &sp, &cp);
	sin_cos(DEG2RAD(angles.y), &sy, &cy);
	sin_cos(DEG2RAD(angles.z), &sr, &cr);

	if (forward) {
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if (right) {
		right->x = -1 * sr * sp * cy + -1 * cr * -sy;
		right->y = -1 * sr * sp * sy + -1 * cr * cy;
		right->z = -1 * sr * cp;
	}

	if (up) {
		up->x = cr * sp * cy + -sr * -sy;
		up->y = cr * sp * sy + -sr * cy;
		up->z = cr * cp;
	}
}

Vector Math::CalcAng(const Vector & a, const Vector & b) {
	Vector angles;
	Vector delta;
	delta.x = (a.x - b.x);
	delta.y = (a.y - b.y);
	delta.z = (a.z - b.z);

	float hyp = sqrt(delta.x * delta.x + delta.y * delta.y);
	angles.x = static_cast<float>(atanf(delta.z / hyp) * 57.295779513082f);
	angles.y = static_cast<float>(atanf(delta.y / delta.x) * 57.295779513082f);

	angles.z = 0.0f;
	if (delta.x >= 0.0) { angles.y += 180.0f; }
	return angles;
}

float Math::NormEuler(float angle)
{
	if (angle > 180.f || angle < -180.f)
	{
		auto revolutions = round(abs(angle / 360.f));

		if (angle < 0.f)
		{
			angle += 360.f * revolutions;
		}
		else
		{
			angle -= 360.f * revolutions;
		}
	}

	return angle;
}

void sMatrixGetColumn(const matrix3x4_t& in, int column, Vector& out)
{
	out.x = in[0][column];
	out.y = in[1][column];
	out.z = in[2][column];
}

void sMatrixAngles(const matrix3x4_t& matrix, Vector& angles) // valve sdk
{
	float forward[3];
	float left[3];
	float up[3];

	forward[0] = matrix[0][0];
	forward[1] = matrix[1][0];
	forward[2] = matrix[2][0];
	left[0] = matrix[0][1];
	left[1] = matrix[1][1];
	left[2] = matrix[2][1];
	up[2] = matrix[2][2];

	float xyDist = sqrtf(forward[0] * forward[0] + forward[1] * forward[1]);

	if (xyDist > 0.001f)
	{
		angles[1] = RAD2DEG(atan2f(forward[1], forward[0]));
		angles[0] = RAD2DEG(atan2f(-forward[2], xyDist));
		angles[2] = RAD2DEG(atan2f(left[2], up[2]));
	}
	else
	{
		angles[1] = RAD2DEG(atan2f(-left[0], left[1]));
		angles[0] = RAD2DEG(atan2f(-forward[2], xyDist));
		angles[2] = 0;
	}
}

void Math::MatrixAngles(const matrix3x4_t& matrix, Vector& angles, Vector& position)
{
	sMatrixGetColumn(matrix, 3, position);
	sMatrixAngles(matrix, angles);
}