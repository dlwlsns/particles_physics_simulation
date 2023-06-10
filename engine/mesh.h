#pragma once
#include "node.h"
#include "material.h"

class LIB_API Mesh : public Node
{
	private:
		unsigned int vaoGlobal;
		unsigned int vboVertex;
		unsigned int vboNormals;
		unsigned int vboTex;
		unsigned int vboFace;
		std::vector<glm::vec3> verticies;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> texCoords;
		std::vector<unsigned int> faces;
		Material* material;
		bool isVaoInit;

		float boundingSphereRadius;
		bool boundingSphere;
	public:
		Mesh(char* name);
		virtual ~Mesh();

		Material* getMaterial();
		void setMaterial(Material* material);

		void addVertex(glm::vec3 vertex);
		std::vector<glm::vec3> getVertecies();
		void addNormal(glm::vec3 normal);
		void addFace(unsigned int face[3]);
		std::vector<unsigned int> getFaces();
		void addTexCoords(glm::vec2 texCoords);
		void setBoundingSphereRadius(float radius);
		float getBoundingSphereRadius();
		void toggleBoundingSphere();
		bool isBoundingSphereEnabled();

		void initVAO();

		void enableShadow(bool hasShadow);
		bool hasShadow();

		virtual void render(glm::mat4 inverseCamera_M);
		void renderShadow(glm::mat4 inverseCamera_M);
};

