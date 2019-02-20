#pragma once
#include "Component.h"
#include <functional>

class EnemyComponent : public Component
{
public:
	EnemyComponent(class Actor* owner);
	~EnemyComponent();

	void SetOnCollect(std::function<void()> onCollect);
	void TakeDamage(int amount);

private:
	int hp;
	int damage;
	std::function<void()> mOnDeath;
	std::function<void()> mOnTakeDamage;
};