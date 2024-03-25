#pragma once
#include <memory>
#include <vector>
#include "glm/glm.hpp"
#include "assimp/material.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/VertexBufferLayout.h"

struct aiMesh;
struct aiNode;
struct aiScene;

class Texture2D;

struct BaseVertex
{
	glm::vec3 m_Position;
	glm::vec2 m_TexCoord;
};

struct SubMesh
{
	SubMesh(const std::vector<BaseVertex>& m_Vertices, const std::vector<uint16_t>& m_Indices);
	virtual ~SubMesh();

	void Draw();

	std::vector<BaseVertex> m_Vertices;
	std::vector<uint16_t> m_Indices;

	std::unique_ptr<VertexBuffer> m_VertexBuffer = nullptr;
	std::unique_ptr<IndexBuffer> m_IndexBuffer = nullptr;
	std::unique_ptr<VertexArray> m_VertexArray = nullptr;
	VertexBufferLayout m_Layout;

	std::shared_ptr<Texture2D> m_DiffuseTexture = nullptr;
};

class Mesh
{
public:

	Mesh() = default;
	Mesh(Mesh&) = delete;
	Mesh(Mesh&&) = delete;
	virtual ~Mesh();

	// todo: change and create mesh manager instead of something like this
	static std::shared_ptr<Mesh> CreateMesh(const std::string& filePath);

	void Draw();

protected:

	std::vector<std::shared_ptr<SubMesh>> m_SubMeshes;

	std::string m_FileName;

private:

	bool LoadModel(const std::string& filePath);
	void ProcessNode(aiNode* node, const aiScene* scene);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void LoadMaterialTexture(const aiMaterial* material, const aiTextureType textureType, const std::string& typeName, const aiScene* scene, std::shared_ptr<Texture2D>& texture);
};
