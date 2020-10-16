#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <random>
#include <glm/glm.hpp>

namespace Define {
	using namespace glm;

	const std::string str_Chapter = "01_in_a_Weekend";
	const std::string str_Subchapter = "11_Camera";
	const std::string str_WindowTitle = str_Chapter + "_" + str_Subchapter;

	const size_t val_fps = 60;
	const float infinity = (float)std::numeric_limits<double>::infinity();
	const float pi = 3.141592653589793f;

	inline float random_float()
	{
		static std::uniform_real_distribution<double> distribution(0.0, 1.0);
		static std::mt19937 generator;
		return (float)distribution(generator);
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

	vec3 reflect(const vec3& v, const vec3& n) {
		return v - 2 * dot(v, n)*n;
	}

	vec3 refract(const vec3& uv, const vec3& n, float etai_over_etat) {
		auto cos_theta = dot(-uv, n);
		vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
        auto length_squared = length(r_out_perp);
		length_squared *= length_squared;
		vec3 r_out_parallel = -sqrt(fabs(1.0f - length_squared)) * n;
		return r_out_perp + r_out_parallel;
	}

	float schlick(float cosine, float ref_idx) {
		float r0 = (1.0f - ref_idx) / (1.0f + ref_idx);
		r0 = r0 * r0;
		return r0 + (1.0f - r0) * pow((1.0f - cosine), 5);
	}

	float degrees_to_radians(float vfov)
	{
		return vfov * pi / 180.0f;
	}
}

#endif // !_DEFINES_H_


