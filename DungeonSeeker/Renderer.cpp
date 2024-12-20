#include "Renderer.h"

#include <unordered_set>

void Renderer::InitRenderData()
{
    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &colourVBO);
    glGenBuffers(1, &matBuffer);
    glGenBuffers(1, &indexVBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::Draw(Texture texture, std::vector<glm::mat4> instancedMatrices, std::vector<glm::vec3> instancedColours)
{
    glBindVertexArray(this->VAO);

    // refresh colour buffer
    glBindBuffer(GL_ARRAY_BUFFER, colourVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * instancedColours.size(), &instancedColours.data()[0], GL_STATIC_DRAW);

    // colour instructions
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    
    glVertexAttribDivisor(1, 1);

    // refresh matrix buffer
    glBindBuffer(GL_ARRAY_BUFFER, matBuffer);
    glBufferData(GL_ARRAY_BUFFER, instancedMatrices.size() * sizeof(glm::mat4), &instancedMatrices.data()[0], GL_STATIC_DRAW);

    // matrix instructions
    std::size_t vec4Size = sizeof(glm::vec4);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, instancedMatrices.size());
    glBindVertexArray(0);
}

void Renderer::Draw(std::vector<glm::mat4> instancedMatrices, std::vector<glm::vec3> instancedColours, std::vector<GLuint> textureIDs)
{
    glBindVertexArray(this->VAO);

    Shader& shader = ResourceManager::GetShader("spriteShader").Use();

    // make unique IDs
    std::vector<GLuint> uniqueID;
    for (size_t i = 0; i < textureIDs.size(); i++)
    {
        if (std::find(uniqueID.begin(), uniqueID.end(), textureIDs[i]) == uniqueID.end()) {
            uniqueID.push_back(textureIDs[i]);
        }
    }

    // make IDs for shader
    std::vector<GLuint> sTextureIDs; 
    for (size_t i = 0; i < textureIDs.size(); i++)
    {
        for (size_t j = 0; j < uniqueID.size(); j++)
        {
            if (textureIDs[i] == uniqueID[j]) sTextureIDs.push_back(j);
        }
    }

    // Refresh texture ID buffer
    glBindBuffer(GL_ARRAY_BUFFER, indexVBO);
    glBufferData(GL_ARRAY_BUFFER, sTextureIDs.size() * sizeof(GLuint), sTextureIDs.data(), GL_STATIC_DRAW);

    // IDs instructions
    glEnableVertexAttribArray(1);
    glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, sizeof(GLuint), (void*)0);
    glVertexAttribDivisor(1, 1);

    // ------------------------------------------
    // refresh colour buffer
    glBindBuffer(GL_ARRAY_BUFFER, colourVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * instancedColours.size(), instancedColours.data(), GL_STATIC_DRAW);

    // colour instructions
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glVertexAttribDivisor(2, 1);

    // ------------------------------------------
    // refresh matrix buffer
    glBindBuffer(GL_ARRAY_BUFFER, matBuffer);
    glBufferData(GL_ARRAY_BUFFER, instancedMatrices.size() * sizeof(glm::mat4), instancedMatrices.data(), GL_STATIC_DRAW);

    // matrix instructions
    std::size_t vec4Size = sizeof(glm::vec4);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    for (size_t i = 0; i < uniqueID.size(); i++)
    {
        glUniform1i(glGetUniformLocation(shader.GetID(), ("image[" + std::to_string(i) + "]").c_str()), i);
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, uniqueID[i]);
    }

    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, instancedMatrices.size());

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(0);
}

void Renderer::DrawTexture(Texture texture)
{
    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Renderer::~Renderer()
{
    glDeleteVertexArrays(1, &this->VAO);
}