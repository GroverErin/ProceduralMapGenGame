#pragma once

#include <Game/View.h>
#include <Components/TransformComponent.h>
#include <Components/TextureComponent.h>
#include <Components/TextRenderComponent.h>

#include "World/WorldGeneration/WorldGenerator.h"
#include "World/CloudGeneration/CloudGenerator.h"
#include "World/FireGeneration/FireGenerator.h"

class GeneratorView
	: public Exelius::IView
{
public:

	static const int kLosePercent = 80;

	//------------------------------------------------------------------------------------------------------
	// View Functions
	//------------------------------------------------------------------------------------------------------
	GeneratorView(std::shared_ptr<Exelius::Actor> pActor);

	virtual bool Initialize() final override;
	virtual void UpdateInput(float deltaTime) final override;
	virtual void ViewScene() final override;

private:
	//------------------------------------------------------------------------------------------------------
	// Gameplay Functions & Variables
	//------------------------------------------------------------------------------------------------------
	void PrintInstructions();

	void SetMovementVector();

	void ForcePlayerBounds();

	bool IsPlayerOverWater();

	void ExtinguishFire();

	void RestartGame();

	WorldGenerator m_worldGenerator;

	CloudGenerator m_cloudGenerator;

	FireGenerator m_fireGenerator;

	TileMap m_worldMap;

	Exelius::TransformComponent* m_pPlayerTransform;
	Exelius::TextureComponent* m_pPlayerTexture;
	Exelius::Vector2f m_playerMovement;
	bool m_waterTankFull;

	std::shared_ptr<Exelius::Actor> m_pUI;
	Exelius::TextRenderComponent* m_pUIText;
};