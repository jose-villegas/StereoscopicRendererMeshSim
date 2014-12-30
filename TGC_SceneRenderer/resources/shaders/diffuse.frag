#version 440 core
//--include shared_data.glsl
// Input data
in vec2 texCoord;
in vec3 normal;
in vec3 position;
// Uniforms
uniform sampler2D diffuseMap;
// Output fragment data
layout (location = 0) out vec4 fragColor;

vec3 phong(vec3 pos, vec3 norm, in vec3 srcColor)
{
    vec3 normal = normalize(norm);
    vec3 result = material.ambient * srcColor;
    vec3 viewDirection = normalize(-pos);

    for(uint i = 0; i < light.count; i++) {

        vec3 lightDirection = normalize(light.source[i].position - pos);

        // diffuse
        float diffuseCoefficient = max(0.0f, dot(normal, lightDirection));
        vec3 diffuse = diffuseCoefficient * srcColor * material.diffuse * light.source[i].color * light.source[i].intensity;

        // specular
        float specularCoefficient = 0.0f;

        if(diffuseCoefficient > 0.0f) {
            vec3 reflectDirection = normalize(reflect(-lightDirection, normal));
            specularCoefficient = pow(max(0.0f, dot(viewDirection, reflectDirection)), material.shininess);
        }

        vec3 specular = specularCoefficient * material.specular * light.source[i].color * light.source[i].intensity;

        // Attenuation Calcuation
        float d = length(light.source[i].position - pos);
        float k = light.source[i].attenuation;
        float attenuation = 1.0f / (1.0f +  k * pow(d, 2.0f));

        result += attenuation * (diffuse + specular);

    }

    // Final Color
    return result;
}

vec3 gamma(vec3 color)
{
	return pow(color, vec3(1.0f / 2.2f));
}

void main()
{
	// obtain texture color at current position
	vec4 surfaceColor = texture(diffuseMap, texCoord);
	// calculate phong shading
	vec3 accumColor = phong(position, normal, surfaceColor.rgb);
	// correct gamma values
	accumColor = gamma(accumColor);
	// output fragment color
	fragColor = vec4(accumColor, surfaceColor.a);
}