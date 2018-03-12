
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
	Shader* vshader = dynamic_cast<Shader*>(resourceSystem->LoadResource("color.vs", "Shader"));
	Shader* fshader = dynamic_cast<Shader*>(resourceSystem->LoadResource("color.fs", "Shader"));

	// Create a program
	m_program = new OpenGLShaderProgram();
	m_program->Instantiate(vshader, fshader);
}

void OpenGLDeferredRenderPass::Render(Scene* scene) {
	// Use our program
	m_program->Bind();

	// Get the camera
	CameraComponent* camera = scene->GetCamera();
	camera->SetViewport(m_windowWidth, m_windowHeight);

	// Get matrices
	matrix4 view = camera->GetViewMatrix();
	matrix4 proj = camera->GetProjectionMatrix();

	matrix4 vpMatrix = proj * view;

	// Send view/proj matrix to shader
	m_program->Set("viewprojMatrix", vpMatrix);

	// Set viewport
	glViewport(0, 0, m_windowWidth, m_windowHeight);

	// Clear our buffer
	GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));

	std::vector<MeshComponent*> meshes = scene->GetMeshes();
	std::vector<MeshComponent*>::iterator it = meshes.begin();
	for (; it != meshes.end(); it++) {
		RecursiveRender(*it, matrix4(1.0));
	}

	m_program->Unbind();
}

void OpenGLDeferredRenderPass::RecursiveRender(MeshComponent* mesh, matrix4 parent) {
	Transform* meshTransform = mesh->GetTransform();
	matrix4 mat = meshTransform->GetMatrix();
	matrix4 model = mat * parent;

	std::vector<MeshComponent*> children = mesh->GetChildren();
	if (children.size() > 0) {
		std::vector<MeshComponent*>::iterator it = children.begin();
		for (; it != children.end(); it++) {
			RecursiveRender(*it, model);
		}
		return;
	}

	m_program->Set("modelMatrix", model);

	Mesh* m = mesh->GetMesh();
	VertexBuffer* vertexBuffer = m->vertexBuffer;
	VertexType* vertexType = vertexBuffer->GetType();
	int vertexCount = vertexBuffer->GetCount();

	vertexType->Bind();
	vertexBuffer->Bind();

	// Enable the attributes we need
	vertexType->EnableAttribute(0, VERTEXTYPE_ATTRIB_POSITION);
	vertexType->EnableAttribute(1, VERTEXTYPE_ATTRIB_COLOR);

	GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, vertexCount));

	vertexBuffer->Unbind();
	vertexType->Unbind();
}