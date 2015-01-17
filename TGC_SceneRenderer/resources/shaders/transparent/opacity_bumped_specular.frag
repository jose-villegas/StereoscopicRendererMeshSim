#version 440 core
//--include shared_data.glsl
// Input data
in vec2 texCoord;
in vec3 normal;
in vec3 position;
in mat3 TBN;
// Uniforms maps
uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D normalsMap;
uniform sampler2D opacityMap;
// Output fragment data
layout (location = 0) out vec4 fragColor;

vec3 phong(vec3 pos, vec3 norm, in vec3 fragDiffuse, in vec3 fragSpecular)
{
    vec3 normal = normalize(norm);
    vec3 result = material.ambient * fragDiffuse * light.ambientLight;
    vec3 viewDirection = normalize(TBN * (-pos));

    for(uint i = 0; i < light.count; i++) {
        vec3 lightDirection = normalize(TBN * (light.source[i].position - pos));
        float attenuationFactor = light.source[i].attenuation;
        // default to no spotlight mode
        float spotLightFactor = 1.0f;

        if(light.source[i].lightType == LIGHT_SPOT) {
            vec3 spotDirection = normalize(TBN * light.source[i].direction);
            float cosAngle = dot(-lightDirection, spotDirection);
            float cosInnerMinusOuter = light.source[i].cosInnerConeAngle - light.source[i].cosOuterConeAngle;
            // final spot light factor smooth translation between outer angle and inner angle
            spotLightFactor = smoothstep(0.0f, 1.0f, (cosAngle - light.source[i].cosOuterConeAngle) / cosInnerMinusOuter);
        } else if(light.source[i].lightType == LIGHT_DIRECTIONAL) {
            lightDirection = normalize(TBN * light.source[i].direction);
            attenuationFactor = 0.0f;
        }

        // diffuse
        float diffuseCoefficient = max(0.0f, dot(normal, lightDirection));
        vec3 diffuse = material.diffuse * diffuseCoefficient * fragDiffuse * light.source[i].color * light.source[i].intensity;

        // specular
        float specularCoefficient = 0.0f;

        if(diffuseCoefficient > 0.0f) {
            vec3 reflectDirection = normalize(reflect(-lightDirection, normal));
            specularCoefficient = pow(max(0.0f, dot(viewDirection, reflectDirection)), material.shininess);
        }

        vec3 specular = material.specular * specularCoefficient * fragSpecular * light.source[i].color * light.source[i].intensity;

        // Attenuation Calcuation
        float lightDistance = length(light.source[i].position - pos);
        float attenuation = 1.0f / (1.0f +  attenuationFactor * pow(lightDistance, 2.0f));

        result += spotLightFactor * attenuation * (diffuse + specular);

    }

    // final color
    return result;
}

vec3 blinn_phong(vec3 pos, vec3 norm, in vec3 fragDiffuse, in vec3 fragSpecular)
{
    vec3 normal = normalize(norm);
    vec3 result = material.ambient * fragDiffuse * light.ambientLight;
    vec3 viewDirection = normalize(TBN * (-pos));

    for(uint i = 0; i < light.count; i++) {
        vec3 lightDirection = normalize(TBN * (light.source[i].position - pos));
        float attenuationFactor = light.source[i].attenuation;
        // default to no spotlight mode
        float spotLightFactor = 1.0f;

        if(light.source[i].lightType == LIGHT_SPOT) {
            vec3 spotDirection = normalize(TBN * light.source[i].direction);
            float cosAngle = dot(-lightDirection, spotDirection);
            float cosInnerMinusOuter = light.source[i].cosInnerConeAngle - light.source[i].cosOuterConeAngle;
            // final spot light factor smooth translation between outer angle and inner angle
            spotLightFactor = smoothstep(0.0f, 1.0f, (cosAngle - light.source[i].cosOuterConeAngle) / cosInnerMinusOuter);
        } else if(light.source[i].lightType == LIGHT_DIRECTIONAL) {
            lightDirection = normalize(TBN * light.source[i].direction);
            attenuationFactor = 0.0f;
        }

        // diffuse
        float diffuseCoefficient = max(0.0f, dot(normal, lightDirection));
        vec3 diffuse = material.diffuse * diffuseCoefficient * fragDiffuse * light.source[i].color * light.source[i].intensity;

        // specular
        float specularCoefficient = 0.0f;

        if(diffuseCoefficient > 0.0f) {
            vec3 halfDirection = normalize(TBN * (lightDirection + viewDirection));
            float specularAngle = max(dot(halfDirection, normal), 0.0f);
            // exponent is higher trying to match phong 4 * a
            specularCoefficient = pow(specularAngle, 4.0f * material.shininess);
        }
        
        vec3 specular = material.specular * specularCoefficient * fragSpecular * light.source[i].color * light.source[i].intensity;

        // Attenuation Calcuation
        float lightDistance = length(light.source[i].position - pos);
        float attenuation = 1.0f / (1.0f +  attenuationFactor * pow(lightDistance, 2.0f));

        result += spotLightFactor * attenuation * (diffuse + specular);

    }

    // final color
    return result;
}

