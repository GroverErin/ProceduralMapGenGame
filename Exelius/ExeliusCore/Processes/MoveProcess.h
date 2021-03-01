#pragma once
#include "Processes.h"
#include "Game/Actor.h"

namespace Exelius
{
	class TransformComponent;

	class MoveProcess
		: public IProcess
	{
	public:
		MoveProcess(std::weak_ptr<Actor> pActor,
			float destX, float destY, float time)
			: m_pActor(pActor)
			, m_pTransform(nullptr)
			, m_xDest(destX)
			, m_yDest(destY)
			, m_xSpeed(0.0f)
			, m_ySpeed(0.0f)
			, m_time(time)
		{
			//
		}

		virtual ~MoveProcess() = default;

		virtual bool Initialize() final override;
		virtual void Update(float deltaTime) final override;

	private:
		std::weak_ptr<Actor> m_pActor;
		TransformComponent* m_pTransform;
		float m_xDest;
		float m_yDest;
		float m_xSpeed;
		float m_ySpeed;
		float m_time;
	};
}