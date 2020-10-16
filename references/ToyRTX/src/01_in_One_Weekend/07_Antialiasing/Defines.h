#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <random>
namespace Define {
	const std::string str_Chapter = "01_in_a_Weekend";
	const std::string str_Subchapter = "07_Antialiasing";
	const std::string str_WindowTitle = str_Chapter + "_" + str_Subchapter;

	const size_t val_fps = 60;
	const float infinity = std::numeric_limits<float>::infinity();


	inline float random_float()
	{
		static std::uniform_real_distribution<float> distribution(0.0, 1.0);
		static std::mt19937 generator;
		return distribution(generator);
	}

	inline float random_float(float min, float max) {
		// Returns a random real in [min,max).
		return min + (max - min)*random_float();
	}
}

#endif // !_DEFINES_H_


