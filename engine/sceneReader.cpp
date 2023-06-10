#include "sceneReader.h"
#include "cg_engine.h"
#include <iostream>
#include <string>
#include <glm/gtc/packing.hpp>
#include <glm/gtx/string_cast.hpp>

Material* SceneReader::readMaterial(char* data) {
    unsigned int position = 0;
    
    // Material name:
    char* materialName = (char*) malloc(sizeof(char) * FILENAME_MAX);
    strcpy(materialName, data + position);
    position += (unsigned int)strlen(materialName) + 1;

    // Material term colors, starting with emissive:
    glm::vec3 emission, albedo;
    memcpy(&emission, data + position, sizeof(glm::vec3));
    position += sizeof(glm::vec3);

    // Albedo:
    memcpy(&albedo, data + position, sizeof(glm::vec3));
    position += sizeof(glm::vec3);

    // Roughness factor:
    float roughness;
    memcpy(&roughness, data + position, sizeof(float));
    position += sizeof(float);


    Material* material = new Material(
        materialName,
        glm::vec4(emission.x, emission.y, emission.z, 1.0f),
        glm::vec4(albedo.x * 0.2f, albedo.y * 0.2f, albedo.z * 0.2f, 1.0f),
        glm::vec4(albedo.x * 0.6f, albedo.y * 0.6f, albedo.z * 0.6f, 1.0f),
        glm::vec4(albedo.x * 0.4f, albedo.y * 0.4f, albedo.z * 0.4f, 1.0f),
        (1 - sqrt(roughness)) * 128);

    // Metalness factor:
    position += sizeof(float);

    // Transparency factor:
    position += sizeof(float);

    // Albedo texture filename, or [none] if not used:
    char textureName[FILENAME_MAX];
    strcpy(textureName, data + position);
    position += (unsigned int)strlen(textureName) + 1;
    if (strcmp(textureName, "[none]") != 0) {
        /*Texture* t = new Texture(textureName, true, 4, 1);
        std::string filename = "../scene/texture/" + std::string(textureName);
        t->loadBitmap(filename.c_str());
        material->setTexture(t);*/
    }

    // Normal map filename, or [none] if not used:
    char normalMapName[FILENAME_MAX];
    strcpy(normalMapName, data + position);
    position += (unsigned int)strlen(normalMapName) + 1;

    // Height map filename, or [none] if not used:
    char heightMapName[FILENAME_MAX];
    strcpy(heightMapName, data + position);
    position += (unsigned int)strlen(heightMapName) + 1;

    // Roughness map filename, or [none] if not used:
    char roughnessMapName[FILENAME_MAX];
    strcpy(roughnessMapName, data + position);
    position += (unsigned int)strlen(roughnessMapName) + 1;

    // Metalness map filename, or [none] if not used:
    char metalnessMapName[FILENAME_MAX];
    strcpy(metalnessMapName, data + position);
    position += (unsigned int)strlen(metalnessMapName) + 1;

    std::cout << "Material - ID: " << material->getId() << ", Name: " << material->getName() << ", Texture: " << textureName << std::endl;

    return material;
}

Node* SceneReader::readNode(char* data) {
    unsigned int position = 0;
    
    // Node name:
    char* nodeName = (char*)malloc(sizeof(char) * FILENAME_MAX);
    strcpy(nodeName, data + position);
    Node* node = new Node(nodeName);
    position += (unsigned int)strlen(nodeName) + 1;

    // Node matrix:
    glm::mat4 matrix;
    memcpy(&matrix, data + position, sizeof(glm::mat4));
    node->setObjectCoordinates(matrix);
    position += sizeof(glm::mat4);

    // Nr. of children nodes:
    unsigned int children;
    memcpy(&children, data + position, sizeof(unsigned int));
    position += sizeof(unsigned int);

    // Optional target node, [none] if not used:
    char targetName[FILENAME_MAX];
    strcpy(targetName, data + position);
    position += (unsigned int)strlen(targetName) + 1;

    for (int i = 0; i < children; i++) {
        node->addChild(dynamic_cast<Node*>(SceneReader::typeSelect(file, materials)));
    }

    std::cout << "Node - ID: " << node->getId() << ", Name: " << node->getName() << ", Children: " << node->getChildrenCount() << std::endl;

    return node;
}

