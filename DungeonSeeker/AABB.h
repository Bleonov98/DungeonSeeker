#ifndef AABB_H
#define AABB_H

#include <glm/glm.hpp>

class AABB {
public:

    // basic

    bool Intersects(const AABB& other) const {
        return (min.x <= other.max.x && max.x >= other.min.x &&
            min.y <= other.max.y && max.y >= other.min.y);
    }

    bool IntersectMap(const glm::vec2 mapPos, const glm::vec2 mapSize) const {
        return (min.x <= mapPos.x + mapSize.x && max.x >= mapPos.x &&
            min.y <= mapPos.y + mapSize.y && max.y >= mapPos.y);
    }

    bool IntersectPoint(const glm::vec2 point) const {
        return (min.x <= point.x && max.x >= point.x &&
            min.y <= point.y && max.y >= point.y);
    }

    // sides

    bool IntersectBottom(const AABB& other) {
        return (min.x <= other.max.x - 5.0f && max.x >= other.min.x + 5.0f &&
            max.y >= other.min.y && max.y <= other.min.y + 7.0f);
    }

    bool IntersectTop(const AABB& other) {
        return (min.x <= other.max.x - 5.0f && max.x >= other.min.x + 5.0f &&
            min.y <= other.max.y && min.y >= other.max.y - 7.0f);
    }

    bool IntersectLeft(const AABB& other) {
        return (min.x <= other.max.x && min.x >= other.max.x - 7.0f &&
            min.y <= other.max.y - 5.0f && max.y >= other.min.y + 5.0f);
    }

    bool IntersectRight(const AABB& other) {
        return (max.x <= other.min.x + 7.0f && max.x >= other.min.x &&
            min.y <= other.max.y - 5.0f && max.y >= other.min.y + 5.0f);
    }

    // update

    void SetBorder(const glm::vec2& newMin, const glm::vec2& newMax) {
        min = newMin;
        max = newMax;
    }

private:
    glm::vec2 min;
    glm::vec2 max;
};

#endif // !AABB_H