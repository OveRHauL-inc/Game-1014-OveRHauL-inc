#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Target.h"
#include "StarShip.h"
#include "BasicEnemy.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene() override;

	// Scene LifeCycle Functions
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;
	virtual void HandleEvents() override;
	virtual void Start() override;
private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;

	// Scene Game Objects
	Target* m_pTarget;
	StarShip* m_pStarShip;
	BasicEnemy* m_pBasicEnemy;
	std::vector<BasicEnemy*> m_pBasicEnemyVec;
	glm::vec2 m_mousePosition;

	// Debugging Variables
	bool m_bDebugView;
};

#endif /* defined (__PLAY_SCENE__) */