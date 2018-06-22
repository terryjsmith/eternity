
/**
 * In variables
 */
layout(location = 0) in vec3 vertex_position;
layout(location = 5) in vec4 vertex_bones;
layout(location = 6) in vec4 vertex_weights;

/**
 * Uniforms
 */
uniform mat4 mvpMatrix;
uniform mat4 modelMatrix;

#if defined(HAS_BONES)
uniform mat4 boneMatrix[50];
#endif

/**
 * Out variables
 */
out vec4 frag_position;

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
    
    gl_Position = mvpMatrix * vec4(transformed_position.xyz, 1.0);
    
    // Send through position
    frag_position = modelMatrix * vec4(transformed_position.xyz, 1.0);
}
