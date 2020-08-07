#include "collisions.h"

//returns wether a rectangle intersects with another rectangle
bool collides2d(rectangle2 r1, rectangle2 r2)
{
	return r1.pos00.x + r1.size.x > r2.pos00.x && r1.pos00.x < r2.pos00.x + r2.size.x &&//x
		r1.pos00.y + r1.size.y > r2.pos00.y && r1.pos00.y < r2.pos00.y + r2.size.y;//y
}

//returns wether a image intersects with another image on alpha
bool collides2d(Image* img1, mat3x3 transform1, Image* img2, mat3x3 transform2)
{
	//inverse transform of img2
	mat3x3 inverse2 = transform2.Inverse();//graphics to image2
	mat3x3 tr1to2 = mat3x3::cross(inverse2, transform1);//image1 to graphics to image2
	cint precision = 5;
	//now check for every 5 pixels
	for (int j = 0; j < img1->height; j += precision)
	{
		for (int i = 0; i < img1->width; i+= precision)
		{
			color* imgi = img1->colors + i + j * img1->width;
			if (imgi->a) {//filled at img1
				const vec2 projected = tr1to2.multPointMatrix(vec2(i, j));// projected on image 2
				if (projected.x >=0 && projected.y >= 0 && projected.x < img2->width && projected.y < img2->height &&//pixel in bounds of img2
					(img2->colors + (int)projected.x + (int)projected.y * img2->width)->a)//filled at img2, collision
				{
					return true;//collided
				}
			}
		}
	}
	return false;
}

//returns wether a complex 2d shape intersects with another complex 2d shape
bool collides2d(const std::vector<vec2> v1, std::vector<vec2> v2, const bool& checkinside)
{
	vec2 lastv1 = *(v1.end() - 1);
	for (vec2 curv1 : v1)
	{
		bool inside = false;
		vec2 lastv2 = *(v2.end() - 1);
		for (vec2 curv2 : v2)
		{
			if (lines_intersect_2d(lastv1, curv1, lastv2, curv2))return true;
			if (pointleftofline(curv1, lastv2, curv2))inside = !inside;
			lastv2 = curv2;
		}
		lastv1 = curv1;
		if (inside)return true;
	}
	return false;
}

bool collides3d(const vec3& p0, const vec3& p1, const vec3& boxp0, const vec3& boxp1)
{
	fp tmin, tmax;
	return collidedistance3d(p0, p1, boxp0, boxp1, tmin, tmax);
}
//collision of a ray with a box
//p0: from
//p1: to
//source:
//https://tavianator.com/fast-branchless-raybounding-box-intersections/
//explanation:
//https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
//collision point = p0 + (p1 - p0).normalized() * t
bool collidedistance3d(const vec3& p0, const vec3& p1, const vec3& boxp0, const vec3& boxp1, fp& tmin, fp& tmax)
{
	const vec3 normal = (p1 - p0).normalized();
	const vec3 inversenormal = 1 / normal;
	fp tx0 = (boxp0.x - p0.x) * inversenormal.x;//t when intersecting with the boxp0.x plane
	fp tx1 = (boxp1.x - p0.x) * inversenormal.x;//t when intersecting with the boxp1.x plane

	tmin = min(tx0, tx1);
	tmax = max(tx0, tx1);


	fp ty0 = (boxp0.y - p0.y) * inversenormal.y;//t when intersecting with the boxp0.y plane
	fp ty1 = (boxp1.y - p0.y) * inversenormal.y;//t when intersecting with the boxp1.y plane

	tmin = max(tmin, min(ty0, ty1));
	tmax = min(tmax, max(ty0, ty1));
	
	fp tz0 = (boxp0.z - p0.z) * inversenormal.z;//t when intersecting with the boxp0.z plane
	fp tz1 = (boxp1.z - p0.z) * inversenormal.z;//t when intersecting with the boxp1.z plane

	tmin = max(tmin, min(tz0, tz1));
	tmax = min(tmax, max(tz0, tz1));

	//nearest point?
	return tmax >= tmin && tmax >= 0;//tmax >= 0 because else you could hit behind you
}

