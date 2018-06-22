
/**
 * In variables
 */
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;
layout(location = 2) in vec3 vertex_normal;
layout(location = 3) in vec2 vertex_texcoord0;
layout(location = 4) in vec2 vertex_texcoord1;
layout(location = 5) in vec4 vertex_bones;
layout(location = 6) in vec4 vertex_weights;

/**
 * Uniforms
 */
uniform mat4 projectionMatrix;
uniform mat4 modelviewMatrix;

#if defined(HAS_BONES)
uniform mat4 boneMatrix[50];
#endif

/**
 * Out variables
 */
out vec4 frag_position;
out vec3 frag_normal;
out vec2 frag_texcoord0;
out vec2 frag_texcoord1;

#if defined(HAS_COLOR)
out vec3 frag_vcolor;
#endif

/**
 * Main (default)
 */
void main () {
    vec3 transformed_position = vertex_position.xyz;
    
#if defined(HAS_BONES)
    mat4 BoneTransform = boneMatrix[int(vertex_bones[0])] * vertex_weights[0];
    BoneTransform += boneMatrix[int(vertex_bones[1])] * vertex_weights[1];
    BoneTransform += boneMatrix[int(vertex_bones[2])] * vertex_weights[2];
    BoneTransform += boneMatrix[int(vertex_bones[3])] * vertex_weights[3];
    
    vec4 bposition = BoneTransform * vec4(transformed_position.xyz, 1.0);
    transformed_position = bposition.xyz;
#endif
    
    mat4 mvpMatrix = projectionMatrix * modelviewMatrix;
    gl_Position = mvpMatrix * vec4(transformed_position.xyz, 1.0);
    
    // Send through position, normal and texture coords
    frag_position = modelviewMatrix * vec4(transformed_position.xyz, 1.0);
    frag_normal = mat3(modelviewMatrix) * normalize(vertex_normal);
    frag_texcoord0 = vertex_texcoord0;
    
#ifdef HAS_TEXCOORD1
    frag_texcoord1 = vertex_texcoord1;
#endif
    
#if defined(HAS_COLOR)
    frag_vcolor = vertex_color;
#endif
}
