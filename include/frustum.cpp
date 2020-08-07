#include "frustum.h"

enum Planes
{
	Near,
	Far,
	Left,
	Right,
	Top,
	Bottom,
};

fp Plane::distanceToPoint(const vec3& point) const
{
	return vec3::dot(point, normal) + distanceToOrigin;
}

void ViewFrustum::update(const mat4x4& mat) noexcept
{
	// left
	m_planes[Planes::Left].normal.x = mat.m03 + mat.m00;
	m_planes[Planes::Left].normal.y = mat.m13 + mat.m10;
	m_planes[Planes::Left].normal.z = mat.m23 + mat.m20;
	m_planes[Planes::Left].distanceToOrigin = mat.m33 + mat.m30;

	// right
	m_planes[Planes::Right].normal.x = mat.m03- mat.m00;
	m_planes[Planes::Right].normal.y = mat.m13- mat.m10;
	m_planes[Planes::Right].normal.z = mat.m23- mat.m20;
	m_planes[Planes::Right].distanceToOrigin = mat.m33- mat.m30;

	// bottom
	m_planes[Planes::Bottom].normal.x = mat.m03+ mat.m01;
	m_planes[Planes::Bottom].normal.y = mat.m13+ mat.m11;
	m_planes[Planes::Bottom].normal.z = mat.m23+ mat.m21;
	m_planes[Planes::Bottom].distanceToOrigin = mat.m33+ mat.m31;

	// top
	m_planes[Planes::Top].normal.x = mat.m03- mat.m01;
	m_planes[Planes::Top].normal.y = mat.m13- mat.m11;
	m_planes[Planes::Top].normal.z = mat.m23- mat.m21;
	m_planes[Planes::Top].distanceToOrigin = mat.m33- mat.m31;

	// near
	m_planes[Planes::Near].normal.x = mat.m03+ mat.m02;
	m_planes[Planes::Near].normal.y = mat.m13+ mat.m12;
	m_planes[Planes::Near].normal.z = mat.m23+ mat.m22;
	m_planes[Planes::Near].distanceToOrigin = mat.m33+ mat.m32;

	// far
	m_planes[Planes::Far].normal.x = mat.m03- mat.m02;
	m_planes[Planes::Far].normal.y = mat.m13- mat.m12;
	m_planes[Planes::Far].normal.z = mat.m23- mat.m22;
	m_planes[Planes::Far].distanceToOrigin = mat.m33- mat.m32;

	for (auto& plane : m_planes)
	{
		fp length = plane.normal.length();
		plane.normal /= length;
		plane.distanceToOrigin /= length;
	}

}
//sources:
//AABB: C:\Users\Gebruiker\Downloads\MineCraft-One-Week-Challenge-master\MineCraft-One-Week-Challenge-master\Source\Physics
//frustum: C:\Users\Gebruiker\Downloads\MineCraft-One-Week-Challenge-master\MineCraft-One-Week-Challenge-master\Source\Maths
bool ViewFrustum::isBoxInFrustum(const vec3 position, const vec3 size) const noexcept
{
	//return true;
	bool result = true;
	for (auto& plane : m_planes)
	{
		vec3 res = position;
		vec3 normal = plane.normal;
		//getvp
		if (normal.x > 0)res.x += size.x;
		if (normal.y > 0)res.y += size.y;
		if (normal.z > 0)res.z += size.z;
		if (plane.distanceToPoint(res) < 0) 
			return false;
		else 
		{
			res = position;
			//getvn
			if (normal.x < 0)res.x += size.x;
			if (normal.y < 0)res.y += size.y;
			if (normal.z < 0)res.z += size.z;
			if (plane.distanceToPoint(res) < 0)
				result = true;
		}
	}
	return result;
}