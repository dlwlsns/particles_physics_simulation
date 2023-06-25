#pragma once
#include "node.h"
#include "material.h"

class LIB_API Mesh : public Node
{
	private:
		unsigned int vaoGlobal;
		unsigned int vboVertex;
		unsigned int vboNormals;
		unsigned int vboFace;
		unsigned int vboColor;
		unsigned int vboTransform;
		unsigned int ssboTransform;
		unsigned int ssboVelocity;
		unsigned int ssboAcceleration;

		std::vector<glm::vec3> verticies;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> faces;
		std::vector<glm::vec4> velocities;
		

		std::vector<glm::vec4> matrices;
		std::vector<glm::vec4> pingPongMatrices;

		Material* material;
		bool isVaoInit;

		float boundingSphereRadius;
		bool boundingSphere;
	public:
		std::vector<glm::vec3> colors;
		Mesh(char* name);
		virtual ~Mesh();

		Material* getMaterial();
		void setMaterial(Material* material);

		void addVertex(glm::vec3 vertex);
		std::vector<glm::vec3> getVertecies();
		void addNormal(glm::vec3 normal);
		void addFace(unsigned int face[3]);
		std::vector<unsigned int> getFaces();
		void setBoundingSphereRadius(float radius);
		float getBoundingSphereRadius();
		void toggleBoundingSphere();
		bool isBoundingSphereEnabled();

		void addTransform(glm::vec4 matrix);
		std::vector<glm::vec4> getMatrices();
		void addVelocity(glm::vec4 matrix);
		std::vector<glm::vec4> getVelocities();

		void initVAO();
		unsigned int getVAO();
		void pingPongBufferSwap();

		virtual void render(glm::mat4 inverseCamera_M);
};

