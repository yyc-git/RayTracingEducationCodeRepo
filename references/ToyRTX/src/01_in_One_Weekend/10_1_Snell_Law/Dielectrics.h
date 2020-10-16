#ifndef _REFRACTION_H_
#define _REFRACTION_H_

#include "Material.h"
#include "Defines.h"
#include "Ray.h"

namespace RTX {
	using namespace CppUtil::Basic;
	using namespace Define;
	class Refraction : public Material{
	public:
		Refraction(float refract, float r = 0.0f, float g = 0.0f, float b = 0.0f, float fuzz = 0.0f) :refract(refract), albedo(r,g,b), fuzz(glm::min(fuzz, 1.0f)) {}
		Refraction(float refract, const rgb & specular = rgb(0.0f), float fuzz = 0.0f) :refract(refract), albedo(specular), fuzz(fuzz) {}
		// ����ֵΪ true ˵�����߼�������
		// ����ֵΪ false ˵�����߲��ٴ���
		virtual bool scatter(CppUtil::Basic::Ptr<Ray> ray, const Hit_record& rec, rgb& attenuation, CppUtil::Basic::Ptr<Ray>& scattered) const override
		{
			attenuation = rgb(1.0);
			//attenuation = albedo;
			float etai_over_etat = rec.front_face ? (1.0f / refract) : refract;

			vec3 unit_direction = normalize(ray->direction());
			vec3 refracted = Define::refract(unit_direction, rec.normal, etai_over_etat);
			scattered = ToPtr(new Ray(rec.p, refracted + fuzz * random_in_unit_sphere()));
			return true;
		}

		float Fuzz() const { return fuzz; }
	protected:
		float fuzz;
		float refract;
		rgb albedo;
	};
}

#endif // !_REFRACTION_H_