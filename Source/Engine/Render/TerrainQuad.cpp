
#include <Render/TerrainQuad.h>
#include <Render/MaterialSystem.h>
#include <Render/RenderSystem.h>
#include <Core/Application.h>

TerrainQuad::TerrainQuad() {
    m_loaded = false;
    m_width = 0;
    m_startX = m_startZ = 0.0f;
    splat = 0;
    m_heightmap = 0;
    textures[0] = textures[1] = textures[2] = textures[3] = 0;
}

TerrainQuad::~TerrainQuad() {
    if(m_mesh) {
        delete m_mesh;
        m_mesh = 0;
    }
    
    for(size_t i = 0; i < m_children.size(); i++) {
        delete m_children[i];
    }
}

void TerrainQuad::Initialize(float startX, float startZ, float width, Texture2D* heightmap, Texture2D* splat) {
    m_aabb.Create(vector3(startX, -256.0f, startZ), vector3(startX + width, 256.0f, startZ + width));
    m_heightmap = heightmap;
    this->splat = splat;
    
    if(width > TERRAIN_QUAD_SIZE) {
        float newWidth = width / 2.0f;
        
        TerrainQuad* first = new TerrainQuad();
        first->Initialize(startX, startZ, newWidth, heightmap, splat);
        m_children.push_back(first);
        
        TerrainQuad* second = new TerrainQuad();
        second->Initialize(startX, startZ + newWidth, newWidth, heightmap, splat);
        m_children.push_back(second);
        
        TerrainQuad* third = new TerrainQuad();
        third->Initialize(startX + newWidth, startZ, newWidth, heightmap, splat);
        m_children.push_back(third);
        
        TerrainQuad* fourth = new TerrainQuad();
        fourth->Initialize(startX + newWidth, startZ + newWidth, newWidth, heightmap, splat);
        m_children.push_back(fourth);
    }
}

void TerrainQuad::SetTexture(int slot, Texture2D* texture) {
    GIGA_ASSERT(slot > 0, "Invalid texture slot.");
    GIGA_ASSERT(slot < 4, "Invalid texture slot.");
    
    textures[slot] = texture;
}

