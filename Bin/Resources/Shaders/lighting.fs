
/**
 * Structs
 */
struct Material {
    vec3 diffuse;
    vec3 specular;
    vec3 emissionColor;
    float emissionStrength;
    float shininess;
};

/**
 * Constants
 */
const int LIGHTTYPE_DIRECTIONAL = 1;
const int LIGHTTYPE_POINT = 2;
const int LIGHTTYPE_SPOTLIGHT = 3;

/**
 * In variables
 */
in vec2 frag_texcoord;

/**
 * Uniforms
 */

// Sample textures
uniform sampler2D textureDiffuse;
uniform sampler2D texturePosition;
uniform sampler2D textureNormal;
uniform sampler2D textureMaterial;
uniform sampler2D textureMaterialLookup;

uniform vec3 cameraPosition;
uniform vec3 lightPosition;
uniform vec3 lightDirection;
uniform float lightAngle;
uniform vec3 lightColour;
uniform int lightType;
uniform sampler2D lightShadowMap[3];
uniform samplerCube lightShadowMap3D;
uniform mat4 lightSpaceMatrix[3];
uniform float farPlane;

// Model view matrix
uniform mat4 worldviewInverseMatrix;

/**
 * Out variables
 */
layout (location = 0)out vec3 out_lighting;

/**
 * Functions
 */
vec3 ApplyLight(vec3 colour, vec3 normal, vec3 position, vec4 light_position, vec3 eye, Material material) {
    // Attenuation from light
    float attenuation = 1.0;
    
    // Surface to light
    vec3 surfaceToLight = normalize(light_position.xyz - position.xyz);
    
    // Directional light
    if(lightType == LIGHTTYPE_DIRECTIONAL) {
        surfaceToLight = normalize(-light_position.xyz);
    }
    else {
        // Non-directional lights
        float distanceToLight = length(light_position.xyz - position);
        
        attenuation = clamp(1.0 - distanceToLight*distanceToLight/(farPlane*farPlane), 0.0, 1.0);
        attenuation *= attenuation;
    }
    
    if(lightType == LIGHTTYPE_SPOTLIGHT) {
        vec3 ld = normalize(surfaceToLight);
        vec3 sd = normalize(-lightDirection);
        
        // inside the cone?
        if (dot(sd,ld) < lightAngle) {
            attenuation = 0;
        }
    }
    
    // Calculate diffuse lighting factor
    float NdotL = max(dot(normalize(normal), surfaceToLight), 0.0);
    vec3 diffuse = NdotL * lightColour * material.diffuse;
    
    // And specular
    vec3 viewDir = normalize(cameraPosition - position);
    vec3 incidenceVec = reflect(-surfaceToLight, normal);
    float spec = pow(max(dot(viewDir, incidenceVec), 0.0), material.shininess);
    vec3 specular = lightColour * (spec * material.specular);

    //specular = vec3(0);
    return(attenuation * (diffuse + specular));
}

float ApplyShadow2D(sampler2D tex, vec4 position, vec4 light_position) {
    float bias = 0.005;
    
    vec3 projcoords = position.xyz / position.w;
    projcoords = projcoords * 0.5 + 0.5;
    
    // Get the closest depth to the light at this position
    float shadow = 0.0;
    float closestDepth = texture(tex, projcoords.xy).r;
    
    // Get the current depth at this position
    float currentDepth = length(position.xyz) / farPlane;
    
    // Are we in the shadow?
    shadow = (currentDepth - bias) > closestDepth ? 1.0 : 0.0;
    
    // inside the cone?
    if (projcoords.x < 0 || projcoords.x > 1) {
        shadow = 0;
    }
    
    if (projcoords.y < 0 || projcoords.y > 1) {
        shadow = 0;
    }
    
    return(shadow);
}

float ApplyShadowCube(vec4 position, vec4 light_position, vec3 globalNormal) {
    vec3 fragToLight = vec3(position) - vec3(light_position);
    
    float bias = 0.005;
    
    // Get the closest depth to the light at this position
    float closestDepth = texture(lightShadowMap3D, fragToLight).r;

    // Get the current depth at this position
    float currentDepth = length(fragToLight) / farPlane;
    
    // Are we in the shadow?
    float shadow = (currentDepth - bias) > closestDepth ? 1.0 : 0.0;
    
    //return(closestDepth);
    return(shadow);
}

/**
 * Main
 */
void main()
{
    vec4 diffuse = texture(textureDiffuse, frag_texcoord.xy);
    vec4 position = texture(texturePosition, frag_texcoord.xy);
    vec3 normal = texture(textureNormal, frag_texcoord.xy).xyz;
    
    // Re-construct our material from our material texture
    Material mat;
    float materialID = texture(textureMaterial, frag_texcoord.xy).y;
    float y = materialID / 1024.0;
    
    mat.diffuse = texture(textureMaterialLookup, vec2(0.0, y)).xyz;
    mat.specular = texture(textureMaterialLookup, vec2(0.25, y)).xyz;
    mat.emissionColor = texture(textureMaterialLookup, vec2(0.50, y)).xyz;
    vec3 extra = texture(textureMaterialLookup, vec2(0.75, y)).xyz;
    mat.emissionStrength = extra.x;
    mat.shininess = extra.y;
    
    int cascadeIndex = 0;
    if(position.z < -15.0) {
        cascadeIndex = 1;
    }
    if(position.z < -40.0) {
        cascadeIndex = 2;
    }
    
    // Iterate over our lights to calculate colours
    vec3 linearColor = vec3(0);
    float shadow = 0.0;
    vec4 globalPosition = worldviewInverseMatrix * position;
    vec3 globalNormal = mat3(worldviewInverseMatrix) * normal;
    
    // For point-lights, no transform
    vec4 lightSpacePosition = lightSpaceMatrix[0] * globalPosition;
    vec4 light_position = lightSpaceMatrix[0] * vec4(lightPosition, 1.0);
    vec3 light_normal = mat3(lightSpaceMatrix[0]) * globalNormal;
    
    // Use the light type to determine passes for shadow maps
    if(lightType == LIGHTTYPE_DIRECTIONAL) {
        shadow = ApplyShadow2D(lightShadowMap[cascadeIndex], lightSpacePosition, light_position);
    }
    
    if(lightType == LIGHTTYPE_POINT) {
        shadow = ApplyShadowCube(globalPosition, vec4(lightPosition, 1.0), globalNormal);
    }
    
    if(lightType == LIGHTTYPE_SPOTLIGHT) {
        shadow = ApplyShadow2D(lightShadowMap[0], lightSpacePosition, light_position);
    }
    
    
    
    //if(shadow < 1.0) {
        if(lightType == LIGHTTYPE_POINT) {
            linearColor = ApplyLight(diffuse.rgb, globalNormal, globalPosition.xyz, vec4(lightPosition, 1.0), cameraPosition, mat);
            
        }
        
        if(lightType == LIGHTTYPE_SPOTLIGHT) {
            linearColor = ApplyLight(diffuse.rgb, globalNormal, globalPosition.xyz, vec4(lightPosition, 1.0), cameraPosition, mat);
        }
    //}

    // Set shadow to max
    out_lighting = linearColor.xyz * vec3(1.0 - shadow);
}
