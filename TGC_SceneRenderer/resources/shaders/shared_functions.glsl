// commom functions
vec3 gamma_correction(vec3 color)
{
    return pow(color, vec3(1.0f / 2.2f));
}

// shadow calculation
vec3 calculateVisibility(vec4 shadowCoords, uint mapSize, sampler2DShadow shadowMap, vec3 shadowStrength)
{
    vec3 lightPos = shadowCoords.xyz / shadowCoords.w;
    vec2 shadowCoord;
    shadowCoord.x = 0.5 * lightPos.x + 0.5;
    shadowCoord.y = 0.5 * lightPos.y + 0.5;
    float z = 0.5 * lightPos.z + 0.5;
  
    float xOffset = 1.f / mapSize;
    float yOffset = 1.f / mapSize;

    float shadowFactor = 0.0;

    for (int y = -1 ; y <= 1 ; y++) {
        for (int x = -1 ; x <= 1 ; x++) {
            vec2 offsets = vec2(x * xOffset, y * yOffset);
            vec3 uvc = vec3(shadowCoord + offsets, z + EPSILON);
            shadowFactor += texture(shadowMap, uvc);
        }
    }
    
    return shadowStrength + vec3(shadowFactor / 18.0);
}

vec3 diffusePlusSpecular(Light lght, vec3 lightDirection, vec3 position, vec3 normal, vec3 diffuseFrag, vec3 specularFrag) {                                                                   
    vec3 diffuse  = vec3(0.f);                                                  
    vec3 specular = vec3(0.f);                                                  
                                                                                            
    float lambertian = dot(normal, -lightDirection);  
    diffuse = lght.color * material.diffuse * diffuseFrag * lght.intensity * lambertian;  
                                                                                            
    vec3 viewDirection = normalize(-position);                             
    vec3 lightReflection = normalize(reflect(lightDirection, normal)); 

    float specularFactor = max(dot(viewDirection, lightReflection), 0.f);                              
    specularFactor = pow(specularFactor, material.shininess);   
                                                        
    specular = lght.color * material.specular * specularFrag * specularFactor;                         

    return (diffuse + specular);       
}

vec3 diffuseLight(Light lght, vec3 lightDirection, vec3 normal, vec3 diffuseFrag) {
    vec3 diffuse  = vec3(0.f); 

    float lambertian = max(dot(normal, -lightDirection), 0.0f); 
    diffuse = lght.color * material.diffuse * diffuseFrag * lght.intensity * lambertian;  

    return diffuse;
}

vec3 lightAmbient(Light lght, vec3 ambientFrag) {
    return lght.color * material.ambient * ambientFrag * light.ambientLight;
}

vec3 calculatePointLight(Light lght, vec3 position, vec3 normal, vec3 diffuseFrag, vec3 specularFrag) {
    // light direction from fragment to light
    vec3 lightDirection = position - lght.position;
    // distance from fragment to light position, for attenuation
    float lightDistance = length(lightDirection);
    lightDirection = normalize(lightDirection);

    // shadowing calculation
    vec3 visibility = vec3(1.f);

    if(shadowing.enabled > 0 && lght.lightType == LIGHT_SPOT /* temporal only spotlights available */) {
        for(int i = 0; i < shadowing.shadowCount; i++) {
            visibility *= calculateVisibility(shadowCoord[i], shadowing.source[i].mapSize, shadowMap0, shadowing.source[i].strength);
        }
    }
    // calculate attenuation from point light distance to frag
    float attenuationFactor =  (1.0f + lght.attenuation * pow(lightDistance, 2.0f));   
    // calculate lighting
    vec3 color = lightAmbient(lght, diffuseFrag) + diffusePlusSpecular(lght, lightDirection, position, normal, diffuseFrag, specularFrag) * visibility / attenuationFactor;                             
    // apply attenuation factor                                                                           
    return color; 
}

