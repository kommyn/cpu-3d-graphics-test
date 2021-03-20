#pragma once

#include "GraphUtils.h"

class BaseCamera
{
protected:
	e3Dg::Vector3f m_cameraPos;
	e3Dg::Vector3f m_cameraForward;
	e3Dg::Vector3f m_cameraUp;
	e3Dg::Vector3f m_cameraRight;
	float m_cameraPitch;
	float m_cameraYaw;
	float m_cameraSpeed;

public:
	virtual ~BaseCamera() = default;

	
};

