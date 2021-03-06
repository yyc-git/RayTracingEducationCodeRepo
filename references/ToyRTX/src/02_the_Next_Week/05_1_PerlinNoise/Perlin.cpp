#include "Perlin.h"
#include "Defines.h"
using namespace glm;
using namespace RTX;

Perlin::Perlin() {
	ranfloat.resize(MaxCount);
	for (int i = 0; i < MaxCount; ++i)
		ranfloat[i] = Define::random_float();
	perlin_generate_perm(perm_x);
	perlin_generate_perm(perm_y);
	perlin_generate_perm(perm_z);
}

float Perlin::noise(const vec3& p) const {
	auto u = p.x - floor(p.x);
	auto v = p.y - floor(p.y);
	auto w = p.z - floor(p.z);

	auto i = static_cast<int>(4 * p.x) & 255;
	auto j = static_cast<int>(4 * p.y) & 255;
	auto k = static_cast<int>(4 * p.z) & 255;

	return ranfloat[perm_x[i] ^ perm_y[j] ^ perm_z[k]];
}

void Perlin::perlin_generate_perm(std::vector<int>& perm) {
	//auto p = new int[MaxCount];
	perm.resize(MaxCount);
	for (int i = 0; i < Perlin::MaxCount; i++)
		perm[i] = i;

	permute(perm, MaxCount);
}

void Perlin::permute(std::vector<int>& perm, int n) {
	for (int i = n - 1; i > 0; i--) {
		int target = Define::random_int(0, i);
		int tmp = perm[i];
		perm[i] = perm[target];
		perm[target] = tmp;
	}
}
