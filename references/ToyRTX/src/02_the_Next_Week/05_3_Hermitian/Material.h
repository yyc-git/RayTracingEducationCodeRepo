#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <CppUtil/Basic/HeapObj.h>
#include <glm/glm.hpp>
#include <vector>

#include "Ray.h"
#include "Hitable.h"

namespace RTX {
	class Material : public CppUtil::Basic::HeapObj{
		HEAP_OBJ_SETUP(Material)
	public:
		// 返回值为 true 说明光线继续传播
		// 返回值为 false 说明光线不再传播
		Hit_record r;
		virtual bool scatter(CppUtil::Basic::Ptr<Ray> ray, const Hit_record& rec, rgb& attenuation, CppUtil::Basic::Ptr<Ray>& scattered) const = 0;
	};
}

#endif // !_MATERIAL_H_
