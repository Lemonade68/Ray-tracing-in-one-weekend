#ifndef HITTABLE_H
#define HITTABLE_H

#include"ray.h"

class material;

//要传递参数的打包
struct hit_record {			//记录交点
	vec3 p;					//交点位置
	vec3 normal;			//法相
	double t;				//t的值
	bool front_face;
	shared_ptr<material> mat_ptr;	//材质指针

	//记录面法相
	inline void set_face_normal(const ray& r, const vec3& outward_normal) {
		front_face = dot(outward_normal, r.direction()) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class hittable {
public:
	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec)const = 0;
};

#endif // !HITTABLE_H
