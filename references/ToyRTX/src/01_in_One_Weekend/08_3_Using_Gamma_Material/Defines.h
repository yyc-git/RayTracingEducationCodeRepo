#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <random>
#include <glm/glm.hpp>
namespace Define {
	using namespace glm;

	const std::string str_Chapter = "01_in_a_Weekend";
	const std::string str_Subchapter = "08_3_Using_Gamma_Material";
	const std::string str_WindowTitle = str_Chapter + "_" + str_Subchapter;

	const size_t val_fps = 60;
	const float infinity = std::numeric_limits<double>::infinity();


	inline float random_float()
	{
		static std::uniform_real_distribution<double> distribution(0.0, 1.0);
		static std::mt19937 generator;
		return distribution(generator);
	}

	inline float random_float(float min, float max) {
		// Returns a random real in [min,max).
		return min + (max - min)*random_float();
	}

	inline static vec3 random() {
		return vec3(random_float(), random_float(), random_float());
	}

	inline static vec3 random(float min, float max) {
		return vec3(random_float(min, max), random_float(min, max), random_float(min, max));
	}
	vec3 random_in_unit_sphere() {
		while (true) {
			auto p = random(-1.0f, 1.0f);
			float length = p.x * p.x + p.y * p.y + p.z * p.z;
			if (length >= 1) continue;
			return p;
		}
	}
}

#endif // !_DEFINES_H_


