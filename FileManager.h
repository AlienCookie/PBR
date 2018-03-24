#ifndef FILE_MEN
#define FILE_MEN

#include <fstream>
#include <streambuf>
#include <vector>


struct vec2f {
	float x, y;
	bool operator ==(vec2f & v2) { return ((this->x == v2.x) && (this->y == v2.y)); }
};

struct vec3f {
	float x, y, z;
	bool operator ==(vec3f & v2) { return ((this->x == v2.x) && (this->y == v2.y) && (this->z == v2.z)); }
};

struct vec4f {
	float x, y, z, w;
	bool operator ==(vec4f & v2) { return ((this->x == v2.x) && (this->y == v2.y) && (this->z == v2.z) && (this->w == v2.w)); }
};

class FileManager
{
public:
	// convert file into char array
	static char* fileAsCharArray(std::string source);
	static bool loadOBJ(
		const char * path, 
		std::vector<vec3f> & out_vertices,
		std::vector<vec2f> & out_uvs, 
		std::vector<vec3f> & out_normals,
		std::vector<unsigned> & indices,
		unsigned int *divise = NULL);
};

#endif