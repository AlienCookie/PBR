#ifndef GEOMT
#define GEOMT


typedef float vec4f[4];
typedef float vec3f[3];
typedef float vec2f[2];

class Geometry
{
public:
	static bool rayTriangleIntersection(vec3f V1, vec3f V2, vec3f V3, vec3f O, vec3f D, float & point);
	static bool checkEdge(vec3f V1, vec3f V2, vec3f TV1, vec3f TV2, vec3f TV3);
	static bool triangleTriangleIntersection(vec3f T1V1, vec3f T1V2, vec3f T1V3, vec3f T2V1, vec3f T2V2, vec3f T2V3);
};

#endif