#include "Box2DPhysicsComponent.h"
#include "Components/TransformComponent.h"
#include "ApplicationLayer.h"
#include "Game/Physics/Box2D/Box2DPhysics.h"

namespace Exelius
{
	Box2DPhysicsComponent::Box2DPhysicsComponent(Actor* pOwner, IPhysicsManager* pPhysics)
		: IPhysicsComponent(pOwner)
		, m_pBody(nullptr)
		, m_pFixture(nullptr)
		, m_pTransform(nullptr)
		, m_pWorld(nullptr)
		, m_width(0.0f)
		, m_height(0.0f)
		, m_density(0.0f)
		, m_friction(0.0f)
		, m_bodyType(b2BodyType::b2_kinematicBody)
	{
		m_pPhysics = reinterpret_cast<Box2DPhysics*>(pPhysics);

		// Check if m_pPhysics exist
		if (m_pPhysics)
			m_pWorld = m_pPhysics->GetWorld();
	}

	Box2DPhysicsComponent::~Box2DPhysicsComponent()
	{
		// Unregister the body 
		m_pBody->SetUserData(nullptr);
		m_pFixture->SetUserData(nullptr);
		// Destroy Fixture
		m_pBody->DestroyFixture(m_pFixture);
		// Destroy Body
		m_pWorld->DestroyBody(m_pBody);
	}

	bool Box2DPhysicsComponent::Initialize(tinyxml2::XMLElement* pData)
	{
		auto pElement = pData->FirstChildElement("Info");
		if (pElement != nullptr)
		{
			// Determine Body Type
			std::string bodyType = pElement->Attribute("type");
			if (bodyType == "dynamic")
				m_bodyType = b2BodyType::b2_dynamicBody;
			else if (bodyType == "static")
				m_bodyType = b2BodyType::b2_staticBody;
			else if (bodyType == "kinematic")
				m_bodyType = b2BodyType::b2_kinematicBody;
			else
			{
				IApplicationLayer::GetInstance()->GetLogger().LogSevere("Not a valid Box2D Body Type!");
				return false;
			}

			// Determine body type
			m_bodyDef.type = m_bodyType;

			// Determine body sensor
			bool isSensor = pElement->BoolAttribute("isSensor");
			m_fixtureDef.isSensor = isSensor;

			// Determine body rotation
			bool rotatableBody = pElement->BoolAttribute("rotatableBody");
			m_bodyDef.fixedRotation = !rotatableBody;

			// Determine Category
			std::string category = pElement->Attribute("category");
			m_fixtureDef.filter.categoryBits = m_pPhysics->GetColliderCode(category);
			m_category = category;

			// Determine Mask
			std::string mask = pElement->Attribute("mask");
			m_fixtureDef.filter.maskBits = ParseMaskString(mask);
		}

		pElement = pData->FirstChildElement("Box");
		if (pElement != nullptr)
		{
			// Determine collision dimention
			m_width = pElement->FloatAttribute("width");
			m_height = pElement->FloatAttribute("height");
		}

		pElement = pData->FirstChildElement("Density");
		if (pElement != nullptr)
		{
			// Determine object density
			m_density = pElement->FloatAttribute("density");
		}

		pElement = pData->FirstChildElement("Friction");
		if (pElement != nullptr)
		{
			// Determine object friction
			m_friction = pElement->FloatAttribute("friction");
		}

		// boyd has pointer to owner unit
		m_bodyDef.userData = GetOwner();

		// Set Fixture Details
		m_polyShape.SetAsBox(m_width / 2.0f, m_height / 2.0f);
		m_fixtureDef.shape = &m_polyShape;
		m_fixtureDef.density = m_density;
		m_fixtureDef.friction = m_friction;
		m_fixtureDef.userData = GetOwner();

		return true;
	}

	bool Box2DPhysicsComponent::PostInit()
	{
		// Grab the transform component of the owner
		m_pTransform = GetOwner()->GetComponent<TransformComponent>();

		if (!m_pTransform)
		{
			//LOG_ERROR("Unable to Find Transform Component!");
			return false;
		}

		// Set body position
		m_bodyDef.position.Set(m_pTransform->GetX() + m_pTransform->GetW() / 2.f, m_pTransform->GetY() + m_pTransform->GetH() / 2.f);

		// create body / fixture
		m_pBody = m_pWorld->CreateBody(&m_bodyDef);
		// fixture can only be created inside the body itself
		m_pFixture = m_pBody->CreateFixture(&m_fixtureDef);

		return true;
	}

	void Box2DPhysicsComponent::Update([[maybe_unused]] float deltaTime)
	{
		// Check Body Type
		if (m_bodyType == b2BodyType::b2_staticBody)
			return;

		// Update Transform Position
		auto& pos = m_pBody->GetPosition();

		if (m_category == "PacMan")
		{
			m_category = "PacMan";
		}
		m_pTransform->SetX(pos.x - m_width / 2.0f);
		m_pTransform->SetY(pos.y - m_height / 2.0f);
	}

