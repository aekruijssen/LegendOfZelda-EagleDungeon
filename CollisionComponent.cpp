#include "CollisionComponent.h"
#include "Actor.h"
#include <algorithm>


CollisionComponent::CollisionComponent(class Actor* owner)
:Component(owner)
,mWidth(0.0f)
,mHeight(0.0f)
{
	mOwner = owner;
	ownerScale = mOwner->GetScale();
}

bool CollisionComponent::Intersect(const CollisionComponent* other)
{
	//A is to the left of B
	bool case1 = (GetMax().x < other->GetMin().x);

	//A is above B
	bool case2 = (other->GetMax().y < GetMin().y);

	//A is to the right of B
	bool case3 = (other->GetMax().x < GetMin().x);

	//A is below B
	bool case4 = (GetMax().y < other->GetMin().y);

	if (!(case1 || case2 || case3 || case4)) {
		return true;
	}
	else {
		return false;
	}
}

Vector2 CollisionComponent::GetMin() const
{
	Vector2 min;
	min.x = (mOwner->GetPosition()).x - (mWidth*ownerScale/2.0f);
	min.y = (mOwner->GetPosition()).y - (mHeight * ownerScale/2.0f);
	
	if (min.x != NULL && min.y != NULL) {
		return min;
	}
	return Vector2::Zero;
}

bool comp(float a, float b) {
	return (a < b);
}
Vector2 CollisionComponent::GetMax() const
{
	Vector2 max;
	max.x = (mOwner->GetPosition()).x + ((mWidth*(mOwner->GetScale()) / 2.0f));
	max.y = (mOwner->GetPosition()).y + ((mHeight * (mOwner->GetScale()) / 2.0f));

	if (max.x != NULL && max.y != NULL) {
		return max;
	}
	return Vector2::Zero;
}

const Vector2& CollisionComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

CollSide CollisionComponent::GetMinOverlap(
	const CollisionComponent* other, Vector2& offset)
{
	offset = Vector2::Zero;

	if (!Intersect(other)) {
		return CollSide::None;
	}
	
	//“this” corresponds to the player in the diagram, and “other” corresponds to the block
	//differences are always “other” side minus “this” side
	//calculate dx1, dx2, dy1, and dy2
	float dx1 = other->GetMin().x - GetMax().x;
	float dx2 = other->GetMax().x - GetMin().x;
	float dy1 = other->GetMin().y - GetMax().y;
	float dy2 = other->GetMax().y - GetMin().y;
	
	//Whichever of dx1, dx2, dy1, dy2 has the lowest absolute value is the side of “other” that is minimally overlapped

	//float lowest = std::min({abs(dx1), abs(dx2), abs(dy1), abs(dy2)}, comp);
	float lowest;
	if (abs(dx1) < abs(dx2)) {
		if (abs(dx1) < abs(dy1)) {
			if (abs(dx1) < abs(dy2)) {
				lowest = dx1;
			}
			else {
				lowest = dy2;
			}
		}
		else if(abs(dy1) < abs(dy2)){
			lowest = dy1;
		}
		else {
			lowest = dy2;
		}
	}
	else if (abs(dx2) < abs(dy1)) {
		if (abs(dx2) < abs(dy2)) {
			lowest = dx2;
		}
		else {
			lowest = dy2;
		}
	}
	else if(abs(dy1) < abs(dy2)){
		lowest = dy1;
	}
	else {
		lowest = dy2;
	}

	// If you collide with the left / right side of “other”, you should change offset.x
	if (lowest == dx1) {
		offset.x += dx1;
		return CollSide::Left;
	}
	else if (lowest == dx2) {
		offset.x += dx2;
		return CollSide::Right;
	}
	// If you collide with the top / bottom side of “other”, you should change offset.y
	else if (lowest == dy1) {
		offset.y += dy1;
		return CollSide::Top;
	}
	else if (lowest == dy2) {
		offset.y += dy2;
		return CollSide::Bottom;
	}

	return CollSide::None;
}