Light* SceneReader::readLight(char* data) {
    unsigned int position = 0;
    
    Light* light;

    // Light name:
    char* lightName = (char*)malloc(sizeof(char) * FILENAME_MAX);
    strcpy(lightName, data + position);
    position += (unsigned int)strlen(lightName) + 1;

    // Light matrix:
    glm::mat4 matrix;
    memcpy(&matrix, data + position, sizeof(glm::mat4));
    position += sizeof(glm::mat4);

    // Nr. of children nodes:
    unsigned int children;
    memcpy(&children, data + position, sizeof(unsigned int));
    position += sizeof(unsigned int);

    // Optional target node name, or [none] if not used:
    char targetName[FILENAME_MAX];
    strcpy(targetName, data + position);
    position += (unsigned int)strlen(targetName) + 1;

    // Light subtype (see OvLight SUBTYPE enum):
    unsigned char subtype;
    memcpy(&subtype, data + position, sizeof(unsigned char));
    char subtypeName[FILENAME_MAX];
    position += sizeof(unsigned char);

    // Light color:
    glm::vec3 color;
    memcpy(&color, data + position, sizeof(glm::vec3));
    position += sizeof(glm::vec3);

    // Influence radius:
    float radius;
    memcpy(&radius, data + position, sizeof(float));
    position += sizeof(float);

    // Direction:
    glm::vec3 direction;
    memcpy(&direction, data + position, sizeof(glm::vec3));
    position += sizeof(glm::vec3);

    // Cutoff:
    float cutoff;
    memcpy(&cutoff, data + position, sizeof(float));
    position += sizeof(float);

    // Exponent:
    float spotExponent;
    memcpy(&spotExponent, data + position, sizeof(float));
    position += sizeof(float);

    // Cast shadow flag:
    unsigned char castShadows;
    memcpy(&castShadows, data + position, sizeof(unsigned char));
    position += sizeof(unsigned char);

    // Volumetric lighting flag:
    unsigned char isVolumetric;
    memcpy(&isVolumetric, data + position, sizeof(unsigned char));
    position += sizeof(unsigned char);

    switch (subtype)
    {
    case 1: // Directional
    {
        strcpy(subtypeName, "directional");
        light = new DirectionalLight(lightName, radius);

        break;
    }
    break;

    case 0: // Omnidirectional / Point
    {
        strcpy(subtypeName, "omni");
        light = new PointLight(lightName, radius);

        break;
    }

    case 2: // Spotlight
    {
        strcpy(subtypeName, "spot");
        light = new SpotLight(lightName, direction, radius);
        light->setCutoff(cutoff);

        break;
    }

    default: strcpy(subtypeName, "UNDEFINED");
    }

    light->setObjectCoordinates(matrix);

    light->setAmbient(glm::vec4(color.r, color.g, color.b, 1.0f));
    light->setDiffuse(glm::vec4(color.r, color.g, color.b, 1.0f));
    light->setSpecular(glm::vec4(1.0f));

    for (int i = 0; i < children; i++) {
        light->addChild(dynamic_cast<Node*>(SceneReader::typeSelect(file, materials)));
    }

    std::cout << "Light - ID: " << light->getId() << ", Name: " << light->getName() << ", Children: " << light->getChildrenCount() << ", Type: " << subtypeName << std::endl;

    return light;
}

