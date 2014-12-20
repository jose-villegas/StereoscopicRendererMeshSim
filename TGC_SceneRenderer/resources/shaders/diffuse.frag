#version 440 core
//--include shared_data.glsl
// Input data
in vec2 texCoord;
in vec3 normal;
in vec3 position;
// Uniforms
uniform sampler2D diffuseMap;
uniform Material material;
//uniform Light light[MAX_LIGHTS];
// Output fragment data
layout (location = 0) out vec4 fragColor;

void phong(vec3 pos, vec3 norm, out vec3 ambient, out vec3 diffuse, out vec3 specular)
{
	vec3 normal = normalize(norm);
	vec3 lightDirection = normalize(light.source[0].position - pos);
	vec3 viewDirection = normalize(-pos);
	vec3 reflectDirection = normalize(reflect(lightDirection, normal));
	// Calc Ambient
	ambient = material.ambient;
}

void main()
{
	vec3 normal = normalize(normal);
	vec4 surfaceColor = texture(diffuseMap, texCoord);
	vec3 lightDirection = normalize(light.source[0].position - position);
	vec3 viewDirection = normalize(-position); // Camera position at vec3(0)

	// Calculate Ambient
	vec3 ambient = material.ambient + light.source[0].color * light.source[0].intensity * surfaceColor.rgb;

	// Diffuse
	float diffuseCoefficient = max(0.0f, dot(normal, lightDirection));
	vec3 diffuse = diffuseCoefficient * surfaceColor.rgb * material.diffuse * light.source[0].color * light.source[0].intensity;

	// Specular
	float specularCoefficient = 0.0;
	if(diffuseCoefficient > 0.0) {
		vec3 reflectDirection = normalize(reflect(-lightDirection, normal));
		specularCoefficient = pow(max(0.0, dot(viewDirection, reflectDirection)), material.shininess);
	}
	vec3 specular = specularCoefficient * material.specular * light.source[0].color * light.source[0].intensity;

	// Attenuation 
	float distanceToLight = length(lightDirection);
	float attenuation = 1.0 / (1.0 + light.source[0].attenuation * pow(distanceToLight, 2));

	// Final Color
	vec3 fColor = ambient + attenuation * (diffuse + specular);

	fragColor = vec4(fColor, surfaceColor.a);
}