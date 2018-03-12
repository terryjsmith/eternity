
/**
 * Structs
 */

/**
 * In variables
 */
in vec3 frag_vcolor;

/**
 * Out variables
 */
layout (location = 0) out vec4 frag_color;

/**
 * Main
 */
void main () {
    // Sample our texture
    frag_color = vec4(frag_vcolor, 1.0);
}