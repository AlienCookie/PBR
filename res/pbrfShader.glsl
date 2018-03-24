in vec3 v_pos;      // pixel view space position
in vec2 v_texcoord; // texture coords
in vec3 v_normal;   // normal
in vec3 v_binormal; // binormal (for TBN basis calc)

out vec4 color;

vec3 ambient[3];
vec3 diffuse[3];
vec3 specular[3];
vec3 transmittance[3];
vec3 emission[3];
float shininess;
float ior;       // index of refraction
float dissolve;

//uniform sampler2D map_Ka; // ambient texture
//uniform sampler2D map_Kd; // difuse texture
//uniform sampler2D map_Ks; // specular texture
//uniform sampler2D map_Ns; // specular highlight texture
//uniform sampler2D map_d;  // alpha texture

// PBR

float roughness;            // [0, 1] default 0
float metallic;             // [0, 1] default 0
float sheen;                // [0, 1] default 0
float clearcoat_thickness;  // [0, 1] default 0
float clearcoat_roughness;  // [0, 1] default 0
float anisotropy;           // aniso. [0, 1] default 0
float anisotropy_rotation;  // anisor. [0, 1] default 0

//uniform sampler2D map_Pr; // roughness texture
//uniform sampler2D map_Pm; // metalic texture
//uniform sampler2D map_Ps; // sheen texture
//uniform sampler2D map_Ke; // emissive texture
//uniform sampler2D norm;   // normal texture

