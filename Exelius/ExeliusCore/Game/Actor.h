#pragma once
#include <unordered_map>
#include <memory>
#include <functional>

#include "ResourceManagement/Resource.h"
#include "ThirdParty/TinyXML2/tinyxml2.h"

#define DEFINE_COMPONENT(className) \
    inline static Id kIdentifier = HashName(#className); \
    virtual Id GetId() const final override { return kIdentifier; }

namespace Exelius
{
	class Actor;
	class IGraphics;
    class IGameLayer;

	class IActorComponent
	{
	public:
		typedef uint32_t Id;

		IActorComponent(Actor* pOwner)
			: m_pOwner(pOwner)
		{
			//
		}

        virtual ~IActorComponent() = default;

		virtual bool Initialize(tinyxml2::XMLElement* pData) = 0;
		virtual bool PostInit() { return true; }
        virtual void RegisterWithScript() {}
		virtual void Update([[maybe_unused]] float deltaTime) {}
		virtual void Render([[maybe_unused]] const std::unique_ptr<Exelius::IGraphics>& graphics) {}

        virtual Id GetId() const = 0;
		Actor* GetOwner() { return m_pOwner; }

		static Id HashName(const char* pName)
		{
			std::hash<std::string> hasher;
			return static_cast<Id>(hasher(std::string(pName)));
		}

	private:
		Actor* m_pOwner;
	};

    class Actor
    {
    public:
        typedef uint32_t Id;

        Actor(Id id)
            : m_id(id)
			, m_alive(true)
        {
        }
        ~Actor() = default;

        bool Initialize(tinyxml2::XMLElement* pData);
        bool PostInit();
        void RegisterWithScript();
        void Destroy();
        void Update(float deltaTime);
        void Render(const std::unique_ptr<Exelius::IGraphics>& graphics);
		bool IsValid() const { return m_alive; }
		void Expire() { m_alive = false; }

        void AddComponent(std::unique_ptr<IActorComponent> pComponent);
        IActorComponent* GetComponent(IActorComponent::Id id);

        template<class ComponentType>
        ComponentType* GetComponent()
        {
            return (ComponentType*)GetComponent(ComponentType::kIdentifier);
        }

        Id GetId() const { return m_id; }

    private:
        Id m_id;
		bool m_alive;
        std::unordered_map<IActorComponent::Id, std::unique_ptr<IActorComponent>> m_components;
    };

    class ActorFactory
    {
    public:
        using ComponentFunction = std::function<std::unique_ptr<IActorComponent>(Actor*)>;

        ActorFactory()
            : m_nextActorId(0)
        {
        }
        ~ActorFactory() = default;

        std::shared_ptr<Actor> CreateActor(std::shared_ptr<Resource> pResource);
        void RegisterComponentCreator(const char* pComponentName, ComponentFunction pFunction)
        {
            m_actorComponentCreatorMap[pComponentName] = pFunction;
        }

        //Returns THEN increments.
        Actor::Id GetNextActorId() { return m_nextActorId++; }

    private:
        std::unique_ptr<IActorComponent> CreateComponent(tinyxml2::XMLElement* pData, Actor* pOwner);

        Actor::Id m_nextActorId;
        std::unordered_map<std::string, ComponentFunction> m_actorComponentCreatorMap;
    };
}