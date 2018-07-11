
#include <Render/DeferredRenderer/LightingPass.h>
#include <Render/RenderSystem.h>
#include <Render/Defines.h>
#include <Render/MaterialSystem.h>
#include <IO/ResourceSystem.h>
#include <Core/Application.h>

LightingPass::LightingPass() {
	m_buffer = 0;
	m_null = 0;
}

void LightingPass::Initialize(int windowWidth, int windowHeight) {
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
   
	std::vector<Framebuffer*>::iterator it = m_framebuffers.begin();
	for (; it != m_framebuffers.end(); it++) {
		(*it)->Destroy();
	}

	m_framebuffers.clear();

    Framebuffer* lightingFramebuffer = renderSystem->CreateFramebuffer();
    lightingFramebuffer->Initialize();
    
    // Create our textures
    Texture2D* diffuseTexture = renderSystem->CreateTexture2D();
    diffuseTexture->Initialize(windowWidth, windowHeight, COLOR_RGB16F, COLOR_RGB);
    lightingFramebuffer->AddTexture(diffuseTexture, FRAMEBUFFER_SLOT_0);
    
    m_framebuffers.push_back(lightingFramebuffer);
   
    // Get resource system
    ResourceSystem* resourceSystem = GetSystem<ResourceSystem>();
    
    // Load shaders
    Shader* vshader = dynamic_cast<Shader*>(resourceSystem->LoadResource("ortho.vs", "Shader"));
    Shader* fshader = dynamic_cast<Shader*>(resourceSystem->LoadResource("lighting.fs", "Shader"));
    
    // Create our ortho screen matrix
    m_ortho = glm::ortho(0.0f, (float)windowWidth, (float)windowHeight, 0.0f);
    
    // Create programs
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
    
	if (m_null == 0) {
		m_null = renderSystem->CreateTexture3D();
	}
}

void LightingPass::Render(Scene* scene) {
    m_framebuffers[0]->Bind();
    
    // Disable depth read/write, clear
    RenderSystem* renderSystem = GetSystem<RenderSystem>();
    renderSystem->DisableDepthTest();
    renderSystem->Clear(COLOR_BUFFER_BIT);
    
    // Set cumulative blending
    renderSystem->EnableBlending();
    renderSystem->SetBlendFunc(BLEND_ONE, BLEND_ONE);
    
    // Set up ortho projection
    m_program->Bind();
    m_program->Set("ortho", m_ortho);
    
    // Get the far plane distance
    CameraComponent* camera = scene->GetCamera();
    m_program->Set("cameraPosition", camera->GetTransform()->GetWorldPosition());
    
    // Get view matrix
    matrix4 view = camera->GetViewMatrix();
    matrix4 worldViewInverse = glm::inverse(view);
    m_program->Set("worldviewInverseMatrix", worldViewInverse);
    
    // Set textures
    m_diffuseTexture->Bind(0);
    m_program->Set("textureDiffuse", 0);
    
    m_positionTexture->Bind(1);
    m_program->Set("texturePosition", 1);
    
    m_normalTexture->Bind(2);
    m_program->Set("textureNormal", 2);
    
    m_materialTexture->Bind(3);
    m_program->Set("textureMaterial", 3);
    
    // Get our material texture
    MaterialSystem* materialSystem = GetSystem<MaterialSystem>();
    Texture* materialTexture = materialSystem->GetTexture();
    
    materialTexture->Bind(4);
    m_program->Set("textureMaterialLookup", 4);
    
    materialTexture->SetTextureFilter(FILTER_NEAREST);
    
    VertexType* vertexType = m_buffer->GetType();
    vertexType->Bind();
    
    // Bind vertex attrib object and buffer
    m_buffer->Bind();
    
    // Enable vertex attribs we want to use
    vertexType->EnableAttribute(0, VERTEXTYPE_ATTRIB_POSITION);
    vertexType->EnableAttribute(3, VERTEXTYPE_ATTRIB_TEXCOORD0);
    
    // Loop through each light
    std::vector<LightComponent*> lights = scene->GetLights();
    std::vector<LightComponent*>::iterator i = lights.begin();
    for (i; i != lights.end(); i++) {
        // Get light "camera"
        CameraComponent* cc = (*i)->GetCamera();
        
        // Bind vars
        m_program->Set("lightPosition", (*i)->GetTransform()->GetWorldPosition());
        m_program->Set("lightColour", (*i)->GetColor());
        m_program->Set("lightType", (*i)->GetType());
        m_program->Set("farPlane", (*i)->GetAttenuation());
        m_program->Set("lightDirection", cc->GetTransform()->GetLook());
        m_program->Set("lightAngle", glm::cos(glm::radians(cc->GetFOV() / 2.0f)));
    
        matrix4 viewproj = cc->GetProjectionMatrix();
        if((*i)->GetType() != LightComponent::LIGHT_POINT) {
            viewproj = viewproj * cc->GetViewMatrix();
        }
        m_program->Set("lightSpaceMatrix[0]", viewproj);
        
        (*i)->GetDepthTexture()->Bind(5);
        if((*i)->GetType() == LightComponent::LIGHT_POINT) {
            m_program->Set("lightShadowMap3D", 5);
        }
        
        if((*i)->GetType() == LightComponent::LIGHT_SPOT) {
            m_program->Set("lightShadowMap[0]", 5);
            m_null->Bind(6);
            m_program->Set("lightShadowMap3D", 6);
        }
        
        if((*i)->GetType() == LightComponent::LIGHT_DIRECTIONAL) {
            
        }
        
        // Render
        renderSystem->Draw(DRAW_TRIANGLE_STRIP, 4);
    }
    
    //m_framebuffers[0]->GetTexture(0)->Save("lighting.bmp");
    
    vertexType->Unbind();
    m_buffer->Unbind();
    
    renderSystem->DisableBlending();
    m_framebuffers[0]->Unbind();
}
