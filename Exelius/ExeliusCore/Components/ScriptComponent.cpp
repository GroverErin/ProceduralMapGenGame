#include "ScriptComponent.h"
#include "ApplicationLayer.h"

namespace Exelius
{
	bool ScriptComponent::Initialize(tinyxml2::XMLElement* pData)
	{
		//Should return the value of "path" in xml.
		if (pData->FindAttribute("path"))
		{
			m_scriptPath = pData->FindAttribute("path")->Value();
			return true;
		}
		return false;
	}

	void ScriptComponent::RegisterWithScript()
	{
		auto pGameLayer = IApplicationLayer::GetInstance()->GetGameLayer();
		auto& scriptManager = pGameLayer->GetScriptManager();

		scriptManager.GetGlobal("g_logic");
		scriptManager.GetFromTable("Actors");
		scriptManager.GetFromTable(GetOwner()->GetId());

		scriptManager.CreateTable();
		scriptManager.AddToTable("this", this);


		scriptManager.AddToTable("ScriptComponent");
		scriptManager.PopAll();
	}
}