void TerrainQuad::MaybeLoad(BoundingBox* aabb) {
    // See if the bounding box intersects ours
    if(m_aabb.Intersects(aabb) == false) {
        return;
    }
    
    if(m_loaded == true) {
        return;
    }
    
    if(m_children.size()) {
        for(size_t i = 0; i < m_children.size(); i++) {
            TerrainQuad* child = (TerrainQuad*)m_children[i];
            child->MaybeLoad(aabb);
        }
        
        return;
    }
    
    // Assertions
    GIGA_ASSERT(m_width > 0, "Width not set.");
    GIGA_ASSERT(m_heightmap != 0, "Heightmap not set to load from.");
    
    // Read data from our texture
    unsigned char* data = (unsigned char*)m_heightmap->GetData();
    
    // Get width of source texture
    int srcWidth = m_heightmap->GetWidth();
    
    // Create vertex type
    VertexType* type = new VertexType();
    type->AddVertexAttrib(VERTEXTYPE_ATTRIB_POSITION, 3, 0);
    type->AddVertexAttrib(VERTEXTYPE_ATTRIB_NORMAL, 3, 3);
    type->AddVertexAttrib(VERTEXTYPE_ATTRIB_TEXCOORD0, 2, 5);
    type->AddVertexAttrib(VERTEXTYPE_ATTRIB_TEXCOORD1, 2, 7);
    
    // Get vertex size
    int vertexSize = type->GetVertexSize();
    
    // Create vertex data
    int vertexCount = (m_width + 1) * (m_width + 1);
    vector3* positions = (vector3*)malloc(sizeof(vector3) * vertexCount);
    vector3* normals = (vector3*)malloc(sizeof(vector3) * vertexCount);
    
    // Track min/max Y for bounding box
    float minY, maxY;
    
    for(int z = 0; z <=  m_width; z++) {
        for(int x = 0; x <= m_width; x++) {
            int srcOffset = ((z + m_startZ) * srcWidth) + (x + m_startX);
            int dstOffset = ((z * m_width) + x);
            
            // X, Z are offset
            positions[dstOffset].x = x;
            positions[dstOffset].z = z;
            
            // Y is height
            positions[dstOffset].y = (float)data[srcOffset];
            
            minY = std::min(positions[dstOffset].y, minY);
            maxY = std::max(positions[dstOffset].y, maxY);
        }
    }
    
    // Free original data
    free(data);
    
    // First pass at normals
    for(int z = 0; z <=  m_width; z++) {
        for(int x = 0; x <= m_width; x++) {
            int dstOffset = ((z * m_width) + x);
            
            // Start with our current normal
            vector3 avg = normals[dstOffset];
            int counter = 0;
            
            // Iterate over 4 directions in a clockwise manner
            for(int i = 0; i < 4; i++) {
                int firstOffset = dstOffset;
                int secondOffset = dstOffset;
                
                // Go up and right first
                if(i == 0 && z < m_width && x < m_width) {
                    firstOffset = dstOffset + m_width;
                    secondOffset = dstOffset + 1;
                }
                
                // Then go right and down
                if(i == 1 && x < m_width && z > 0) {
                    firstOffset = dstOffset + 1;
                    secondOffset = dstOffset - m_width;
                }
                
                // Then go down and left
                if(i == 2 && x > 0 && z > 0) {
                    firstOffset = dstOffset - m_width;
                    secondOffset = dstOffset - 1;
                }
                
                // Finally, left and up
                if(i == 3 && x > 0 && z < m_width) {
                    firstOffset = dstOffset - 1;
                    secondOffset = dstOffset + m_width;
                }
                
                // One = this, two = up, three = right
                vector3 one = positions[dstOffset];
                vector3 two = positions[firstOffset];
                vector3 thr = positions[secondOffset];
                
                // Get actual vectors
                vector3 first = two - one;
                vector3 second = thr - one;
                
                // Get cross product for normal
                vector3 normal = glm::cross(first, second);
                avg += normal;
                counter++;
            }
            
            // Divide
            avg /= counter;
            
            // Save
            normals[dstOffset] = avg;
        }
    }
    
    // Save/create buffer
    float* vertexData = (float*)malloc((m_width + 1) * (m_width + 1) * vertexSize);
    
    // Set data
    for(int z = 0; z <=  m_width; z++) {
        for(int x = 0; x <= m_width; x++) {
            int srcOffset = ((z * m_width) + x);
            int dstOffset = ((z * m_width) + x) * vertexSize;
            
            vertexData[dstOffset + 0] = positions[srcOffset].x;
            vertexData[dstOffset + 1] = positions[srcOffset].y;
            vertexData[dstOffset + 2] = positions[srcOffset].z;
            
            vertexData[dstOffset + 3] = normals[srcOffset].x;
            vertexData[dstOffset + 4] = normals[srcOffset].y;
            vertexData[dstOffset + 5] = normals[srcOffset].z;
            
            vertexData[dstOffset + 6] = (float)x / m_width;
            vertexData[dstOffset + 7] = (float)z / m_width;
            
            vertexData[dstOffset + 8] = (float)x;
            vertexData[dstOffset + 9] = (float)z;
        }
    }
    
    // Free data
    free(positions);
    free(normals);
    
    // Create indices
    int indexCount = m_width * m_width * 6; // Normally (width - 1)^2 * 6, but we have one extra
    unsigned int* indices = (unsigned int*)malloc(indexCount);
    for(int z = 0; z <  m_width; z++) {
        for(int x = 0; x < m_width; x++) {
            int dstOffset = ((z * m_width) + x) * 6;
            
            // First triangle
            indices[dstOffset + 0] = ((z * m_width) + x);
            indices[dstOffset + 1] = (((z + 1) * m_width) + x);
            indices[dstOffset + 2] = ((z * m_width) + (x+ 1));
            
            // Second triangle
            indices[dstOffset + 3] = ((z * m_width) + (x+ 1));
            indices[dstOffset + 4] = (((z + 1) * m_width) + x);
            indices[dstOffset + 5] = (((z + 1) * m_width) + (x + 1));
        }
    }
    
    // Get our render system to create platform-dependent objects
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    
    // Create mesh
    m_mesh = new Mesh();
    
    // Create vertex buffer
    m_mesh->vertexBuffer = renderSystem->CreateVertexBuffer();
    m_mesh->vertexBuffer->Create(type, vertexCount, vertexData, false);
    
    // Create index buffer
    m_mesh->indexBuffer = renderSystem->CreateIndexBuffer();
    m_mesh->indexBuffer->Create(indexCount, indices);
    
    free(indices);
    
    // Set triangle count
    m_mesh->numTriangles = indexCount / 3;
    
    // Set up bounding box
    m_mesh->aabb.Create(vector3(0, minY, 0), vector3(m_width, maxY, m_width));
    
    // Set a default material
    MaterialSystem* materialSystem = GetSystem<MaterialSystem>();
    m_mesh->material = materialSystem->GetDefaultMaterial();
    
    // Finalize our bounding box
    m_aabb.Create(vector3(0, minY, 0), vector3(m_width, maxY, m_width));
    
    // Set our transform
    GetTransform()->SetLocalPosition(vector3(m_startX, 0, m_startZ));
    
    m_loaded = true;
}

void TerrainQuad::Unload() {
    delete m_mesh;
    m_loaded = false;
}
