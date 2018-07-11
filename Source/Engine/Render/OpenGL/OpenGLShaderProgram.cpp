
#include <Render/OpenGL/OpenGLShaderProgram.h>
#include <Render/OpenGL/OpenGL.h>
#include <Core/Error.h>

OpenGLShaderProgram::~OpenGLShaderProgram() {
	if (m_vshader) {
		GL_CHECK(glDeleteShader(m_vshader));
		m_vshader = 0;
	}

	if (m_fshader) {
		GL_CHECK(glDeleteShader(m_fshader));
		m_fshader = 0;
	}

	if (m_program) {
		GL_CHECK(glDeleteProgram(m_program));
		m_program = 0;
	}
}

void OpenGLShaderProgram::Bind() {
	if (m_program == 0) {
		// Create a new vertex shader and load source into it
		m_vshader = glCreateShader(GL_VERTEX_SHADER);

		// Also, prepend version (TODO: make this more flexible based on actual version)
		std::string header = "#version 400 core\n";
        
        if(m_definitions) {
            std::map<std::string, bool>::iterator it = m_definitions->definitions.begin();
            for(; it != m_definitions->definitions.end(); it++) {
                header += "#define " + it->first + "\n";
            }
        }

        std::string data = header + m_vertexShader->GetResource()->GetString();

		unsigned long length = data.length();
		char* str = (char*)data.c_str();
		GL_CHECK(glShaderSource(m_vshader, 1, (const char**)&str, (int*)&length));

		// Try to compile
		GL_CHECK(glCompileShader(m_vshader));

		// If that fails, find out why
		int compiled = 0;
		GL_CHECK(glGetShaderiv(m_vshader, GL_COMPILE_STATUS, &compiled));
		if (compiled == 0) {
			GL_CHECK(glGetShaderiv(m_vshader, GL_INFO_LOG_LENGTH, (int*)&length));

			std::string errstr;
			errstr.resize(length);

			GL_CHECK(glGetShaderInfoLog(m_vshader, (int)length, (int*)&length, (char*)errstr.c_str()));

			// ... and throw an error
			Message::Broadcast(new Error(Error::ERROR_WARN, errstr, m_vertexShader->GetResource()->filename), true);
			GIGA_ASSERT(false, "Error compiling shader source.");
		}

		// Create a new vertex shader and load source into it
		m_fshader = glCreateShader(GL_FRAGMENT_SHADER);

		data = header + m_fragmentShader->GetResource()->GetString();

		length = data.length();
		str = (char*)data.c_str();
		GL_CHECK(glShaderSource(m_fshader, 1, (const char**)&str, (int*)&length));

		// Try to compile
		GL_CHECK(glCompileShader(m_fshader));

		// If that fails, find out why
		compiled = 0;
		GL_CHECK(glGetShaderiv(m_fshader, GL_COMPILE_STATUS, &compiled));
		if (compiled == 0) {
			GL_CHECK(glGetShaderiv(m_fshader, GL_INFO_LOG_LENGTH, (int*)&length));

			std::string errstr;
			errstr.resize(length);

			GL_CHECK(glGetShaderInfoLog(m_fshader, (int)length, (int*)&length, (char*)errstr.c_str()));

			// ... and throw an error
			Message::Broadcast(new Error(Error::ERROR_WARN, errstr, m_fragmentShader->GetResource()->filename), true);
			GIGA_ASSERT(false, "Error compiling shader source.");
		}

		m_program = glCreateProgram();
		GL_CHECK(glAttachShader(m_program, m_vshader));
		GL_CHECK(glAttachShader(m_program, m_fshader));

		GL_CHECK(glLinkProgram(m_program));

		int linked = 0;
		GL_CHECK(glGetProgramiv(m_program, GL_LINK_STATUS, &linked));
		if (linked == 0) {
			int length = 0;
			glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &length);

			std::string errstr;
			errstr.resize(length);
			glGetProgramInfoLog(m_program, length, &length, (char*)errstr.c_str());

			// ... and throw an error
			Message::Broadcast(new Error(Error::ERROR_WARN, errstr), true);
			GIGA_ASSERT(false, "Error compiling shader program.");
		}

		// Once we get here, it's time to figure out which attributes and uniforms we have
		GL_CHECK(glUseProgram(m_program));

		GLint activeAttribs = 0;
		GLint activeUniforms = 0;
		GL_CHECK(glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTES, &activeAttribs));
		GL_CHECK(glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &activeUniforms));

		// Start with the uniform variables
		for (int i = 0; i < activeUniforms; i++) {
			int size = 0;
			int length = 0;
			unsigned int type = 0;

			char name[256];
			memset(name, 0, 256);

			// Get the name of the uniform from the program
			GL_CHECK(glGetActiveUniform(m_program, i, 256, &length, &size, &type, name));

			name[length] = '\0';

			// Then tell the program to go get and store its location
			Uniform(name);
		}

		// Then do the same for vertex attributes
		for (int i = 0; i < activeAttribs; i++) {
			int size = 0;
			int length = 0;
			unsigned int type = 0;

			char name[256];
			memset(name, 0, 256);

			// Get the name of the uniform from the program
			GL_CHECK(glGetActiveAttrib(m_program, i, 256, &length, &size, &type, name));

			name[length] = '\0';

			// Then tell the program to go get and store its location
			Attribute(name);
		}
	}

	glUseProgram(m_program);
}

