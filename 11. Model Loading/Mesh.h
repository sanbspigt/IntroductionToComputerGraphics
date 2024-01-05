#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> // Glad is used to manage OpenGL function pointers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <ShaderH.h> // Include the Shader class

#include <string>
#include <vector>
using namespace std;

#define MAX_BONE_INFLUENCE 4 // Maximum number of bones that can influence a vertex

// Struct to define a vertex
struct Vertex {
    glm::vec3 Position;    // Position of the vertex
    glm::vec3 Normal;      // Normal vector for lighting calculations
    glm::vec2 TexCoords;   // Texture coordinates for texture mapping
    glm::vec3 Tangent;     // Tangent vector for normal mapping
    glm::vec3 Bitangent;   // Bitangent vector for normal mapping
    int m_BoneIDs[MAX_BONE_INFLUENCE]; // Bone IDs for skeletal animation
    float m_Weights[MAX_BONE_INFLUENCE]; // Weights for each bone influence
};

// Struct to define a texture
struct Texture {
    unsigned int id;   // Texture ID
    string type;       // Type of texture (diffuse, specular, etc.)
    string path;       // Path of the texture file
};

// Class representing a mesh in 3D space
class Mesh {
public:
    vector<Vertex> vertices;         // Store vertices of the mesh
    vector<unsigned int> indices;    // Store indices for indexed drawing
    vector<Texture> textures;        // Store textures associated with the mesh
    unsigned int VAO;                // Vertex Array Object ID

    // Constructor to initialize the mesh with vertices, indices, and textures
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        // Set up the vertex buffer, index buffer, and attribute pointers.
        setupMesh();
    }

    // Function to render the mesh
    void Draw(Shader& shader)
    {
        // Bind appropriate textures
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // Activate texture unit
            // Retrieve texture number (N in diffuse_textureN)
            string number;
            string name = textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++);
            else if (name == "texture_normal")
                number = std::to_string(normalNr++);
            else if (name == "texture_height")
                number = std::to_string(heightNr++);

            // Set the sampler to the correct texture unit
            glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
            // Bind the texture
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        // Draw the mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Reset to default
        glActiveTexture(GL_TEXTURE0);
    }

private:
    unsigned int VBO, EBO; // Vertex Buffer Object and Element Buffer Object IDs

    // Initializes the buffer objects and arrays for the mesh
    void setupMesh()
    {
        // Create buffers and arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        // Load data into vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // Set the vertex attribute pointers
        // Vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // Vertex Normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // Vertex Texture Coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        // Vertex Tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        // Vertex Bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
        // Bone IDs
        glEnableVertexAttribArray(5);
        glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));
        // Bone Weights
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));

        glBindVertexArray(0);
    }
};
#endif
