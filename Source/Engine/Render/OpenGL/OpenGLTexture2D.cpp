
#include <Render/OpenGL/OpenGLTexture2D.h>
#include <Render/OpenGL/OpenGL.h>
#include <SOIL.h>

OpenGLTexture2D::~OpenGLTexture2D() {
	if (m_texture) {
		GL_CHECK(glDeleteTextures(1, &m_texture));
		m_texture = 0;
	}
}

void OpenGLTexture2D::Initialize(int width, int height, int format, int type) {
    if (m_texture) {
        GL_CHECK(glDeleteTextures(1, &m_texture));
        m_texture = 0;
    }
    
    GL_CHECK(glGenTextures(1, &m_texture));
    
    m_width = width;
    m_height = height;
    m_channels = 0;
    
    switch(type) {
        case GL_DEPTH_COMPONENT:
            m_channels = 1;
            break;
        case GL_RED:
            m_channels = 1;
            break;
        case GL_RGB:
            m_channels = 3;
            break;
        case GL_RGBA:
            m_channels = 4;
            break;
        default:
            break;
    };
    
    m_dataType = GL_FLOAT;
    
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_texture));
    GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, type, GL_FLOAT, NULL));
    
    GL_CHECK(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CHECK(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
}

void OpenGLTexture2D::SetData(int width, int height, int format, int type, void* data) {
    GL_CHECK(glActiveTexture(GL_TEXTURE0));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_texture));
    GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, type, GL_FLOAT, data));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
    
    m_dataType = GL_FLOAT;
}

void OpenGLTexture2D::Bind(int slot) {
    GL_CHECK(glActiveTexture(GL_TEXTURE0 + slot));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_texture));
    m_slot = slot;
}

void OpenGLTexture2D::Unbind() {
    GL_CHECK(glActiveTexture(GL_TEXTURE0 + m_slot));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
}


void OpenGLTexture2D::ProcessData() {
    int flags = 0;//SOIL_FLAG_MIPMAPS;
    std::string extension = m_resource->extension;
    if(extension == "bmp" || extension == "png") {
        flags |= SOIL_FLAG_INVERT_Y;
    }
    
    unsigned char* data = m_resource->GetData();
    unsigned int length = m_resource->filesize;
    
    m_texture = SOIL_load_OGL_texture_from_memory(data, length, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, flags);
    
    m_dataType = GL_UNSIGNED_BYTE;
    
    // Get the width, height and channels back out
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &m_width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &m_height);
    
    int format = 0;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &format);
    
    switch(format) {
        case GL_RED:
            m_channels = 1;
            break;
        case GL_RGB:
        case GL_RGB8:
            m_channels = 3;
            break;
        case GL_RGBA:
        case GL_RGBA8:
            m_channels = 4;
            break;
        default: break;
    }
}

void OpenGLTexture2D::SetTextureFilter(int filter) {
    // Make sure material texture does not filter
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter));
}

void OpenGLTexture2D::Save(std::string filename) {
    // Dump out normal texture
    glBindTexture(GL_TEXTURE_2D, m_texture);
    float* pixels = (float*)malloc(m_width * m_height * sizeof(float) * m_channels);
    glGetTexImage(GL_TEXTURE_2D, 0, m_channels == 3 ? GL_RGB : GL_DEPTH_COMPONENT, GL_FLOAT, pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // Convert from float to unsigned char
    unsigned char* data = (unsigned char*)malloc(m_width * m_height * m_channels);
    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            int offset = ((y * m_width) + x) * m_channels;
            for(int i = 0; i < m_channels; i++) {
                float r = pixels[offset + i];
                data[offset + i] = r * 255.0f;
            }
        }
    }
    
    SOIL_save_image(filename.c_str(), SOIL_SAVE_TYPE_BMP, m_width, m_height, m_channels, data);
    free(data);
    free(pixels);
}

unsigned int OpenGLTexture2D::GetTarget(int slot) {
    return GL_TEXTURE_2D;
}

void* OpenGLTexture2D::GetData() {
    glBindTexture(GL_TEXTURE_2D, m_texture);
    void* pixels = (void*)malloc(m_width * m_height * m_channels * ((m_dataType == GL_FLOAT) ? sizeof(float) : 1));
    int format = GL_RGB;
    if(m_channels == 1) {
        format = GL_RED;
    }
    if(m_channels == 4) {
        format = GL_RGBA;
    }
    
    glGetTexImage(GL_TEXTURE_2D, 0, format, m_dataType, pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    return(pixels);
}
