#include "TransformComponent.h"
#include "ApplicationLayer.h"


namespace Lua
{
	static int GetX(lua_State* pState)
	{
		auto pTransform = reinterpret_cast<Exelius::TransformComponent*>(lua_touserdata(pState, 1));
		lua_pop(pState, 1); //Pop off the transform from the stack.
		lua_pushnumber(pState, pTransform->GetX());
		return 1;
	}

	static int GetY(lua_State* pState)
	{
		auto pTransform = reinterpret_cast<Exelius::TransformComponent*>(lua_touserdata(pState, 1));
		lua_pop(pState, 1); //Pop off the transform from the stack.
		lua_pushnumber(pState, pTransform->GetY());
		return 1;
	}

	static int SetX(lua_State* pState)
	{
		auto pTransform = reinterpret_cast<Exelius::TransformComponent*>(lua_touserdata(pState, 1));
		float x = static_cast<float>(luaL_checknumber(pState, 2));
		pTransform->SetX(x);
		lua_pop(pState, 2);
		return 0;
	}

	static int SetY(lua_State* pState)
	{
		auto pTransform = reinterpret_cast<Exelius::TransformComponent*>(lua_touserdata(pState, 1));
		float y = static_cast<float>(luaL_checknumber(pState, 2));
		pTransform->SetY(y);
		lua_pop(pState, 2);
		return 0;
	}
}

namespace Exelius
{
	bool TransformComponent::Initialize(tinyxml2::XMLElement* pData)
	{
		auto pElement = pData->FirstChildElement("Position");
		if (pElement != nullptr)
		{
			m_x = pElement->FloatAttribute("x");
			m_y = pElement->FloatAttribute("y");
		}

		// Dali: Temporary change for fixing the issue where no w and h can be used to for animation render
		pElement = pData->FirstChildElement("Dimension");
		if (pElement != nullptr)
		{
			m_w = pElement->FloatAttribute("w");
			m_h = pElement->FloatAttribute("h");
		}

		return true;
	}

	void TransformComponent::RegisterWithScript()
	{
		auto pGameLayer = IApplicationLayer::GetInstance()->GetGameLayer();
		auto& scriptManager = pGameLayer->GetScriptManager();

		scriptManager.GetGlobal("g_logic");
		scriptManager.GetFromTable("Actors");
		scriptManager.GetFromTable(GetOwner()->GetId());

		scriptManager.CreateTable();
		scriptManager.AddToTable("this", this);

		scriptManager.AddToTable("GetX", Lua::GetX);
		scriptManager.AddToTable("GetY", Lua::GetY);
		scriptManager.AddToTable("SetX", Lua::SetX);
		scriptManager.AddToTable("SetY", Lua::SetY);

		scriptManager.AddToTable("TransformComponent");
		scriptManager.PopAll();
	}
}