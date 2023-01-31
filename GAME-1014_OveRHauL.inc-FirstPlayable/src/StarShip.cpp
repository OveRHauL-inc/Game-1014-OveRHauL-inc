#include "StarShip.h"
#include "TextureManager.h"
#include "Util.h"
#include "Game.h"

StarShip::StarShip()
{
	TextureManager::Instance().Load("../Assets/textures/ncl.png", "StarShip");

	const auto size = TextureManager::Instance().GetTextureSize("StarShip");
	SetWidth(static_cast<int>(size.x));
	SetHeight(static_cast<int>(size.y));
	GetTransform()->position = glm::vec2(0.0f, 0.0f);
	GetRigidBody()->bounds = glm::vec2(GetWidth(), GetHeight());
	GetRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	GetRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	GetRigidBody()->isColliding = false;

	// starting motion properties
	m_maxSpeed = 50.0f; // max number of pixels moved per frame
	m_turnRate = 5.0f; // max number of degrees to turn per time step
	m_accelerationRate = 4.0f; // max number of pixels to add to the velocity each frame

	SetCurrentDirection(glm::vec2(1.0f, 0.0f)); // facing right

	SetType(GameObjectType::AGENT);
}

StarShip::~StarShip() = default;

void StarShip::Draw()
{
	// draw the target
	TextureManager::Instance().Draw("StarShip", GetTransform()->position, static_cast<double>(GetCurrentHeading()), 255, true);
}

void StarShip::Update()
{
	m_move();
}

void StarShip::Clean()
{
}

float StarShip::GetMaxSpeed() const
{
	return m_maxSpeed;
}

float StarShip::GetTurnRate() const
{
	return m_turnRate;
}

float StarShip::GetAccelerationRate() const
{
	return m_accelerationRate;
}

glm::vec2 StarShip::GetDesiredVelocity() const
{
	return m_desiredVelocity;
}

void StarShip::SetMaxSpeed(const float speed)
{
	m_maxSpeed = speed;
}

void StarShip::SetTurnRate(const float angle)
{
	m_turnRate = angle;
}

void StarShip::SetAccelerationRate(const float rate)
{
	m_accelerationRate = rate;
}

void StarShip::SetDesiredVelocity(const glm::vec2 target_position)
{
	SetTargetPosition(target_position);
	m_desiredVelocity = Util::Normalize(target_position - GetTransform()->position) * GetMaxSpeed();
	GetRigidBody()->velocity = m_desiredVelocity - GetRigidBody()->velocity;
}

void StarShip::Seek()
{
	SetDesiredVelocity(GetTargetPosition());

	const glm::vec2 steering_direction = GetDesiredVelocity() - GetCurrentDirection();

	LookWhereYoureGoing(steering_direction);

	GetRigidBody()->acceleration = GetCurrentDirection() * GetAccelerationRate();
}

void StarShip::LookWhereYoureGoing(glm::vec2 target_direction)
{
	const float target_rotation = Util::SignedAngle(GetCurrentDirection(), target_direction);

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

void StarShip::m_move()
{
	Seek();

	// maybe switch - case

	// switch (behaviour)
	//		case(seek)
	//		case(arrive)
	//		case(flee)
	//		case(avoidance)

	// kinematic equation -> position final = position initial + velocity initial * (time) + (0.5) * acceleration initial * (time*time)
	//                       Pf             = Pi +               Vi *               (time) + (0.5) * Ai *                   (time*time)
	const float dt = Game::Instance().GetDeltaTime();
	std::cout << "DeltaTime: " << dt << std::endl;

	// compute position term
	const glm::vec2 initial_position = GetTransform()->position;

	// compute velocity term
	const glm::vec2 velocity_term = GetRigidBody()->velocity * dt; 

	// compute acceleration term
	const glm::vec2 acceleration_term = GetRigidBody()->acceleration * 0.5f; // * dt*dt


	// compute new position
	glm::vec2 final_position = initial_position + velocity_term + acceleration_term;

	GetTransform()->position = final_position;

	// add acceleration to velocity
	GetRigidBody()->velocity += GetRigidBody()->acceleration;

	// clamp our velocity to max speed
	GetRigidBody()->velocity = Util::Clamp(GetRigidBody()->velocity, GetMaxSpeed());

}

void StarShip::Move()
{
	GetTransform()->position = GetTransform()->position + GetRigidBody()->velocity * 5.0f;
}


void StarShip::CheckBounds()
{
}

void StarShip::Reset()
{
}


