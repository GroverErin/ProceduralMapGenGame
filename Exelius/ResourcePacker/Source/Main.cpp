#include <fstream>

#include <ResourceManagement/Resource.h>
#include <Managers/System.h>
int main(int argc, char* argv[])
{
	if (argc < 2)
		return 1;

	std::string path = argv[1];
	auto pSystem = Exelius::ISystem::Create();

	auto files = pSystem->GetDirectoryFiles(path);

	Exelius::ResourceFile resources;
	for (auto& file : files)
	{
		std::string resourcePath = path + "/" + file;
		std::fstream resourceFile(resourcePath, std::ios_base::in | std::ios_base::binary);
		if (resourceFile.is_open())
		{
			resourceFile.seekg(0, resourceFile.end);
			size_t fileSize = static_cast<size_t>(resourceFile.tellg());
			resourceFile.seekg(0, resourceFile.beg);

			std::vector<char> data(fileSize);
			resourceFile.read(data.data(), fileSize);

			resources.AddResource(file, std::move(data));
		}
	}

	resources.Save(argv[2]);
	return 0;
}