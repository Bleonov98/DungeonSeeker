#include "Enemy.h"

#include "Potion.h"
#include "Upgrade.h"

#include <random>

void Enemy::CheckPlayer(glm::vec2 playerPos)
{
	if (glm::distance(this->position, playerPos) <= visionRange)
		aState = ATTACK;
	else
		aState = IDLE;
}

void Enemy::Move(glm::vec2 playerPos, float dt)
{
	if (damaged) return;

	glm::vec2 direction = glm::normalize(playerPos - position);
	this->position += direction * speed * dt;

	if (direction.x < 0.0f)
		flipHorizontally = true;
	else
		flipHorizontally = false;
}

std::vector<std::shared_ptr<Item>> Enemy::GetLoot()
{
	std::vector<std::shared_ptr<Item>> loot;
	std::vector<ItemID> itemIDs;

	int totalWeight = 0;
	for (const auto& entry : drop) {
		totalWeight += entry.dropChance;
	}

	if (totalWeight == 0) { // mb isn't needed
		return loot; 
	}

	std::random_device rd;
	std::mt19937 gen(rd());
	
	std::uniform_int_distribution<> dis(0, totalWeight);
	std::uniform_int_distribution<> randIt(1, 2); // amount of items

	int accumulatedWeight = 0, itemAmount = randIt(gen);
	for (size_t i = 0; i < itemAmount; i++)
	{
		int num = dis(gen);
		for (const auto& entry : drop) {
			accumulatedWeight += entry.dropChance;
			if (num <= accumulatedWeight) {
				itemIDs.push_back(entry.itemID);
				break;
			}
		}
	}

	// size of item
	std::uniform_real_distribution<> sizeDis(0.0, 1.0);
	PotionSize pSize;
	(sizeDis(gen) > 0.5f) ? pSize = SMALL : pSize = BIG;

	std::shared_ptr<Item> item;
	for (auto i : itemIDs)
	{
		switch (i)
		{
		case ItemID::HP_POTION:
		{
			std::shared_ptr<HealthPotion> hpPotion = std::make_shared<HealthPotion>(position, size / 2.0f, pSize);
			item = hpPotion;
		}
		break;
		case ItemID::MS_POTION:
		{
			std::shared_ptr<MSPotion> msPotion = std::make_shared<MSPotion>(position, size / 2.0f, pSize);
			item = msPotion;
		}
		break;
		case ItemID::STAT_UPGRADE:
		{
			std::shared_ptr<StatsUpgrade> statsUpgrade = std::make_shared<StatsUpgrade>(position, size / 2.0f);
			item = statsUpgrade;
		}
		break;
		case ItemID::TYPE_UPGRADE:
		{
			std::shared_ptr<DamageTypeUpgrade> typeUpgrade = std::make_shared<DamageTypeUpgrade>(position, size / 2.0f);
			item = typeUpgrade;
		}
		break;
		default:
			break;
		}

		loot.push_back(item);
	}

	return loot;
}

void Skull::Move(glm::vec2 playerPos, float dt)
{
	if (damaged) return;

	enum TpDirection { TOP, RIGHT, BOT, LEFT };
	TpDirection tpDir = static_cast<TpDirection>(rand() % (LEFT + 1));

	glm::vec2 direction = glm::normalize(playerPos - position);

	// teleport to player
	tpTimer += dt;
	if (tpTimer >= tpDelay) 
	{
		float disValue = 125.0f;
		glm::vec2 distance;

		switch (tpDir)
		{
		case TOP:
			distance = glm::vec2(0.0f, -disValue);
			break;
		case RIGHT:
			distance = glm::vec2(disValue, 0.0f);
			break;
		case BOT:
			distance = glm::vec2(0.0f, disValue);
			break;
		case LEFT:
			distance = glm::vec2(-disValue, 0.0f);
			break;
		default:
			break;
		}

		position = playerPos + distance;
		tpTimer = 0.0f;
	}
	else {
		this->position += direction * speed * dt;
	}

	if (direction.x < 0.0f)
		flipHorizontally = true;
	else
		flipHorizontally = false;
}

std::shared_ptr<MagicSphere> Vampire::Attack(glm::vec2 playerPos, float dt)
{
	atkTimer += dt;
	if (atkTimer >= atkDelay)
	{
		glm::vec2 projectileDir = glm::normalize(playerPos - position);

		MagicSphere projectile(shared_from_this(), projectileDir);
		for (size_t i = 0; i < 5; i++)
		{
			projectile.SetTexture("fireball" + std::to_string(i));
		}
		projectile.AddAnimation("main", 0, 5, 0.1f, true);

		atkTimer = 0.0f;
		return std::make_shared<MagicSphere>(projectile);
	}
	else return nullptr;
}