void OpenGLShaderProgram::Unbind() {
	glUseProgram(0);
}

int OpenGLShaderProgram::_var(std::string name, bool uniform) {
	// First, try finding it in our list
	for (size_t i = 0; i < m_vars.size(); i++) {
		if (m_vars[i]->name == name && m_vars[i]->uniform == uniform) {
			return(m_vars[i]->location);
		}
	}

	// Otherwise, we need to get it from OpenGL
	int location = -1;
	if (uniform) {
		GL_CHECK(location = glGetUniformLocation(m_program, name.c_str()));
	}
	else {
		GL_CHECK(location = glGetAttribLocation(m_program, name.c_str()));
	}

	if (location >= 0) {
		ShaderVariable* var = new ShaderVariable();
		var->location = location;
		var->uniform = uniform;
		var->name = name;

		m_vars.push_back(var);
	}

	return(location);
}

int OpenGLShaderProgram::Uniform(std::string name) {
	return(_var(name, true));
}

int OpenGLShaderProgram::Attribute(std::string name) {
	return(_var(name, false));
}

void OpenGLShaderProgram::Set(std::string name, float value) {
	char* cname = (char*)name.c_str();
	int location = Uniform(cname);
	if (location < 0) {
		location = Attribute(cname);
	}

	if (location < 0) {
		return;
	}

	GL_CHECK(glUniform1f(location, value));
}

void OpenGLShaderProgram::Set(std::string name, vector3 value) {
	char* cname = (char*)name.c_str();
	int location = Uniform(cname);
	if (location < 0) {
		location = Attribute(cname);
	}

	if (location < 0) {
		return;
	}

	GL_CHECK(glUniform3fv(location, 1, &(value)[0]));
}

void OpenGLShaderProgram::Set(std::string name, vector2 value) {
	char* cname = (char*)name.c_str();
	int location = Uniform(cname);
	if (location < 0) {
		location = Attribute(cname);
	}

	if (location < 0) {
		return;
	}

	GL_CHECK(glUniform2fv(location, 1, &(value)[0]));
}

void OpenGLShaderProgram::Set(std::string name, vector4 value) {
	char* cname = (char*)name.c_str();
	int location = Uniform(cname);
	if (location < 0) {
		location = Attribute(cname);
	}

	if (location < 0) {
		return;
	}

	GL_CHECK(glUniform4fv(location, 1, &(value)[0]));
}

void OpenGLShaderProgram::Set(std::string name, int value) {
	char* cname = (char*)name.c_str();
	int location = Uniform(cname);
	if (location < 0) {
		location = Attribute(cname);
	}

	if (location < 0) {
		return;
	}

	GL_CHECK(glUniform1i(location, value));
}

void OpenGLShaderProgram::Set(std::string name, unsigned int value) {
	char* cname = (char*)name.c_str();
	int location = Uniform(cname);
	if (location < 0) {
		location = Attribute(cname);
	}

	if (location < 0) {
		return;
	}

	GL_CHECK(glUniform1ui(location, value));
}

void OpenGLShaderProgram::Set(std::string name, matrix4 value) {
	char* cname = (char*)name.c_str();
	int location = Uniform(cname);
	if (location < 0) {
		location = Attribute(cname);
	}

	if (location < 0) {
		return;
	}

	GL_CHECK(glUniformMatrix4fv(location, 1, GL_FALSE, &(value)[0][0]));
}
