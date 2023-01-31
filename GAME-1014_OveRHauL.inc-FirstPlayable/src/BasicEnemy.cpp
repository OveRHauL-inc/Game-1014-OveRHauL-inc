#include "BasicEnemy.h"
#include "TextureManager.h"
#include "Util.h"
#include "Game.h"

BasicEnemy::BasicEnemy()
{
	TextureManager::Instance().Load("../Assets/textures/BasicEnemy.png", "BasicEnemy"); // todo: resize BasicEnemy and make it face left

	auto size = TextureManager::Instance().GetTextureSize("BasicEnemy");
	SetWidth(static_cast<int>(size.x));
	SetHeight(static_cast<int>(size.y));
	GetTransform()->position = glm::vec2(glm::vec2((float)(rand() % 50 + 600), (float)(rand() % 400)));
	GetRigidBody()->bounds = glm::vec2(GetWidth(), GetHeight());
	GetRigidBody()->velocity = glm::vec2(-0.15f, 0.0f);
	GetRigidBody()->isColliding = false;

	m_maxSpeed = 0.15f;
	m_turnRate = 5.0f; 

	SetCurrentDirection(glm::vec2(-1.0f, 1.0f)); // facing left

	SetType(GameObjectType::AGENT);
}

BasicEnemy::~BasicEnemy() = default;

void BasicEnemy::Draw()
{
	TextureManager::Instance().Draw("BasicEnemy", GetTransform()->position, static_cast<double>(GetCurrentHeading()), 255, true);
}

void BasicEnemy::Update()
{
	m_move();
}

void BasicEnemy::Clean()
{

}

float BasicEnemy::GetMaxSpeed() const
{
	return m_maxSpeed;
}

float BasicEnemy::GetTurnRate() const
{
	return m_turnRate;
}

glm::vec2 BasicEnemy::GetDesiredVelocity() const
{
	return m_desiredVelocity;
}

void BasicEnemy::SetMaxSpeed(const float speed)
{
	m_maxSpeed = speed;
}

void BasicEnemy::SetTurnRate(const float angle)
{
	m_turnRate = angle;
}

void BasicEnemy::SetDesiredVelocity(glm::vec2 player_position)
{
	SetTargetPosition(player_position);
	m_desiredVelocity = Util::Normalize(player_position - GetTransform()->position) * GetMaxSpeed();
	GetRigidBody()->velocity = m_desiredVelocity - GetRigidBody()->velocity;
}

void BasicEnemy::Seek()
{
	SetDesiredVelocity(GetTargetPosition());
	const glm::vec2 steering_direction = GetDesiredVelocity() - GetCurrentDirection();

	LookWhereYoureGoing(steering_direction);
}

void BasicEnemy::LookWhereYoureGoing(glm::vec2 player_direction)
{
	const float target_rotation = Util::SignedAngle(GetCurrentDirection(), player_direction);
	const float turn_sensitivity = 5.0f;

	if (abs(target_rotation) > turn_sensitivity)
	{
		if (target_rotation > 0.0f)
		{
			// turn right
			SetCurrentHeading(GetCurrentHeading() + GetTurnRate());
		}
		else if (target_rotation < 0.0f)
		{
			// turn left
			SetCurrentHeading(GetCurrentHeading() - GetTurnRate());
		}
	}
}

void BasicEnemy::m_move()
{
	const float dt = Game::Instance().GetDeltaTime();

	const glm::vec2 initial_position = GetTransform()->position;
	const glm::vec2 velocity_term = GetRigidBody()->velocity * dt;
	glm::vec2 final_position = initial_position + velocity_term;
	GetTransform()->position = final_position;
	GetTransform()->position = GetTransform()->position + GetRigidBody()->velocity * 5.0f;
	GetRigidBody()->velocity = Util::Clamp(GetRigidBody()->velocity, GetMaxSpeed());
}

void BasicEnemy::CheckBounds()
{

}

void BasicEnemy::Reset()
{

}