Mesh* SceneReader::readMesh(char* data, std::vector<Material*> materials) {
    unsigned int position = 0;

    // Mesh name:
    char* meshName = (char*)malloc(sizeof(char) * FILENAME_MAX);
    strcpy(meshName, data + position);
    Mesh* mesh = new Mesh(meshName);
    position += (unsigned int)strlen(meshName) + 1;

    // Mesh matrix:
    glm::mat4 matrix;
    memcpy(&matrix, data + position, sizeof(glm::mat4));
    mesh->setObjectCoordinates(matrix);
    position += sizeof(glm::mat4);

    // Mesh nr. of children nodes:
    unsigned int children;
    memcpy(&children, data + position, sizeof(unsigned int));
    position += sizeof(unsigned int);

    // Optional target node, or [none] if not used:
    char targetName[FILENAME_MAX];
    strcpy(targetName, data + position);
    position += (unsigned int)strlen(targetName) + 1;

    // Mesh subtype (see OvMesh SUBTYPE enum): (SKIPPED)
    position += sizeof(unsigned char);

    // Material name, or [none] if not used:
    char materialName[FILENAME_MAX];
    strcpy(materialName, data + position);


    for (int i = 0; i < materials.size(); i++)
    {
        if (strcmp(materials[i]->getName(), materialName) == 0) {
            mesh->setMaterial(materials[i]);
            break;
        }
    }

    position += (unsigned int)strlen(materialName) + 1;

    // Mesh bounding sphere radius:
    float radius;
    memcpy(&radius, data + position, sizeof(float));
    mesh->setBoundingSphereRadius(radius);
    position += sizeof(float);

    // Mesh bounding box minimum corner: (SKIPPED)
    position += sizeof(glm::vec3);

    // Mesh bounding box maximum corner: (SKIPPED)
    position += sizeof(glm::vec3);

    // Optional physics properties: (SKIPPED)
    unsigned char hasPhysics;
    memcpy(&hasPhysics, data + position, sizeof(unsigned char));
    position += sizeof(unsigned char);
    if (hasPhysics)
    {
        /**
         * Mesh physics properties.
         */
        struct PhysProps
        {
            // Pay attention to 16 byte alignement (use padding):      
            unsigned char type;
            unsigned char contCollisionDetection;
            unsigned char collideWithRBodies;
            unsigned char hullType;

            // Vector data:
            glm::vec3 massCenter;

            // Mesh properties:
            float mass;
            float staticFriction;
            float dynamicFriction;
            float bounciness;
            float linearDamping;
            float angularDamping;
            unsigned int nrOfHulls;
            unsigned int _pad;

            // Pointers:
            void* physObj;
            void* hull;
        };

        PhysProps mp;
        memcpy(&mp, data + position, sizeof(PhysProps));
        position += sizeof(PhysProps);

        // Custom hull(s) used?
        if (mp.nrOfHulls)
        {
            for (unsigned int c = 0; c < mp.nrOfHulls; c++)
            {
                // Hull number of vertices:
                unsigned int nrOfVertices;
                memcpy(&nrOfVertices, data + position, sizeof(unsigned int));
                position += sizeof(unsigned int);

                // Hull number of faces:
                unsigned int nrOfFaces;
                memcpy(&nrOfFaces, data + position, sizeof(unsigned int));
                position += sizeof(unsigned int);

                // Hull centroid:
                glm::vec3 centroid;
                memcpy(&centroid, data + position, sizeof(glm::vec3));
                position += sizeof(glm::vec3);

                position += sizeof(glm::vec3) * nrOfVertices;
                position += sizeof(unsigned int) * 3 * nrOfFaces;
            }
        }
    }

    // Nr. of LODs:
    unsigned int LODs;
    memcpy(&LODs, data + position, sizeof(unsigned int));
    position += sizeof(unsigned int);


    // For each LOD...:
    std::vector<unsigned int> verticesPerLOD(LODs); // Let's store this information for the skinned part, in case
    for (unsigned int l = 0; l < LODs; l++)
    {
        // Nr. of vertices:
        unsigned int vertices, faces;
        memcpy(&vertices, data + position, sizeof(unsigned int));
        position += sizeof(unsigned int);
        verticesPerLOD[l] = vertices;

        // ...and faces:
        memcpy(&faces, data + position, sizeof(unsigned int));
        position += sizeof(unsigned int);

        // Interleaved and compressed vertex/normal/UV/tangent data:                    
        for (unsigned int c = 0; c < vertices; c++)
        {

            // Vertex coords:    
            glm::vec3 vertex;
            memcpy(&vertex, data + position, sizeof(glm::vec3));
            position += sizeof(glm::vec3);

            // Vertex normal:
            unsigned int normalData;
            memcpy(&normalData, data + position, sizeof(unsigned int));
            glm::vec4 normal = glm::unpackSnorm3x10_1x2(normalData);
            position += sizeof(unsigned int);

            // Texture coordinates:
            unsigned int textureData;
            memcpy(&textureData, data + position, sizeof(unsigned int));
            glm::vec2 uv = glm::unpackHalf2x16(textureData);
            position += sizeof(unsigned int);

            // Tangent vector: (SKIPPED)
            position += sizeof(unsigned int);

            if (l == 0) {
                mesh->addVertex(vertex);
                mesh->addNormal(normal);
                mesh->addTexCoords(uv);
            }
        }

        // Faces:
        for (unsigned int c = 0; c < faces; c++)
        {
            // Face indexes:
            unsigned int face[3];
            memcpy(face, data + position, sizeof(unsigned int) * 3);
            if (l == 0) {
                mesh->addFace(face);
            }

            position += sizeof(unsigned int) * 3;
        }
    }


    for (int i = 0; i < children; i++) {
        mesh->addChild(dynamic_cast<Node*>(SceneReader::typeSelect(file, materials)));
    }

    

    std::cout << "Mesh - ID: " << mesh->getId() << ", Name: " << mesh->getName() << ", Children: " << mesh->getChildrenCount() << ", Material: " << materialName << std::endl;

    mesh->initVAO();


    return mesh;
}

