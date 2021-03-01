#include "Resource.h"

#include <algorithm>
#include <ThirdParty/TinyXML2/tinyxml2.h>
#define ZLIB_WINAPI
#include <ThirdParty/Middleware/zlib/include/zlib.h>

namespace Exelius
{
	void ResourceFile::AddResource(std::string path, std::vector<char> data)
	{
		std::transform(path.begin(), path.end(), path.begin(), [this](unsigned char c) -> unsigned char
			{
				return (unsigned char)std::tolower(c);
			});
		replace(path.begin(), path.end(), '\\', '/');

		ResourceInfo info;
		info.m_size = static_cast<uint32_t>(data.size());
		info.m_offset = m_currentOffset;

		std::vector<char> compressedData;
		compressedData.resize(data.size());

		z_stream stream;
		memset(&stream, 0, sizeof(stream));
		stream.avail_in = static_cast<uint32_t>(data.size());
		stream.next_in = reinterpret_cast<uint8_t*>(data.data());
		stream.avail_out = static_cast<uint32_t>(compressedData.size());
		stream.next_out = reinterpret_cast<uint8_t*>(compressedData.data());
		int result = deflateInit(&stream, Z_DEFAULT_COMPRESSION);
		if (result != Z_OK)
		{
			return;
		}

		result = deflate(&stream, Z_FINISH);
		if (result == Z_STREAM_END && stream.total_out < data.size() && stream.avail_in == 0)
		{
			compressedData.resize(stream.total_out);
		}
		else
		{
			//Compressed size is bigger than uncompressed, so just use uncompressed.
			compressedData = std::move(data);
		}

		deflateEnd(&stream);

		info.m_compressed = static_cast<uint32_t>(compressedData.size());
		m_currentOffset += info.m_compressed;
		m_info[path] = info;
		m_pendingData.emplace_back(std::move(compressedData));
	}

	std::shared_ptr<Resource> ResourceFile::LoadResource(std::string path)
	{
		if (!m_file.is_open())
			return nullptr;

		std::transform(path.begin(), path.end(), path.begin(), [this](unsigned char c) -> unsigned char
			{
				return (unsigned char)std::tolower(c);
			});
		replace(path.begin(), path.end(), '\\', '/');

		auto itr = m_info.find(path);
		if (itr == m_info.end())
			return nullptr;

		std::vector<char> compressed(itr->second.m_compressed);
		m_file.seekg(itr->second.m_offset);
		m_file.read(compressed.data(), compressed.size());

		if (itr->second.m_size == itr->second.m_compressed)
		{
			return std::make_shared<Resource>(path, std::move(compressed));
		}

		std::vector<char> data(itr->second.m_size);

		z_stream stream;
		memset(&stream, 0, sizeof(stream));
		stream.avail_in = static_cast<uint32_t>(compressed.size());
		stream.next_in = reinterpret_cast<uint8_t*>(compressed.data());
		stream.avail_out = static_cast<uint32_t>(data.size());
		stream.next_out = reinterpret_cast<uint8_t*>(data.data());
		int result = inflateInit(&stream);
		if (result != Z_OK)
			return nullptr;

		result = inflate(&stream, Z_FINISH);
		inflateEnd(&stream);
		if (result != Z_STREAM_END)
			return nullptr;

		return std::make_shared<Resource>(path, std::move(data));
	}

	void ResourceFile::Save(const std::string& path)
	{
		tinyxml2::XMLDocument doc;
		tinyxml2::XMLElement* pRoot = doc.NewElement("ResourceFile");
		doc.InsertFirstChild(pRoot);

		for (auto& info : m_info)
		{
			tinyxml2::XMLElement* pElement = doc.NewElement("Resource");
			pElement->SetAttribute("Path", info.first.c_str());
			pElement->SetAttribute("Compressed", info.second.m_compressed);
			pElement->SetAttribute("Size", info.second.m_size);
			pElement->SetAttribute("Offset", info.second.m_offset);
			pRoot->InsertEndChild(pElement);
		}

		tinyxml2::XMLPrinter print;
		doc.Print(&print);

		m_file.open(path, std::ios_base::out | std::ios_base::binary);
		if (m_file.is_open() && m_file.good())
		{
			for (auto& data : m_pendingData)
			{
				m_file.write(data.data(), data.size());
			}
			m_file.write(print.CStr(), print.CStrSize());

			// write out the size of our header
			int size = print.CStrSize();
			m_file.write(reinterpret_cast<char*>(&size), sizeof(int));

			m_pendingData.clear();
			m_info.clear();
			m_currentOffset = 0;
		}
		m_file.close();
	}

	bool ResourceFile::Load(const std::string& path)
	{
		m_file.open(path, std::ios_base::in | std::ios_base::binary);
		if (m_file.is_open())
		{
			int sizeofInt = sizeof(int);
			m_file.seekg(-sizeofInt, m_file.end);
			int headerSize = 0;
			m_file.read(reinterpret_cast<char*>(&headerSize), sizeofInt);
			m_file.seekg(-(headerSize + sizeofInt), m_file.cur);

			std::vector<char> header(headerSize);
			m_file.read(header.data(), headerSize);

			tinyxml2::XMLDocument doc;
			tinyxml2::XMLError error = doc.Parse(header.data(), headerSize);
			if (error != tinyxml2::XML_SUCCESS)
			{
				return false;
			}

			tinyxml2::XMLElement* pRoot = doc.FirstChildElement();
			std::string name(pRoot->Name());
			if (name == "ResourceFile")
			{
				for (auto pElement = pRoot->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
				{
					std::string elementName(pElement->Name());
					if (elementName == "Resource")
					{
						ResourceInfo info;
						info.m_compressed = pElement->UnsignedAttribute("Compressed");
						info.m_size = pElement->UnsignedAttribute("Size");
						info.m_offset = pElement->UnsignedAttribute("Offset");
						std::string rPath = pElement->Attribute("Path");
						if (!rPath.empty())
							m_info[rPath] = info;
					}
				}
			}
		}
		else
		{
			return false;
		}

		return true;
	}
}