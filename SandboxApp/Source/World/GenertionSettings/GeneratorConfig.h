#pragma once
#include <stdint.h>
#include <Utilities/Color.h>

//----------------------------------------------------------------------------------------------------
// Default World Map Settings
//----------------------------------------------------------------------------------------------------

static constexpr unsigned int kWorldWidth = 1280;
static constexpr unsigned int kWorldHeight = 720;

//----------------------------------------------------------------------------------------------------
// Default Height Noise and Value Parameters
//----------------------------------------------------------------------------------------------------

static constexpr float kHeightNoiseDivisor = 2.0f;
static constexpr float kHeightNoiseExponent = 0.1f;

static constexpr unsigned int kDefaultHeightOctaves = 6;
static constexpr unsigned int kDefaultHeightInputRange = 2;
static constexpr float kDefaultHeightPersistance = 0.5f;

//----------------------------------------------------------------------------------------------------
// Default Moisture Noise and Value Parameters
//----------------------------------------------------------------------------------------------------

static constexpr unsigned int kDefaultMoistureOctaves = 6;
static constexpr unsigned int kDefaultMoistureInputRange = 15;
static constexpr float kDefaultMoisturePersistance = 0.5f;

//----------------------------------------------------------------------------------------------------
// Default Tempurature Value Parameters
//----------------------------------------------------------------------------------------------------

static constexpr float kDefaultPoleTempuratureNormal = 0.0f;
static constexpr float kDefaultEquatorTempuratureNormal = 1.0f;
static constexpr float kDefaultTempuratureFalloffExponent = 0.7f;

//----------------------------------------------------------------------------------------------------
// Default Cloud Noise Parameters
//----------------------------------------------------------------------------------------------------

// The dimensions of the generated world in tiles.
static constexpr unsigned int kCloudWidth = kWorldWidth;
static constexpr unsigned int kCloudHeight = kWorldHeight;

static constexpr float kCloudNoiseDivisor = 2.0f;
static constexpr float kCloudNoiseExponent = 8.0f;

static constexpr unsigned int kDefaultCloudOctaves = 6;
static constexpr unsigned int kDefaultCloudInputRange = 2;
static constexpr float kDefaultCloudPersistance = 0.5f;

//----------------------------------------------------------------------------------------------------
// Default Cloud Gameplay Parameters
//----------------------------------------------------------------------------------------------------

static constexpr float kCloudScrollSpeedMin = 20.0f;
static constexpr float kCloudScrollSpeedMax = 70.0f;

//----------------------------------------------------------------------------------------------------
// Default Fire Gameplay Parameters
//----------------------------------------------------------------------------------------------------

static constexpr float kFireLifetime = 1.0f;
static constexpr float kFireTickTime = 0.1f;

// The chance a tile has to catch fire at game start.
static constexpr float kGrasslandCombustionChance = 0.00001f;
static constexpr float kForestCombustionChance = 0.0001f;
static constexpr float kSavannaCombustionChance = 0.0005f;
static constexpr float kSwampCombustionChance = 0.000001f;

// The chance a tile has to catch fire when adjacent to a burning tile.
static constexpr float kGrasslandBurnChance = 0.03f;
static constexpr float kForestBurnChance = 0.04f;
static constexpr float kSavannaBurnChance = 0.05f;
static constexpr float kSwampBurnChance = 0.005f;

//----------------------------------------------------------------------------------------------------
// Cellular Automata - Rocks, Trees, and Cliff chances.
//----------------------------------------------------------------------------------------------------

static constexpr int kNumCellularAutomataIterations = 10;

static constexpr float kSaltGrassToRockChance = 0.0025f;
static constexpr float kSaltGrassToTreeChance = 0.005f;

static constexpr float kSaltSavannaToRockChance = 0.005f;
static constexpr float kSaltSavannaToCliffChance = 0.01f;

static constexpr float kSaltSnowtoRockChance = 0.005f;
static constexpr float kSaltSnowtoTreeChance = 0.01f;

static constexpr float kSaltDesertToRockChance = 0.001f;

static constexpr float kSaltGlacierToRockChance = 0.001f;

static constexpr float kSaltSwamptoRockChance = 0.001f;

static constexpr float kForestGrowthChance = 0.25f;
static constexpr float kRockGrowthChance = 0.2f;
static constexpr float kCliffGrowthChance = 0.15f;

//----------------------------------------------------------------------------------------------------
// Biome Configuration
//----------------------------------------------------------------------------------------------------

// Height
static constexpr float kMaxHeight = 8000.0f;
static constexpr float kHeightToTempDivisor = 1000.0f;

static constexpr float kOceanRange = 4200.0f;
static constexpr float kReefRange = 4500.0f;
static constexpr float kWaterRange = 1500.0f;
static constexpr float kMountainRange = 6200.0f;

// Tempurature
static constexpr float kHeightToTempRateOfChange = -6.5f;
static constexpr float kTundraRange = -5.0f;
static constexpr float kTaigaRange = 0.0f;
static constexpr float kTemperateRange = 18.0f;
static constexpr float kTropicalRange = 30.0f;
static constexpr float kMaxTemp = 50.0f;

// Moisture
static constexpr float kLandHeightRange = kMaxHeight - kWaterRange;
static constexpr float kDuneMoisture = 250.0f;
static constexpr float kSavannaMoisture = 375.0f;
static constexpr float kSnowMoisture = 500.0f;
static constexpr float kGrasslandMoisture = 600.0f;
static constexpr float kMaxMoisture = 500.0f;

//----------------------------------------------------------------------------------------------------
// Biome Colors
//----------------------------------------------------------------------------------------------------

static constexpr Exelius::Color kOcean = Exelius::Color(81, 92, 129);
static constexpr Exelius::Color kReef = Exelius::Color(79, 100, 146);

static constexpr Exelius::Color kRock = Exelius::Color(125, 125, 125);
static constexpr Exelius::Color kGlacier = Exelius::Color(189, 198, 206);
static constexpr Exelius::Color kSnow = Exelius::Color(160, 183, 168);

static constexpr Exelius::Color kDesert = Exelius::Color(191, 176, 132);
static constexpr Exelius::Color kDunes = Exelius::Color(163, 171, 123);
static constexpr Exelius::Color kCliff = Exelius::Color(139, 123, 95);

static constexpr Exelius::Color kSavanna = Exelius::Color(161, 154, 110);
static constexpr Exelius::Color kGrassland = Exelius::Color(123, 165, 110);
static constexpr Exelius::Color kForest = Exelius::Color(96, 127, 88);
static constexpr Exelius::Color kSwamp = Exelius::Color(123, 148, 129);

static constexpr Exelius::Color kBlackScorch = Exelius::Color(107, 113, 100);

static constexpr Exelius::Color kError = Exelius::Colors::Red;