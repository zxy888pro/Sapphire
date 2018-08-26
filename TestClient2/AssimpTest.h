#pragma once

#include <Graphics.h>
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
	//顶点结构体
	struct Vertex {

		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};


	class ITexture;

	/*struct Texture2D {

		uint id;
		std::string type;
		aiString path;
		};*/


	unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma);

	class Mesh {
	public:
		/*  网格数据  */
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		//网格纹理列表
		std::vector<ITexture*> textures;
		/*  函数  */
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<ITexture*> textures);
		void Draw(Shader shader);
	private:
		/*  渲染数据  */
		unsigned int VAO, VBO, EBO;
		/*  函数  */
		void setupMesh();
	};


	//模型
	class Model
	{
	public:
		/*  函数   */
		Model(char *path)
		{
			loadModel(path);
		}
		void Draw(Shader shader);
	private:
		/*  模型数据  */
		//模型可以包含多个子网格
		std::vector<Mesh> meshes;
		std::string directory;
		/*  函数   */
		void loadModel(std::string path);
		//解析Assimp节点
		void processNode(aiNode *node, const aiScene *scene);
		//解析网格
		Mesh processMesh(aiMesh *mesh, const aiScene *scene);
		//加载纹理
		std::vector<ITexture*> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
			std::string typeName);
	};
}
