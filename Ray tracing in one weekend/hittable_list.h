#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
public:
	hittable_list() {}
	hittable_list(shared_ptr<hittable> object) { add(object); }
	
	void clear() { objects.clear(); }
	void add(shared_ptr<hittable> object) { objects.push_back(object); }

	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;

public:
	std::vector<shared_ptr<hittable>> objects;
};

//对于一条特定光线，对所有物体进行判断，找到最近的点，后面的被遮挡
bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	hit_record temp_rec;
	bool hit_anything = false;
	auto closest_so_far = t_max;

	for (const auto& object : objects) {
		//这边传入的tmax是closest_so_far，为了寻找更近的点
		if (object->hit(r, t_min, closest_so_far, temp_rec)) {
			hit_anything = true;
			//找到当前t的最小值，从而找到离相机最近的点
			closest_so_far = temp_rec.t;	
			rec = temp_rec;
		}
	}

	return hit_anything;
}

#endif