Object* SceneReader::typeSelect(FILE* dat, std::vector<Material*> materials) {
    Object* obj = nullptr;

    unsigned int chunkId, chunkSize;
    fread(&chunkId, sizeof(unsigned int), 1, dat);
    if (feof(dat))
        return obj;
    fread(&chunkSize, sizeof(unsigned int), 1, dat);

    //std::cout << "\n[chunk id: " << chunkId << ", chunk size: " << chunkSize;

    // Load whole chunk into memory:
    char* data = new char[chunkSize];
    if (fread(data, sizeof(char), chunkSize, dat) != chunkSize)
    {
        std::cout << "ERROR: unable to read from file '" << std::endl;
        fclose(dat);
        delete[] data;
        return obj;
    }

    unsigned int position = 0;
    switch (chunkId)
    {
        case 0: //Object
        {
            // OVO revision number:
            unsigned int versionId;
            memcpy(&versionId, data + position, sizeof(unsigned int));
            position += sizeof(unsigned int);

            break;
        }

        case 1: // Node
            obj = SceneReader::readNode(data);
            break;

        case 9: // Material
            obj = SceneReader::readMaterial(data);
            break;

        case 16: // Light
            obj = SceneReader::readLight(data);
            break;

        case 18: // Mesh
            obj = SceneReader::readMesh(data, materials);
            break;

        case 25: // Terminator
        {
            std::cout << "End" << std::endl;
            break;
        }

        default: //
        {
            std::cout << "UNKNOWN]" << std::endl;

            if (chunkId >= 0 && chunkId <= 25) {
                std::cout << "ERROR: Unsupported OVO Object" << std::endl;
            }
            else {
                std::cout << "ERROR: corrupted or bad data in file " << std::endl;
            }

            delete[] data;
            break;
        }
    }
    // Release chunk memory:
    delete[] data;

    return obj;
}