#include "rectangle2.h"
#include "image.h"
#include "mat3x3.h"
#pragma once
bool collides2d(rectangle2 r1, rectangle2 r2);
bool collides2d(Image* img1, mat3x3 transform1, Image* img2, mat3x3 transform2);
bool collides2d(std::vector<vec2> v1, const std::vector<vec2> v2, const bool& checkinside);
bool collides3d(const vec3& p0, const vec3& p1, const vec3& boxp0, const vec3& boxp1);
bool PointInPolygon(const vec2& point, const std::vector<vec2>&  points);
bool collidedistance3d(const vec3& p0, const vec3& p1, const vec3& boxp0, const vec3& boxp1, fp& tmin, fp& tmax);
bool pointleftofline(const vec2& point, const vec2& p0, const vec2& p1);
bool lines_intersect_2d(vec2 const& p0, vec2 const& p1, const vec2& p2, vec2 const& p3);

bool collideraysphere(const vec3& rayorigin, const vec3& raydirection, const fp radius, fp& t0, fp& t1);
bool collideraysphere(const vec3& rayorigin, const vec3& raydirection, const vec3& spherecentre, const fp radius, fp& t0, fp& t1);
bool collidespheresSpeed(const vec2& spherePos1, cfp& sphereRadius1, cvec2& sphereVelocity1, cvec2& spherePos2, cfp& sphereRadius2, cvec2& sphereVelocity2, fp& time);
//watch out! velocities being modified
inline void sphereCollisionResponse(const vec2& spherePos1, cfp& sphereMass1, cvec2& spherePos2, cfp& sphereMass2, vec2& sphereVelocity1, vec2& sphereVelocity2)
{
	vec2 U1x, U1y, U2x, U2y, V1x, V1y, V2x, V2y;


	float m1, m2, x1, x2;
	vec2 v1temp, v1, v2, v1x, v2x, v1y, v2y, x(spherePos1 - spherePos2);

	x.normalize();
	v1 = sphereVelocity1;
	x1 = vec2::dot(x, v1);
	v1x = x * x1;
	v1y = v1 - v1x;
	m1 = sphereMass1;

	x = x * -1;
	v2 = sphereVelocity2;
	x2 = vec2::dot(x, v2);
	v2x = x * x2;
	v2y = v2 - v2x;
	m2 = sphereMass2;

	cfp frictionMult = 1;// 0.9;
	vec2 withoutFriction1 = vec2(v1x * (m1 - m2) / (m1 + m2) + v2x * (2 * m2) / (m1 + m2) + v1y);
	sphereVelocity2 = vec2(v1x * (2 * m1) / (m1 + m2) + v2x * (m2 - m1) / (m1 + m2) + v2y);
	sphereVelocity1 = lerp(sphereVelocity2, withoutFriction1, frictionMult);
	sphereVelocity2 = lerp(withoutFriction1, sphereVelocity2, frictionMult);
}