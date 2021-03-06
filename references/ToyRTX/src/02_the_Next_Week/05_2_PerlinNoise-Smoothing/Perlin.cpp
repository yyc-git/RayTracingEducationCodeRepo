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

	int i = floor(p.x);
	int j = floor(p.y);
	int k = floor(p.z);
	float c[2][2][2];

	for (int di = 0; di < 2; di++)
		for (int dj = 0; dj < 2; dj++)
			for (int dk = 0; dk < 2; dk++)
				c[di][dj][dk] = ranfloat[
					perm_x[(i + di) & 255] ^
						perm_y[(j + dj) & 255] ^
						perm_z[(k + dk) & 255]
				];

	return Define::trilinear_interp(c, u, v, w);
}

void Perlin::perlin_generate_perm(std::vector<int>& perm) {
	perm.resize(MaxCount);
	for (int i = 0; i < Perlin::MaxCount; i++)
		perm[i] = i;

	permute(perm);
}

void Perlin::permute(std::vector<int>& perm) {
	for (int i = perm.size() - 1; i > 0; --i) {
		int target = Define::random_int(0, i);
		int tmp = perm[i];
		perm[i] = perm[target];
		perm[target] = tmp;
	}
}
