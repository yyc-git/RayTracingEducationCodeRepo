#ifndef _DIFFUSELIGHT_H_
#define _DIFFUSELIGHT_H_

#include "Material.h"
#include "Defines.h"
#include "Ray.h"
#include "Texture.h"

namespace RTX {
	using namespace CppUtil::Basic;
	class DiffuseLight : public Material {
	public:
		DiffuseLight(const vec3& a) : albedo(ToPtr(new SolidColor(a))) {}
		DiffuseLight(CppUtil::Basic::Ptr<Texture> a) : albedo(a) {}

		//virtual bool scatter(CppUtil::Basic::Ptr<Ray> ray, const Hit_record& rec, rgb& attenuation, CppUtil::Basic::Ptr<Ray>& scattered) const override
		//{
		//	return false;
		//}
		virtual rgb emitted(const vec2& uv, const vec3& p) const override 
		{
			return albedo->value(uv, p);
		}
	protected:
		CppUtil::Basic::Ptr<Texture> albedo;
	};
}

#endif // !_DIFFUSELIGHT_H_
