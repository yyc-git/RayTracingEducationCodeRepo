#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <random>
#include <glm/glm.hpp>
namespace Define {
	using namespace glm;

	const std::string str_Chapter = "01_in_a_Weekend";
	const std::string str_Subchapter = "08_6_Diffuse_Formulation";
	const std::string str_WindowTitle = str_Chapter + "_" + str_Subchapter;

	const size_t val_fps = 60;
	const float infinity = std::numeric_limits<double>::infinity();
	const float pi = 3.141592653589793f;

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
	vec3 random_unit_vector() {
		auto a = random_float(0.0f, 2.0f * pi);
		auto z = random_float(-1.0f, 1.0f);
		auto r = sqrt(1 - z * z);
		return vec3(r*cos(a), r*sin(a), z);
	}

	vec3 random_in_hemisphere(const vec3& normal) {
		vec3 in_unit_sphere = random_in_unit_sphere();
		if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
			return in_unit_sphere;
		else
			return -in_unit_sphere;
	}

}

#endif // !_DEFINES_H_