vec3 oren_nayar_blinn(vec3 pos, vec3 norm, in vec3 fragDiffuse, in vec3 fragSpecular, float roughness) 
{
    vec3 normal = normalize(norm);
    vec3 result = material.ambient * fragDiffuse * light.ambientLight;
    vec3 viewDirection = normalize(TBN * (-pos));

    for(uint i = 0; i < light.count; i++) {
        vec3 lightDirection = normalize(TBN * (light.source[i].position - pos));
        float attenuationFactor = light.source[i].attenuation;
        // default to no spotlight mode
        float spotLightFactor = 1.0f;

        if(light.source[i].lightType == LIGHT_SPOT) {
            vec3 spotDirection = normalize(TBN * light.source[i].direction);
            float cosAngle = dot(-lightDirection, spotDirection);
            float cosInnerMinusOuter = light.source[i].cosInnerConeAngle - light.source[i].cosOuterConeAngle;
            // final spot light factor smooth translation between outer angle and inner angle
            spotLightFactor = smoothstep(0.0f, 1.0f, (cosAngle - light.source[i].cosOuterConeAngle) / cosInnerMinusOuter);
        } else if(light.source[i].lightType == LIGHT_DIRECTIONAL) {
            lightDirection = normalize(TBN * light.source[i].direction);
            attenuationFactor = 0.0f;
        }

        // attenuation calcuation
        float lightDistance = length(light.source[i].position - pos);
        float attenuation = 1.0f / (1.0f +  attenuationFactor * pow(lightDistance, 2.0f));

        // calculate oren-nayar diffuse
        float nDotL = dot(normal, lightDirection);
        float nDotV = dot(normal, viewDirection);
        float vDotN = dot(viewDirection, normal);
        float lDotN = dot(lightDirection, normal);
        float angleVN = acos(nDotV);
        float angleLN = acos(nDotL);
        float alpha = max(angleVN, angleLN);
        float beta = min(angleVN, angleLN);
        float gamma = dot(viewDirection - normal * vDotN, lightDirection - normal * lDotN);
        float roughnessSquared = roughness * roughness;

        // calculate oren-nayar A
        float A = 1.f - 0.5f * (roughnessSquared / (roughnessSquared  + 0.57));
        float B = 0.45f * (roughnessSquared / (roughnessSquared + 0.09f));
        float C = sin(alpha) * tan(beta);

        // oren-nayar final result
        float L1 = max(0.0, nDotL) * (A + B * max(0.0f, gamma) * C);
        vec3 diffuse = material.diffuse * L1 * fragDiffuse * light.source[i].color * light.source[i].intensity;

        // calculate blinn specular
        float specularCoefficient = 0.0f;

        if(L1 > 0.0f) {
            vec3 halfDirection = normalize(TBN * (lightDirection + viewDirection));
            float specularAngle = max(dot(halfDirection, normal), 0.0f);
            // exponent is higher trying to match phong 4 * a
            specularCoefficient = pow(specularAngle, 4.0f * material.shininess);
        }

        vec3 specular = material.specular * specularCoefficient * fragSpecular * light.source[i].color * light.source[i].intensity;

        // guarante energy conserving diffuse with specular value
        diffuse /= min(max((1.f - specular), 0.f), 1.f);

        result += spotLightFactor * attenuation * (diffuse + specular);

    }

    // final color
    return result;
}

void main()
{
    vec4 opacityValue = texture(opacityMap, texCoord);
    
    if(opacityValue.r <= alphaCutoff) { 
        discard;
    }
    // obtain texture color at current position
    vec4 diffuseColor = texture(diffuseMap, texCoord);
    vec4 specularColor = texture(specularMap, texCoord);
    vec3 normalFromMap = texture(normalsMap, texCoord).rgb * 2.f - 1.f;
    // calculate phong shading
    vec3 accumColor = blinn_phong(position, normalFromMap, diffuseColor.rgb, specularColor.rgb);
    // correct gama values
    accumColor = gamma_correction(accumColor);
    // output fragment color
    fragColor = vec4(accumColor, opacityValue.r);
}