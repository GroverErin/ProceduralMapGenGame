#include "Input.h"

namespace Exelius
{
	/// <summary>
	/// Create the keyboard.
	/// </summary>
	/// <returns>(std::unique_ptr<GenericKeyboard>) The keyboard that is created.</returns>
	std::unique_ptr<GenericKeyboard> GenericKeyboard::Create()
	{
		return std::make_unique<GenericKeyboard>();
	}

	/// <summary>
	/// Create the mouse.
	/// </summary>
	/// <returns>(std::unique_ptr<GenericMouse>) The mouse that is created.</returns>
	std::unique_ptr<GenericMouse> GenericMouse::Create()
	{
		return std::make_unique<GenericMouse>();
	}
}