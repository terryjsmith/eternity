
#include <Render/OpenGL/OpenGLDeferredRenderPass.h>
#include <Render/OpenGL/OpenGLShaderProgram.h>
#include <Render/OpenGL/OpenGL.h>
#include <IO/ResourceSystem.h>
#include <Core/Application.h>

void OpenGLDeferredRenderPass::Initialize(int windowWidth, int windowHeight) {
	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;

	ResourceSystem* resourceSystem = GetSystem<ResourceSystem>();
	// Load shaders
	Shader* vshader = dynamic_cast<Shader*>(resourceSystem->LoadResource("mesh.vs", "Shader"));
	Shader* fshader = dynamic_cast<Shader*>(resourceSystem->LoadResource("mesh.fs", "Shader"));

	// Create a program
	m_program = new OpenGLShaderProgram();
	m_program->Instantiate(vshader, fshader, 0);
}

void OpenGLDeferredRenderPass::Render(Scene* scene) {
	// Use our program
	m_program->Bind();
    
    GL_CHECK(glEnable(GL_DEPTH_TEST));
    GL_CHECK(glDepthFunc(GL_LEQUAL));
    
    // Clear our buffer
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	// Get the camera
	CameraComponent* camera = scene->GetCamera();
	camera->SetViewport(m_windowWidth, m_windowHeight);

	// Get matrices
	matrix4 view = camera->GetViewMatrix();
	matrix4 proj = camera->GetProjectionMatrix();

    m_program->Set("projectionMatrix", proj);

	// Set viewport
	GL_CHECK(glViewport(0, 0, m_windowWidth, m_windowHeight));

	std::vector<MeshComponent*> meshes = scene->GetMeshes();
	std::vector<MeshComponent*>::iterator it = meshes.begin();
	for (; it != meshes.end(); it++) {
		RecursiveRender(*it, view, matrix4(1.0));
	}
    
    GL_CHECK(glDisable(GL_DEPTH_TEST));

	m_program->Unbind();
}

void OpenGLDeferredRenderPass::RecursiveRender(MeshComponent* mesh, matrix4 view, matrix4 parent) {
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
        
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
    }
    
    if(m->normalTexture) {
        m->normalTexture->Bind(1);
        m_program->Set("normalTexture", 1);
    }
    
    // Specify material
    m_program->Set("materialID", (float)m->material->GetMaterial());
    
    // Draw
    if(m->indexBuffer) {
        GL_CHECK(glDrawElements(GL_TRIANGLES, m->numTriangles * 3, GL_UNSIGNED_INT, 0));
    }
    else {
        GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, vertexCount));
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
