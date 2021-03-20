#include "Camera.h"

Camera::Camera() {
	m_pos = { 0.0f, 0.0f, 2.0f };
	m_dir = { 0.0f, 0.0f, 1.0f };
	m_baseUp = { 0.0f, 1.0f, 0.0f };
	m_sensitivity = 0.00025f;
	m_pitch = 0.0f;
	m_yaw = M_PI / 2;
	m_speed = 0.5f;
	m_lookAt = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
}

void Camera::SetSpeed(const float& speed) {
	m_speed = speed;
}

void Camera::SetSensetivity(const float& sensetivity) {
	m_sensitivity = sensetivity;
}

const vgu::Matrix4x4& Camera::GetLookAtMatrix() const {
	return m_lookAt;
}
const vgu::Vector3f& Camera::GetPos() const {
	return m_pos;
}

const vgu::Vector3f& Camera::GetDir() const {
	return m_dir;
}

const vgu::Vector3f& Camera::GetUp() const {
	return m_up;
}

const vgu::Vector3f& Camera::GetRight() const {
	return m_right;
}

const float Camera::GetYaw() const {
	return m_yaw;
}

const float Camera::GetPitch() const {
	return m_pitch;
}

const float Camera::GetSpeed() const {
	return m_speed;
}

void Camera::RecalculateLookAtMatrix() {
	const float pitchCos = std::cos(m_pitch);
	const float pitchSin = std::sin(m_pitch);
	const float yawCos = std::cos(m_yaw);
	const float yawSin = std::sin(m_yaw);
	m_dir = vgu::normalize(vgu::Vector3f(yawCos * pitchCos, pitchSin, yawSin * pitchCos));
	vgu::Vector3f forward = -m_dir;
	m_right = vgu::normalize(crossProduct(m_baseUp, forward));
	m_up = vgu::crossProduct(m_right, forward);

	m_lookAt = {
		m_right[0], m_right[1], m_right[2], -vgu::dotProduct(m_pos, m_right),
		m_up[0], m_up[1], m_up[2], -vgu::dotProduct(m_pos, m_up),
		forward[0], forward[1], forward[2], -vgu::dotProduct(m_pos, forward),
		0, 0, 0, 1
	};
}

void Camera::RecalculatePosition(const PressedMovingKeys& keys, const float& elapsedTime) {
	if (keys.left) {
		m_pos -= m_right * m_speed * elapsedTime;
	}
	if (keys.right) {
		m_pos += m_right * m_speed * elapsedTime;
	}
	if (keys.forward) {
		m_pos -= m_dir * m_speed * elapsedTime;
	}
	if (keys.back) {
		m_pos += m_dir * m_speed * elapsedTime;
	}
	RecalculateLookAtMatrix();
}

void Camera::RecalculateAngles(const int& diffX, const int& diffY) {
	m_yaw -= diffX * m_sensitivity;
	if (m_yaw < 0) {
		m_yaw += 2 * M_PI;
	}
	if (m_yaw >= 2 * M_PI) {
		m_yaw -= 2 * M_PI;
	}
	m_pitch -= diffY * 0.00025;
	const float divededPi = M_PI / 2;
	if (m_pitch < -divededPi) {
		m_pitch = -divededPi + 0.0001;
	}
	if (m_pitch > divededPi) {
		m_pitch = divededPi - 0.0001;
	}
	RecalculateLookAtMatrix();
}
