#include "GraphicsEngine.pch.h"
#include "Shader.h"

void Shader::DeleteData()
{
	delete myBlob;
}
