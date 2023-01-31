#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include <thread>
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"
#include <vector>

PlayScene::PlayScene()
{
	PlayScene::Start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::Draw()
{
	DrawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance().GetRenderer(), 255, 255, 255, 255);
	if(m_bDebugView)
	{
		Util::DrawCircle(m_pTarget->GetTransform()->position, m_pTarget->GetWidth() * 0.5f);

		if(m_pStarShip->IsEnabled())
		{
			Util::DrawRect(m_pStarShip->GetTransform()->position - glm::vec2(m_pStarShip->GetWidth() * 0.5f, m_pStarShip->GetHeight() * 0.5f), 
				m_pStarShip->GetWidth(), m_pStarShip->GetHeight());

			CollisionManager::RotateAABB(m_pStarShip, m_pStarShip->GetCurrentHeading());
		}
		if(m_pBasicEnemy->IsEnabled())
		{
			for (int i = 0; i < 5; i++)
			{
				Util::DrawRect(m_pBasicEnemyVec[i]->GetTransform()->position - glm::vec2(m_pBasicEnemyVec[i]->GetWidth() * 0.5f, m_pBasicEnemyVec[i]->GetHeight() * 0.5f),
					m_pBasicEnemyVec[i]->GetWidth(), m_pBasicEnemyVec[i]->GetHeight());

				CollisionManager::RotateAABB(m_pBasicEnemyVec[i], m_pBasicEnemyVec[i]->GetCurrentHeading());
			}
		}
	}
}

void PlayScene::Update()
{
	UpdateDisplayList();
	if(m_pStarShip->IsEnabled())
	{
		CollisionManager::CircleAABBCheck(m_pStarShip,m_pTarget);
	}
	if (m_pBasicEnemy->IsEnabled())
	{
		for(int i = 0; i < 5; i++)
		{
			CollisionManager::CircleAABBCheck(m_pBasicEnemyVec[i], m_pStarShip);
		}
	}
}

void PlayScene::Clean()
{
	RemoveAllChildren();
}

void PlayScene::HandleEvents()
{
	EventManager::Instance().Update();

	// handle player movement if no Game Controllers found
	if (SDL_NumJoysticks() < 1)
	{

	}
	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_ESCAPE))
	{
		Game::Instance().Quit();
	}

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_1))
	{
		Game::Instance().ChangeSceneState(SceneState::START);
	}

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_2))
	{
		Game::Instance().ChangeSceneState(SceneState::END);
	}
}

