#include <Graphics.h>
#include <GraphicDriver.h>
#include <ITexture.h>
#include <ITextureMgr.h>
#include "AssimpTest.h"
#include <logUtil.h>
#include <stringHelper.h>


namespace Sapphire
{

	unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma)
	{
		std::string filename = std::string(path);
		filename = directory + '/' + filename;

		unsigned int textureID = 0;
		//glGenTextures(1, &textureID);

		/*int width, height, nrComponents;
		unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
		}*/

		return textureID;
	}

	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<ITexture*> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		setupMesh();
	}

	void Mesh::Draw(Shader shader)
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		GraphicDriver* pGD = GraphicDriver::GetSingletonPtr();
		//������������
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // �ڰ�֮ǰ������Ӧ������Ԫ
			// ��ȡ������ţ�diffuse_textureN �е� N��
			std::string number;
			ITexture* pTexture = textures[i];
			if (pTexture && pGD->getTextureMgr()->VerifyHWUID(pTexture->getUID())==1)
			{
				/*std::string name = pTexture.type;
				if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
				else if (name == "texture_specular")
				number = std::to_string(specularNr++);*/

				//Ϊÿ��sampler2D��������Ԫ
				std::string  samplerName = StringFormatA("texture_diffuse%d",i);
				shader.setFloat(samplerName, i);
				pGD->BindTexture(pTexture, (TextureUnit)i);
			}
			
		}
		glEnable(GL_DEPTH_TEST);
		//�󶨻�Ĭ������
		glActiveTexture(GL_TEXTURE0);
		
		// ��������
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		pGD->BindTexture(0, TextureUnit::TU_DIFFUSE);
		glDisable(GL_DEPTH_TEST);
	}

	void Mesh::setupMesh()
	{
		//��VAO
		glGenVertexArrays(1, &VAO);

		//��VBO
		glGenBuffers(1, &VBO);
		//��EBO
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//���ö�������
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
		//���ö�����������
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
			&indices[0], GL_STATIC_DRAW);

		// ָ���������ĸ�������
		// ����λ��
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// ���㷨��
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		// ������������
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

		//���VAO
		glBindVertexArray(0);
	}

	void Model::Draw(Shader shader)
	{
		for (int i = 0; i < this->meshes.size(); ++i)
		{
			meshes[i].Draw(shader);
		}
	}

	void Model::loadModel(std::string path)
	{
		//Assimp��������˼��ز�ͬ�ļ���ʽ�����м���ϸ��
		Assimp::Importer importer;
		//Scene��Assimp���ݽṹ�ĸ��ڵ�
		//aiProcess_Triangulate �����λ�: �������ȫ���������ι��ɵģ������еĶ����ת��Ϊ������
		//aiProcess_FlipUVs:��תy���������� OpenGL�󲿷�ͼ������y���Ƿ���
		//aiProcess_GenNormals�����ģ�Ͳ������������Ļ�����Ϊÿ�����㴴�����ߡ�
		//aiProcess_SplitLargeMeshes�����Ƚϴ������ָ�ɸ�С����������������Ⱦ����󶥵������ƣ�ֻ����Ⱦ��С��������ô����ǳ����á�
		//aiProcess_OptimizeMeshes�����ϸ�ѡ���෴�����Ὣ���С����ƴ��Ϊһ��������񣬼��ٻ��Ƶ��ôӶ������Ż���
		const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
		//
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			LogUtil::LogMsgLn(StringFormatA("ERROR::ASSIMP::%s", importer.GetErrorString()));
			return;
		}
		directory = path.substr(0, path.find_last_of('/'));

		//�Ӹ��ڵ㿪ʼ�������нڵ�
		processNode(scene->mRootNode, scene);
	}

	void Model::processNode(aiNode *node, const aiScene *scene)
	{
		// ����ڵ����е���������еĻ���
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}
		// �������������ӽڵ��ظ���һ����
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}

	}

	Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<ITexture*> textures;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			// ������λ�á����ߺ���������
			// ����λ��
			glm::vec3 vector;
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
			//����
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
			if (mesh->mTextureCoords[0]) // �����Ƿ����������ꣿ
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}
		// ��������
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			//����ÿ����
			aiFace face = mesh->mFaces[i];
			//ÿ����Ķ�������
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

			// �������
			if (mesh->mMaterialIndex >= 0)
			{
				aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
				//��������������
				std::vector<ITexture*> diffuseMaps = loadMaterialTextures(material,
					aiTextureType_DIFFUSE, "texture_diffuse");
				textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
				//���ظ߹�����
				std::vector<ITexture*> specularMaps = loadMaterialTextures(material,
					aiTextureType_SPECULAR, "texture_specular");
				textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			}

		return Mesh(vertices, indices, textures);
	}

	std::vector<ITexture*> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
	{
		std::vector<ITexture*> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			ITexture* pTexture = NULL;
			Path path = directory;
			path = path.getParentDir();
			path = path.addTailSlash() + str.C_Str();
			GraphicDriver* pGraphicDriver = GraphicDriver::GetSingletonPtr();
			pTexture = pGraphicDriver->getTextureMgr()->CreateTexture2DFromFile(path.c_str());
			if (pTexture == NULL)
			{
				LogUtil::LogMsgLn(StringFormatA("Load %s Failed!", path.c_str()));
			}
			else
			{
				/*if (typeName == "texture_specular")
				{
					pGraphicDriver->BindTexture(pTexture, TU_SPECULAR);
				}*/
				textures.push_back(pTexture);
			}
			//texture.id = TextureFromFile(str.C_Str(), directory);
			//texture.type = typeName;
			//texture.path = str;
			
		}
		return textures;
	}

}