#pragma once
#include "Component.h"
#include <functional>

class EnemyComponent : public Component
{
public:
	EnemyComponent(class Actor* owner);
	~EnemyComponent();

	void SetOnTakeDamage(std::function<void()> onDamage);
	void SetOnDeath(std::function<void()> onDeath);
	void TakeDamage(int amount);

	int hp;
	int amt;
	int inv;

private:

	int damage;
	std::function<void()> mOnDeath;
	std::function<void()> mOnTakeDamage;
};