
// utility constants
const float PI          = 3.14159f;
const float EPSILON     = 0.00001;
const float alphaCutoff = 0.1f;
// Rendering Constrains
const uint MAX_LIGHTS            = 32;
const uint MAX_SHADOWS           = 1;
const uint MATERIAL_2DMAPS_COUNT = 11;

// texture maps
uniform sampler2D noneMap;         // unit = 0
uniform sampler2D diffuseMap;      // unit = 1
uniform sampler2D specularMap;     // unit = 2
uniform sampler2D ambientMap;      // unit = 3
uniform sampler2D emissiveMap;     // unit = 4
uniform sampler2D heightMap;       // unit = 5
uniform sampler2D normalsMap;      // unit = 6
uniform sampler2D shininessMap;    // unit = 7
uniform sampler2D opacityMap;      // unit = 8
uniform sampler2D displacementMap; // unit = 9
uniform sampler2D lightmapMap;     // unit = 10
uniform samplerCube reflectionMap; // unit = 11

// shadow maps avaible locations
uniform sampler2DShadow depthMap[MAX_SHADOWS];

// Shared Structures
struct Light {
    vec3 position;
    vec3 direction;           // For Spot and Spot Light
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

struct Shadow {
    uint mapSize;
    vec3 strength;
    mat4 biasModelViewProjection;
};

// Shared Uniform Blocks
layout(std140) uniform sharedMatrices {
    mat4 modelViewProjection;
    mat4 modelView;
    mat4 model;
    mat4 view;
    mat4 projection;
    mat4 normal;
} matrix;

layout(std140) uniform sharedLights {
    Light source[MAX_LIGHTS];
    vec3 ambientLight;
    uint count;
} light;

layout(std140) uniform sharedShadowing {
    Shadow source[MAX_LIGHTS];
    uint enabled;
    uint shadowCount;
} shadowing;

// Common Uniforms Variables
uniform Material material;

// type identifiers
const uint LIGHT_POINT       = 0;
const uint LIGHT_SPOT        = 1;
const uint LIGHT_DIRECTIONAL = 2;