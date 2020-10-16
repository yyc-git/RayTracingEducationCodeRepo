#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <CppUtil/RTX/ImgWindow.h>
#include <CppUtil/Basic/Image.h>
#include <CppUtil/Basic/LambdaOp.h>
#include <CppUtil/Basic/Timer.h>

#include <omp.h>
#include <random>
#include <glm/glm.hpp>

namespace Define {
	using namespace glm;
    using namespace RTX;
    const std::string str_Chapter = "02_the_Next_Week";
	const std::string str_Subchapter = "10_Scene_Testing_All";
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

	inline float length_2(const vec3& pos)
	{
		return pos.x * pos.x + pos.y * pos.y + pos.z * pos.z;
	}

    inline vec3 random_in_unit_sphere() {
		while (true) {
			auto p = random(-1.0f, 1.0f);
            float len = length_2(p);
			if (len >= 1) continue;
			return p;
		}
	}

    inline vec3 random_unit_vector() {
		auto a = random_float(0.0f, 2.0f * pi);
		auto z = random_float(-1.0f, 1.0f);
		auto r = sqrt(1 - z * z);
		return vec3(r*cos(a), r*sin(a), z);
	}

    inline vec3 random_in_hemisphere(const vec3& normal) {
		vec3 in_unit_sphere = random_in_unit_sphere();
		if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
			return in_unit_sphere;
		else
			return -in_unit_sphere;
	}

    inline vec3 reflect(const vec3& v, const vec3& n) {
		return v - 2 * dot(v, n)*n;
	}

    inline vec3 refract(const vec3& uv, const vec3& n, float etai_over_etat) {
		auto cos_theta = dot(-uv, n);
		vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
        auto length_squared = length_2(r_out_perp);
		//length_squared *= length_squared;
		vec3 r_out_parallel = -sqrt(fabs(1.0f - length_squared)) * n;
		return r_out_perp + r_out_parallel;
	}

    inline float schlick(float cosine, float ref_idx) {
		float r0 = (1.0f - ref_idx) / (1.0f + ref_idx);
		r0 = r0 * r0;
		return r0 + (1.0f - r0) * pow((1.0f - cosine), 5);
	}

    inline float degrees_to_radians(float vfov)
	{
		return vfov * pi / 180.0f;
	}

    inline vec3 random_in_unit_disk( ) {
        while (true) {
            auto p = vec3(random_float(-1.0f, 1.0f), random_float(-1.0f, 1.0f), 0.0f);
            auto len = length_2(p);
            if (len >= 1.0f) continue;
            return p;
        }
    }

    inline int random_int(int min, int max) {
        // Returns a random integer in [min,max].
        return static_cast<int>(random_float(min, max + 1));
    }

    inline void get_sphere_uv(const vec3& p, vec2& uv) {
        auto phi = atan2(p.z, p.x);
        auto theta = asin(p.y);
        uv.x = 1.0f - (phi + pi) / (2.0f * pi);
        uv.y = (theta + pi / 2.0f) / pi;
    }

	inline float trilinear_interp(float c[2][2][2], float u, float v, float w) {
		auto accum = 0.0;
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				for (int k = 0; k < 2; k++)
					accum += (i * u + (1.0 - i) * (1.0 - u)) *
					(j * v + (1.0 - j) * (1.0 - v)) *
					(k * w + (1.0 - k) * (1.0 - w)) * c[i][j][k];

		return accum;
	}
}

#endif // !_DEFINES_H_


