#ifndef _ISOTROPIC_H_
#define _ISOTROPIC_H_

#include "Material.h"
#include "Defines.h"
#include "Ray.h"
#include "Texture.h"

namespace RTX {
	using namespace CppUtil::Basic;
	class Isotropic : public Material {
	public:
		Isotropic(const vec3& a) : albedo(ToPtr(new SolidColor(a))) {}
		Isotropic(CppUtil::Basic::Ptr<Texture> a) : albedo(a) {}

		virtual bool scatter(CppUtil::Basic::Ptr<Ray> ray, const Hit_record& rec, rgb& attenuation, CppUtil::Basic::Ptr<Ray>& scattered) const override
		{
			scattered = ToPtr(new Ray(rec.p, Define::random_in_unit_sphere(), ray->time()));
			attenuation = albedo->value(rec.uv, rec.p);
			return true;
		}
	protected:
		CppUtil::Basic::Ptr<Texture> albedo;
	};
}
#endif // !_ISOTROPIC_H_