void PlayScene::Start()
{
	// Set GUI Title
	m_guiTitle = "CLUSTERSCAPE";
	m_bDebugView = false; // turn off debug colliders

	// Add Target to the Scene
	m_pTarget = new Target(); // instantiate an object of type Target
	m_pTarget->GetTransform()->position = glm::vec2(500.0f, 100.0f);
	AddChild(m_pTarget);


	// Add StarShip to Scene
	m_pStarShip = new StarShip();
	m_pStarShip->GetTransform()->position = glm::vec2(100.0f, 400.0f);
	m_pStarShip->SetCurrentDirection(glm::vec2(1.0f, 0.0f)); // looking right
	m_pStarShip->SetTargetPosition(m_pTarget->GetTransform()->position);
	m_pStarShip->SetEnabled(false);
	AddChild(m_pStarShip);

	// Add BasicEnemy to Scene
	m_pBasicEnemy = new BasicEnemy();
	if (m_pBasicEnemy->IsEnabled())
	{
		for (int i = 0; i < 5; i++)
		{
			m_pBasicEnemyVec.push_back(new BasicEnemy());
			m_pBasicEnemyVec[i]->GetTransform()->position = glm::vec2((float)(rand() % 50 + 750), (float)(rand() % 554 + 23));
			m_pBasicEnemyVec[i]->SetCurrentDirection(glm::vec2(-1.0f, 0.0f));
			AddChild(m_pBasicEnemyVec[i]);
		}
	}
	//m_pBasicEnemy->GetTransform()->position = glm::vec2(600.0f, 100.0f);
	//m_pBasicEnemy->SetCurrentDirection(glm::vec2(-1.0f, 0.0f)); // looking left
	//m_pBasicEnemy->SetTargetPosition(m_pStarShip->GetTransform()->position);
	m_pBasicEnemy->SetEnabled(true);


	//auto enemyDirection = Util::Normalize(m_pStarShip->GetTransform()->position - m_pBasicEnemy->GetTransform()->position);
	//m_pBasicEnemy->GetRigidBody()->velocity = enemyDirection * m_pBasicEnemy->GetMaxSpeed();

	// Pre Load sounds
	SoundManager::Instance().Load("../Assets/Audio/yay.ogg", "yay", SoundType::SOUND_SFX);

	auto direction = Util::Normalize(m_pTarget->GetTransform()->position - m_pStarShip->GetTransform()->position);
	m_pStarShip->GetRigidBody()->velocity = direction * m_pStarShip->GetMaxSpeed();

	/*std::cout << "Current Direction: " << m_pStarShip->GetCurrentDirection().x << ", " << m_pStarShip->GetCurrentDirection().y << std::endl;
	std::cout << "Current Heading: " << m_pStarShip->GetCurrentHeading() << std::endl;*/


	ImGuiWindowFrame::Instance().SetGuiFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::GUI_Function() 
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();

	ImGui::Begin("Lab 2", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	ImGui::Separator();

	// Debug Properties
	static bool toggleDebug = false;
	if(ImGui::Checkbox("Toggle Debug", &toggleDebug))
	{
		m_bDebugView = toggleDebug;
	}

	ImGui::Separator();

	// Target Properties
	static float position[2] = { m_pTarget->GetTransform()->position.x, m_pTarget->GetTransform()->position.y };
	if(ImGui::SliderFloat2("Target Position", position, 0.0f, 800.0f))
	{
		m_pTarget->GetTransform()->position = glm::vec2(position[0], position[1]);
		m_pStarShip->SetTargetPosition(m_pTarget->GetTransform()->position);
	}

	ImGui::Separator();

	// StarShip properties
	static bool toggleSeek = m_pStarShip->IsEnabled();
	if(ImGui::Checkbox("Toggle Seek", &toggleSeek))
	{
		m_pStarShip->SetEnabled(toggleSeek);
	}

	ImGui::Separator();

	static float acceleration = m_pStarShip->GetAccelerationRate();
	if(ImGui::SliderFloat("Acceleration Rate", &acceleration, 0.0f, 50.0f))
	{
		m_pStarShip->SetAccelerationRate(acceleration);
		m_pStarShip->GetRigidBody()->acceleration = m_pStarShip->GetCurrentDirection() * m_pStarShip->GetAccelerationRate();
	}

	ImGui::Separator();

	static float turn_rate = m_pStarShip->GetTurnRate();
	if (ImGui::SliderFloat("Turn Rate", &turn_rate, 0.0f, 20.0f))
	{
		m_pStarShip->SetTurnRate(turn_rate);
	}

	ImGui::Separator();

	if(ImGui::Button("Reset"))
	{
		// reset StarShip Position
		m_pStarShip->GetTransform()->position = glm::vec2(100.0f, 400.0f);

		// reset target position
		m_pTarget->GetTransform()->position = glm::vec2(500.0f, 100.0f);

		// reset current heading (orientation)
		m_pStarShip->SetCurrentHeading(0.0f);
		m_pStarShip->SetCurrentDirection(glm::vec2(1.0f, 0.0f));

		// reset acceleration
		m_pStarShip->SetAccelerationRate(4.0f);

		// reset turn rate
		m_pStarShip->SetTurnRate(5.0f);

		// reset the target for the StarShip
		m_pStarShip->SetTargetPosition(m_pTarget->GetTransform()->position);
	}

	ImGui::End();
}
