// Rendering Constrains
const int MAX_LIGHTS = 4;

// Shared Structures
struct Matrix {
    mat4 modelViewProjection;
    mat4 modelView;
    mat4 model;
    mat4 view;
    mat4 projection;
    mat3 normal;
};

struct Light {
    vec3 position;
    vec3 direction; // For Directional
    vec3 color;
    float intensity;
    float attenuation;
    // Spot Light
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