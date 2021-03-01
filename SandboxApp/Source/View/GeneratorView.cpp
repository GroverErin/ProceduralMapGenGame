#include "GeneratorView.h"
#include "World/GenertionSettings/GeneratorConfig.h"

#include <ApplicationLayer.h>
#include <Managers/Input.h>
#include <Components/TransformComponent.h>

// Temporary solution to
// broken Engine Log.
#include <iostream>
#include <string>

GeneratorView::GeneratorView(std::shared_ptr<Exelius::Actor> pActor)
	: IView(pActor)
	, m_worldGenerator()
	, m_cloudGenerator()
	, m_fireGenerator()
	, m_worldMap(kWorldWidth, kWorldHeight)
	, m_pPlayerTransform(nullptr)
	, m_pPlayerTexture(nullptr)
	, m_playerMovement({ 0.0f, -1.0f })
	, m_waterTankFull(false)
	, m_pUIText(nullptr)
{
}

bool GeneratorView::Initialize()
{
	auto* pGameLayer = Exelius::IApplicationLayer::GetInstance()->GetGameLayer();
	m_pPlayerTransform = m_pOwner.lock()->GetComponent<Exelius::TransformComponent>();
	m_pPlayerTexture = m_pOwner.lock()->GetComponent<Exelius::TextureComponent>();

	std::string testActor = "Actors/TestObj.xml";
	auto& resourceFile = pGameLayer->GetResourceFile();
	m_pUI = pGameLayer->GetActorFactory().CreateActor(resourceFile.LoadResource(testActor.c_str()));
	pGameLayer->AddActor(m_pUI->GetId(), m_pUI);

	m_pUIText = m_pUI->GetComponent<Exelius::TextRenderComponent>();

	PrintInstructions();
	m_worldGenerator.GenerateWorld(m_worldMap);
	m_cloudGenerator.GenerateClouds();
	m_fireGenerator.StartFire(m_worldMap);
	return true;
}

void GeneratorView::UpdateInput(float deltaTime)
{
	if (deltaTime > 0.0333333f)
		deltaTime = 0.033333f;

	float fireRatio = m_fireGenerator.GetBurnPercentage() * 100.0f;
	if (m_waterTankFull)
	{
		m_pUIText->ChangeText(("Water Tank is Full! \t" + std::to_string((int)fireRatio) + "% of land Destroyed!").c_str(), 255, 0, 0);
	}
	else
	{
		m_pUIText->ChangeText(("Water Tank is Empty! \t" + std::to_string((int)fireRatio) + "% of land Destroyed!").c_str(), 255, 0, 0);
	}

	auto* pKeyboard = Exelius::IApplicationLayer::GetInstance()->GetKeyboardInput();

	// If player presses 'esc'...
	if (pKeyboard->IsKeyReleased(Exelius::GenericKeyboard::Code::kCodeEsc) || (int)fireRatio >= kLosePercent)
	{
		std::cout << "YOU LOST!\n";
		RestartGame();
		return;
	}

	else if (pKeyboard->IsKeyPressed(Exelius::GenericKeyboard::Code::kCodeQ))
	{
		Exelius::IApplicationLayer::GetInstance()->GetWindow()->Quit();
		return;
	}

	else if (pKeyboard->IsKeyPressed(Exelius::GenericKeyboard::Code::kCodeSpace))
	{
		bool wasTankFull = m_waterTankFull;
		m_waterTankFull = IsPlayerOverWater();

		if (wasTankFull && m_waterTankFull)
		{
			std::cout << "Water Tank Already Full!\n";
		}
		else if (!wasTankFull && m_waterTankFull)
		{
			std::cout << "Water Tank Refilled!\n";
		}
		else if (!wasTankFull && !m_waterTankFull)
		{
			std::cout << "Water Tank Is Empty, Go Fill Up Over Water!\n";
		}
		else
		{
			std::cout << "Dumping Water!\n";
			ExtinguishFire();
		}
	}

	SetMovementVector();
	
	float faceDirection = 0.0f;
	faceDirection = ((std::atan2f(m_playerMovement.y, m_playerMovement.x) * 180.0f) / Exelius::PI) + 90.0f;
	m_pPlayerTexture->SetAngle(faceDirection);

	float playerSpeed = 100.0f;
	m_pPlayerTransform->Move(m_playerMovement.x * playerSpeed * deltaTime, m_playerMovement.y * playerSpeed * deltaTime);

	ForcePlayerBounds();

	m_cloudGenerator.UpdateClouds(deltaTime);
	bool gameOver = m_fireGenerator.PropagateFire(deltaTime);

	if (gameOver)
	{
		std::cout << "YOU WIN!\n";
		RestartGame();
	}
}

