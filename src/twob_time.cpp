#include "twob_time.h"

using namespace twob;

float Time::m_delta = 0.0f;
float Time::m_last_frame = 0.0f;

void Time::update_delta(float current_frame)
{
	m_delta = current_frame - m_last_frame;
	m_last_frame = current_frame;
}

float Time::delta()
{
	return m_delta;
}
