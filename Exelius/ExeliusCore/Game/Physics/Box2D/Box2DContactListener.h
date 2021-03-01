#pragma once
#include <ThirdParty/Middleware/Box2D/include/box2d.h>
#include <vector>

namespace Exelius
{
	class Actor;

	class Box2DContactListener
		: public b2ContactListener
	{
	public:
		struct Contact
		{
			Actor* pActorA;
			Actor* pActorB;
			bool isOverlap;
			bool isBeginContact;
		};

		struct Overlap
		{
			Actor* pActorA;
			Actor* pActorB;
		};

		//virtual ~Box2DContactListener() = default;

		/// <summary>
		/// This is an override function for Box2d, so this means that BeginContact is called by Box2D.
		/// </summary>
		/// <param name="pContact">(b2Contact*) A pointer to the contact object given by Box2D.</param>
		virtual void BeginContact(b2Contact* pContact) final override;

		/// <summary>
		/// This is an override function for Box2d, so this means that EndContact is called by Box2D.
		/// </summary>
		/// <param name="pContact">(b2Contact*) A pointer to the contact object given by Box2D.</param>
		virtual void EndContact(b2Contact* pContact) final override;

		void ClearContacts();

		void AddOverlap(Contact& overlap);
		void RemoveOverlap(Contact& overlap);
		void ClearOverlaps();
		void RemoveInvalidOverlappingPair();

		/// <summary>
		/// Get the list of collisions known by the listener.
		/// </summary>
		/// <returns>(std::vector<Contact>&) List of sets of Actors (2) and if it began contact or ended contact.</returns>
		std::vector<Contact>& GetBeginContacts() { return m_contacts; }

		/// <summary>
		/// Get the list of overlaps known by the listener.
		/// </summary>
		/// <returns>(std::vector<Contact>&) List of sets of Actors (2) and if it began contact or ended contact.</returns>
		std::vector<Overlap>& GetOverlappingContacts() { return m_overlaps; }

	private:

		void HandleContacts(b2Contact* pContact, bool isBeginContact);

		std::vector<Contact> m_contacts;
		std::vector<Overlap> m_overlaps;
	};
}