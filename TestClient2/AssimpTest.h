#pragma once

#include <Sapphire.h>
#include <string>
#include <glm.hpp>
#include <gtc/vec1.inl>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include "Shader.h"


namespace Sapphire
{
	//����ṹ��
	struct Vertex {

		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};


	struct Texture2D {

		uint id;
		std::string type;
		aiString path;
	};


	unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma);

	class Mesh {
	public:
		/*  ��������  */
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		//���������б�
		std::vector<Texture2D> textures;
		/*  ����  */
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture2D> textures);
		void Draw(Shader shader);
	private:
		/*  ��Ⱦ����  */
		unsigned int VAO, VBO, EBO;
		/*  ����  */
		void setupMesh();
	};


	//ģ��
	class Model
	{
	public:
		/*  ����   */
		Model(char *path)
		{
			loadModel(path);
		}
		void Draw(Shader shader);
	private:
		/*  ģ������  */
		//ģ�Ϳ��԰������������
		std::vector<Mesh> meshes;
		std::string directory;
		/*  ����   */
		void loadModel(std::string path);
		void processNode(aiNode *node, const aiScene *scene);
		Mesh processMesh(aiMesh *mesh, const aiScene *scene);
		//��������
		std::vector<Texture2D> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
			std::string typeName);
	};
}
