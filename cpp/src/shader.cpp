#include <iostream>
#include <fstream>
#include <shaders/shader.h>

Shader::Shader() {}

Shader::~Shader() {
	if (m_shader != 0) {
		glDeleteShader(m_shader);
	}
}

bool Shader::InitFromFile(std::string filePath, GLenum shaderType) {

	// std::ifstream file(filePath);

	// if (!file.good()) 	{
	// 	std::cout << "Can't read file: " << filePath << std::endl;
	// 	return false;
	// }

	// file.seekg(0, std::ios::end);

	// std::string shaderCode;
	// shaderCode.resize((size_t)file.tellg());

	// file.seekg(0, std::ios::beg);
	// file.read(&shaderCode[0], shaderCode.size());

	// file.close();

	std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();

	return InitFromString(content, shaderType, filePath);
}

bool Shader::InitFromString(std::string shaderCode, GLenum shaderType, std::string filePath) {
	m_type = shaderType;
	m_shader = glCreateShader(shaderType);

	const char* shaderCodePointer = shaderCode.data();
	int shaderCodeLength = shaderCode.size();

	glShaderSource(m_shader, 1, &shaderCodePointer, &shaderCodeLength);
	glCompileShader(m_shader);

	GLint isCompiled;

	glGetShaderiv(m_shader, GL_COMPILE_STATUS, &isCompiled);

	if (!isCompiled) {
		char infolog[1024];
		glGetShaderInfoLog(m_shader, 1024, NULL, infolog);
		std::cout << shaderCode << " Shader compile failed with error: " << std::endl << infolog << std::endl;

		glDeleteShader(m_shader);
		m_shader = 0;
		return false;
	} else {
		return true;
	}
}


void Shader::AttachTo(GLuint program) {
	if (m_shader != 0) {
		glAttachShader(program, m_shader);
	} else {
		std::cout << "Failed to attach shader: Shader not initialized." << std::endl;
	}
}