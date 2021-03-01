#pragma once
#include <ThirdParty/Lua/lua.hpp>
#include <memory>
#include <string>

#define LUA_GETGLUE(functionName, memberType, memberName) \
	memberType functionName() const { return m_##memberName; }
#define LUA_SETGLUE(functionName, memberType, memberName) \
	void functionName(memberType memberName) { m_##memberName = memberName; }
#define LUA_REGISTER() \
	virtual void RegisterWithScript() override;

namespace Exelius
{
	class Resource;
	class ScriptManager
	{
	public:
		ScriptManager()
			: m_pState(nullptr, nullptr)
			, m_stackSize(0)
		{
			//
		}

		/// <summary>
		/// Sets the Lua state and opens all the libraries necessary.
		/// </summary>
		/// <returns>True if successful</returns>
		bool Initialize();

		bool RunScript(std::shared_ptr<Resource> pResource);
		bool RunScriptFromFile(const std::string& filename);

		void CreateTable();

		void AddToTableIndex(size_t index);
		void AddToTable(const char* pKey);
		void AddToTable(const char* pKey, lua_CFunction pFunction);
		void AddToTable(const char* pKey, void* pUserData);

		void GetFromTable(const char* pKey);
		void GetFromTable(uint32_t index);

		void GetGlobal(const char* pName);
		void SetGlobal(const char* pName);

		void PushNumber(double number);
		void PushString(const char* pString);

		void StartFunction(const char* pFunction);
		bool CallFunction(int32_t numReturn);

		void PopAll();

	private:
		void PrintLuaError();

		std::unique_ptr<lua_State, decltype(&lua_close)> m_pState;
		int32_t m_stackSize;
	};
}