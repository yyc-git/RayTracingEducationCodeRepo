#ifndef _PERLIN_H_
#define _PERLIN_H_

#include "Defines.h"
#include "glm/glm.hpp"

namespace RTX {
	using namespace glm;
	class Perlin {
	public:
		Perlin();
		float noise(const vec3& p) const;
		float turb(const vec3& p, int depth = 7) const;
	private:
		static const int MaxCount = 256;
		std::vector<vec3> ranvec;
		std::vector<int> perm_x;
		std::vector<int> perm_y;
		std::vector<int> perm_z;

		static void perlin_generate_perm(std::vector<int>& perm);
		static void permute(std::vector<int>& perm);
		static float perlin_interp(vec3 c[2][2][2], float u, float v, float w);

	};
}
#endif 
