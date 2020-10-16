#ifndef _LAMBERTIAN_H_
#define _LAMBERTIAN_H_

#include "Material.h"
#include "Defines.h"
#include "Ray.h"
#include "Texture.h"

namespace RTX {
	using namespace CppUtil::Basic;
	class Lambertian : public Material {
	public:
		Lambertian(const vec3& a) : albedo(ToPtr(new SolidColor(a))) {}
		Lambertian(CppUtil::Basic::Ptr<Texture> a) : albedo(a) {}

		virtual bool scatter(CppUtil::Basic::Ptr<Ray> ray, const Hit_record& rec, rgb& attenuation, CppUtil::Basic::Ptr<Ray>& scattered) const override
		{
			vec3 scatter_direction = rec.normal + Define::random_unit_vector();
			scattered = ToPtr(new Ray(rec.p, scatter_direction,ray->time()));
			attenuation = albedo->value(rec.uv, rec.p);
			return true;
		}
	protected:
		CppUtil::Basic::Ptr <Texture> albedo;
	};
}

#endif // !_LAMBERTIAN_H_
