#include "Audio.h"
#include "ApplicationLayer.h"
#include "ResourceManagement/Resource.h"

#include <ThirdParty/Middleware/SDL2/include/SDL.h>
#include <ThirdParty/Middleware/SDL2_mixer/include/SDL_mixer.h>
#include <unordered_map>

class SDLAudio
	: public Exelius::IAudio
{
public:
	SDLAudio()
		: m_initialized(false)
		, m_pMusic(nullptr, nullptr)
	{
		//
	}

	virtual ~SDLAudio()
	{
		m_sounds.clear();
		m_pMusic = nullptr;

		if (m_initialized)
		{
			Mix_CloseAudio();
			Mix_Quit();
			SDL_QuitSubSystem(SDL_INIT_AUDIO);
		}
	}

	virtual bool Initialize() override
	{
		auto& logger = Exelius::IApplicationLayer::GetInstance()->GetLogger();

		if (SDL_InitSubSystem(SDL_INIT_AUDIO))
		{
			logger.LogFatal("SDL_INIT_AUDIO has failed: ", false);
			logger.LogFatal(SDL_GetError());
			return false;
		}

		m_initialized = true;

		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT
			, MIX_DEFAULT_CHANNELS, 1024))
		{
			logger.LogFatal("Mix_OpenAudio has failed: ", false);
			logger.LogFatal(Mix_GetError());
			return false;
		}

		int mixFlags = MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG;
		int initialized = Mix_Init(mixFlags);
		if (initialized != mixFlags)
		{
			logger.LogFatal("Unable to initialize SDL Mixer with all formats requested: ", false);
			logger.LogFatal(Mix_GetError());
			return false;
		}

		return true;
	}

	virtual bool PlayMusic(std::shared_ptr<Exelius::Resource> pResource) override
	{
		auto& logger = Exelius::IApplicationLayer::GetInstance()->GetLogger();

		m_pMusic = std::unique_ptr<Mix_Music, decltype(&Mix_FreeMusic)>
			(Mix_LoadMUS_RW(SDL_RWFromMem(
				pResource->GetData().data(),
				(int)pResource->GetData().size()), 0), &Mix_FreeMusic);

		if (m_pMusic == nullptr)
		{
			logger.LogDebug("Mix_LoadMUS has failed for File: ", false);
			logger.LogDebug(pResource->GetName().c_str());
			logger.LogDebug("Mix_LoadMUS Error: ", false);
			logger.LogDebug(Mix_GetError());
			return false;
		}

		if (Mix_PlayMusic(m_pMusic.get(), -1))
		{
			logger.LogDebug("Mix_PlayMusic has failed for File: ", false);
			logger.LogDebug(pResource->GetName().c_str());
			logger.LogDebug("Mix_PlayMusic Error: ", false);
			logger.LogDebug(Mix_GetError());
			return false;
		}

		return true;
	}

	virtual bool PlaySound(std::shared_ptr<Exelius::Resource> pResource) override
	{
		auto& logger = Exelius::IApplicationLayer::GetInstance()->GetLogger();

		auto soundIter = m_sounds.find(pResource->GetName());
		if (soundIter == m_sounds.end())
		{
			Mix_Chunk* pChunk = Mix_LoadWAV_RW(SDL_RWFromMem(
				pResource->GetData().data(),
				(int)pResource->GetData().size()), 0);
			if (pChunk != nullptr)
			{
				soundIter = m_sounds.emplace(pResource->GetName()
					, std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)>
					(pChunk, &Mix_FreeChunk)).first;
			}
			else
			{
				logger.LogDebug("Mix_LoadWAV has failed for file: ", false);
				logger.LogDebug(pResource->GetName().c_str());
				logger.LogDebug("Mix_LoadWAV Error: ", false);
				logger.LogDebug(Mix_GetError());
				return false;
			}
		}

		Mix_PlayChannel(-1, soundIter->second.get(), 0);
		return true;
	}

	virtual bool SetMusicVolume(const int volume)
	{
		auto& logger = Exelius::IApplicationLayer::GetInstance()->GetLogger();

		if (Mix_VolumeMusic(volume) < 0)
		{
			logger.LogDebug("Volume change Error: ", false);
			logger.LogDebug(Mix_GetError());
			return false;
		}
		return true;
	}

	virtual void PauseMusic()
	{
		Mix_PauseMusic();
	}

	virtual void ContinueMusic()
	{
		Mix_ResumeMusic();
	}

	virtual void StopMusic()
	{
		Mix_HaltMusic();
	}

	virtual bool PlayNextSong(std::shared_ptr<Exelius::Resource> pResource, const int fadeOutTimeMS, const int fadeInTimeMS)
	{
		auto& logger = Exelius::IApplicationLayer::GetInstance()->GetLogger();

		Mix_FadeOutMusic(fadeOutTimeMS);

		m_pMusic = nullptr;
		m_pMusic = std::unique_ptr<Mix_Music, decltype(&Mix_FreeMusic)>
			(Mix_LoadMUS_RW(SDL_RWFromMem(
				pResource->GetData().data(),
				(int)pResource->GetData().size()), 0), &Mix_FreeMusic);

		if (m_pMusic == nullptr)
		{
			logger.LogDebug("Mix_LoadMUS has failed for File: ", false);
			logger.LogDebug(pResource->GetName().c_str());
			logger.LogDebug("Mix_LoadMUS Error: ", false);
			logger.LogDebug(Mix_GetError());
			return false;
		}

		if (Mix_FadeInMusic(m_pMusic.get(), -1, fadeInTimeMS))
		{
			logger.LogDebug("Mix_PlayMusic has failed for File: ", false);
			logger.LogDebug(pResource->GetName().c_str());
			logger.LogDebug("Mix_PlayMusic Error: ", false);
			logger.LogDebug(Mix_GetError());
			return false;
		}

		return true;
	}


private:
	bool m_initialized;
	std::unique_ptr<Mix_Music, decltype(&Mix_FreeMusic)> m_pMusic;
	std::unordered_map<std::string, std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)>> m_sounds;
};

std::unique_ptr<Exelius::IAudio> Exelius::IAudio::Create()
{
	return std::make_unique<SDLAudio>();
}