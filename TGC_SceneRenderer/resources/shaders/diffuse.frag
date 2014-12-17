#version 440 core
// Rendering Params Constants
//--include Constrains
// Structures
//--include Structures
struct Matrix
{
	mat4 modelViewProjection;
	mat4 modelView;
	mat4 model;
	mat4 view;
	mat4 projection;
	mat3 normal;
};
struct Light 
{	
	vec3 position;
	vec3 color;
	float intensity;
	float attenuation;
	// Spot Light - Directional
	vec3 direction;
	float innerConeAngle;
	float outerConeAngle;
	// Control parameters
	int lightType;
};
struct Material {
	vec3 ka;
	vec3 kd;
	vec3 ks;
	float shininess;
};
// Input data
in vec2 texCoord;
in vec3 normal;
in vec3 position;
// Uniforms
uniform sampler2D diffuseMap;
uniform int lightsCount;
uniform Matrix inputMatrices;
uniform Material material;
uniform Light light[MAX_LIGHTS];
// Output fragment data
layout (location = 0) out vec4 fragColor;

void phong(vec3 pos, vec3 norm, out vec3 ambient, out vec3 diffuse, out vec3 specular, Light pLight)
{
	vec3 normal = normalize(norm);
	vec3 lightVector = pLight.position - pos;
	float lightDistance = length(lightVector);
	vec3 lightDirection = normalize(lightVector);
	float lambertTerm = max(dot(normal, lightDirection), 0.0);

	ambient = pLight.intensity * pLight.color * material.ka;
	diffuse = specular = vec3(0.0f);

	if(lambertTerm > 0.0f)
	{
		diffuse = material.kd * pLight.intensity * pLight.color * lambertTerm;
		vec3 viewDirection = normalize(-pos);
		vec3 reflectDirection = reflect(-lightDirection, normal);
		float specAngle = max(dot(reflectDirection, viewDirection), 0.0f);
		specular =  material.ks * pLight.intensity * pLight.color + pow(specAngle, material.shininess);
	}
}

void main()
{
	vec3 ambientPerLight, diffusePerLight, specularPerLight;
	vec3 ambientTotal, diffuseTotal, specularTotal;
	vec4 texColor = texture(diffuseMap, texCoord);
	// Set Initial Value
	ambientTotal = diffuseTotal = specularTotal = vec3(0.0f);

	for(int i = 0; i < lightsCount; i++) {
		ambientPerLight = diffusePerLight = specularPerLight = vec3(0.0f);
		phong(position, normal, ambientPerLight, diffusePerLight, specularPerLight, light[i]);
		// Acumulate per light
		ambientTotal += ambientPerLight;
		diffuseTotal += diffusePerLight;
		specularTotal += specularPerLight;
	}

	fragColor = vec4(ambientTotal + diffuseTotal + specularTotal, 1.0f) * texColor;
}