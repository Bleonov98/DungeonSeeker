#include "Character.h"

bool Character::ProcessCollision(GameObject& other, bool first, float dt)
{
    // to prevent double collision for the same side
    const int arrSize = 3;
    static bool collision[arrSize]{ false }; // LEFT - 0, RIGHT - 1, TOP - 2;
    if (first) memset(collision, false, arrSize * sizeof(collision[0])); // reset when first iteration

    // offsets depending on hitBox size
    const float topOffset = hBox.GetMin().y - position.y;
    const float leftOffset = hBox.GetMin().x - position.x;
    const float rightOffset = (position.x + size.x) - hBox.GetMax().x;

    if (BottomCollision(other))
        position.y = other.GetPos().y - size.y;
    else if (LeftCollision(other) && !collision[0]) {
        position.x = other.GetPos().x + other.GetSize().x - leftOffset;
        collision[0] = true;
    }
    else if (RightCollision(other) && !collision[1]) {
        position.x = other.GetPos().x - size.x + rightOffset;
        collision[1] = true;
    }
    else if (TopCollision(other) && !collision[2]) {
        position.y = other.GetPos().y + other.GetSize().y - topOffset;
        collision[2] = true;
    }
    else return false;

    return true;
}

void Character::Hit(float damage, AttackType type)
{
    if (damaged) return;

    if (type == PHYSICAL)
        hp -= damage - armor;
    else if (type == MAGICAL)
        hp -= damage - resist;
    else 
        hp -= damage;
}

void Character::Push(glm::vec2 position)
{
    if (damaged) return;

    pushDirection = glm::normalize(this->position - position);

    colour = glm::vec3(1.0f, 0.0f, 0.0f);
    damaged = true;
}

void Character::DamageAnimation(float dt)
{
    if (!damaged) return;

    damageTime += dt;
    
    float pushValue = pushStrength * (1.0f - damageTime);
    position += pushDirection * pushValue * dt;
    
    colour = glm::vec3(1.0f, damageTime, damageTime);
    if (damageTime >= damageDelay) {
        damaged = false;
        colour = glm::vec3(1.0f);
        damageTime = 0.0f;

        if (hp <= 0.0f) 
            isDead = true;
    }
}