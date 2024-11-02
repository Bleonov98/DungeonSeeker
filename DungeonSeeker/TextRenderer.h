#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "ResourceManager.h"
#include <functional>

struct Symbol {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // size of glyph
    glm::ivec2   Bearing;   // offset from baseline to left/top of glyph
    unsigned int Advance;   // horizontal offset to advance to next glyph
};

class TextRenderer
{
public:

    TextRenderer(unsigned int width, unsigned int height);

    void Load(std::string font, unsigned int fontSize);

    void RenderText(std::string text, glm::vec2 position, float scale, glm::vec3 colour = glm::vec3(1.0f));

protected:

    std::map<char, Symbol> Characters;

    unsigned int VAO, VBO;
};

class TextButton : public TextRenderer
{
public:

    TextButton(std::string text, glm::vec2 position, unsigned int width, unsigned height) : TextRenderer(width, height) {
        this->position = position;
        this->text = text;
    };

    void SetTextColour(glm::vec3 colour) { this->colour = colour; }

    bool TextCollision(float xMouse, float yMouse);

    void RenderButton(float scale);

    template<typename Func, typename Obj, typename... Args>
    void SetFunction(Func&& func, Obj&& obj, Args&&... args) {
        function = std::bind(std::forward<Func>(func), std::forward<Obj>(obj), std::forward<Args>(args)...);
    }

    template<typename Func>
    void SetFunction(Func&& func) {
        function = std::forward<Func>(func);
    }

    void CallFunction() { function(); }

private:

    std::string text;
    glm::vec2 position, size;
    glm::vec3 colour = glm::vec3(0.75f);

    std::function<void()> function;
};

#endif // TEXT_RENDERER_H