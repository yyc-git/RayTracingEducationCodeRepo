#ifndef _RECTANGLE_H_
#define _RECTANGLE_H_
#include "Hitable.h"

namespace RTX {
	using namespace glm;
	using namespace RTX;

	class RectangleXY : public Hitable
	{
	public:
		RectangleXY(const vec2& x, const vec2& y, float k, CppUtil::Basic::Ptr<Material> material = nullptr);
		virtual bool hit(CppUtil::Basic::Ptr<Ray> ray, float t_min, float t_max, Hit_record& rec) const override;
        virtual bool boundingBox(float t0, float t1, AABB& output_box) const override;
	protected:
		vec2 x, y;
		float k;
		CppUtil::Basic::Ptr<Material> material;
	};

	class RectangleYZ : public Hitable
	{
	public:
		RectangleYZ(const vec2& y, const vec2& z, float k, CppUtil::Basic::Ptr<Material> material = nullptr);
		virtual bool hit(CppUtil::Basic::Ptr<Ray> ray, float t_min, float t_max, Hit_record& rec) const override;
		virtual bool boundingBox(float t0, float t1, AABB& output_box) const override;
	protected:
		vec2 y, z;
		float k;
		CppUtil::Basic::Ptr<Material> material;
	};

	class RectangleXZ : public Hitable
	{
	public:
		RectangleXZ(const vec2& x, const vec2& z, float k, CppUtil::Basic::Ptr<Material> material = nullptr);
		virtual bool hit(CppUtil::Basic::Ptr<Ray> ray, float t_min, float t_max, Hit_record& rec) const override;
		virtual bool boundingBox(float t0, float t1, AABB& output_box) const override;
	protected:
		vec2 x, z;
		float k;
		CppUtil::Basic::Ptr<Material> material;
	};
}
#endif 
