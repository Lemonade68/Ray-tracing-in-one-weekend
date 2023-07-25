#ifndef VEC3H
#define VEC3H

#include"rtweekend.h"

#include<math.h>
#include<stdlib.h>
#include<iostream>	

class vec3 {
public:
	vec3(){}
	vec3(double e0, double e1, double e2) { e[0] = e0, e[1] = e1, e[2] = e2; }
	
	inline double x() { return e[0]; }
	inline double y() { return e[1]; }
	inline double z() { return e[2]; }

	inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }	//负号
	inline double operator[](int i) const { return e[i]; }		//返回赋值——原值不可修改
	inline double& operator[](int i) { return e[i]; }			//返回引用——可修改

	inline vec3& operator+=(const vec3 &v2);
	inline vec3& operator*=(const double t);
	inline vec3& operator/=(const double t);

	inline double length() const { 
		return sqrt(squared_length()); 
	}

	inline double squared_length() const { 
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; 
	}

	void write_color(std::ostream &os, int samples_per_pixel) {
		// Divide the color total by the number of samples.
		auto scale = 1.0 / samples_per_pixel;
		auto r = sqrt(scale * e[0]);
		auto g = sqrt(scale * e[1]);
		auto b = sqrt(scale * e[2]);

		//clamp找不到标识符的原因：rtweekend.h中在clamp函数之前有include"vec.h"
		// Write the translated [0,255] value of each color component.
		os << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
			<< static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
			<< static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
	}

	//随机生成单位球体内的点
	inline static vec3 random() {
		return vec3(random_double(), random_double(), random_double());
	}

	inline static vec3 random(double min, double max) {
		return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}

public:
	double e[3];
};

vec3& vec3::operator+=(const vec3 &v2) {
	e[0] += v2.e[0];
	e[1] += v2.e[1];
	e[2] += v2.e[2];
	return *this;
}

vec3& vec3::operator*=(const double t) {
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

vec3& vec3::operator/=(const double t) {
	return *this *= (1 / t);
}

inline vec3 operator+(const vec3 &v1, const vec3 &v2) {
	return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline vec3 operator-(const vec3 &v1, const vec3 &v2) {
	return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

//用vec表示颜色时使用
inline vec3 operator*(const vec3 &v1, const vec3 &v2) {
	return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline vec3 operator*(const vec3 &v1, const double t) {
	//return v1 *= t;		//不能直接对const的v1进行乘法
	return vec3(t*v1.e[0], t*v1.e[1], t*v1.e[2]);
}

inline vec3 operator*(const double t, const vec3 &v1) {
	return v1 * t;			//直接利用上面写的*
}

inline vec3 operator/(const vec3 &v1, const double t) {
	return v1 * (1 / t);
}

//点乘
inline double dot(const vec3& v1, const vec3 &v2) {
	return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

//叉乘
inline vec3 cross(const vec3 &v1, const vec3& v2) {
	return vec3((v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]),
		-(v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0]),
		(v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]));
}

inline vec3 unit_vector(vec3 v) {
	return v / v.length();
}

inline std::ostream& operator<<(std::ostream& os, const vec3& v) {
	return os << v.e[0] << " " << v.e[1] << " " << v.e[2];
}

//球体内随机生成一点
vec3 random_in_unit_sphere() {
	while (true) {
		auto p = vec3::random(-1, 1);
		if (p.squared_length() >= 1) continue;
		return p;
	}
}

//球面上随机生成一点：用极坐标形式
vec3 random_unit_vector() {
	auto a = random_double(0, 2 * pi);
	auto z = random_double(-1, 1);
	auto r = sqrt(1 - z * z);
	return vec3(r*cos(a), r*sin(a), z);
}

//计算反射光线
vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2 * dot(v, n)*n;
}

//计算折射光线
vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
	auto cos_theta = dot(-uv, n);
	vec3 r_out_parallel = etai_over_etat * (uv + cos_theta * n);
	vec3 r_out_perp = -sqrt(1.0 - r_out_parallel.squared_length()) * n;
	return r_out_parallel + r_out_perp;
}

double schlick(double cosine, double ref_idx) {
	auto r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0)*pow((1 - cosine), 5);
}

//从一个单位小圆盘射出光线
vec3 random_in_unit_disk() {
	while (true) {
		auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
		if (p.squared_length() >= 1) continue;
		return p;
	}
}

using point3 = vec3;
using color = vec3;

#endif
