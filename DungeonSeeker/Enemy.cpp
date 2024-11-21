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
	//
	static int multiplier = 0;

	std::vector<std::shared_ptr<Item>> loot;
	std::vector<ItemID> itemIDs;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 100);

	int increasedRate = 2 * multiplier;
	int num = dis(gen);
	if (num > 10 + increasedRate) num -= increasedRate; // multiplier doesn't work for rare+ items
	for (const auto& entry : drop) {
		if (num <= entry.dropChance) {
			itemIDs.push_back(entry.itemID);
			break;
		}
	}

	// increase the rates if there hasn’t been any loot for a long time
	if (loot.empty()) multiplier++;
	else multiplier = 0;

	std::shared_ptr<Item> item;
	for (auto i : itemIDs)
	{
		switch (i)
		{
		case ItemID::smallHealthPotion:
		{
			std::shared_ptr<HealthPotion> hpPotion = std::make_shared<HealthPotion>(position, size / 2.0f, PotionSize::SMALL);
			item = hpPotion;
		}
		break;
		case ItemID::healthPotion:
		{
			std::shared_ptr<HealthPotion> hpPotion = std::make_shared<HealthPotion>(position, size / 2.0f, PotionSize::BIG);
			item = hpPotion;
		}
		break;
		case ItemID::smallMsPotion:
		{
			std::shared_ptr<MSPotion> msPotion = std::make_shared<MSPotion>(position, size / 2.0f, PotionSize::SMALL);
			item = msPotion;
		}
		break;
		case ItemID::msPotion:
		{
			std::shared_ptr<MSPotion> msPotion = std::make_shared<MSPotion>(position, size / 2.0f, PotionSize::BIG);
			item = msPotion;
		}
		break;
		case ItemID::statsUpgrade:
		{
			std::shared_ptr<Upgrade> statsUpgrade = std::make_shared<Upgrade>(position, size / 2.0f, UpgradeType::UpgradeStats);
			item = statsUpgrade;
		}
		break;
		case ItemID::typeUpgrade:
		{
			std::shared_ptr<Upgrade> typeUpgrade = std::make_shared<Upgrade>(position, size / 2.0f, UpgradeType::UpgradeType);
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
