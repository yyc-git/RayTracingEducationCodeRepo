#ifndef _MOVESPHERE_H_
#define _MOVESPHERE_H_
#include "Hitable.h"
#include "Ray.h"

namespace RTX {
	using namespace glm;
	using namespace RTX;
	class MoveSphere : public Hitable
	{
	public:
        MoveSphere(const vec3& center0 = vec3(0.0f), const vec3& center1 = vec3(0.0f),
            float t0 = 0.0f, float t1 = 0.0f, float radius = 1.0f,
            CppUtil::Basic::Ptr<Material> material = nullptr);
		virtual bool hit(CppUtil::Basic::Ptr<Ray> ray, float t_min, float t_max, Hit_record& rec) const override;

        vec3 MoveSphere::center(float time) const;
		float Radius() { return radius; }
	protected:
		vec3 center0, center1;
		float radius, time0, time1;
		CppUtil::Basic::Ptr<Material> material;
	};
}
#endif 
