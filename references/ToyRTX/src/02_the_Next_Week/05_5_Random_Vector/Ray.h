#ifndef _RAY_H_
#define _RAY_H_

#include <CppUtil/Basic/HeapObj.h>
#include <glm/glm.hpp>

namespace RTX 
{
	using namespace glm;
	class Ray : public CppUtil::Basic::HeapObj {
		HEAP_OBJ_SETUP(Ray)
	public:
        Ray(const vec3 & origin = vec3(0), const vec3 & dir = vec3(1), float t = 0.0f) : orig(origin), dir(dir), times(t) {}

        inline vec3 origin( ) const { return orig; }
        inline vec3 direction( ) const { return dir; }
        inline float time( ) const { return times; }

        inline vec3 at(float t) const { return orig + t * dir; }
    private:
        vec3 orig;
        vec3 dir;
        float times;
	};
}
#endif //!_RAY_H_