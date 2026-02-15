#include "Object.h"

#include <GLFW/glfw3.h>

namespace Grafyte 
{
	Object::Object(const void* positions, unsigned int positionsSize, unsigned int vertexCount, const unsigned int* indices, unsigned int indicesCount, const std::string& shaderSourcePath)
		: vertexCount(vertexCount),
		  renderData(ObjectRenderData{
			  IndexBuffer(indices, indicesCount), VertexArray(), VertexBuffer(positions, positionsSize),
			  VertexBufferLayout(), Shader(shaderSourcePath), Texture(), false, 0
		  })
	{

		// Copy vertex data
		data.resize(positionsSize / sizeof(float));
		memcpy(data.data(), positions, positionsSize);
	}

	Object::~Object()
	{
		if (!glfwGetCurrentContext()) {
			return;
		}

		renderData.va.Unbind();
		renderData.ib.Unbind();
		renderData.vb.Unbind();
		renderData.shader.Unbind();
		renderData.texture.Unbind();
	}

	void Object::SetTexture(const std::string& textureSourcePath, unsigned int slot) {
		renderData.shader.Bind();
		renderData.texture.Set(textureSourcePath);
		renderData.texture.Bind(slot);
		renderData.shader.SetUniform1i("u_Texture", slot);
		renderData.hasTexture = true;
		renderData.textureSlot = slot;
	}

	void Object::AddLayoutSlot(std::string type, unsigned int size)
	{
		if (type == "float")
			renderData.layout.Push<float>(size);
		else if (type == "unsigned int")
			renderData.layout.Push<unsigned int>(size);
		else if (type == "unsigned char")
			renderData.layout.Push<unsigned char>(size);
		else
			__debugbreak();
	}

	void Object::AddBufferToVertexArray()
	{
		renderData.va.Bind();
		renderData.va.AddBuffer(renderData.vb, renderData.layout);
	}

	void Object::SetShaderUniform1f(const std::string& name, float value)
	{
		renderData.shader.Bind();
		renderData.shader.SetUniform1f(name, value);
	}

	void Object::SetShaderUniform4f(const std::string& name, float float_x, float float_y, float float_z, float float_w)
	{
		renderData.shader.Bind();
		renderData.shader.SetUniform4f(name, float_x, float_y, float_z, float_w);
	}

	void Object::SetShaderUniformMat4f(const std::string& name, const glm::mat4& matrix)
	{
		renderData.shader.Bind();
		renderData.shader.SetUniformMat4f(name, matrix);
	}

	void Object::Move(float offsetX, float offsetY)
	{
		unsigned int floatsPerVertex = 0;
		for (const auto& element : renderData.layout.GetElements()) {
			if (element.type == GL_FLOAT)
				floatsPerVertex += element.count;
		}

		if (floatsPerVertex == 0) return;

		for (unsigned int i = 0; i < vertexCount; i++) {
			data[i * floatsPerVertex] += offsetX;
			data[i * floatsPerVertex + 1] += offsetY;
		}

		UpdateVertexBuffer();
	}


	void Object::UpdateVertexBuffer()
	{
		renderData.vb.Bind();
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), static_cast<void*>(data.data()), GL_STATIC_DRAW);
	}

}
