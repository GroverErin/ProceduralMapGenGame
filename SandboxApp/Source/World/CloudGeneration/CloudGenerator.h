#pragma once
#include "World/GenertionSettings/NoiseParameters.h"
#include "World/TileMap/TileMap.h"
#include <Utilities/Random/Noise/PerlinNoise.h>
#include <Utilities/Random/Random.h>

#include <thread>

namespace Exelius
{
	class ITexture;
}

/// <summary>
/// Generates the terrain/map of the world.
/// Clouds:
///		Cloud generation is just a Perlin Noise height map. There are
///		levels in the map that effects the level of transparency of the
///		pixels.
/// </summary>
class CloudGenerator
{
	static constexpr unsigned int kMaxThreads = 7;
	std::thread* m_pThreadPool = nullptr;

	Exelius::PerlinNoise m_noise;
	Exelius::Random m_rand;

	std::shared_ptr<Exelius::ITexture> m_cloudTextureA;
	std::shared_ptr<Exelius::ITexture> m_cloudTextureB;

	float m_renderOffset;

public:
	NoiseParameters m_cloudParameters;

	CloudGenerator();
	~CloudGenerator();

	CloudGenerator(const CloudGenerator&) = default;
	CloudGenerator(CloudGenerator&&) = default;
	CloudGenerator& operator=(const CloudGenerator&) = default;
	CloudGenerator& operator=(CloudGenerator&&) = default;

	void ResetGenerator();

	/// <summary>
	/// Generate the Clouds.
	/// </summary>
	void GenerateClouds();

	void UpdateClouds(float deltaTime);

	/// <summary>
	/// Draw the cloud layer.
	/// </summary>
	void Render();

private:

	void GenerateCloudNoise(size_t startIndex, size_t endIndex, TileMap& map);
};