
/**
 * In variables
 */
in vec2 frag_texcoord0;
in vec3 frag_normal;
in vec4 frag_position;

#ifdef HAS_TEXCOORD1
in vec2 frag_texcoord1;
#endif

#ifdef HAS_COLOR
in vec3 frag_vcolor;
#endif

/**
 * Uniforms
 */

// Textures
uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;

// Index into scene
uniform float sceneIndex;
uniform float materialID;

/**
 * Out variables
 */
layout (location = 0) out vec3 out_diffuse;
layout (location = 1) out vec3 out_position;
layout (location = 2) out vec3 out_normal;
layout (location = 3) out vec3 out_index;

/**
 * Main
 */
void main () {
    vec4 frag;
    
    // Sample our texture
#ifdef FRAG_COLOR
    frag = vec4(frag_vcolor, 1.0);
#endif
    
    frag = texture(diffuseTexture, frag_texcoord0.st);
    
    // If this fragment is transparent, discard it
    if(frag.a < 0.5) {
        discard;
    }
    
    out_diffuse = frag.rgb;
    out_position = frag_position.xyz;
    out_normal = frag_normal;
    out_index = vec3(sceneIndex, materialID, 0);
}
