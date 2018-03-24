#version 440 core

in vec2 TexCoords;
in vec3 WorldPos;
in mat3 TBN;//vec3 Normal;

// Lights
//const int lightSourceNum = 10;
vec3 lightPositions[20];
vec3 lightColors[20];

uniform vec3 camPos;
uniform mat3 normalMatrix;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

uniform samplerCube skybox;

float PI = 3.141592;

out vec4 FragColor;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return nom / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}


vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}


void main()
{

	// Material params
	vec3 al = texture(albedoMap, TexCoords).rgb;
	vec3 albedo = vec3(pow(al.r, 2.2), pow(al.g, 2.2), pow(al.b, 2.2));
    vec3 norm = texture(normalMap, TexCoords).rgb;
    float metallic = texture(metallicMap, TexCoords).r;
    float roughness = pow(texture(roughnessMap, TexCoords).r, 1.5);
    float ao = texture(aoMap, TexCoords).r;
	
	norm = normalize(norm * 2.0 - 1.0);
	//vec3 shft = vec3(0.0, 0.0, 1.0) - norm;
	//mat3 TBN2 = transpose(TBN);
	vec3 normal = TBN * norm;//vec3(Normal.x + shft.x, Normal.y + shft.y, -Normal.z + shft.z);

	vec3 N = normalize(normal);
    vec3 V = TBN * normalize(-camPos - WorldPos);

    vec3 F0 = vec3(0.04);
    F0      = mix(F0, albedo, metallic);
    vec3 F  = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
	
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;	  
           
    // reflectance equation
    vec3 Lo = vec3(0.0);
	vec3 I = -V;
	vec3 R = reflect(I, normalize(normal));
    for(int i = 0; i < 20; i++)
    {
		R = normalize(vec3(R.x + sin(i/100), R.y - sin(i/100), R.z + sin(i/200)));
		//lightPositions[i] = R;
		
        // calculate per light radiance
        vec3 L =  TBN * normalize(lightPositions[i] - WorldPos);
        vec3 H = normalize(V + L);
        float dist = length(lightPositions[i] - WorldPos);
        float intens = 1.0;//1.0 / (dist * dist);
		
		//float ratio = 1.00 / 1.52;
		//vec3 Rr = refract(I, normalize(normal), ratio);
		vec3 coll = texture(skybox, R).rgb;
		lightColors[i] = vec3(pow(coll.r, 2), pow(coll.g, 2), pow(coll.b, 2));
		
        vec3 radiance  = lightColors[i] * intens;        
        
        // cook-torrance brdf
        float NDF = DistributionGGX(N, H, roughness);        
        float G   = GeometrySmith(N, V, L, roughness);      
        
        vec3 nominator    = NDF * G * F;
        float denominator = 4 * max(dot(V, H), 0.0) * max(dot(L, N), 0.0) + 0.001; 
        vec3 brdf = nominator / denominator;
            
        // add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);                
        Lo += (kD * albedo / PI + brdf) * radiance * NdotL; 
    }   
  
    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;
	
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));
	

    FragColor = vec4(color, 1.0);
}