void GeneratorView::ViewScene()
{
	auto& graphics = Exelius::IApplicationLayer::GetInstance()->GetGraphicsRef();
	graphics->StartDrawing(81, 92, 129);

	// These should become actors and added to the list.
	// Technically, this code could live in the engine entirely.
	m_worldMap.RenderMap();

	auto* pGameLayer = Exelius::IApplicationLayer::GetInstance()->GetGameLayer();
	for (auto& actorPair : pGameLayer->GetActors())
	{
		actorPair.second->Render(graphics);
	}

	m_cloudGenerator.Render();

	graphics->EndDrawing();
}

void GeneratorView::PrintInstructions()
{
	std::cout << "Press 'Esc' to regenerate the world.\n\n";
	std::cout << "Press 'Q' to close the application.\n\n";
	std::cout << "Press Arrow Keys to change direction.\n\n";
	std::cout << "Press 'Space' while over water to fill up the water tank.\n\n";
	std::cout << "Press 'Space' while over fire to extinguish it.\n\n";
	std::cout << "Win by extinguishing all the fire before losing 80% of the land!.\n\n";
}

void GeneratorView::SetMovementVector()
{
	auto* pKeyboard = Exelius::IApplicationLayer::GetInstance()->GetKeyboardInput();

	Exelius::Vector2f newDirection = { 0.0f, 0.0f };

	if (pKeyboard->IsKeyDown(Exelius::GenericKeyboard::Code::kCodeArrowUp))
	{
		newDirection.y = -1.0f;
	}

	else if (pKeyboard->IsKeyDown(Exelius::GenericKeyboard::Code::kCodeArrowDown))
	{
		newDirection.y = 1.0f;
	}

	if (pKeyboard->IsKeyDown(Exelius::GenericKeyboard::Code::kCodeArrowLeft))
	{
		newDirection.x = -1.0f;
	}

	else if (pKeyboard->IsKeyDown(Exelius::GenericKeyboard::Code::kCodeArrowRight))
	{
		newDirection.x = 1.0f;
	}

	m_playerMovement = (m_playerMovement + newDirection).Normalize();
}

void GeneratorView::ForcePlayerBounds()
{
	if (m_pPlayerTransform->GetX() > (m_worldMap.GetMapWidth() - 24))
	{
		m_pPlayerTransform->SetX((float)(m_worldMap.GetMapWidth() - 24));
		m_playerMovement = m_playerMovement * -1;
	}

	if (m_pPlayerTransform->GetY() > (m_worldMap.GetMapHeight() - 24))
	{
		m_pPlayerTransform->SetY((float)(m_worldMap.GetMapHeight() - 24));
		m_playerMovement = m_playerMovement * -1;
	}

	if (m_pPlayerTransform->GetX() < 0.0f)
	{
		m_pPlayerTransform->SetX(0.0f);
		m_playerMovement = m_playerMovement * -1;
	}

	if (m_pPlayerTransform->GetY() < 0.0f)
	{
		m_pPlayerTransform->SetY(0.0f);
		m_playerMovement = m_playerMovement * -1;
	}
}

bool GeneratorView::IsPlayerOverWater()
{
	// Get all the tiles underneath the player.
	auto tiles = m_worldMap.GetTilesOfColorInArea({ (int)m_pPlayerTransform->GetX(), (int)m_pPlayerTransform->GetY(), 48, 48 }, kOcean);

	if (tiles.size() < 1500)
		return false;

	// If there is a non-water tile in the group
	/*for (auto tile : tiles)
	{
		if (m_worldMap.GetTileColor(tile) != kOcean.GetHex())
			return false;
	}*/

	return true;
}

void GeneratorView::ExtinguishFire()
{
	// Get all the tiles underneath the player.
	auto tiles = m_worldMap.GetTilesOfColorInArea({ (int)m_pPlayerTransform->GetX(), (int)m_pPlayerTransform->GetY(), 48, 48 }, Exelius::Colors::Flame);

	// If there is a non-water tile in the group
	for (auto tile : tiles)
	{
		m_fireGenerator.ExtinguishTile(tile);
	}
}

void GeneratorView::RestartGame()
{
	// Reset the generator to default settings and tilemap to new map.
	m_worldGenerator.ResetGenerator();
	m_worldGenerator.GenerateWorld(m_worldMap);
	m_fireGenerator.ResetFireGenerator();
	m_fireGenerator.StartFire(m_worldMap);

	m_pPlayerTransform->SetX(kWorldWidth / 2);
	m_pPlayerTransform->SetY(kWorldHeight / 2);

	m_playerMovement = { 0.0f, -1.0f };
	m_waterTankFull = false;
}
