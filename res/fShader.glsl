#version 440 core

in vec3 position;
in vec2 texCor;
in vec3 normal;


layout(location = 0)out vec4 o_fragColor;

uniform sampler2D map_Kd;
uniform sampler2D map_Bump;
uniform mat3 normalMatrix;

uniform vec3 lightPos;
 
uniform vec3 mambient;  //gl_FrontMaterial
uniform vec3 mdiffuse;
uniform vec3 mspecular;
uniform vec3 transmittance;
uniform float shininess;
 
uniform vec3 lambient;  //gl_LightSource[0]
uniform vec3 ldiffuse;
uniform vec3 lspecular;

// ----- Constants field -----
const float cfZero = 0.0;
const float cfOne = 1.0;

void main()
{
		vec4 normal4 = texture(map_Bump, texCor);
		//normal = normalMatrix * normal;//4.xyz;

		float dist=length(position-lightPos)/10.;   //distance from light-source to surface
        float att=1.0/(1.0+0.1*dist+0.01*dist*dist);    //attenuation (constant,linear,quadric)
        vec3 ambient=mambient*lambient; //the ambient light
       
        vec3 surf2light=normalize(lightPos-position);
        vec3 norm=normalize(normalMatrix * (normal));
        float dcont=max(0.0,dot(-norm,surf2light));
        vec3 diffuse=dcont*(mdiffuse*ldiffuse);
       
        vec3 surf2view=normalize(-position);
        vec3 reflection=reflect(surf2light,-norm);
       
        float scont=pow(max(0.0,dot(surf2view,reflection)),shininess);
        vec3 specular=scont*lspecular*mspecular;
       
       // gl_FragColor=vec4((ambient+diffuse+specular)*att,1.0);  //<- don't forget the parenthesis (ambient+diffuse+specular)
	   
		o_fragColor = /*normalize(texture(map_Kd, texCor) + */vec4((ambient+diffuse+specular)*att, transmittance[0])/*)*/;
		//o_fragColor = texture(map_Kd, texCor);
}