#include "pch.h"
#include "Mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "Renderer/TexturesManager.h"
#include "Renderer/Texture2D.h"

SubMesh::SubMesh(const std::vector<BaseVertex>& m_Vertices, const std::vector<uint16_t>& m_Indices)
	: m_Vertices(m_Vertices), m_Indices(m_Indices)
{
	m_VertexBuffer = std::make_unique<VertexBuffer>(m_Vertices.data(), m_Vertices.size() * sizeof(BaseVertex));
	m_IndexBuffer = std::make_unique<IndexBuffer>(m_Indices.data(), m_Indices.size() * sizeof(uint16_t));
	m_VertexArray = std::make_unique<VertexArray>();
	m_Layout.Push(3, GL_FLOAT, false);
	m_Layout.Push(2, GL_FLOAT, false);

	m_VertexArray->AddBuffer(*m_VertexBuffer, m_Layout, *m_IndexBuffer);
}

SubMesh::~SubMesh()
{
}

void SubMesh::Draw()
{
	/*
	Mby each submesh should set its texture etc.
	But some general thing should set matrices for whole mesh
	And even more general should set some uniforms per shader call such lightning etc.
	*/

	m_VertexArray->Bind();

	if (m_DiffuseTexture)
	{
		m_DiffuseTexture->Bind(0);
	}

	glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_SHORT, (const void*)0);
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
		aiProcess_GenSmoothNormals |
		aiProcess_RemoveRedundantMaterials |
		aiProcess_OptimizeMeshes |
		aiProcess_OptimizeGraph
	);

	if (scene == nullptr)
	{
		return false;
		assert(false);
	}

	m_FileName = filePath;

	ProcessNode(scene->mRootNode, scene);

	return true;
}

void Mesh::Draw()
{
	for (auto subMesh : m_SubMeshes)
	{
		subMesh->Draw();
	}
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

		//vertex.normal.x = mesh->mNormals[i].x;
		//vertex.normal.y = mesh->mNormals[i].y;
		//vertex.normal.z = mesh->mNormals[i].z;
		//
		//if (mesh->HasTangentsAndBitangents())
		//{
		//	vertex.tangent.x = mesh->mTangents[i].x;
		//	vertex.tangent.y = mesh->mTangents[i].y;
		//	vertex.tangent.z = mesh->mTangents[i].z;
		//}

		if (mesh->HasTextureCoords(0))
		{
			vertex.m_TexCoord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}

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
		LoadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_diffuse", scene, m_SubMeshes.back()->m_DiffuseTexture);
	
		//aiColor3D colorTemp(0.0f);
	
		//if (material->Get(AI_MATKEY_COLOR_AMBIENT, colorTemp) == AI_SUCCESS)
		//{
		//	meshMaterial.m_BaseData.m_AmbientColor.x = colorTemp.r;
		//	meshMaterial.m_BaseData.m_AmbientColor.y = colorTemp.g;
		//	meshMaterial.m_BaseData.m_AmbientColor.z = colorTemp.b;
		//	meshMaterial.m_BaseData.m_AmbientColor.w = 1.0f;
		//}
		//
		//if (material->Get(AI_MATKEY_COLOR_DIFFUSE, colorTemp) == AI_SUCCESS)
		//{
		//	meshMaterial.m_BaseData.m_DiffuseColor.x = colorTemp.r;
		//	meshMaterial.m_BaseData.m_DiffuseColor.y = colorTemp.g;
		//	meshMaterial.m_BaseData.m_DiffuseColor.z = colorTemp.b;
		//	meshMaterial.m_BaseData.m_DiffuseColor.w = 1.0f;
		//}
		//
		//if (material->Get(AI_MATKEY_COLOR_SPECULAR, colorTemp) == AI_SUCCESS)
		//{
		//	meshMaterial.m_BaseData.m_SpecularColor.x = colorTemp.r;
		//	meshMaterial.m_BaseData.m_SpecularColor.y = colorTemp.g;
		//	meshMaterial.m_BaseData.m_SpecularColor.z = colorTemp.b;
		//}
		//
		//if (material->Get(AI_MATKEY_COLOR_REFLECTIVE, colorTemp) == AI_SUCCESS)
		//{
		//	meshMaterial.m_BaseData.m_ReflectiveColor.x = colorTemp.r;
		//	meshMaterial.m_BaseData.m_ReflectiveColor.y = colorTemp.g;
		//	meshMaterial.m_BaseData.m_ReflectiveColor.z = colorTemp.b;
		//	meshMaterial.m_BaseData.m_ReflectiveColor.w = 1.0f;
		//}
		//
		//float tempExp = 0.0f;
		//if (material->Get(AI_MATKEY_SHININESS, tempExp))
		//{
		//	meshMaterial.m_BaseData.m_SpecularExp = tempExp;
		//}
	}

}

void Mesh::LoadMaterialTexture(const aiMaterial* material, const aiTextureType textureType, const std::string& typeName, const aiScene* scene, std::shared_ptr<Texture2D>& texture)
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