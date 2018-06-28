
#pragma optionNV(unroll all)

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
uniform vec3 lightColour;
uniform int lightType;
uniform sampler2D lightShadowMap[6];
uniform samplerCube lightShadowMap3D;
uniform mat4 lightSpaceMatrix[6];
uniform float farPlane;

// Model view matrix
uniform mat4 worldviewInverseMatrix;

/**
 * Out variables
 */
layout (location = 0) out vec3 out_lighting;

/**
 * Functions
 */
vec3 ApplyLight(vec3 colour, vec3 normal, vec3 position, vec3 eye, Material material) {
    // Attenuation from light
    float attenuation = 1.0;
    vec4 light_position = vec4(lightPosition, 1.0);

    // Surface to light
    vec3 surfaceToLight = normalize(light_position.xyz - position.xyz);
    
    // Directional light
    if(lightType == LIGHTTYPE_DIRECTIONAL) {
        surfaceToLight = normalize(-light_position.xyz);
    }
    else {
        // Non-directional lights
        float distanceToLight = length(light_position.xyz - position);
		
		// calculate normalized light vector and distance to sphere light surface
		float r = 10.0;
		float cutoff = 1.0 / 256.0;
		float d = max(distanceToLight - r, 0);
		surfaceToLight /= distanceToLight;
     
		// calculate basic attenuation
		float denom = d/r + 1;
		float attenuation = 1 / (denom*denom);
     
		// scale and bias attenuation such that:
		//   attenuation == 0 at extent of max influence
		//   attenuation == 1 when d == 0
		attenuation = (attenuation - cutoff) / (1 - cutoff);
		attenuation = max(attenuation, 0);
    }
    
    // Calculate diffuse lighting factor
    float NdotL = max(dot(normalize(normal), surfaceToLight), 0.0);
    vec3 diffuse = NdotL * lightColour * material.diffuse;
	
	// And specular
	vec3 viewDir = normalize(cameraPosition - position);
    vec3 incidenceVec = reflect(-surfaceToLight, normal);
	float spec = pow(max(dot(viewDir, incidenceVec), 0.0), material.shininess);
	vec3 specular = lightColour * (spec * material.specular);
    
	return(attenuation * (diffuse + specular));
}

float ApplyShadow(sampler2D tex, vec4 position) {
    float bias = 0.0005;
    
    // Get coordinates from -1 to 1
    vec3 projCoords = position.xyz / position.w;
    
    // Tranlate that into texture coords (0 to 1)
    projCoords = projCoords * 0.5 + 0.5;
    
    // Get the closest depth to the light at this position
    float shadow = 0.0;
        float closestDepth = texture(tex, projCoords.xy).r;
    
        // Get the current depth at this position
        float currentDepth = projCoords.z;
    
        // Are we in the shadow?
        float curr = (currentDepth - bias) > closestDepth ? 1.0 : 0.0;
    
        if(projCoords.z > 1.0)
            curr = 0.0;
        
        shadow = curr;
    
    return(shadow);
}

float ApplyShadow(samplerCube tex, vec4 position) {
    float bias = 0.0005;
    
    vec3 fragToLight = vec3(position) - lightPosition;
    
    // Get the closest depth to the light at this position
    float shadow = 0.0;
    float closestDepth = texture(tex, fragToLight).r;
    closestDepth *= farPlane;
    
    // Get the current depth at this position
    float currentDepth = length(fragToLight);
    
    // Are we in the shadow?
    float curr = (currentDepth - bias) > closestDepth ? 1.0 : 0.0;
    shadow = curr;
    
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
        
    // Use the light type to determine passes for shadow maps
    if(lightType == LIGHTTYPE_DIRECTIONAL) {
        vec4 lightSpacePosition = lightSpaceMatrix[cascadeIndex] * globalPosition;
		shadow = ApplyShadow(lightShadowMap[cascadeIndex], lightSpacePosition);
    }
    else {
        float current = ApplyShadow(lightShadowMap3D, globalPosition);
        shadow = min(current + shadow, 1.0);
    }
	
	if(shadow < 1.0) {
		// Loop over lights
		linearColor = ApplyLight(diffuse.rgb, globalNormal, globalPosition.xyz, cameraPosition, mat);
	}
    
    // Set shadow to max
    out_lighting = linearColor.xyz;
}
