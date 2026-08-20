#include "EngineBase.h"

EngineBase::EngineBase() {
	m_elapsedTime = 1;
	m_time = 1;
}

void EngineBase::ComputeElapsedTime() {
	const double time = highResClockRatio * std::chrono::high_resolution_clock::now().time_since_epoch().count();
	m_elapsedTime = static_cast<float>(time - m_time);
	m_time = time;
}