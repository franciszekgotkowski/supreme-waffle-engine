#include <external/glad/glad.h>
#include <assert.h>
#include <export/platform/shader.h>
#include <export/platform/file_io.h>
#include <primitives/typedefs.h>
#include <primitives/errors.h>
#include <stdio.h>
#include <unistd.h>

static i32 CompileVertexShader(
	str vertexShaderSource
) {
	assert(vertexShaderSource);

	u32 vertexID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexID, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexID);

	i32 success;
	char infoLog[512];
	glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexID, 512, NULL, infoLog);
		printf("Error compiling vertex shader:\n%s\n\n", infoLog);
	}
	assert(success);
	return vertexID;
}


static i32 CompileFragmentShader(
	str fragmentShaderSource
) {
	assert(fragmentShaderSource);

	u32 fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentID, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentID);

	i32 success;
	char infoLog[512];
	glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentID, 512, NULL, infoLog);
		printf("Error compiling fragment shader:\n%s\n\n", infoLog);
	}
	assert(success);
	return fragmentID;
}

static i32 LinkShaderProgram(
	i32 vertexShaderID,
	i32 fragmentShaderID
) {

	i32 shaderProgramID = glCreateProgram();

	glAttachShader(shaderProgramID, vertexShaderID);
	glAttachShader(shaderProgramID, fragmentShaderID);
	glLinkProgram(shaderProgramID);

	i32 success;
	char infoLog[512];
	glGetShaderiv(shaderProgramID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderProgramID, 512, NULL, infoLog);
		printf("Error compiling shader program:\n%s\n\n", infoLog);
	}
	assert(success);

	return shaderProgramID;
}

ShaderProgramID CreateShaderProgram(
	str vertexShaderPath,
	str fragmentShaderPath
) {
	assert(vertexShaderPath);
	assert(fragmentShaderPath);

	Error err;
	FileData vertexShaderFile;
	FileData fragmentShaderFile;

	err = readEntireTextFile(vertexShaderPath, &vertexShaderFile);
	assert(err == OK);

	err = readEntireTextFile(fragmentShaderPath, &fragmentShaderFile);
	assert(err == OK);

	i32 vertexShaderID = CompileVertexShader(vertexShaderFile.ptr);
	i32 fragmentShaderID = CompileFragmentShader(fragmentShaderFile.ptr);

	ShaderProgramID shaderProgramID = LinkShaderProgram(vertexShaderID, fragmentShaderID);

	err = freeEntireTextFile(vertexShaderFile);
	assert(err == OK);
	err = freeEntireTextFile(fragmentShaderFile);
	assert(err == OK);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return shaderProgramID;
}

void DeleteShaderProgram(
	ShaderProgramID shaderProgramID
) {
	glDeleteProgram(shaderProgramID);
}

void UseShaderProgram(
	ShaderProgramID shaderProgramID
) {
	glUseProgram(shaderProgramID);
}