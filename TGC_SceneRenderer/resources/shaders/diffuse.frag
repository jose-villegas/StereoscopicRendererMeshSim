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
        float attenuationFactor = light.source[i].attenuation;
        // default to no spotlight mode
        float spotLightFactor = 1.0f;

        if(light.source[i].lightType == LIGHT_SPOT) {
        	vec3 spotDirection = normalize(light.source[i].direction);
        	float cosAngle = dot(-lightDirection, spotDirection);
        	float cosInnerMinusOuter = light.source[i].cosInnerConeAngle - light.source[i].cosOuterConeAngle;
        	// final spot light factor smooth translation between outer angle and inner angle
        	spotLightFactor = clamp((cosAngle - light.source[i].cosOuterConeAngle) / cosInnerMinusOuter, 0.0f, 1.0f);
        } else if(light.source[i].lightType == LIGHT_DIRECTIONAL) {
        	lightDirection = normalize(light.source[i].direction);
        	attenuationFactor = 0.0f;
        }

        // diffuse
        float diffuseCoefficient = max(0.0f, dot(normal, lightDirection));
        vec3 diffuse = material.diffuse * diffuseCoefficient * srcColor * light.source[i].color * light.source[i].intensity;

        // specular
        float specularCoefficient = 0.0f;

        if(diffuseCoefficient > 0.0f) {
            vec3 reflectDirection = normalize(reflect(-lightDirection, normal));
            specularCoefficient = pow(max(0.0f, dot(viewDirection, reflectDirection)), material.shininess);
        }

        vec3 specular = material.specular * specularCoefficient * light.source[i].color * light.source[i].intensity;

        // Attenuation Calcuation
        float lightDistance = length(light.source[i].position - pos);
        float attenuation = 1.0f / (1.0f +  attenuationFactor * pow(lightDistance, 2.0f));

        result += spotLightFactor * attenuation * (diffuse + specular);

    }

    // final color
    return result;
}

vec3 blinn_phong(vec3 pos, vec3 norm, in vec3 srcColor)
{
    vec3 normal = normalize(norm);
    vec3 result = material.ambient * srcColor;
    vec3 viewDirection = normalize(-pos);

    for(uint i = 0; i < light.count; i++) {
        vec3 lightDirection = normalize(light.source[i].position - pos);
        float attenuationFactor = light.source[i].attenuation;
        // default to no spotlight mode
        float spotLightFactor = 1.0f;

        if(light.source[i].lightType == LIGHT_SPOT) {
        	vec3 spotDirection = normalize(light.source[i].direction);
        	float cosAngle = dot(-lightDirection, spotDirection);
        	float cosInnerMinusOuter = light.source[i].cosInnerConeAngle - light.source[i].cosOuterConeAngle;
        	// final spot light factor smooth translation between outer angle and inner angle
        	spotLightFactor = clamp((cosAngle - light.source[i].cosOuterConeAngle) / cosInnerMinusOuter, 0.0f, 1.0f);
        } else if(light.source[i].lightType == LIGHT_DIRECTIONAL) {
        	lightDirection = normalize(light.source[i].direction);
        	attenuationFactor = 0.0f;
        }

        // diffuse
        float diffuseCoefficient = max(0.0f, dot(normal, lightDirection));
        vec3 diffuse = material.diffuse * diffuseCoefficient * srcColor * light.source[i].color * light.source[i].intensity;

        // specular
        float specularCoefficient = 0.0f;

        if(diffuseCoefficient > 0.0f) {
        	vec3 halfDirection = normalize(lightDirection + viewDirection);
            float specularAngle = max(dot(halfDirection, normal), 0.0f);
            // exponent is higher trying to match phong 4 * a
            specularCoefficient = pow(specularAngle, 4.0f * material.shininess);
        }
        
        vec3 specular = material.specular * specularCoefficient * light.source[i].color * light.source[i].intensity;

        // Attenuation Calcuation
        float lightDistance = length(light.source[i].position - pos);
        float attenuation = 1.0f / (1.0f +  attenuationFactor * pow(lightDistance, 2.0f));

        result += spotLightFactor * attenuation * (diffuse + specular);

    }

    // final color
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
	vec3 accumColor = blinn_phong(position, normal, surfaceColor.rgb);
	// correct gamma values
	accumColor = gamma(accumColor);
	// output fragment color
	fragColor = vec4(accumColor, surfaceColor.a);
}