#pragma once

namespace twob
{

	class Time
	{
	public:
		static void update_delta(float current_frame);

		// returns delta time.
		static float delta();

	private:
		static float m_delta;
		static float m_last_frame;
	};


}
