#ifndef _MOVECAMERA_H_
#define _MOVECAMERA_H_

#include <CppUtil/Basic/HeapObj.h>
#include <glm/glm.hpp>
#include "Ray.h"
#include "Defines.h"
#define Defocus 0
namespace RTX {
    using namespace glm;
    using namespace Define;
    class MoveCamera : public CppUtil::Basic::HeapObj {
        HEAP_OBJ_SETUP(MoveCamera)
    public:
        // fov is top to bottom in degrees
        MoveCamera(vec3 lookfrom, vec3 lookat, vec3 vup,
            float vfov, float aspect_ratio, float aperture, float focus_dist, float t0 = 0.0f, float t1 = 0.0f)
        {
#if Defocus
            float theta = degrees_to_radians(vfov);
            float h = tan(theta / 2.0f);
            float viewport_height = 2.0f * h;
            float viewport_width = aspect_ratio * viewport_height;

            w = normalize(lookfrom - lookat);
            u = normalize(cross(vup, w));
            v = cross(w, u);

            origin = lookfrom;
            horizontal = focus_dist * viewport_width * u;
            vertical = focus_dist * viewport_height * v;
            lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - focus_dist * w;
            lens_radius = aperture / 2.0f;
#else
            float theta = degrees_to_radians(vfov);
            float h = tan(theta / 2.0f);
            float viewport_height = 2.0f * h;
            float viewport_width = aspect_ratio * viewport_height;

            auto w = normalize(lookfrom - lookat);
            auto u = normalize(cross(vup, w));
            auto v = cross(w, u);

            origin = lookfrom;
            horizontal = viewport_width * u;
            vertical = viewport_height * v;
            lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - w;
#endif
            time0 = t0;
            time1 = t1;
        }

        virtual CppUtil::Basic::Ptr<Ray> GenRay(float s, float t) const
        {
#if Defocus
            vec3 rd = lens_radius * random_in_unit_disk( );
            vec3 offset = u * rd.x + v * rd.y;
            return ToPtr(new Ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset, random_float(time0, time1)));
#else
            return ToPtr(new Ray(origin, lower_left_corner + s * horizontal + t * vertical - origin, random_float(time0, time1)));
#endif
        }

    private:
        vec3 origin;
        vec3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
        vec3 u, v, w;
        float lens_radius;
        float time0, time1;
    };
}

#endif // !_MOVECAMERA_H_
