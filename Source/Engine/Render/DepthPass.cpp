
#include <Render/DepthPass.h>
#include <Render/RenderSystem.h>
#include <Render/Defines.h>
#include <IO/ResourceSystem.h>
#include <Core/Application.h>

void DepthPass::Initialize(int width, int height) {
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    
    m_framebuffer = renderSystem->CreateFramebuffer();
    m_framebuffer->Initialize();
    
    m_depthTexture = renderSystem->CreateTexture2D();
    m_depthTexture->Initialize(width, height, COLOR_DEPTH_COMPONENT32F, COLOR_DEPTH_COMPONENT);
    
    m_framebuffer->AddTexture(m_depthTexture, FRAMEBUFFER_SLOT_DEPTH);
    
    ResourceSystem* resourceSystem = GetSystem<ResourceSystem>();
    
    // Load shaders
    Shader* vshader = dynamic_cast<Shader*>(resourceSystem->LoadResource("depth.vs", "Shader"));
    Shader* fshader = dynamic_cast<Shader*>(resourceSystem->LoadResource("depth.fs", "Shader"));
    
    // Create a program
    m_program = renderSystem->CreateShaderProgram();
    m_program->Instantiate(vshader, fshader, 0);
}

void DepthPass::SetTexture(Texture2D* tex) {
    m_framebuffer->SetTexture(tex, FRAMEBUFFER_SLOT_DEPTH);
}

void DepthPass::SetTexture(Texture3D* tex, int slot) {
    m_framebuffer->SetTexture(tex, FRAMEBUFFER_SLOT_DEPTH, slot);
}

void DepthPass::Render(Scene* scene) {
    // Use our program
    m_program->Bind();
    
    // Bind our FBO and set the viewport to the proper size
    m_framebuffer->Bind();
    
    // Get render system
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    renderSystem->EnableDepthTest(TEST_LEQUAL);
    
    // Clear our buffer
    //renderSystem->SetClearColor(vector4(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX));
    renderSystem->Clear(DEPTH_BUFFER_BIT);
    
    // Tell the system depth write only
    renderSystem->SetDrawBuffer(DRAW_BUFFER_NONE);
    
    // Get matrices
    matrix4 view = m_camera->GetViewMatrix();
    matrix4 proj = m_camera->GetProjectionMatrix();
    
    m_program->Set("projectionMatrix", proj);
    
    std::vector<MeshComponent*> meshes = scene->GetMeshes();
    std::vector<MeshComponent*>::iterator it = meshes.begin();
    for (; it != meshes.end(); it++) {
        RecursiveRender(*it, view, matrix4(1.0));
    }
    
    renderSystem->SetDrawBuffer(DRAW_BUFFER_BACK);
    
    renderSystem->DisableDepthTest();
    renderSystem->SetClearColor(vector4(0, 0, 0, 1));
    
    m_framebuffer->Unbind();
    m_program->Unbind();
}

void DepthPass::RecursiveRender(MeshComponent* mesh, matrix4 view, matrix4 parent) {
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
    vertexType->EnableAttribute(5, VERTEXTYPE_ATTRIB_BONES);
    vertexType->EnableAttribute(6, VERTEXTYPE_ATTRIB_BONEWEIGHTS);
    
    // Get render system
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    
    // Draw
    if(m->indexBuffer) {
        renderSystem->DrawIndexed(DRAW_TRIANGLES, m->numTriangles * 3);
    }
    else {
        renderSystem->Draw(DRAW_TRIANGLES, vertexCount);
    }
    
    vertexBuffer->Unbind();
    vertexType->Unbind();
}
