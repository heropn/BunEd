#include "pch.h"
#include "Mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "Renderer/TexturesManager.h"
#include "Renderer/Texture2D.h"

SubMesh::SubMesh(const std::vector<BaseVertex>& m_Vertices, const std::vector<uint16_t>& m_Indices)
#ifdef _DEBUG
	: m_Vertices(m_Vertices), m_Indices(m_Indices)
#endif
{
	m_VertexBuffer = std::make_unique<VertexBuffer>(m_Vertices.data(), m_Vertices.size() * sizeof(BaseVertex));
	m_IndexBuffer = std::make_unique<IndexBuffer>(m_Indices.data(), m_Indices.size() * sizeof(uint16_t));
	m_VertexArray = std::make_unique<VertexArray>();
	m_Layout.Push(3, GL_FLOAT, false);
	m_Layout.Push(3, GL_FLOAT, false);

	m_VertexArray->AddBuffer(*m_VertexBuffer, m_Layout, *m_IndexBuffer);
}

SubMesh::~SubMesh()
{
}

std::shared_ptr<Mesh> Mesh::CreateMesh(const std::string& filePath)
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

	if (mesh->LoadModel(filePath))
	{
		return mesh;
	}

	return nullptr;
}

Mesh::~Mesh()
{
}

bool Mesh::LoadModel(const std::string& filePath)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filePath,
		aiProcess_Triangulate |
		aiProcess_RemoveRedundantMaterials |
		aiProcess_OptimizeMeshes |
		aiProcess_GenNormals// |
		//aiProcess_CalcTangentSpace
	);

	if (scene == nullptr)
	{
		return false;
	}

	m_FileName = filePath;

	ProcessNode(scene->mRootNode, scene);

	return true;
}

void Mesh::ProcessNode(aiNode* node, const aiScene* scene)
{
	// Process all meshes in node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene);
	}

	// Process all child nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

void Mesh::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	// Data to fill
	std::vector<BaseVertex> vertices;
	vertices.reserve(mesh->mNumVertices);
	std::vector<uint16_t> indices;
	indices.reserve(mesh->mNumVertices / 3);

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		BaseVertex vertex;

		vertex.m_Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.m_Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		//vertex.m_Tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);

		//if (mesh->HasTextureCoords(0))
		//{
		//	vertex.m_TexCoord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		//}

		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		const aiFace& face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	m_SubMeshes.emplace_back(std::make_shared<SubMesh>(vertices, indices));

	if (mesh->mMaterialIndex >= 0)
	{
		const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		LoadMaterialTexture(material, aiTextureType_DIFFUSE, scene, m_SubMeshes.back()->m_MaterialData.m_DiffuseTexture);
		LoadMaterialTexture(material, aiTextureType_SPECULAR, scene, m_SubMeshes.back()->m_MaterialData.m_SpecularTexture);
		LoadMaterialTexture(material, aiTextureType_NORMALS, scene, m_SubMeshes.back()->m_MaterialData.m_NormalsTexture);
	
		// todo: fix, somehow even tho 'Ns' value is specified in .obj file is properly parsed to shininess value
		float shininess = 0.0f;
		if (material->Get(AI_MATKEY_SHININESS, shininess))
		{
			m_SubMeshes.back()->m_MaterialData.m_Shininess = shininess;
		}

		aiColor4D color;
		if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color))
		{
			m_SubMeshes.back()->m_MaterialData.m_Color = glm::vec4(color.r, color.g, color.b, color.a);
		}
	}

}

void Mesh::LoadMaterialTexture(const aiMaterial* material, const aiTextureType textureType, const aiScene* scene, std::shared_ptr<Texture2D>& texture)
{
	for (unsigned int i = 0; i < material->GetTextureCount(textureType); i++)
	{
		assert(i < 1);
		aiString str;
		material->GetTexture(textureType, i, &str);
	
		const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(str.C_Str());
		if (embeddedTexture)
		{
			// todo: embedded textures not supported yet
			assert(false);
		}
		else
		{
			const std::string& dir = m_FileName.substr(0, m_FileName.find_last_of("/\\"));
			const std::string& textureFileName = dir + '/' + std::string(str.C_Str());
			texture = TexturesManager::Get().FindOrCreateTexture2D(textureFileName);
		}
	
		assert(texture);
	}
}
