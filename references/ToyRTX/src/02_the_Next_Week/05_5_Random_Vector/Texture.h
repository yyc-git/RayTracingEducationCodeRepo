#ifndef TEXTURE_H
#define TEXTURE_H

#include <CppUtil/Basic/HeapObj.h>
#include <glm/glm.hpp>
#include "Perlin.h"

namespace RTX {
	using namespace glm;
	class Texture : public CppUtil::Basic::HeapObj {
		HEAP_OBJ_SETUP(Texture)
	public:
		virtual vec3 value(const vec2& uv, const vec3& p) const = 0;
	};

	class SolidColor : public Texture {
	public:
		SolidColor() {}
		SolidColor(vec3 c) : color(c) {}

		SolidColor(float red, float green, float blue)
			: SolidColor(vec3(red, green, blue)) {}

		virtual vec3 value(const vec2& uv, const vec3& p) const override {
			return color;
		}

	private:
		vec3 color;
	};

	class CheckerTexture : public Texture {
	public:
		CheckerTexture() {}
		CheckerTexture(CppUtil::Basic::Ptr<Texture> t0, CppUtil::Basic::Ptr<Texture> t1)
			: even(t0), odd(t1) {}
		CheckerTexture(rgb& c1, rgb& c2): even(ToPtr(new SolidColor(c1))),
			odd(ToPtr(new SolidColor(c2))) {}

		virtual rgb value(const vec2& uv, const vec3& p) const override {
			auto sines = sin(10.0f * p.x) * sin(10.0f * p.y) * sin(10.0f * p.z);
			if (sines < 0)
				return odd->value(uv, p);
			else
				return even->value(uv, p);
		}

	public:
		CppUtil::Basic::Ptr<Texture> odd;
		CppUtil::Basic::Ptr<Texture> even;
	};

	class NoiseTexture : public Texture {
	public:
		NoiseTexture(float sc = 0.2f) : scale(sc) {}

		virtual rgb value(const vec2& uv, const vec3& p) const override {
			return rgb(1.0f) * 0.5f * (1.0f + noise.noise(scale * p));
		}

	public:
		float scale;
		Perlin noise;
	};
}
#endif