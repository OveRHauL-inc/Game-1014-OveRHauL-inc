#pragma once
#ifndef __BASIC_ENEMY__
#define __BASIC_ENEMY__

#include "Agent.h"
#include "DisplayObject.h"

class BasicEnemy : public Agent
{
public:
	BasicEnemy();
	~BasicEnemy() override;

	void Draw() override;
	void Update() override;
	void Clean() override;

	float GetMaxSpeed() const;
	float GetTurnRate() const;
	glm::vec2 GetDesiredVelocity() const;

	void SetMaxSpeed(float speed);
	void SetTurnRate(float angle);
	void SetDesiredVelocity(glm::vec2 player_position);

	void Seek();
	void LookWhereYoureGoing(glm::vec2 player_direction);
	bool deleteMe;

private:
	float m_maxSpeed;
	float m_turnRate;
	glm::vec2 m_desiredVelocity;

	void m_move();
	void CheckBounds();
	void Reset();
};

#endif
