
#include <Render/Mesh.h>
#include <IO/ResourceSystem.h>
#include <Render/MaterialSystem.h>
#include <Render/RenderSystem.h>
#include <IO/MemoryReader.h>
#include <Core/Application.h>

Mesh::~Mesh() {
	if (vertexBuffer) {
		delete(vertexBuffer);
		vertexBuffer = 0;
	}
}

/**
 * Load mesh from .g3d file, format:
 * 
 * Mesh header:
 *    - File version (float)
 *
 * Mesh data:
 *    - number of children (unsigned uint32_t)
 *    - child data:
 *      - vertex format (unsigned uint32_t)
 *      - vertex size (unsigned uint32_t)
 *      - material data:
 *        - diffuse (RGBA - float x4)
 *        - ambient (RGBA - float x4)
 *        - specular (RGBA - float x4)
 *        - shininess (float)
 *      - number of textures (unsigned uint32_t)
 *      - texture data:
 *        - texture type (unsigned uint32_t)
 *        - texture filename length (unsigned uint32_t)
 *        - texture filename (char*)
 *      - aabb vectors (float * 6)
 *      - number of vertices (unsigned uint32_t)
 *      - vertex data (float*)
 *      - number of indices (unsigned uint32_t)
 *      - index data (unsigned uint32_t*)
 *
 * Bone data:
 *    - number of bones (int)
 *    - bone data:
 *      - bone name length (int)
 *      - bone name (char[])
 *
 * Animation data:
 *    - number of animations (int)
 *    - animation data:
 *      - length of name (int)
 *      - name (char[])
 *      - duration in ticks (int)
 *      - ticks per second (int)
 *      - number of frames (int)
 *      - frame data:
 *        - time (in ticks) (int)
 *        - number of transforms (int)
 *        - transform data:
 *          - affected object ID (int)
 *          - transform matrix (float[16])
 */
