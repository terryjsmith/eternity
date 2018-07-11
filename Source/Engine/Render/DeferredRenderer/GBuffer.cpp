
#include <Render/DeferredRenderer/Gbuffer.h>
#include <Render/RenderSystem.h>
#include <Render/Defines.h>
#include <IO/ResourceSystem.h>
#include <Core/Application.h>
#include <Core/Application.h>

void GBuffer::Initialize(int windowWidth, int windowHeight) {
    m_windowWidth = windowWidth;
    m_windowHeight = windowHeight;
    
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    
	std::vector<Framebuffer*>::iterator it = m_framebuffers.begin();
	for(; it != m_framebuffers.end(); it++) {
		(*it)->Destroy();
	}

	m_framebuffers.clear();

    Framebuffer* gbufferFramebuffer = renderSystem->CreateFramebuffer();
    gbufferFramebuffer->Initialize();
    
    // Create our textures
    Texture2D* diffuseTexture = renderSystem->CreateTexture2D();
    diffuseTexture->Initialize(windowWidth, windowHeight, COLOR_RGB16F, COLOR_RGB);
    gbufferFramebuffer->AddTexture(diffuseTexture, FRAMEBUFFER_SLOT_0);
    
    Texture2D* positionTexture = renderSystem->CreateTexture2D();
    positionTexture->Initialize(windowWidth, windowHeight, COLOR_RGB16F, COLOR_RGB);
    gbufferFramebuffer->AddTexture(positionTexture, FRAMEBUFFER_SLOT_1);
    
    Texture2D* normalTexture = renderSystem->CreateTexture2D();
    normalTexture->Initialize(windowWidth, windowHeight, COLOR_RGB16F, COLOR_RGB);
    gbufferFramebuffer->AddTexture(normalTexture, FRAMEBUFFER_SLOT_2);
    
    Texture2D* indexTexture = renderSystem->CreateTexture2D();
    indexTexture->Initialize(windowWidth, windowHeight, COLOR_RGB16F, COLOR_RGB);
    gbufferFramebuffer->AddTexture(indexTexture, FRAMEBUFFER_SLOT_3);
    
    Texture2D* depthTexture = renderSystem->CreateTexture2D();
    depthTexture->Initialize(windowWidth, windowHeight, COLOR_DEPTH_COMPONENT24, COLOR_DEPTH_COMPONENT);
    gbufferFramebuffer->AddTexture(depthTexture, FRAMEBUFFER_SLOT_DEPTH);
    
    m_framebuffers.push_back(gbufferFramebuffer);
    
    ResourceSystem* resourceSystem = GetSystem<ResourceSystem>();
    
    // Load shaders
    Shader* vshader = dynamic_cast<Shader*>(resourceSystem->LoadResource("gbuffer.vs", "Shader"));
    Shader* fshader = dynamic_cast<Shader*>(resourceSystem->LoadResource("gbuffer.fs", "Shader"));
    
    // Create a program
	if (m_program == 0) {
		m_program = renderSystem->CreateShaderProgram();
		m_program->Instantiate(vshader, fshader, 0);
	}
}

void GBuffer::Render(Scene* scene) {
    // Use our program
    m_program->Bind();

    // Bind our FBO and set the viewport to the proper size
    m_framebuffers[0]->Bind();
    
    // Get render system
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    renderSystem->EnableDepthTest(TEST_LEQUAL);
    
    // Clear our buffer
    renderSystem->Clear(COLOR_BUFFER_BIT | DEPTH_BUFFER_BIT);
    
    // Get the camera
    CameraComponent* camera = scene->GetCamera();
    camera->SetAspectRatio((float)m_windowWidth / m_windowHeight);
    
    // Get matrices
    matrix4 view = camera->GetViewMatrix();
    matrix4 proj = camera->GetProjectionMatrix();
    
    m_program->Set("projectionMatrix", proj);
    
    std::vector<MeshComponent*> meshes = scene->GetMeshes();
    std::vector<MeshComponent*>::iterator it = meshes.begin();
    for (; it != meshes.end(); it++) {
        RecursiveRender(*it, view, matrix4(1.0));
    }
    
    renderSystem->DisableDepthTest();
    
    m_framebuffers[0]->Unbind();
    m_program->Unbind();
}

void GBuffer::RecursiveRender(MeshComponent* mesh, matrix4 view, matrix4 parent) {
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
    vertexType->EnableAttribute(1, VERTEXTYPE_ATTRIB_COLOR);
    vertexType->EnableAttribute(2, VERTEXTYPE_ATTRIB_NORMAL);
    vertexType->EnableAttribute(3, VERTEXTYPE_ATTRIB_TEXCOORD0);
    vertexType->EnableAttribute(4, VERTEXTYPE_ATTRIB_TEXCOORD1);
    vertexType->EnableAttribute(5, VERTEXTYPE_ATTRIB_BONES);
    vertexType->EnableAttribute(6, VERTEXTYPE_ATTRIB_BONEWEIGHTS);
    
    // Bind textures
    if(m->diffuseTexture) {
        m->diffuseTexture->Bind(0);
        m_program->Set("diffuseTexture", 0);
        
        // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
    }
    
    if(m->normalTexture) {
        m->normalTexture->Bind(1);
        m_program->Set("normalTexture", 1);
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

