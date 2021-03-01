#pragma once
#include "Actor.h"

//Exelius Engine namespace. Used for all Engine related code.
namespace Exelius
{
	/// <summary>
	/// Interface for all the possible views created by a game.
	/// </summary>
	class IView
	{
	public:
		IView(std::shared_ptr<Actor> pActor)
			: m_pOwner(pActor)
			, m_indexInViewList(0)
		{
			//
		}

		/// <summary>
		/// Virtual View Destructor.
		/// </summary>
		virtual ~IView() = default;

		/// <summary>
		/// Initialized the view.
		/// </summary>
		/// <returns>(bool) True if successful, false if an error occured.</returns>
		virtual bool Initialize() = 0;

		/// <summary>
		/// Updates the input for the view.
		/// </summary>
		virtual void UpdateInput(float deltaTime) = 0;

		/// <summary>
		/// Render the view.
		/// </summary>
		virtual void ViewScene() = 0;

		Actor* GetOwner() const { return m_pOwner.lock().get(); }
		void SetViewListIndex(size_t index) { m_indexInViewList = index; }
		size_t GetIndexInViewList() const { return m_indexInViewList; }

	protected:

		std::weak_ptr<Actor> m_pOwner;
		size_t m_indexInViewList;
	};
}