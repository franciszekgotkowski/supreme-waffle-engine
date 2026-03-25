#pragma once

#include <primitives/typedefs.h>

typedef i32 ShaderProgramID;

ShaderProgramID CreateShaderProgram(
	str vertexShaderPath,
	str fragmentShaderPath
);

void DeleteShaderProgram(
	ShaderProgramID shaderProgramID
);

void UseShaderProgram(
	ShaderProgramID shaderProgramID
);