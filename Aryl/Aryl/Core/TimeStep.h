#pragma once

namespace Aryl 
{
	class Timestep
	{
	public:
		Timestep() {}
		Timestep(double time);

		inline double GetSeconds() const { return myTime; }
		inline double GetMilliseconds() const { return myTime * 1000; }

		operator double() { return myTime; }
		operator const double() const { return myTime; }

	private:
		double myTime = 0;
	};

}