//checks if an endless ray from p to the right intersects with the line
bool pointleftofline( const vec2& point, const vec2& p0, const vec2& p1) 
{
	return ((p0.y >= point.y) != (p1.y >= point.y)) &&//check y
		(point.x <= (p1.x - p0.x) * (point.y - p0.y) / (p1.y - p0.y) + p0.x);//check x
}
//https://stackoverflow.com/questions/11716268/point-in-polygon-algorithm
bool PointInPolygon(const vec2& point, const std::vector<vec2>& points) {
	int i, j, nvert = points.size();
	bool c = false;

	for (i = 0, j = nvert - 1; i < nvert; j = i++) {
		if (pointleftofline(point,points[i],points[j]))
			c = !c;
	}

	return c;
}
//returns wether two lines intersect
//source:
//https://gamedev.stackexchange.com/questions/12243/how-do-i-find-the-intersection-of-two-lines
bool lines_intersect_2d( const vec2& p0,  const vec2& p1,  const vec2& p2,const vec2 & p3
	//, vec2* i const = 0
)
{
	vec2 const s1 = p1 - p0;
	vec2 const s2 = p3 - p2;

	vec2 const u = p0 - p2;

	fp const ip = 1.f / (-s2.x * s1.y + s1.x * s2.y);

	fp const s = (-s1.y * u.x + s1.x * u.y) * ip;
	fp const t = (s2.x * u.y - s2.y * u.x) * ip;

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
		//if (i)* i = p0 + (s1 * t);
		return true;
	}

	return false;
}
//http://viclw17.github.io/2018/07/16/raytracing-ray-sphere-intersection/
//center = vec3(0,0,0)
bool collideraysphere(const vec3& rayorigin, const vec3& raydirection, const fp radius, fp& t0, fp& t1) {
	const vec3 oc = rayorigin;// - center;
	cfp a = raydirection.lengthsquared();
	cfp b = 2.0 * vec3::dot(oc, raydirection);
	cfp c = vec3::dot(oc, oc) - radius * radius;
	cfp discriminant = b * b - 4 * a * c;
	if (discriminant < 0) {
		return false;
	}
	else {
		cfp division2a = (2.0 * a);
		cfp middle = -b / division2a;
		cfp radiusatintersection = sqrt(discriminant) / division2a;
		t0 = middle - radiusatintersection;
		t1 = middle + radiusatintersection;
		return true;
	}
}
bool collideraysphere(const vec3& rayorigin, const vec3& raydirection, const vec3& spherecentre, const fp radius, fp& t0, fp& t1)
{
	return collideraysphere(rayorigin - spherecentre, raydirection, radius, t0, t1);
}

//https://studiofreya.com/3d-math-and-physics/little-more-advanced-collision-detection-spheres/
bool collidespheresSpeed(const vec2& spherePos1, cfp& sphereRadius1, cvec2& sphereVelocity1, cvec2& spherePos2, cfp& sphereRadius2, cvec2& sphereVelocity2, fp& timePoint)
{


	vec2 s = spherePos1 - spherePos2; // vector between the centers of each sphere
	vec2 v = sphereVelocity1 - sphereVelocity2; // relative velocity between spheres
	fp r = sphereRadius1 + sphereRadius2;

	fp c = s.lengthsquared() - r * r; // if negative, they overlap
	if (c < 0.0) // if true, they already overlap
	{
		timePoint = .0;
		return true;
	}

	float a = v.lengthsquared();

	float b = vec2::dot(v, s);
	if (b >= 0.0)
		return false; // does not move towards each other

	float d = b * b - a * c;
	if (d < 0.0)
		return false; // no real roots ... no collision

	timePoint = (-b - sqrt(d)) / a;

	return true;

}
