#pragma once

#include "GraphUtils.h"

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

struct PressedMovingKeys {
	bool left, right, forward, back;
};

// TODO: Try to create abstract class for every camera that can be created (for this I need to read something about different
//       camera types
class Camera
{
private:
	e3Dg::Vector3f m_pos;
	e3Dg::Vector3f m_dir;
	e3Dg::Vector3f m_up;
	e3Dg::Vector3f m_right;
	e3Dg::Vector3f m_baseUp;
	float m_pitch;
	float m_yaw;
	float m_speed;
	float m_sensitivity;
	e3Dg::Matrix4x4 m_lookAt;

	void RecalculateLookAtMatrix();
public:
	Camera();

	void SetSpeed(const float& speed);
	void SetSensetivity(const float& sensetivity);

	const e3Dg::Matrix4x4& GetLookAtMatrix() const;
	const e3Dg::Vector3f& GetPos() const;
	const e3Dg::Vector3f& GetDir() const;
	const e3Dg::Vector3f& GetUp() const;
	const e3Dg::Vector3f& GetRight() const;
	const float GetYaw() const;
	const float GetPitch() const;
	const float GetSpeed() const;

	void RecalculateAngles(const int& diffX, const int& diffY);
	void RecalculatePosition(const PressedMovingKeys& keys, const float& elapsedTime);
};

