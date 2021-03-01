#include "MoveProcess.h"
#include "Components/TransformComponent.h"

namespace Exelius
{
	bool MoveProcess::Initialize()
	{
		auto pActor = m_pActor.lock();
		if (!pActor)
			return false;

		m_pTransform = pActor->GetComponent<TransformComponent>();
		if (!m_pTransform)
			return false;

		m_xSpeed = (m_xDest - m_pTransform->GetX()) / m_time;
		m_ySpeed = (m_yDest - m_pTransform->GetY()) / m_time;
		return true;
	}

	void MoveProcess::Update(float deltaTime)
	{
		if (m_pActor.expired())
		{
			Failed();
			return;
		}

		if (m_time <= deltaTime)
		{
			m_pTransform->SetX(m_xDest);
			m_pTransform->SetY(m_yDest);
			Succeeded();
		}
		else
		{
			m_pTransform->Move(m_xSpeed * deltaTime, m_ySpeed * deltaTime);
			m_time -= deltaTime;
		}
	}
}