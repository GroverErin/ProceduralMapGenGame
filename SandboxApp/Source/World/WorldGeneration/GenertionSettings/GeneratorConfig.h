#pragma once
#include <stdint.h>
#include <Utilities/Color.h>

//----------------------------------------------------------------------------------------------------
// Default Noise Parameters
//----------------------------------------------------------------------------------------------------

static constexpr unsigned int kDefaultHeightOctaves = 6;
static constexpr unsigned int kDefaultMoistureOctaves = 6;
static constexpr unsigned int kDefaultTempuratureOctaves = 5;

static constexpr unsigned int kDefaultHeightInputRange = 2;
static constexpr unsigned int kDefaultMoistureInputRange = 15;
static constexpr unsigned int kDefaultTempuratureInputRange = 8;

static constexpr float kDefaultPersistance = 0.5f;

//----------------------------------------------------------------------------------------------------
// Biome Noise Ranges
// Biome color data is taken roughly from this chart:
// https://en.wikipedia.org/wiki/Biome#/media/File:Climate_influence_on_terrestrial_biome.svg
//----------------------------------------------------------------------------------------------------

// Tempurature
static constexpr float kTundraRange = -5.0f;
static constexpr float kTaigaRange = 0.0f;
static constexpr float kTemperateRange = 18.0f;
static constexpr float kTropicalRange = 30.0f;
static constexpr float kMaxTemp = 50.0f;

// Height based off of Mt. Everest being ~9,000 m.
static constexpr float kMaxHeight = 8000.0f;
static constexpr float kHeightToTempDivisor = 1000.0f;
static constexpr float kHeightToTempRateOfChange = -6.5f;

static constexpr float kOceanRange = 4200.0f;
static constexpr float kReefRange = 4500.0f;
static constexpr float kWaterRange = 1500.0f;
static constexpr float kMountainRange = 6200.0f;

static constexpr float kLandHeightRange = kMaxHeight - kWaterRange;
static constexpr float kMaxMoisture = 500.0f;

//----------------------------------------------------------------------------------------------------
// Biome Colors
//----------------------------------------------------------------------------------------------------

static constexpr Exelius::Color kOcean = Exelius::Color(81, 92, 129);
static constexpr Exelius::Color kReef = Exelius::Color(79, 100, 146);
static constexpr Exelius::Color kWater = Exelius::Color(62, 136, 193);

static constexpr Exelius::Color kRock = Exelius::Color(125, 125, 125);
static constexpr Exelius::Color kGlacier = Exelius::Color(189, 198, 206);
static constexpr Exelius::Color kSnow = Exelius::Color(160, 183, 168);

static constexpr Exelius::Color kBlackMarsh = Exelius::Color(107, 113, 100);
static constexpr Exelius::Color kDesert = Exelius::Color(191, 176, 132);
static constexpr Exelius::Color kDunes = Exelius::Color(163, 171, 123);
static constexpr Exelius::Color kCliff = Exelius::Color(139, 123, 95);

static constexpr Exelius::Color kSavanna = Exelius::Color(161, 154, 110);
static constexpr Exelius::Color kGrassland = Exelius::Color(123, 165, 110);
static constexpr Exelius::Color kForest = Exelius::Color(96, 127, 88);
static constexpr Exelius::Color kSwamp = Exelius::Color(123, 148, 129);

static constexpr Exelius::Color kError = Exelius::Colors::Red;