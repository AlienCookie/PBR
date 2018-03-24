#include "stdafx.h"
#include "Geometry.h"
#include <cmath>
#include <iostream>

const float EPSILON = 0.00001f;

_inline float dotProduct(vec3f vec1, vec3f vec2) {
	return vec1[0] * vec2[0] + vec1[1] * vec2[1] + vec1[2] * vec2[2];
}

_inline void crossProduct(vec3f& res, vec3f vec1, vec3f vec2) {
	res[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
	res[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
	res[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];
}

_inline void subtract(vec3f& res, vec3f vec1, vec3f vec2) {
	res[0] = vec1[0] - vec2[0];
	res[1] = vec1[1] - vec2[1];
	res[2] = vec1[2] - vec2[2];
}

_inline void mult(vec3f& res, vec3f vec1,  float p) {
	res[0] = vec1[0] * p;
	res[1] = vec1[1] * p;
	res[2] = vec1[2] * p;
}

_inline void add(vec3f& res, vec3f vec1, vec3f vec2) {
	res[0] = vec1[0] + vec2[0];
	res[1] = vec1[1] + vec2[1];
	res[2] = vec1[2] + vec2[2];
}

_inline float length(vec3f vec) {
	return sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}

_inline float dist(vec3f vec1, vec3f vec2) {
	return sqrt(
		(vec1[0] - vec2[0]) * (vec1[0] - vec2[0]) +
		(vec1[1] - vec2[1]) * (vec1[1] - vec2[1]) +
		(vec1[2] - vec2[2]) * (vec1[2] - vec2[2]));
}

_inline void normalize(vec3f& vec) {
	float len = length(vec);
	vec[0] = vec[0] / len;
	vec[1] = vec[1] / len;
	vec[2] = vec[2] / len;
}

#define OUTV(v) std::cout<<v[0]<<" "<<v[1]<<" "<<v[2]<<std::endl;


bool Geometry::rayTriangleIntersection(vec3f V1, vec3f V2, vec3f V3, vec3f O, vec3f D, float& point)
{
	/* To check if ray intersect triangle
	*  we are using Möller–Trumbore algorithm
	*  due to it's speed and simpleness
	*/
	vec3f e1, e2;  //Edge1, Edge2
	vec3f P, Q, T;
	float det, inv_det, u, v;
	float t;
	

	//Find vectors for two edges sharing V1
	subtract(e1, V2, V1);
	subtract(e2, V3, V1);
	//Begin calculating determinant - also used to calculate u parameter
	crossProduct(P, D, e2);
	//if determinant is near zero, ray lies in plane of triangle or ray is parallel to plane of triangle
	det = dotProduct(e1, P);
	//NOT CULLING
	if (det > -EPSILON && det < EPSILON) return false;
	inv_det = 1.f / det;

	//calculate distance from V1 to ray origin
	subtract(T, O, V1);

	//Calculate u parameter and test bound
	u = dotProduct(T, P) * inv_det;
	//The intersection lies outside of the triangle
	if (u < 0.f || u > 1.f) return false;

	//Prepare to test v parameter
	crossProduct(Q, T, e1);

	//Calculate V parameter and test bound
	v = dotProduct(D, Q) * inv_det;
	//The intersection lies outside of the triangle
	if (v < 0.f || u + v  > 1.f) return false;

	t = dotProduct(e2, Q) * inv_det;

	if (t > EPSILON) { //ray intersection
		point = t;
		return true;
	}

	// No hit, no win
	return false;
}


/*float triangle_square(float a, float b, float c) {
	float p = (a + b + c) / 2;
	return sqrt(p*(p - a)*(p - b)*(p - c));
}

bool inside_triangle(vec3f P, vec3f A, vec3f B, vec3f C) {
	
	float AB = sqrt((A[0] - B[0])*(A[0] - B[0]) + (A[1] - B[1])*(A[1] - B[1]) + (A[2] - B[2])*(A[2] - B[2]));
	float BC = sqrt((B[0] - C[0])*(B[0] - C[0]) + (B[1] - C[1])*(B[1] - C[1]) + (B[2] - C[2])*(B[2] - C[2]));
	float CA = sqrt((A[0] - C[0])*(A[0] - C[0]) + (A[1] - C[1])*(A[1] - C[1]) + (A[2] - C[2])*(A[2] - C[2]));

	float AP = sqrt((P[0] - A[0])*(P[0] - A[0]) + (P[1] - A[1])*(P[1] - A[1]) + (P[2] - A[2])*(P[2] - A[2]));
	float BP = sqrt((P[0] - B[0])*(P[0] - B[0]) + (P[1] - B[1])*(P[1] - B[1]) + (P[2] - B[2])*(P[2] - B[2]));
	float CP = sqrt((P[0] - C[0])*(P[0] - C[0]) + (P[1] - C[1])*(P[1] - C[1]) + (P[2] - C[2])*(P[2] - C[2]));
	float diff = (triangle_square(AP, BP, AB) + triangle_square(AP, CP, CA) + triangle_square(BP, CP, BC)) - triangle_square(AB, BC, CA);
	if (fabs(diff) < EPSILON)
		return true;

	return false;
}*/


bool Geometry::checkEdge(vec3f V1, vec3f V2, vec3f TV1, vec3f TV2, vec3f TV3)
{
	float inters;
	vec3f origin = { V1[0], V1[1], V1[2] };
	vec3f dir;
	vec3f inPoint;
	vec3f temp;

	subtract(dir, V2, V1);
	normalize(dir);

	if (rayTriangleIntersection(TV1, TV2, TV3, origin, dir, inters)) 
	{
		mult(temp, dir, inters);
		add(inPoint, origin, temp);
		float a = abs((dist(inPoint, V1) + dist(inPoint, V2)) - dist(V1, V2));
		if (a < EPSILON) {
			return true;
		}
	}

	return false;
}

bool Geometry::triangleTriangleIntersection(
	vec3f T1V1, vec3f T1V2, vec3f T1V3,
	vec3f T2V1, vec3f T2V2, vec3f T2V3)
{

	if (
		(checkEdge(T1V1, T1V2, T2V1, T2V2, T2V3)) ||
		(checkEdge(T1V1, T1V3, T2V1, T2V2, T2V3)) ||

		(checkEdge(T2V1, T2V2, T1V1, T1V2, T1V3)) ||
		(checkEdge(T2V1, T2V3, T1V1, T1V2, T1V3))
		) {
		return true;
	}
	return false;
}