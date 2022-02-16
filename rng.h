#ifndef DESERT_RACER_RNG
#define DESERT_RACER_RNG

#include <random>


namespace desert {
	class RNG
	{
	public:
		RNG(int min, int max);
		int getNumber();
		float getDecimalPoint();
	protected:
		static constexpr float kHundred = 100.0f;
		std::random_device mRandomDevice;
		std::mt19937 mGenerator;
		std::uniform_int_distribution<int> mDistribution;
	};
};

#endif