#include "Perlin.h"
#include "Defines.h"
using namespace glm;
using namespace RTX;

Perlin::Perlin() {
	ranvec.resize(MaxCount);
	for (int i = 0; i < MaxCount; ++i)
		ranvec[i] = Define::random(-1.0f,1.0f);
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
	vec3 c[2][2][2];

	for (int di = 0; di < 2; di++)
		for (int dj = 0; dj < 2; dj++)
			for (int dk = 0; dk < 2; dk++)
				c[di][dj][dk] = ranvec[
					perm_x[(i + di) & 255] ^
						perm_y[(j + dj) & 255] ^
						perm_z[(k + dk) & 255]
				];

	return perlin_interp(c, u, v, w);
}
float Perlin::perlin_interp(vec3 c[2][2][2], float u, float v, float w) {
	auto uu = u * u * (3 - 2 * u);
	auto vv = v * v * (3 - 2 * v);
	auto ww = w * w * (3 - 2 * w);
	auto accum = 0.0;

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			for (int k = 0; k < 2; k++) {
				vec3 weight_v(u - i, v - j, w - k);
				accum += (i * uu + (1 - i) * (1 - uu))
					* (j * vv + (1 - j) * (1 - vv))
					* (k * ww + (1 - k) * (1 - ww))
					* dot(c[i][j][k], weight_v);
			}

	return accum;
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
