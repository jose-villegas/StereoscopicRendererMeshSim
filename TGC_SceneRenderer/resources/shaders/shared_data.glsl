
// Rendering Constrains
const uint MAX_LIGHTS = 32;

// Shared Structures
struct Light {
    vec3 position;
    vec3 direction;           // For Directional and Spot Light
    vec3 color;
    float intensity;
    float attenuation;
    float innerConeAngle;     // Spot Light
    float outerConeAngle;     // Spot Light
    float cosInnerConeAngle;  // Spot Light
    float cosOuterConeAngle;  // Spot Light
    uint lightType;	          // Control parameters
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

// Shared Uniform Blocks
layout ( std140 ) uniform sharedLights {
    Light source[MAX_LIGHTS];
    uint count;
} light;

layout ( std140 ) uniform sharedMatrices {
    mat4 modelViewProjection;
    mat4 modelView;
    mat4 model;
    mat4 view;
    mat4 projection;
    mat4 normal;
} matrix;

// Common Uniforms Variables
uniform Material material;

// type identifiers
const uint LIGHT_POINT       = 0;
const uint LIGHT_SPOT        = 1;
const uint LIGHT_DIRECTIONAL = 2;