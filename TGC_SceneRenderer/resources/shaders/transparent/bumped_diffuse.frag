#version 440 core

//--include shared_data.glsl

// Input data
in vec2 texCoord;
in vec3 normal;
in vec3 position;

in vec3 tangent;
in vec3 bitangent;
in vec3 normalView;
// shadowing data
in vec4 shadowCoord[MAX_SHADOWS];
// Output fragment data
layout (location = 0) out vec4 fragColor;

//--include shared_functions.glsl

void main()
{
	// obtain texture color at current position
	vec4 diffuseColor = texture(diffuseMap, texCoord);

    if(diffuseColor.a <= alphaCutoff) { 
        discard;
    }

    vec3 normalFromMap = calculateBumpedNormal(normalView, tangent, bitangent, normalsMap, texCoord);
    // calculate phong shading
    vec3 accumColor = vec3(0.f);

    for(int i = 0; i < light.count; i++) {

        if(light.source[i].lightType == LIGHT_SPOT) {
            accumColor += calculateSpotLight(light.source[i], position, normalFromMap, diffuseColor.rgb);
        } else if(light.source[i].lightType == LIGHT_DIRECTIONAL) {
            accumColor += calculateDirectionalLight(light.source[i], position, normalFromMap, diffuseColor.rgb);
        } else {
            accumColor += calculatePointLight(light.source[i], position, normalFromMap, diffuseColor.rgb);
        }
    }
	// correct gama values
	accumColor = gamma_correction(accumColor);
	// output fragment color
	fragColor = vec4(accumColor, 1.f);
}