#ifndef _PERLIN_H_
#define _PERLIN_H_
//#include "Hitable.h"
#include "Defines.h"
#include "glm/glm.hpp"

namespace RTX {
	using namespace glm;
	class Perlin {
	public:
		Perlin();
		float noise(const vec3& p) const;
	private:
		static const int MaxCount = 256;
		std::vector<float> ranfloat;
		std::vector<int> perm_x;
		std::vector<int> perm_y;
		std::vector<int> perm_z;

		static void perlin_generate_perm(std::vector<int>& perm);
		static void permute(std::vector<int>& perm, int n);
	};
}
#endif 
