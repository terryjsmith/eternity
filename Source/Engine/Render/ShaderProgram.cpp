
#include <Render/ShaderProgram.h>

ShaderProgram::ShaderProgram() {
	m_vertexShader = 0;
	m_fragmentShader = 0;
}

void ShaderProgram::Instantiate(Shader* vertexShader, Shader* fragmentShader) {
	m_vertexShader = vertexShader;
	m_fragmentShader = fragmentShader;
}