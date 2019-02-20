#include "MoveComponent.h"
#include "Actor.h"

// MoveComponent constructor - initialize order to 50 and speeds to 0
MoveComponent::MoveComponent(class Actor* owner)
:Component(owner, 50)
,mAngularSpeed(0.0f)
,mForwardSpeed(0.0f)
{
	
}


// Update - updates position/rotation of Actor
void MoveComponent::Update(float deltaTime)
{
	// if angular speed is not near zero -> increment rotation by angular speed * deltaTime
	if (!Math::NearZero(mAngularSpeed)) {
		float rot = mOwner->GetRotation();
		rot += mAngularSpeed * deltaTime;
		mOwner->SetRotation(rot);
	}
	// If forward speed is not near zero -> increment position by forward vector * forward speed * delataTime
	if (!Math::NearZero(mForwardSpeed)) {
		Vector2 pos = mOwner->GetPosition();
		pos += mOwner->GetForward() * mForwardSpeed * deltaTime;
		mOwner->SetPosition(pos);
	}
}
