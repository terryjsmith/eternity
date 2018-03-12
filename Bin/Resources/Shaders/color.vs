
/**
 * Structs
 */

/**
 * In variables
 */
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;

/**
 * Uniforms
 */
uniform mat4 viewprojMatrix;
uniform mat4 modelMatrix;

/**
 * Out variables
 */
out vec3 frag_vcolor;

/**
 * Main
 */
void main()
{
    gl_Position = (viewprojMatrix * modelMatrix) * vec4(vertex_position, 1.0f);
    frag_vcolor = vertex_color;
}