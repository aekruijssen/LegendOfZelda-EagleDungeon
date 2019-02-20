#include "Actor.h"
#include "Game.h"
#include "Component.h"

#include <algorithm>

// Actor constructor - initialize game, state, position, scale, rotation, and add Actor to game
Actor::Actor(Game* game)
	:mGame(game)
	,mState(ActorState::Active) //state = active
	,mPosition(Vector2::Zero) //position = 0
	,mScale(1.0f) //scale = 1
	,mRotation(0.0f) //rotation = 0
{
	// Adds itself to the game via AddActor(remember Actor has an mGame member variable)
	mGame->AddActor(this);
}


// Actor deconstructor - remove Actor from game, delete all components
Actor::~Actor()
{
	// Automatically remove themselves from the Game’s Actor vector
	mGame->RemoveActor(this);
	// Delete each component and then clear the vector
	for (int i = 0; i < mComponents.size(); i++) {
		delete mComponents.at(i);
	}
	mComponents.clear();
}


// Update - update components
void Actor::Update(float deltaTime)
{
	// If state is Active, call Update on all components in vector, then call OnUpdate
	if (mState == ActorState::Active) {
		for (auto component : mComponents) {
			component->Update(deltaTime);
		}
		OnUpdate(deltaTime);
	}
}


// OnUpdate - implemented in subclasses
void Actor::OnUpdate(float deltaTime)
{
}


// ProcessInput - process input for components
void Actor::ProcessInput(const Uint8* keyState)
{
	//if Actor’s state is Active, call ProcessInput on all components, then call OnProcessInput
	if (mState == ActorState::Active) {
		for (auto component : mComponents) {
			component->ProcessInput(keyState);
		}
		OnProcessInput(keyState);
	}
}


// OnProcessInput - implemented in subclasses (Ship)
void Actor::OnProcessInput(const Uint8* keyState)
{
}


// AddComponent - add component to vector
void Actor::AddComponent(Component* c)
{
	mComponents.emplace_back(c);
	std::sort(mComponents.begin(), mComponents.end(), [](Component* a, Component* b) {
		return a->GetUpdateOrder() < b->GetUpdateOrder();
	});
}


// GetForward - returns forward direction vector (used by MoveComponent's Update function)
Vector2 Actor::GetForward() {
	return Vector2(Math::Cos(mRotation), -Math::Sin(mRotation));
}

