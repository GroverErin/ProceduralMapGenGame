#include "ScriptManager.h"
#include "ApplicationLayer.h"
#include "ResourceManagement/Resource.h"

#include <assert.h>
#include <iostream>

namespace Exelius
{
	bool ScriptManager::Initialize()
	{
		m_pState = std::unique_ptr<lua_State, decltype(&lua_close)>(
			luaL_newstate(), &lua_close);

		if (!m_pState)
			return false;

		luaL_openlibs(m_pState.get());
		return true;
	}

	bool ScriptManager::RunScript(std::shared_ptr<Resource> pResource)
	{
		bool error = luaL_loadbuffer(m_pState.get(),
			pResource->GetData().data(), pResource->GetData().size(), "script") ||
			lua_pcall(m_pState.get(), 0, 0, 0);

		if (error)
			PrintLuaError();
		return !error;
	}

	bool ScriptManager::RunScriptFromFile(const std::string& filename)
	{
		bool error = luaL_dofile(m_pState.get(), filename.c_str());
		if (error)
			PrintLuaError();
		return !error;
	}

	void ScriptManager::CreateTable()
	{
		lua_newtable(m_pState.get());
		++m_stackSize;
	}

	void ScriptManager::AddToTableIndex(size_t index)
	{
		lua_rawseti(m_pState.get(), -2, index);
		--m_stackSize;
	}

	void ScriptManager::AddToTable(const char* pKey)
	{
		//This will add whatever is currently on the stack,
		//useful for adding tables to tables.
		lua_setfield(m_pState.get(), -2, pKey);
		--m_stackSize;
	}

	void ScriptManager::AddToTable(const char* pKey, lua_CFunction pFunction)
	{
		lua_pushcfunction(m_pState.get(), pFunction);
		lua_setfield(m_pState.get(), -2, pKey);
	}

	void ScriptManager::AddToTable(const char* pKey, void* pUserData)
	{
		lua_pushlightuserdata(m_pState.get(), pUserData);
		lua_setfield(m_pState.get(), -2, pKey);
	}

	void ScriptManager::GetFromTable(const char* pKey)
	{
		lua_getfield(m_pState.get(), -1, pKey);
		++m_stackSize;
	}

	void ScriptManager::GetFromTable(uint32_t index)
	{
		lua_geti(m_pState.get(), -1, index);
		++m_stackSize;
	}

	void ScriptManager::GetGlobal(const char* pName)
	{
		lua_getglobal(m_pState.get(), pName);
		++m_stackSize;
	}

	void ScriptManager::SetGlobal(const char* pName)
	{
		lua_setglobal(m_pState.get(), pName);
		--m_stackSize;
	}

	void ScriptManager::PushNumber(double number)
	{
		lua_pushnumber(m_pState.get(), number);
		++m_stackSize;
	}

	void ScriptManager::PushString(const char* pString)
	{
		lua_pushstring(m_pState.get(), pString);
		++m_stackSize;
	}

	void ScriptManager::StartFunction(const char* pFunction)
	{
		lua_getglobal(m_pState.get(), pFunction);
	}

	bool ScriptManager::CallFunction(int32_t numReturn)
	{
		if (lua_pcall(m_pState.get(), m_stackSize, numReturn, 0) != 0)
		{
			PrintLuaError();
			m_stackSize = 0;
			return false;
		}

		m_stackSize = numReturn;
		return true;
	}

	void ScriptManager::PopAll()
	{
		lua_pop(m_pState.get(), m_stackSize);
		assert(lua_gettop(m_pState.get()) == 0);
		m_stackSize = 0;
	}

	void ScriptManager::PrintLuaError()
	{
		const char* pError = lua_tostring(m_pState.get(), -1);
		auto pApplication = IApplicationLayer::GetInstance();
		if (pApplication)
		{
			auto& logger = pApplication->GetLogger();
			logger.LogSevere(pError);
		}
		else
		{
			std::cout << pError << '\n';
		}
		lua_pop(m_pState.get(), 1);
	}
}
