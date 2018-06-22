
/**
 * Input variables
 */
layout(location = 0) in vec2 vertex_position;
layout(location = 3) in vec2 vertex_texcoord;

/**
 * Uniforms
 */
uniform mat4 ortho;

/**
 * Output variables
 */
out vec2 frag_texcoord;

/**
 * Main
 */
void main () {
    frag_texcoord = vertex_texcoord;
    gl_Position = ortho * vec4(vertex_position, 0.0, 1.0);
}
