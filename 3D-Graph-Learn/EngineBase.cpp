#include "EngineBase.h"

EngineBase::EngineBase() {
	m_elapsedTime = 1;
	m_time = 1;
}

void EngineBase::ComputeElapsedTime() {
	const float time = highResClockRatio * std::chrono::high_resolution_clock::now().time_since_epoch().count();
	m_elapsedTime = time - m_time;
	m_time = time;
}