vec3 calculatePointLight(Light lght, vec3 position, vec3 normal, vec3 diffuseFrag) {
    // light direction from fragment to light
    vec3 lightDirection = position - lght.position;
    // distance from fragment to light position, for attenuation
    float lightDistance = length(lightDirection);
    lightDirection = normalize(lightDirection);

    // shadowing calculation
    vec3 visibility = vec3(1.f);

    if(shadowing.enabled > 0 && lght.lightType == LIGHT_SPOT /* temporal only spotlights available */) {
        for(int i = 0; i < shadowing.shadowCount; i++) {
            visibility *= calculateVisibility(shadowCoord[i], shadowing.source[i].mapSize, shadowMap0, shadowing.source[i].strength);
        }
    }
    // calculate attenuation from point light distance to frag
    float attenuationFactor = (1.0f + lght.attenuation * pow(lightDistance, 2.0f));    
    // calculate lighting                                               
    vec3 color = lightAmbient(lght, diffuseFrag) + diffuseLight(lght, lightDirection, normal, diffuseFrag) * visibility / attenuationFactor;                            
    // apply attenuation factor                                                                           
    return color; 
}

vec3 calculateSpotLight(Light lght, vec3 position, vec3 normal, vec3 diffuseFrag) {
    vec3 lightDirection = normalize(lght.position - position);
    vec3 spotDirection = normalize(lght.direction);

    float cosAngle = dot(-lightDirection, spotDirection);

    // do not calculate complete lighting outside the light frustum
    if(cosAngle <= 0.f) return lightAmbient(lght, diffuseFrag);

    float cosInnerMinusOuter = lght.cosInnerConeAngle - lght.cosOuterConeAngle;
    // final spot light factor smooth translation between outer angle and inner angle
    float spotLightFactor = smoothstep(0.0f, 1.0f, (cosAngle - lght.cosOuterConeAngle) / cosInnerMinusOuter);
    // final colorbitangent
    return lightAmbient(lght, diffuseFrag) + calculatePointLight(lght, position, normal, diffuseFrag) * spotLightFactor;
}

vec3 calculateSpotLight(Light lght, vec3 position, vec3 normal, vec3 diffuseFrag, vec3 specularFrag) {
    vec3 lightDirection = normalize(lght.position - position);
    vec3 spotDirection = normalize(lght.direction);

    float cosAngle = dot(-lightDirection, spotDirection);

        // do not calculate complete lighting outside the light frustum
    if(cosAngle <= 0.f) return lightAmbient(lght, diffuseFrag);

    float cosInnerMinusOuter = lght.cosInnerConeAngle - lght.cosOuterConeAngle;
    // final spot light factor smooth translation between outer angle and inner angle
    float spotLightFactor = smoothstep(0.0f, 1.0f, (cosAngle - lght.cosOuterConeAngle) / cosInnerMinusOuter);
    // final color
    return lightAmbient(lght, diffuseFrag) + calculatePointLight(lght, position, normal, diffuseFrag, specularFrag) * spotLightFactor;
}

vec3 calculateDirectionalLight(Light lght, vec3 position, vec3 normal, vec3 diffuseFrag, vec3 specularFrag) {
    return lightAmbient(lght, specularFrag) + diffusePlusSpecular(lght, lght.direction, position, normal, diffuseFrag, specularFrag); 
}

vec3 calculateDirectionalLight(Light lght, vec3 position, vec3 normal, vec3 diffuseFrag) {                                                                       
    return lightAmbient(lght, diffuseFrag) + diffuseLight(lght, lght.direction, normal, diffuseFrag); 
}

vec3 calculateBumpedNormal(vec3 normView, vec3 tang, vec3 bitang, sampler2D normals, vec2 texCoord)                                                                     
{                                                                                           
    vec3 normal = normalize(normView);
    vec3 tangent = normalize(tang);
    vec3 bitangent = bitang;

    vec3 bumpedNormal = texture(normals, texCoord).xyz;
    bumpedNormal = 2.0 * bumpedNormal - vec3(1.0, 1.0, 1.0);          
    tangent = normalize(tangent - dot(tangent, normal) * normal);                                                                                                                             

    mat3 TBN = mat3(tangent, bitangent, normal); 
                                                  
    return normalize(TBN * bumpedNormal);                                                                       
}   