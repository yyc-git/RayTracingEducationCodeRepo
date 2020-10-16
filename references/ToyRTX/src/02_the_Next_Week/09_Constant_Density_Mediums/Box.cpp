#include <CppUtil/Basic/Math.h>

#include "Box.h"
#include "Ray.h"
#include "Rectangle.h"
#include "Defines.h"

using namespace RTX;
using namespace CppUtil::Basic;
using namespace glm;

Box::Box(const vec3& p0, const vec3& p1, CppUtil::Basic::Ptr<Material> material)
{
	box_min = p0;
	box_max = p1;

	sides.add(ToPtr(new RectangleXY(vec2(p0.x, p1.x), vec2(p0.y, p1.y), p1.z, material)));
	sides.add(ToPtr(new RectangleXY(vec2(p0.x, p1.x), vec2(p0.y, p1.y), p0.z, material)));

	sides.add(ToPtr(new RectangleXZ(vec2(p0.x, p1.x), vec2(p0.z, p1.z), p1.y, material)));
	sides.add(ToPtr(new RectangleXZ(vec2(p0.x, p1.x), vec2(p0.z, p1.z), p0.y, material)));

	sides.add(ToPtr(new RectangleYZ(vec2(p0.y, p1.y), vec2(p0.z, p1.z), p1.x, material)));
	sides.add(ToPtr(new RectangleYZ(vec2(p0.y, p1.y), vec2(p0.z, p1.z), p0.x, material)));
}

bool Box::hit(CppUtil::Basic::Ptr<Ray> ray, float t_min, float t_max, Hit_record& rec) const
{
	return sides.hit(ray, t_min, t_max, rec);
}

bool Box::boundingBox(float t0, float t1, AABB& output_box) const
{
	output_box = AABB(box_min, box_max);
    return true;
}