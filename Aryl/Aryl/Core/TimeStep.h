#pragma once

namespace Aryl 
{
	class Timestep
	{
	public:
		Timestep() {}
		Timestep(float time);

		inline float GetSeconds() const { return myTime; }
		inline float GetMilliseconds() const { return myTime * 1000.0f; }

		operator float() { return myTime; }

	private:
		float myTime = 0.0f;
	};

}