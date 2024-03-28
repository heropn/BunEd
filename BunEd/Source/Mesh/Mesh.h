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

struct MaterialData
{
	std::shared_ptr<Texture2D> m_DiffuseTexture = nullptr;
	std::shared_ptr<Texture2D> m_SpecularTexture = nullptr;
	std::shared_ptr<Texture2D> m_NormalsTexture = nullptr;
	glm::vec4 m_Color = glm::vec4(1.0f);
	float m_Shininess = 1.0f;
};

struct BaseVertex
{
	glm::vec3 m_Position;
	glm::vec3 m_Normal;
	//glm::vec3 m_Tangent;
	//glm::vec2 m_TexCoord;
};

struct SubMesh
{
	SubMesh(const std::vector<BaseVertex>& m_Vertices, const std::vector<uint16_t>& m_Indices);
	virtual ~SubMesh();

#ifdef _DEBUG
	std::vector<BaseVertex> m_Vertices;
	std::vector<uint16_t> m_Indices;
#endif

	std::unique_ptr<VertexBuffer> m_VertexBuffer = nullptr;
	std::unique_ptr<IndexBuffer> m_IndexBuffer = nullptr;
	std::unique_ptr<VertexArray> m_VertexArray = nullptr;

	VertexBufferLayout m_Layout;
	MaterialData m_MaterialData;
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

	const std::vector<std::shared_ptr<SubMesh>>& GetSubMeshes() const { return m_SubMeshes; }

protected:

	std::vector<std::shared_ptr<SubMesh>> m_SubMeshes;

	std::string m_FileName;

private:

	bool LoadModel(const std::string& filePath);
	void ProcessNode(aiNode* node, const aiScene* scene);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void LoadMaterialTexture(const aiMaterial* material, const aiTextureType textureType, const aiScene* scene, std::shared_ptr<Texture2D>& texture);
};
