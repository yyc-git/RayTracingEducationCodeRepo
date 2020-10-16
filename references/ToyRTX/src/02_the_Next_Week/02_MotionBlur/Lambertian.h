#ifndef _LAMBERTIAN_H_
#define _LAMBERTIAN_H_

#include "Material.h"
#include "Defines.h"
#include "Ray.h"

namespace RTX {
	using namespace CppUtil::Basic;
	class Lambertian : public Material {
	public:
		Lambertian(float r, float g, float b) :albedo(r, g, b) {}
		Lambertian(const rgb & specular) :albedo(specular) {}
		// 返回值为 true 说明光线继续传播
		// 返回值为 false 说明光线不再传播
		virtual bool scatter(CppUtil::Basic::Ptr<Ray> ray, const Hit_record& rec, rgb& attenuation, CppUtil::Basic::Ptr<Ray>& scattered) const override
		{
			vec3 scatter_direction = rec.normal + Define::random_unit_vector();
			scattered = ToPtr(new Ray(rec.p, scatter_direction,ray->time()));
			attenuation = albedo;
			return true;
		}
	protected:
		rgb albedo;
	};
}

#endif // !_LAMBERTIAN_H_
