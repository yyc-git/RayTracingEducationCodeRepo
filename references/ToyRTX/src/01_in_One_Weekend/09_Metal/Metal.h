#ifndef _METAL_H_
#define _METAL_H_

#include "Material.h"
#include "Defines.h"
#include "Ray.h"

namespace RTX {
	using namespace CppUtil::Basic;
	using namespace Define;
	class Metal : public Material{
	public:
		Metal(float r, float g, float b, float fuzz = 0.0f) :albedo(r,g,b), fuzz(glm::min(fuzz, 1.0f)) {}
		Metal(const glm::rgb & specular, float fuzz = 0.0f) :albedo(specular), fuzz(fuzz) {}
		// 返回值为 true 说明光线继续传播
		// 返回值为 false 说明光线不再传播
		virtual bool scatter(CppUtil::Basic::Ptr<Ray> ray, const Hit_record& rec, rgb& attenuation, CppUtil::Basic::Ptr<Ray>& scattered) const override
		{
			vec3 reflected = reflect(normalize(ray->direction()), rec.normal);
			scattered = ToPtr(new Ray(rec.p, reflected + fuzz * random_in_unit_sphere()));
			attenuation = albedo;
			return (dot(scattered->direction(), rec.normal) > 0);
		}

		float Fuzz() const { return fuzz; }
	protected:
		float fuzz;
		rgb albedo;
	};
}

#endif // !_METAL_H_
