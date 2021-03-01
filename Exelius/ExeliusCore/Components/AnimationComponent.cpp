#include "AnimationComponent.h"
#include "TransformComponent.h"
#include "ApplicationLayer.h"
#include "Processes/AnimationProcess.h"

#include <sstream>

namespace Exelius
{
	bool AnimationComponent::Initialize(tinyxml2::XMLElement* pData)
	{
		auto& logger = IApplicationLayer::GetInstance()->GetLogger();

		auto pElement = pData->FirstChildElement("Frames");
		if (pElement)
		{
			auto pFrameElement = pElement->FirstChildElement("Frame");
			while (pFrameElement != nullptr)
			{
				uint16_t xPos = (uint16_t)pFrameElement->UnsignedAttribute("x");
				uint16_t yPos = (uint16_t)pFrameElement->UnsignedAttribute("y");
				uint16_t width = (uint16_t)pFrameElement->UnsignedAttribute("width");
				uint16_t height = (uint16_t)pFrameElement->UnsignedAttribute("height");
				m_frames.emplace_back(Frame{ xPos, yPos, width, height });

				pFrameElement = pFrameElement->NextSiblingElement("Frame");
			}
		}
		else
		{
			logger.LogSevere("Unable to find Frames Element in XML file.");
			return false;
		}

		//Should return the value of "path" in xml.
		const char* pFilePath = nullptr;
		if (pData->FindAttribute("path"))
		{
			pFilePath = pData->FindAttribute("path")->Value();
		}

		auto pGraphics = IApplicationLayer::GetInstance()->GetGraphics();
		auto pGameLayer = IApplicationLayer::GetInstance()->GetGameLayer();
		auto pResource = pGameLayer->GetResourceFile().LoadResource(pFilePath);
		
		if (pFilePath)
			m_pTexture = pGraphics->LoadTexture(pResource);

		if (!m_pTexture)
		{
			logger.LogSevere("m_pTexture failed to load, is nullptr.");
			return false;
		}

		pElement = pData->FirstChildElement("Sequences");
		if (pElement)
		{
			auto pFrameElement = pElement->FirstChildElement("Sequence");
			while (pFrameElement != nullptr)
			{
				//Get the name of this sequence/animation.
				std::string name;
				if (pFrameElement->FindAttribute("name"))
				{
					name = pFrameElement->FindAttribute("name")->Value();
				}

				std::vector<int> indexList;
				std::string indexStr = pFrameElement->GetText();

				//String to Int conversion from: https://www.geeksforgeeks.org/extract-integers-string-c/
				std::stringstream ss;

				/* Storing the whole string into string stream */
				ss << indexStr;

				/* Running loop till the end of the stream */
				std::string temp;
				int found;
				while (!ss.eof()) {

					/* extracting word by word from stream */
					ss >> temp;

					/* Checking the given word is integer or not */
					if (std::stringstream(temp) >> found)
					{
						if ((size_t)found > m_frames.size() || found < 0)
						{
							logger.LogSevere("Frame index for sequence '", false);
							logger.LogSevere(name.c_str(), false);
							logger.LogInfo("' is out of bounds: ", false);
							logger.LogInfo(std::to_string(found).c_str());
							continue;
						}
						indexList.emplace_back(found);
					}

					/* To save from space at the end of string */
					temp = "";
				}

				m_animations.emplace(name, indexList);

				//Get the next "Sequence" child.
				pFrameElement = pFrameElement->NextSiblingElement("Sequence");
			}
		}
		else
		{
			logger.LogSevere("Unable to find Sequences Element in XML file.");
		}

		// Dali: Temporary change just to init the pointer here to prevent program crash
		Resume(m_animations.begin()->first);

		CreateAnimationProcess();
		pGameLayer->GetProcessManager().AttachProcess(m_pAnimationProcess);

		return true;
	}

	bool AnimationComponent::PostInit()
	{
		m_pTransform = GetOwner()->GetComponent<TransformComponent>();

		if (!m_pTransform)
		{
			auto& logger = IApplicationLayer::GetInstance()->GetLogger();
			logger.LogSevere("Unable to find Transform Component on Actor.");
			return false;
		}

		//Possibly should be taken from data probably an attribute on the component.
		// std::string initialAnimation = "RunDown";
		// Resume(initialAnimation);
		// Pause();
		return true;
	}

	void AnimationComponent::Render(const std::unique_ptr<Exelius::IGraphics>& graphics)
	{
		if (m_renderFlag)
		{
			int frameIndex = m_pCurrentAnimation->at(m_currentFrameIndex);
			auto& frame = m_frames[frameIndex];

			graphics->DrawTexture(m_pTexture.get(),
				static_cast<int32_t>(m_pTransform->GetX()),
				static_cast<int32_t>(m_pTransform->GetY()),
				static_cast<int32_t>(m_pTransform->GetW()),
				static_cast<int32_t>(m_pTransform->GetH()),
				frame.fxPos, frame.fyPos,
				frame.fwidth, frame.fheight);
		}
	}

	void AnimationComponent::Pause(bool pause)
	{
		m_paused = pause;
	}

	bool AnimationComponent::Resume(const std::string& animationName)
	{
		m_paused = false;

		auto result = m_animations.find(animationName);
		if (result == m_animations.end())
			return false;

		m_pCurrentAnimation = &result->second; // Get a pointer to the sequence.
		m_currentFrameIndex = 0;
		return true;
	}

	void AnimationComponent::Reverse()
	{
		m_isReversed = !m_isReversed;
	}

	void AnimationComponent::NextFrame()
	{
		if (!m_pCurrentAnimation || m_paused)
			return;

		//If animation is currently reversed
		if (m_isReversed)
		{
			if (m_currentFrameIndex == 0)
			{
				m_currentFrameIndex = (uint16_t)m_pCurrentAnimation->size() - 1;
			}
			else
				--m_currentFrameIndex;
		}
		else
		{
			//The current frame is now the next frame
			++m_currentFrameIndex;
			if (m_currentFrameIndex >= m_pCurrentAnimation->size())
				m_currentFrameIndex = 0;
		}
	}

	void AnimationComponent::CreateAnimationProcess()
	{
		m_pAnimationProcess = std::make_shared<AnimationProcess>(this, 6.0f);
	}
}