
#include <Render/ShaderProgram.h>

ShaderProgram::ShaderProgram() {
	m_vertexShader = 0;
	m_fragmentShader = 0;
    m_definitions = 0;
}

void ShaderProgram::Instantiate(Shader* vertexShader, Shader* fragmentShader, ShaderDefinitionSet* definitions) {
	m_vertexShader = vertexShader;
	m_fragmentShader = fragmentShader;
    
    if(m_definitions) {
        delete m_definitions;
        m_definitions = 0;
    }
    
    m_definitions = definitions;
}
