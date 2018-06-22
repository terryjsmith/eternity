
/**
 * In variables
 */
in vec4 frag_position;

/**
 * Uniforms
 */
uniform vec3 lightPosition;

/**
 * Out variables
 */
layout (location = 0) out float out_depth;

/**
 * Main
 */
void main () {
    // Output depth
    out_depth = length(vec3(frag_position) - lightPosition);
}
