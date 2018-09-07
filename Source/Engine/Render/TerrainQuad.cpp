
#include <Render/TerrainQuad.h>
#include <Render/MaterialSystem.h>
#include <Render/RenderSystem.h>
#include <Core/Application.h>

TerrainQuad::TerrainQuad() {
    m_loaded = false;
    m_width = 0;
    m_startX = m_startZ = 0.0f;
    m_heightmap = 0;
    m_offsetX = m_offsetY = 0;
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

void TerrainQuad::Initialize(float startX, float startZ, int offsetX, int offsetY, float width, Texture2D* heightmap) {
    m_aabb.Create(vector3(startX * TERRAIN_SCALE_FACTOR, -256.0f, startZ * TERRAIN_SCALE_FACTOR), vector3((startX + width) * TERRAIN_SCALE_FACTOR, 256.0f, (startZ + width) * TERRAIN_SCALE_FACTOR));
    m_heightmap = heightmap;
    m_width = width;
    m_startX = startX;
    m_startZ = startZ;
    m_offsetX = offsetX;
    m_offsetY = offsetY;
    
    // Set our transform
    GetTransform()->SetLocalPosition(vector3(m_startX * TERRAIN_SCALE_FACTOR, 0, m_startZ * TERRAIN_SCALE_FACTOR));
    
    if(width > TERRAIN_QUAD_SIZE) {
        int newWidth = (int)width / 2.0f;
        
        TerrainQuad* first = new TerrainQuad();
        first->Initialize(0, 0, offsetX, offsetY, newWidth, heightmap);
        m_children.push_back(first);
        
        TerrainQuad* second = new TerrainQuad();
        second->Initialize(0, newWidth, offsetX, offsetY + newWidth, newWidth, heightmap);
        m_children.push_back(second);
        
        TerrainQuad* third = new TerrainQuad();
        third->Initialize(newWidth, 0, offsetX + newWidth, offsetY, newWidth, heightmap);
        m_children.push_back(third);
        
        TerrainQuad* fourth = new TerrainQuad();
        fourth->Initialize(newWidth, newWidth, offsetX + newWidth, offsetY + newWidth, newWidth, heightmap);
        m_children.push_back(fourth);
    }
}
void TerrainQuad::MaybeLoad(BoundingBox* aabb) {
    // See if the bounding box intersects ours
    if(m_aabb.Intersects(aabb) == false) {
        //return;
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
    
    // Get our render system to create platform-dependent objects
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    
    // Read data from our texture
    unsigned char* data = (unsigned char*)m_heightmap->GetData();
    
    // Get width of source texture
    int srcWidth = m_heightmap->GetWidth() - 1;
    int srcChannels = m_heightmap->GetChannels();
    
    // Create vertex type
    VertexType* type = renderSystem->CreateVertexType();
    type->Initialize();
    
    type->AddVertexAttrib(VERTEXTYPE_ATTRIB_POSITION, 3, 0);
    type->AddVertexAttrib(VERTEXTYPE_ATTRIB_NORMAL, 3, 3);
    type->AddVertexAttrib(VERTEXTYPE_ATTRIB_TEXCOORD0, 2, 6);
    type->AddVertexAttrib(VERTEXTYPE_ATTRIB_TEXCOORD1, 2, 8);
    
    // Get vertex size
    int vertexSize = type->GetVertexSize();
    
    // Create vertex data
    int vertexCount = (m_width + 1) * (m_width + 1);
    //vector3* positions = (vector3*)malloc(sizeof(vector3) * vertexCount);
    std::vector<vector3> positions;
    positions.resize(vertexCount);
    std::vector<vector3> normals;
    normals.resize(vertexCount);
    
    // Track min/max Y for bounding box
    float minY, maxY;
    
    m_width = m_width + 1;
    
    int srcY = m_offsetY;
    int srcX = m_offsetX;
    
    int counter = 0;
    for(int z = 0; z < m_width; z++) {
        for(int x = 0; x < m_width; x++) {
            int srcOffset = (((z + srcY) * (srcWidth + 1)) + (x + srcX)) * srcChannels;
            int dstOffset = ((z * m_width) + x);
            
            GIGA_ASSERT(srcOffset < (srcWidth + 1) * (srcWidth + 1) * srcChannels, "Invalid source offset.");
            GIGA_ASSERT(dstOffset < vertexCount, "Invalid dest offset.");
            
            // X, Z are offset
            positions[dstOffset].x = x * TERRAIN_SCALE_FACTOR;
            positions[dstOffset].z = z * TERRAIN_SCALE_FACTOR;
            
            // Y is height
            positions[dstOffset].y = (float)data[srcOffset] * TERRAIN_SCALE_FACTOR;
            
            minY = std::min(positions[dstOffset].y, minY);
            maxY = std::max(positions[dstOffset].y, maxY);
            counter++;
        }
    }
    
    // Free original data
    free(data);
    
    // First pass at normals
    counter = 0;
    for(int z = 0; z < m_width; z++) {
        for(int x = 0; x < m_width; x++) {
            int dstOffset = ((z * m_width) + x);
            GIGA_ASSERT(dstOffset < vertexCount, "Invalid dest offset.");
            
            // Start with our current normal
            vector3 avg = normals[dstOffset];
            int avgcount = 0;
            
            // Iterate over 4 directions in a clockwise manner
            for(int i = 0; i < 4; i++) {
                int firstOffset = dstOffset;
                int secondOffset = dstOffset;
                
                // Go up and right first
                if(i == 0 && z < m_width - 1 && x < m_width - 1) {
                    firstOffset = dstOffset + m_width;
                    secondOffset = dstOffset + 1;
                    avgcount++;
                }
                
                // Then go right and down
                if(i == 1 && x < m_width - 1 && z > 0) {
                    firstOffset = dstOffset + 1;
                    secondOffset = dstOffset - m_width;
                    avgcount++;
                }
                
                // Then go down and left
                if(i == 2 && x > 0 && z > 0) {
                    firstOffset = dstOffset - m_width;
                    secondOffset = dstOffset - 1;
                    avgcount++;
                }
                
                // Finally, left and up
                if(i == 3 && x > 0 && z < m_width - 1) {
                    firstOffset = dstOffset - 1;
                    secondOffset = dstOffset + m_width;
                    avgcount++;
                }
                
                GIGA_ASSERT(dstOffset < vertexCount, "Invalid dest offset.");
                GIGA_ASSERT(firstOffset < vertexCount, "Invalid first offset.");
                GIGA_ASSERT(secondOffset < vertexCount, "Invalid second offset.");
                
                // One = this, two = up, three = right
                vector3 one = positions[dstOffset];
                vector3 two = positions[firstOffset];
                vector3 thr = positions[secondOffset];
                
                // Get actual vectors
                vector3 first = two - one;
                vector3 second = thr - one;
                
                // Get cross product for normal
                if(glm::length(first) == 0 && glm::length(second) == 0) {
                    continue;
                }
                
                vector3 normal = glm::normalize(glm::cross(first, second));
                avg += normal;
            }
            
            // Divide
            avg /= avgcount;
            avg = glm::normalize(avg);
            
            // Save
            normals[dstOffset] = avg;
            counter++;
        }
    }
    
    // Save/create buffer
    float* vertexData = (float*)malloc(m_width * m_width * vertexSize * sizeof(float));
    
    // Set data
    counter = 0;
    for(int z = 0; z < m_width; z++) {
        for(int x = 0; x < m_width; x++) {
            int srcOffset = ((z * m_width) + x);
            int dstOffset = ((z * m_width) + x) * vertexSize;
            
            GIGA_ASSERT(srcOffset < vertexCount, "Invalid source offset.");
            GIGA_ASSERT(dstOffset < vertexCount * vertexSize, "Invalid dest offset.");
            
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
            counter++;
        }
    }
    
    // Create indices
    counter = 0;
    int indexCount = m_width * m_width * 6; // Normally (width - 1)^2 * 6, but we have one extra
    unsigned int* indices = (unsigned int*)malloc(indexCount * sizeof(unsigned int));
    for(int z = 0; z < m_width - 1; z++) {
        for(int x = 0; x < m_width - 1; x++) {
            int dstOffset = ((z * m_width) + x) * 6;
            
            GIGA_ASSERT(dstOffset < indexCount - 5, "Invalid dest offset.");
            
            // First triangle
            indices[dstOffset + 0] = ((z * m_width) + x);
            indices[dstOffset + 1] = (((z + 1) * m_width) + x);
            indices[dstOffset + 2] = ((z * m_width) + (x+ 1));
            
            // Second triangle
            indices[dstOffset + 3] = ((z * m_width) + (x+ 1));
            indices[dstOffset + 4] = (((z + 1) * m_width) + x);
            indices[dstOffset + 5] = (((z + 1) * m_width) + (x + 1));
            counter++;
        }
    }
    
    // Create mesh
    m_mesh = new Mesh();
    
    // Create vertex buffer
    m_mesh->vertexBuffer = renderSystem->CreateVertexBuffer();
    m_mesh->vertexBuffer->Create(type, vertexCount, vertexData, false);
    
    free(vertexData);
    
    // Create index buffer
    m_mesh->indexBuffer = renderSystem->CreateIndexBuffer();
    m_mesh->indexBuffer->Create(indexCount, indices);
    
    free(indices);
    
    // Set triangle count
    m_mesh->numTriangles = indexCount / 3;
    
    // Set up bounding box
    m_mesh->aabb.Create(vector3(0, minY, 0), vector3(m_width * TERRAIN_SCALE_FACTOR, maxY, m_width * TERRAIN_SCALE_FACTOR));
    
    // Set a default material
    MaterialSystem* materialSystem = GetSystem<MaterialSystem>();
    m_mesh->material = materialSystem->GetDefaultMaterial();
    
    // Finalize our bounding box
    m_aabb.Create(vector3(0, minY, 0), vector3(m_width * TERRAIN_SCALE_FACTOR, maxY, m_width * TERRAIN_SCALE_FACTOR));
    
    m_loaded = true;
}

void TerrainQuad::Unload() {
    delete m_mesh;
    m_loaded = false;
}

