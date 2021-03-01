#pragma once
#include "Game/Actor.h"

namespace Exelius
{
	class Actor;

	class TransformComponent
		: public IActorComponent
	{
	public:

		DEFINE_COMPONENT(TransformComponent)

		TransformComponent(Actor* pOwner)
			: IActorComponent(pOwner)
			, m_x(0)
			, m_y(0)
			, m_w(0)
			, m_h(0)
		{
			//
		}

		virtual ~TransformComponent() = default;

		virtual bool Initialize(tinyxml2::XMLElement* pData) final override;
		
		virtual void RegisterWithScript() final override;
		//LUA_REGISTER();

		//LUA_GETGLUE(GetX, float, x);
		//LUA_SETGLUE(SetX, float, x);
		//LUA_GETGLUE(GetY, float, y);
		//LUA_SETGLUE(SetY, float, y);

		float GetX() const { return m_x; }
		void SetX(float x) { m_x = x; }
		float GetY() const { return m_y; }
		void SetY(float y) { m_y = y; }

		// Dali: Temporary change for fixing the issue where no w and h can be used to for animation render
		float GetW() const { return m_w; }
		void SetW(float w) { m_w = w; }
		float GetH() const { return m_h; }
		void SetH(float h) { m_h = h; }

		void Move(float x, float y)
		{
			m_x += x;
			m_y += y;
		}

	private:
		float m_x;
		float m_y;

		// Dali: Temporary change for fixing the issue where no w and h can be used to for animation render
		float m_w;
		float m_h;
	};
}