void Mesh::ProcessData() {
    // Get a link to our resource and material systems
    ResourceSystem* resourceSystem = GetSystem<ResourceSystem>();
    MaterialSystem* materialSystem = GetSystem<MaterialSystem>();
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    
    // Get the file data and file size
    unsigned char* data = m_resource->GetData();
    int filesize = m_resource->filesize;
    
    // Create a new memory reader, pointed at our resource data
    MemoryReader* reader = new MemoryReader();
    reader->Initialize(data, filesize);
    
    // File version
    float version = 0.0f;
    reader->Read(&version, sizeof(float));
    
    // Read in the section header
    uint32_t sectionType = 0;
    reader->Read(&sectionType, sizeof(uint32_t));
    
    while(reader->GetPosition() < filesize) {
        if((sectionType == SECTION_MESH) || (sectionType > SECTION_LAST)) {
            // Number of children
            int numChildren = 0;
            reader->Read(&numChildren, sizeof(uint32_t));
            
            for(uint32_t i = 0; i < numChildren; i++) {
                Mesh* child = new Mesh();
                
                // Vertex format
                uint32_t vertexFormat = 0;
                reader->Read(&vertexFormat, sizeof(uint32_t));
                
                // Size of each vertex
                uint32_t vertexSize;
                reader->Read(&vertexSize, sizeof(uint32_t));
                
                // Material
                child->material = new Material();
                
                // Material data - diffuse
                reader->Read(&child->material->diffuse.r, sizeof(float));
                reader->Read(&child->material->diffuse.g, sizeof(float));
                reader->Read(&child->material->diffuse.b, sizeof(float));
                reader->Read(&child->material->diffuse.a, sizeof(float));
                
                // Material data - ambient
                reader->Read(&child->material->ambient.r, sizeof(float));
                reader->Read(&child->material->ambient.g, sizeof(float));
                reader->Read(&child->material->ambient.b, sizeof(float));
                reader->Read(&child->material->ambient.a, sizeof(float));
                
                // Material data - specular
                reader->Read(&child->material->specular.r, sizeof(float));
                reader->Read(&child->material->specular.g, sizeof(float));
                reader->Read(&child->material->specular.b, sizeof(float));
                reader->Read(&child->material->specular.a, sizeof(float));
                
                // Material data - shininess
                reader->Read(&child->material->shininess, sizeof(float));
                
                // TODO: Add to material system
                child->material->SetMaterialID(materialSystem->AddMaterial(child->material));
                
                // Number of textures
                uint32_t numTextures = 0;
                reader->Read(&numTextures, sizeof(uint32_t));
                
                for(uint32_t j = 0; j < numTextures; j++) {
                    Texture2D* texture = renderSystem->CreateTexture2D();
                    
                    // Texture type - diffuse only for now
                    uint32_t textureType = 0;
                    reader->Read(&textureType, sizeof(uint32_t));
                    
                    // Texture filename length (including null byte)
                    int32_t filenameLength = 0;
                    reader->Read(&filenameLength, sizeof(uint32_t));
                    
                    // Texture filename itself
                    char* textureFilename = (char*)malloc(filenameLength);
                    reader->Read(textureFilename, filenameLength);
                    
                    // Attempt to load the texture
                    texture = dynamic_cast<Texture2D*>(resourceSystem->LoadResource(textureFilename, "Texture2D"));
                    
                    free(textureFilename);
                    
                    // TODO: use texture type to assign
                    child->diffuseTexture = texture;
                }
                
                // Read in bounding box data
                vector3 minv, maxv;
                reader->Read(&minv.x, sizeof(float));
                reader->Read(&minv.y, sizeof(float));
                reader->Read(&minv.z, sizeof(float));
                
                reader->Read(&maxv.x, sizeof(float));
                reader->Read(&maxv.y, sizeof(float));
                reader->Read(&maxv.z, sizeof(float));
                
                child->aabb.Create(minv, maxv);
                
                // Number of vertices
                uint32_t vertexCount = 0;
                reader->Read(&vertexCount, sizeof(uint32_t));
                
                // Set the vertex attributes
                int offset = 0;
                VertexType* vertexType = renderSystem->CreateVertexType();
                vertexType->Create();
                
                if(vertexFormat & VERTEXTYPE_ATTRIB_POSITION) {
                    vertexType->AddVertexAttrib(VERTEXTYPE_ATTRIB_POSITION, 3, offset);
                    offset += 3;
                }
                
                if(vertexFormat & VERTEXTYPE_ATTRIB_COLOR) {
                    vertexType->AddVertexAttrib(VERTEXTYPE_ATTRIB_COLOR, 3, offset);
                    offset += 3;
                }
                
                if (vertexFormat & VERTEXTYPE_ATTRIB_NORMAL) {
                    vertexType->AddVertexAttrib(VERTEXTYPE_ATTRIB_NORMAL, 3, offset);
                    offset += 3;
                }
                
                if (vertexFormat & VERTEXTYPE_ATTRIB_TEXCOORD0) {
                    vertexType->AddVertexAttrib(VERTEXTYPE_ATTRIB_TEXCOORD0, 2, offset);
                    offset += 2;
                }
                
                if (vertexFormat & VERTEXTYPE_ATTRIB_TEXCOORD1) {
                    vertexType->AddVertexAttrib(VERTEXTYPE_ATTRIB_TEXCOORD1, 2, offset);
                    offset += 2;
                }
                
                if (vertexFormat & VERTEXTYPE_ATTRIB_BONES) {
                    vertexType->AddVertexAttrib(VERTEXTYPE_ATTRIB_BONES, 4, offset);
                    offset += 4;
                    
                    vertexType->AddVertexAttrib(VERTEXTYPE_ATTRIB_BONEWEIGHTS, 4, offset);
                    offset += 4;
                }
                
                // Vertex data
                float* vertexData = (float*)malloc(sizeof(float) * vertexCount * vertexSize);
                reader->Read((unsigned char*)vertexData, sizeof(float) * vertexCount * vertexSize);
                
                // Create vertex buffer
                child->vertexBuffer = renderSystem->CreateVertexBuffer();
                child->vertexBuffer->Create(vertexType, vertexCount, vertexData, false);
                
                // Number of indices
                uint32_t indexCount = 0;
                reader->Read((unsigned char*)&indexCount, sizeof(uint32_t));
                
                // Set number of triangles
                child->numTriangles = indexCount / 3;
                
                // Actual indices
                uint32_t* indexData = (uint32_t*)malloc(sizeof(uint32_t) * indexCount);
                reader->Read((unsigned char*)indexData, sizeof(uint32_t) * indexCount);
                
                // Create index buffer
                child->indexBuffer = renderSystem->CreateIndexBuffer();
                child->indexBuffer->Create(indexCount, indexData);
                
                free(vertexData);
                free(indexData);
                
                children.push_back(child);
            }
            
            // After reading in all children, compute the bounding box of the parent
            vector3 min = vector3(FLT_MAX);
            vector3 max = vector3(-FLT_MAX);
            
            //GetMinMaxAABB(min, max);
            aabb.Create(min, max);
            
            break;
        }
        
        if(sectionType == SECTION_BONES) {
            // Read in the number of bones
            uint32_t numBones;
            reader->Read(&numBones, sizeof(uint32_t));
            
            // Read in the bones themselves
            for(int i = 0; i < numBones; i++) {
                // Create a new bone
                Bone* bone = new Bone();
                
                // Get the length of the bone name
                int length = 0;
                reader->Read(&length, sizeof(int));
                
                // Read in the name
                bone->name.resize(length);
                reader->Read((void*)bone->name.data(), length);
                
                bones.push_back(bone);
            }
        }
        
        if(sectionType == SECTION_ANIMATIONS) {
            // Number of animations
            uint32_t numAnimations = 0;
            reader->Read(&numAnimations, sizeof(uint32_t));
            
            // Load animations
            for(int i = 0; i < numAnimations; i++) {
                Animation* animation = new Animation();
                
                // Get the length of the animation name
                int length = 0;
                reader->Read(&length, sizeof(int));
                
                // Read in the name
                animation->name.resize(length);
                reader->Read((void*)animation->name.data(), length);
                
                // Read in duration (in ticks)
                reader->Read(&animation->duration, sizeof(uint32_t));
                
                // Read in number of ticks per second
                reader->Read(&animation->framesPerSecond, sizeof(uint32_t));
                
                // Read in the number of frames
                uint32_t numFrames;
                reader->Read(&numFrames, sizeof(uint32_t));
                
                // Load frame data
                for(int j = 0; j < numFrames; j++) {
                    AnimationFrame* frame = new AnimationFrame();
                    
                    // Load the tick count/offset for this frame
                    reader->Read(&frame->time, sizeof(int));
                    
                    // Load the number of transforms in this frame
                    int numTransforms = 0;
                    reader->Read(&numTransforms, sizeof(uint32_t));
                    
                    // Load transforms
                    for(int k = 0; k < numTransforms; k++) {
                        AnimationTransform* transform = new AnimationTransform();
                        
                        // Load affected object ID
                        reader->Read(&transform->objectID, sizeof(uint32_t));
                        
                        // And the transformations
                        reader->Read(&transform->scaling.x, sizeof(float));
                        reader->Read(&transform->scaling.y, sizeof(float));
                        reader->Read(&transform->scaling.z, sizeof(float));
                        
                        reader->Read(&transform->position.x, sizeof(float));
                        reader->Read(&transform->position.y, sizeof(float));
                        reader->Read(&transform->position.z, sizeof(float));
                        
                        reader->Read(&transform->rotation.x, sizeof(float));
                        reader->Read(&transform->rotation.y, sizeof(float));
                        reader->Read(&transform->rotation.z, sizeof(float));
                        reader->Read(&transform->rotation.w, sizeof(float));
                        
                        frame->transforms.push_back(transform);
                    }
                    
                    animation->frames.push_back(frame);
                }
                
                animations.push_back(animation);
            }
        }
        
        if(reader->GetPosition() >= filesize) {
            break;
        }
        
        reader->Read((unsigned char*)&sectionType, sizeof(uint32_t));
    }
    
    delete reader;
}
