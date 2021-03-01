#pragma once
#include <memory>

namespace Exelius
{
	class Resource;

	class IAudio
	{
	public:
		virtual ~IAudio() = default;

		virtual bool Initialize() = 0;

		virtual bool PlayMusic(std::shared_ptr<Resource> pResource) = 0;
		virtual bool PlaySound(std::shared_ptr<Resource> pResource) = 0;

		virtual bool SetMusicVolume(const int volume) = 0;

		virtual void PauseMusic() = 0;
		virtual void ContinueMusic() = 0;
		virtual void StopMusic() = 0;

		virtual bool PlayNextSong(std::shared_ptr<Resource> pResource, const int fadeOutTimeMS = 400, const int fadeInTimeMS = 400) = 0;

		static std::unique_ptr<IAudio> Create();

		//static std::shared_ptr<Mix_Music> CreateMusicFromResource(std::shared_ptr<class ResourceFile> pResourceFile, const char* pPath);
	};

	//template<>
	//inline std::shared_ptr<Mix_Music> ResourceFile::LoadResource<Mix_Music>(const std::string& path)
	//{
		//
	//}
}