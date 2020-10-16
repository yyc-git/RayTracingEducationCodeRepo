#ifndef _RAY_H_
#define _RAY_H_

#include <CppUtil/Basic/HeapObj.h>
#include <glm/glm.hpp>

using namespace glm;
namespace RTX 
{
	class Ray : public CppUtil::Basic::HeapObj {
		HEAP_OBJ_SETUP(Ray)
	public:
		Ray(const glm::vec3 & origin = glm::vec3(0), const glm::vec3 & dir = glm::vec3(1)) : orig(origin), dir(dir) {}
		
		inline glm::vec3 origin() const { return orig; }
		inline glm::vec3 direction() const { return dir; }
		inline glm::vec3 at(float t) const { return orig + t * dir; }
	private:
		glm::vec3 orig;
		glm::vec3 dir;
	};
}
#endif //!_RAY_H_