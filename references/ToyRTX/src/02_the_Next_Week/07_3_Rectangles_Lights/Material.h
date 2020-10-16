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
		// ����ֵΪ true ˵�����߼�������
		// ����ֵΪ false ˵�����߲��ٴ���
		Hit_record r;
		virtual rgb emitted(const vec2& uv, const vec3& p) const {return rgb(0.0f);}
		virtual bool scatter(CppUtil::Basic::Ptr<Ray> ray, const Hit_record& rec,
			rgb& attenuation, CppUtil::Basic::Ptr<Ray>& scattered) const
		{
			return false;
		}
	};
}

#endif // !_MATERIAL_H_
