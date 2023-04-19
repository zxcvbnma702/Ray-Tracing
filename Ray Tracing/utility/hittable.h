#pragma once

#include "ray.h"

struct hit_record{
	point3 p;
	vec3 normal;
	double t;
	//判断光线射入方向
	bool front_face;

	//光线方向与法向向量方向相同
	inline void set_face_normal(const ray& r, const vec3& outword_normal) {
		front_face = dot(r.direction(), outword_normal) < 0;
		normal = front_face ? outword_normal : -outword_normal;
	}
};

class hittable {
public:
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};




