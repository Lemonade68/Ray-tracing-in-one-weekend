#ifndef HITTABLE_H
#define HITTABLE_H

#include"ray.h"

class material;

//Ҫ���ݲ����Ĵ��
struct hit_record {			//��¼����
	vec3 p;					//����λ��
	vec3 normal;			//����
	double t;				//t��ֵ
	bool front_face;
	shared_ptr<material> mat_ptr;	//����ָ��

	//��¼�淨��
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
