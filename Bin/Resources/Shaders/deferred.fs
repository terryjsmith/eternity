
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
uniform sampler2D textureLighting;

// Ambient lighting
uniform vec3 ambientLight;

/**
 * Out variables
 */
layout(location = 0) out vec4 frag_colour;

/**
 * Main
 */
void main()
{
    vec4 diffuse = texture(textureDiffuse, frag_texcoord.xy);
    vec4 position = texture(texturePosition, frag_texcoord.xy);
    vec3 normal = texture(textureNormal, frag_texcoord.xy).xyz;
    vec3 lighting = texture(textureLighting, frag_texcoord.xy).xyz;

    frag_colour = vec4(ambientLight + lighting.rgb, 1.0) * diffuse;
}
