#include "CloudGenerator.h"
#include "World/GenertionSettings/GeneratorConfig.h"

#include <ApplicationLayer.h>
#include <Managers/Graphics.h>

CloudGenerator::CloudGenerator()
	: m_renderOffset(0.0f)
{
	ResetGenerator();

	//Create all of the worker threads that we will need.
	m_pThreadPool = new std::thread[kMaxThreads];
}

CloudGenerator::~CloudGenerator()
{
	delete[] m_pThreadPool;
}

void CloudGenerator::ResetGenerator()
{
	m_cloudParameters.SetParameters(kDefaultCloudOctaves, kDefaultCloudInputRange, kDefaultCloudPersistance, (unsigned int)m_rand.Rand());
}

void CloudGenerator::GenerateClouds()
{
	size_t threadStride = (kCloudWidth * kCloudHeight) / (kMaxThreads + 1);

	size_t startIndex = 0;
	size_t endIndex = threadStride;

	//-----------------------------------------------------------------------------------------------------
	// Height Noise Generation
	//-----------------------------------------------------------------------------------------------------
	TileMap cloudMap(kCloudWidth, kCloudHeight);
	m_noise.SetSeed(m_cloudParameters.GetSeed());

	// Generate the height noise values.
	for (size_t i = 0; i < kMaxThreads; ++i)
	{
		m_pThreadPool[i] = std::thread(&CloudGenerator::GenerateCloudNoise, this, startIndex, endIndex, std::ref(cloudMap));
		startIndex += threadStride;
		endIndex += threadStride;
	}

	GenerateCloudNoise(startIndex, kCloudWidth * kCloudHeight, cloudMap);

	//Wait for the threads to complete the read task.
	for (unsigned int i = 0; i < kMaxThreads; ++i)
	{
		m_pThreadPool[i].join();
	}
	auto& graphics = Exelius::IApplicationLayer::GetInstance()->GetGraphicsRef();
	m_cloudTextureA = graphics->GetTextureFromPixels(cloudMap.GetTiles(), kCloudWidth, kCloudHeight, kCloudWidth * 4);


	m_noise.SetSeed((unsigned int)m_rand.Rand());

	// Generate the height noise values.
	for (size_t i = 0; i < kMaxThreads; ++i)
	{
		m_pThreadPool[i] = std::thread(&CloudGenerator::GenerateCloudNoise, this, startIndex, endIndex, std::ref(cloudMap));
		startIndex += threadStride;
		endIndex += threadStride;
	}

	GenerateCloudNoise(startIndex, kCloudWidth * kCloudHeight, cloudMap);

	//Wait for the threads to complete the read task.
	for (unsigned int i = 0; i < kMaxThreads; ++i)
	{
		m_pThreadPool[i].join();
	}

	m_cloudTextureB = graphics->GetTextureFromPixels(cloudMap.GetTiles(), kCloudWidth, kCloudHeight, kCloudWidth * 4);
}

void CloudGenerator::UpdateClouds([[maybe_unused]] float deltaTime)
{
	m_renderOffset += deltaTime * kCloudScrollSpeedMax;
	if (m_renderOffset > kCloudWidth)
	{
		m_renderOffset = 0.0f;
	}
}

void CloudGenerator::Render()
{
	auto& graphics = Exelius::IApplicationLayer::GetInstance()->GetGraphicsRef();
	graphics->DrawTexture(m_cloudTextureA.get(), (int)m_renderOffset, 0, kCloudWidth, kCloudHeight);
	graphics->DrawTexture(m_cloudTextureB.get(), (int)m_renderOffset - kCloudWidth, 0, kCloudWidth, kCloudHeight);
}

void CloudGenerator::GenerateCloudNoise(size_t startIndex, size_t endIndex, TileMap& map)
{
	m_noise.SetSeed(m_cloudParameters.GetSeed());
	for (size_t i = startIndex; i < endIndex; ++i)
	{
		const Exelius::Vector2f gridPoint = map.GetTilePosition(i);

		float cloudNoise = m_noise.GetAverageNoise(gridPoint.x, gridPoint.y,
			(float)kCloudWidth / kCloudNoiseDivisor, (float)kCloudHeight / kCloudNoiseDivisor,
			m_cloudParameters.GetInputRange(), m_cloudParameters.GetOctaves(), m_cloudParameters.GetPersistance());

		cloudNoise = (cloudNoise * powf(sinf(Exelius::PI * (gridPoint.y / (float)kCloudHeight)) * sinf(Exelius::PI * (gridPoint.x / (float)kCloudWidth)), kCloudNoiseExponent));

		Exelius::Color hexColor;
		hexColor.a = (uint8_t)(cloudNoise * 150.0f);
		map.SetTileColor(gridPoint, hexColor);
	}
}