	void Box2DPhysicsComponent::Render(const std::unique_ptr<Exelius::IGraphics>& graphics)
	{
		if (false)
		{
			graphics->DrawEmptyRectangle((int32_t)m_pTransform->GetX(), (int32_t)m_pTransform->GetY(), (int32_t)m_width, (int32_t)m_height, 255);
		}
	}

	void Box2DPhysicsComponent::SetPosition(Vector2f position)
	{
		m_pBody->SetTransform(b2Vec2(position.x, position.y), 0);
	}

	void Box2DPhysicsComponent::SetDimensions(Vector2f dimensions)
	{
		m_width = dimensions.x;
		m_height = dimensions.y;

		m_pBody->DestroyFixture(m_pFixture);
		m_polyShape.SetAsBox(dimensions.x / 2.0f, dimensions.y / 2.0f);
		m_fixtureDef.shape = &m_polyShape;
		// fixture can only be created inside the body itself
		m_pFixture = m_pBody->CreateFixture(&m_fixtureDef);
	}

	void Box2DPhysicsComponent::ChangePosition(Vector2f position)
	{
		m_pBody->SetTransform(b2Vec2(m_pTransform->GetX() + position.x, m_pTransform->GetY() + position.y), 0);
	}

	void Box2DPhysicsComponent::SetLinearVelocity(Vector2f velocity)
	{
		m_pBody->SetLinearVelocity(b2Vec2(velocity.x, velocity.y));
	}

	void Box2DPhysicsComponent::ApplyForce(const Vector2f& force, const Vector2f& point, bool wake)
	{
		m_pBody->ApplyForce(b2Vec2(force.x, force.y), b2Vec2(point.x, point.y), wake);
	}

	void Box2DPhysicsComponent::ApplyForceToCenter(const Vector2f& force, bool wake)
	{
		m_pBody->ApplyForceToCenter(b2Vec2(force.x, force.y), wake);
	}

	IPhysicsComponent::BodyType Box2DPhysicsComponent::GetType()
	{
		if (m_bodyType == b2BodyType::b2_dynamicBody)
			return IPhysicsComponent::BodyType::kDynamic;
		else if (m_bodyType == b2BodyType::b2_staticBody)
			return IPhysicsComponent::BodyType::kStatic;
		else
			return IPhysicsComponent::BodyType::kKinetic;
	}

	bool Box2DPhysicsComponent::IsColliding()
	{
		// Check the physical body's contact lists, if return nullptr, it means there is no contact
		auto pContactList = m_pBody->GetContactList();
		if (!pContactList || !pContactList->contact->IsTouching())
			return false;

		return true;
	}

	bool Box2DPhysicsComponent::IsCollidingWith(const std::string& category)
	{
		auto pContact = m_pBody->GetContactList();
		uint16_t colliderCode = m_pPhysics->GetColliderCode(category);

		while (pContact)
		{
			auto pFixture = pContact->other->GetFixtureList();
			if (pFixture->GetFilterData().categoryBits == colliderCode)
				return true;
			pContact = pContact->next;
		}

		return false;
	}

	void Box2DPhysicsComponent::EnablePhysics(bool enable)
	{
		m_pBody->SetEnabled(enable);
	}

	uint16_t Box2DPhysicsComponent::ParseMaskString(const std::string& mask)
	{
		if (mask.size() == 0)
		{
			//LOG_WARNING("Empty Mask Input For %s of %s!", GetName().c_str(), GetOwner()->GetName().c_str());
			return 0;
		}

		// Result flags
		uint16_t flags = 0;

		// Set Parsing
		size_t startPos = 0;
		size_t endPos = 0;
		while (startPos != std::string::npos)
		{
			// parse category by '|'
			endPos = mask.find_first_of('|', startPos);
			// add flag
			flags |= m_pPhysics->GetColliderCode(mask.substr(startPos, endPos - startPos));
			// update search position
			startPos = endPos == std::string::npos ? endPos : endPos + 1;
		}

		return flags;
	}

	b2AABB Box2DPhysicsComponent::GetBoundingBox() const
	{
		auto pos = m_pBody->GetPosition();
		b2AABB box;
		box.lowerBound.Set(pos.x - m_width / 2.f, pos.y - m_height / 2.f);
		box.upperBound.Set(pos.x + m_width / 2.f, pos.y + m_height / 2.f);
		return box;
	}

	Vector2f Box2DPhysicsComponent::GetPositionCentered() const
	{
		auto& pos = m_pBody->GetPosition();
		return { pos.x, pos.y };
	}
}
