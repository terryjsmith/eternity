
#include <Render/DeferredRenderer/CombinePass.h>
#include <Render/RenderSystem.h>
#include <Render/Defines.h>
#include <IO/ResourceSystem.h>
#include <Core/Application.h>

CombinePass::CombinePass() {
    m_diffuseTexture = 0;
    m_positionTexture = 0;
    m_normalTexture = 0;
    m_lightingTexture = 0;
    
    m_framebuffer = 0;
	m_buffer = 0;
}

void CombinePass::Initialize(int windowWidth, int windowHeight) {
    m_windowWidth = windowWidth;
    m_windowHeight = windowHeight;
    
    // Create our ortho screen matrix
    m_ortho = glm::ortho(0.0f, (float)windowWidth, (float)windowHeight, 0.0f);
    
    // Get our resource system
    ResourceSystem* resourceSystem = GetSystem<ResourceSystem>();
    
    // Get render system
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    
    // Load shaders
    Shader* vshader = dynamic_cast<Shader*>(resourceSystem->LoadResource("ortho.vs", "Shader"));
    Shader* fshader = dynamic_cast<Shader*>(resourceSystem->LoadResource("deferred.fs", "Shader"));
    
    // Create a program
	if (m_program == 0) {
		m_program = renderSystem->CreateShaderProgram();
		m_program->Instantiate(vshader, fshader, 0);
	}
    
    // Populate our vertex buffer and type
    float box[] = {
        (float)windowWidth, 0, 1, 1,
        0, 0, 0, 1,
        (float)windowWidth, (float)windowHeight, 1, 0,
        0, (float)windowHeight, 0, 0,
    };
    
    // Use our vertex shader
    m_program->Bind();
    
	if (m_buffer == 0) {
		VertexType* type = renderSystem->CreateVertexType();
		type->Initialize();

		type->AddVertexAttrib(VERTEXTYPE_ATTRIB_POSITION, 2, 0);
		type->AddVertexAttrib(VERTEXTYPE_ATTRIB_TEXCOORD0, 2, 2);

		m_buffer = renderSystem->CreateVertexBuffer();
		m_buffer->Create(type, 4, box, false);
	}
}

void CombinePass::Render(View *scene) {
    // Get render system
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    
    // Get the camera
    renderSystem->SetViewport(m_windowWidth, m_windowHeight);
    
    // Bind our FBO and set the viewport to the proper size
    if(m_framebuffer) {
        m_framebuffer->Bind();
    }
    
    m_program->Bind();
    m_buffer->Bind();
    
    // Disable depth testing since we're just drawing the quad to the screen
    // Note: the depth test is already saved from the pre-pass, we're also diabling writing
    renderSystem->DisableDepthTest();
    
    // Clear our buffer
    renderSystem->Clear(COLOR_BUFFER_BIT);
    
    // Set ortho matrix
    m_program->Set("ortho", m_ortho);
    
    // Set ambient light
    m_program->Set("ambientLight", scene->GetAmbientLight());
    
    // Set textures
    m_diffuseTexture->Bind(0);
    m_program->Set("textureDiffuse", 0);
    
    m_positionTexture->Bind(1);
    m_program->Set("texturePosition", 1);

    m_normalTexture->Bind(2);
    m_program->Set("textureNormal", 2);
    
    m_lightingTexture->Bind(3);
    m_program->Set("textureLighting", 3);
    
    VertexType* vertexType = m_buffer->GetType();
    vertexType->Bind();
    
    // Bind vertex attrib object and buffer
    m_buffer->Bind();
    
    // Enable vertex attribs we want to use
    vertexType->EnableAttribute(0, VERTEXTYPE_ATTRIB_POSITION);
    vertexType->EnableAttribute(3, VERTEXTYPE_ATTRIB_TEXCOORD0);
    
    // Render
    renderSystem->Draw(DRAW_TRIANGLE_STRIP, 4);
    
    if(m_framebuffer) {
        m_framebuffer->Unbind();
    }
    
    vertexType->Unbind();
    m_buffer->Unbind();
    m_program->Unbind();
}
