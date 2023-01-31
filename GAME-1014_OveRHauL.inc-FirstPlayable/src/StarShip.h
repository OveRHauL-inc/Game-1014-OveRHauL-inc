#pragma once
#ifndef __STARSHIP__
#define __STARSHIP__

#include "Agent.h"
#include "DisplayObject.h"

class StarShip : public Agent
{
public:
	StarShip();
	~StarShip() override;

	void Draw() override;
	void Update() override;
	void Clean() override;

	// getters and setters
	float GetMaxSpeed() const;
	float GetTurnRate() const;
	float GetAccelerationRate() const;
	glm::vec2 GetDesiredVelocity() const;

	void SetMaxSpeed(float speed);
	void SetTurnRate(float angle);
	void SetAccelerationRate(float rate);
	void SetDesiredVelocity(glm::vec2 target_position);

	void Seek();
	void LookWhereYoureGoing(glm::vec2 target_direction);
private:
	// private movement variables
	float m_maxSpeed;
	float m_turnRate;
	float m_accelerationRate;

	// where we want to go
	glm::vec2 m_desiredVelocity;

	void m_move();

	void Move();
	void CheckBounds();
	void Reset();
	
};
#endif
