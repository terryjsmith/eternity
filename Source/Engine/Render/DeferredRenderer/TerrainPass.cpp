
#include <Render/DeferredRenderer/TerrainPass.h>
#include <IO/ResourceSystem.h>
#include <Render/RenderSystem.h>
#include <Render/TerrainSystem.h>
#include <Core/Application.h>
#include <Render/Defines.h>

void TerrainPass::Initialize(int windowWidth, int windowHeight) {
    ResourceSystem* resourceSystem = GetSystem<ResourceSystem>();
    
    m_windowWidth = windowWidth;
    m_windowHeight = windowHeight;
    
    // Load shaders
    Shader* vshader = dynamic_cast<Shader*>(resourceSystem->LoadResource("terrain.vs", "Shader"));
    Shader* fshader = dynamic_cast<Shader*>(resourceSystem->LoadResource("terrain.fs", "Shader"));
    
    // Create a program
    if (m_program == 0) {
        RenderSystem* renderSystem = GetSystem<RenderSystem>();
        m_program = renderSystem->CreateShaderProgram();
        m_program->Instantiate(vshader, fshader, 0);
    }
}

void TerrainPass::SetFramebuffer(Framebuffer* framebuffer) {
    if(m_framebuffers.size() > 0) {
        m_framebuffers[0] = framebuffer;
        return;
    }
    
    m_framebuffers.push_back(framebuffer);
}

void TerrainPass::Render(View* scene) {
    // Use our program
    m_program->Bind();
    
    // Bind our FBO and set the viewport to the proper size
    m_framebuffers[0]->Bind();
    
    // Get render system
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    renderSystem->EnableDepthTest(TEST_LEQUAL);
    
    // Get the camera
    CameraComponent* camera = scene->GetCamera();
    camera->SetAspectRatio((float)m_windowWidth / m_windowHeight);
    
    // Set matrix
    matrix4 view = camera->GetViewMatrix();
    matrix4 proj = camera->GetProjectionMatrix();
    m_program->Set("projectionMatrix", proj);
    
    // Get the components
    TerrainSystem* terrainSystem = GetSystem<TerrainSystem>();
    std::vector<TerrainComponent*> components = terrainSystem->GetComponents();
    for(size_t i = 0; i < components.size(); i++) {
        if(components[i]->Active() == false) {
            continue;
        }
        
        m_program->Set("sceneIndex", (float)i);
        for(int j = 0; j < 4; j++) {
            TerrainQuad* quad = components[i]->GetQuad(j);
            RecursiveRender(quad, view, matrix4(1.0));
        }
    }
    
    renderSystem->DisableDepthTest();
    
    m_framebuffers[0]->Unbind();
    m_program->Unbind();
}

void TerrainPass::RecursiveRender(MeshComponent* mesh, matrix4 view, matrix4 parent) {
    Transform* meshTransform = mesh->GetTransform();
    matrix4 mat = meshTransform->GetMatrix();
    matrix4 model = mat * parent;
    
    std::vector<MeshComponent*> children = mesh->GetChildren();
    if (children.size() > 0) {
        std::vector<MeshComponent*>::iterator it = children.begin();
        for (; it != children.end(); it++) {
            RecursiveRender(*it, view, model);
        }
        return;
    }
    
    // Get mesh
    TerrainQuad* quad = (TerrainQuad*)mesh;
    if(quad->IsLoaded() == false) {
        return;
    }
    
    // Send view/proj matrix to shader
    matrix4 modelviewMatrix = view * model;
    m_program->Set("modelviewMatrix", modelviewMatrix);
    
    Mesh* m = mesh->GetMesh();
    VertexBuffer* vertexBuffer = m->vertexBuffer;
    VertexType* vertexType = vertexBuffer->GetType();
    int vertexCount = vertexBuffer->GetCount();
    
    vertexType->Bind();
    vertexBuffer->Bind();
    
    if(m->indexBuffer) {
        m->indexBuffer->Bind();
    }
    
    // Enable the attributes we need
    vertexType->EnableAttribute(0, VERTEXTYPE_ATTRIB_POSITION);
    vertexType->EnableAttribute(2, VERTEXTYPE_ATTRIB_NORMAL);
    vertexType->EnableAttribute(3, VERTEXTYPE_ATTRIB_TEXCOORD0);
    vertexType->EnableAttribute(4, VERTEXTYPE_ATTRIB_TEXCOORD1);
    
    // Bind textures
    int textureCounter = 0;
    if(quad->textures[0]) {
        quad->textures[0]->Bind(textureCounter);
        m_program->Set("diffuseTexture[0]", textureCounter);
        
        // Need to make this platform agnostic
        // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
        textureCounter++;
    }
    
    if(quad->textures[1]) {
        quad->textures[1]->Bind(textureCounter);
        m_program->Set("diffuseTexture[1]", textureCounter);
        
        // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
        textureCounter++;
    }
    
    if(quad->textures[2]) {
        quad->textures[2]->Bind(textureCounter);
        m_program->Set("diffuseTexture[2]", textureCounter);
        
        // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
        textureCounter++;
    }
    
    if(quad->textures[3]) {
        quad->textures[3]->Bind(textureCounter);
        m_program->Set("diffuseTexture[3]", textureCounter);
        
        // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
        textureCounter++;
    }
    
    if(quad->splat) {
        quad->splat->Bind(textureCounter);
        m_program->Set("splatTexture", textureCounter);
        textureCounter++;
    }
    
    if(m->normalTexture) {
        m->normalTexture->Bind(textureCounter);
        m_program->Set("normalTexture", textureCounter);
        textureCounter++;
    }
    
    // Specify material
    m_program->Set("materialID", (float)m->material->GetMaterial());
    
    // Get render system
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    
    // Draw
    if(m->indexBuffer) {
        renderSystem->DrawIndexed(DRAW_TRIANGLES, m->numTriangles * 3);
    }
    else {
        renderSystem->Draw(DRAW_TRIANGLES, vertexCount);
    }
    
    if(m->diffuseTexture) {
        m->diffuseTexture->Unbind();
    }
    
    if(m->normalTexture) {
        m->normalTexture->Unbind();
    }
    
    vertexBuffer->Unbind();
    vertexType->Unbind();
}
