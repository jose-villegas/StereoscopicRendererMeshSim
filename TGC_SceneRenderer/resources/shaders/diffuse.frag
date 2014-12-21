#version 440 core
//--include shared_data.glsl
// Input data
in vec2 texCoord;
in vec3 normal;
in vec3 position;
// Uniforms
uniform sampler2D diffuseMap;
//uniform Light light[MAX_LIGHTS];
// Output fragment data
layout (location = 0) out vec4 fragColor;

// Useful functions
vec3 phong(vec3 pos, vec3 norm, in vec3 srcColor)
{
    vec3 normal = normalize(norm);
    vec3 result = material.ambient * srcColor;
    vec3 viewDirection = normalize(-pos);

    for(uint i = 0; i < light.count; i++) {

        vec3 lightDirection = normalize(light.source[i].position - pos);

        // Diffuse
        float diffuseCoefficient = max(0.0f, dot(normal, lightDirection));
        vec3 diffuse = diffuseCoefficient * srcColor * material.diffuse * light.source[i].color * light.source[i].intensity;

        // Specular
        float specularCoefficient = 0.0f;

        if(diffuseCoefficient > 0.0f) {
            vec3 reflectDirection = normalize(reflect(-lightDirection, normal));
            specularCoefficient = pow(max(0.0f, dot(viewDirection, reflectDirection)), material.shininess);
        }

        vec3 specular = specularCoefficient * material.specular * light.source[i].color * light.source[i].intensity;

        // Attenuation Calcuation
        float distanceToLight = length(lightDirection);
        float attenuation = 1.0f / (1.0f + light.source[i].attenuation * pow(distanceToLight, 2));

        result += attenuation * (diffuse + specular);

    }

    // Final Color
    return result;
}

void main()
{
	// Obtain texture color at current position
	vec4 surfaceColor = texture(diffuseMap, texCoord);
	// Calculate Phong Shading
	fragColor = vec4(phong(position, normal, surfaceColor.rgb), surfaceColor.a);
}