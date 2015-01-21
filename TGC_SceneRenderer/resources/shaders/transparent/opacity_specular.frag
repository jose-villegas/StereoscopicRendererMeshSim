#version 440 core

//--include shared_data.glsl

// Input data
in vec2 texCoord;
in vec3 normal;
in vec3 position;

// shadowing
in vec4 shadowCoord[MAX_SHADOWS];

// Output fragment data
layout (location = 0) out vec4 fragColor;

//--include shared_functions.glsl

void main()
{
    // obtain texture color at current position
    vec4 diffuseColor = texture(diffuseMap, texCoord);
    vec4 opacityValue = texture(opacityMap, texCoord);

    if(diffuseColor.a <= alphaCutoff || opacityValue.a <= alphaCutoff || opacityValue.r <= alphaCutoff) { 
        discard;
    }

    vec3 norm = normalize(normal);
    vec4 specularColor = texture(specularMap, texCoord);
    // calculate phong shading
    vec3 accumColor = vec3(0.f);

    for(int i = 0; i < light.count; i++) {

        if(light.source[i].lightType == LIGHT_SPOT) {
            accumColor += calculateSpotLight(light.source[i], position, norm, diffuseColor.rgb, specularColor.rgb);
        } else if(light.source[i].lightType == LIGHT_DIRECTIONAL) {
            accumColor += calculateDirectionalLight(light.source[i], position, norm, diffuseColor.rgb, specularColor.rgb);
        } else {
            accumColor += calculatePointLight(light.source[i], position, norm, diffuseColor.rgb, specularColor.rgb);
        }
    }
    // correct gama values
    accumColor = gammaCorrection(accumColor);
    // output fragment color
    fragColor = vec4(accumColor, opacityValue.a);
}