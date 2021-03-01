#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include <fstream>

namespace Exelius
{
	class Resource
	{
	public:
		Resource(const std::string& name, std::vector<char> data)
			: m_name(name)
			, m_data(std::move(data))
		{
			//
		}

		const std::string& GetName() const { return m_name; }
		std::vector<char>& GetData() { return m_data; }

	private:
		std::string m_name;
		std::vector<char> m_data;
	};

	class ResourceFile
	{
	public:
		ResourceFile()
			: m_currentOffset(0)
		{
			//
		}

		void AddResource(std::string path, std::vector<char> data);
		std::shared_ptr<Resource> LoadResource(std::string path);

		template<typename Type>
		std::shared_ptr<Type> LoadResource(const std::string& path)
		{
			static_assert(false, "Type is not a resource type.");
		}

		void Save(const std::string& path);
		bool Load(const std::string& path);

	private:
		struct ResourceInfo
		{
			uint32_t m_compressed;
			uint32_t m_size;
			uint32_t m_offset;
		};

		std::unordered_map<std::string, ResourceInfo> m_info;

		uint32_t m_currentOffset;
		std::vector<std::vector<char>> m_pendingData;
		std::fstream m_file;
	};
}