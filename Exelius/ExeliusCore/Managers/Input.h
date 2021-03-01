#pragma once
#include <memory>
#include <array>

//Exelius Engine namespace. Used for all Engine related code.
namespace Exelius
{
	/// <summary>
	/// Exelius specific keyboard, used as a keyboard translator.
	/// </summary>
	class GenericKeyboard
	{
	public:
		/// <summary>
		/// Codes for every key on a keyboard (At least the Dev's keyboard).
		/// </summary>
		enum class Code
		{
			//Letters A-Z
			kCodeA,
			kCodeB,
			kCodeC,
			kCodeD,
			kCodeE,
			kCodeF,
			kCodeG,
			kCodeH,
			kCodeI,
			kCodeJ,
			kCodeK,
			kCodeL,
			kCodeM,
			kCodeN,
			kCodeO,
			kCodeP,
			kCodeQ,
			kCodeR,
			kCodeS,
			kCodeT,
			kCodeU,
			kCodeV,
			kCodeW,
			kCodeX,
			kCodeY,
			kCodeZ,

			//Numbers 0-9
			kCode0,
			kCode1,
			kCode2,
			kCode3,
			kCode4,
			kCode5,
			kCode6,
			kCode7,
			kCode8,
			kCode9,

			//F1-F12
			kCodeF1,
			kCodeF2,
			kCodeF3,
			kCodeF4,
			kCodeF5,
			kCodeF6,
			kCodeF7,
			kCodeF8,
			kCodeF9,
			kCodeF10,
			kCodeF11,
			kCodeF12,

			//Number pad numbers 0-9.
			kCodeNumPad1,
			kCodeNumPad2,
			kCodeNumPad3,
			kCodeNumPad4,
			kCodeNumPad5,
			kCodeNumPad6,
			kCodeNumPad7,
			kCodeNumPad8,
			kCodeNumPad9,
			kCodeNumPad0,

			//Other number pad keys.
			kCodeNumPadPlus,
			kCodeNumPadPeriod,
			kCodeNumPadMinus,
			kCodeNumPadMultiply,
			kCodeNumPadDivide,
			kCodeNumPadEnter,

			//Keys
			kCodeSpace,
			kCodeArrowUp,
			kCodeArrowDown,
			kCodeArrowLeft,
			kCodeArrowRight,
			kCodeEsc,
			kCodeShiftLeft,
			kCodeShiftRight,
			kCodeAltLeft,
			kCodeAltRight,
			kCodeCtrlLeft,
			kCodeCtrlRight,
			kCodeEquals,
			kCodeBackslash,
			kCodeBackspace,
			kCodeCapsLock,
			kCodeComma,
			kCodeDelete,
			kCodeEnd,
			kCodeHome,
			kCodeInsert,
			kCodeBracketLeft,
			kCodeBracketRight,
			kCodeMinus,
			kCodePageDown,
			kCodePageUp,
			kCodePeriod,
			kCodePrintScreen,
			kCodeEnter,
			kCodeSemiColon,
			kCodeForwardSlash,
			kCodeTab,
			kCodeApostrophe,
			kCodeTilde,

			//Maximum Key
			kCodeMax
		};

		/// <summary>
		/// Virtual Keyboard Destructor.
		/// </summary>
		virtual ~GenericKeyboard() = default;

		/// <summary>
		/// Initializes the key states to "not-pressed".
		/// </summary>
		/// <returns>(bool) Always true.</returns>
		virtual bool Initialize()
		{
			m_keyState = { false };
			return true;
		}

		/// <summary>
		/// Sets the input key to be up or down.
		/// </summary>
		/// <param name="key">(Code) Key to set.</param>
		/// <param name="down">(bool) True if down, false if up.</param>
		virtual void SetKeyState(Code key, bool down)
		{
			//If the key is not the maximum key.
			if (key != Code::kCodeMax)
			{
				//Set it's state.
				m_keyState[(size_t)key] = down;
			}
		}

		/// <summary>
		/// Check if input key is currently down.
		/// </summary>
		/// <param name="key">(Code) Key to check.</param>
		/// <returns>(bool) True if key is down, false if key is up.</returns>
		virtual bool IsKeyDown(Code key) { return m_keyState[(size_t)key]; }

		/// <summary>
		/// Check if input key was pressed in this frame.
		/// </summary>
		/// <param name="key">(Code) Key to check.</param>
		/// <returns>(bool) True if key was pressed, false if key was not pressed.</returns>
		virtual bool IsKeyPressed(Code key)
		{
			return (m_keyState[(size_t)key] && !m_previousKeyState[(size_t)key]);
		}

		/// <summary>
		/// Check if input key was released in this frame.
		/// </summary>
		/// <param name="key">(Code) Key to check.</param>
		/// <returns>(bool) True if key was released, false if key was not released.</returns>
		virtual bool IsKeyReleased(Code key)
		{
			return (!m_keyState[(size_t)key] && m_previousKeyState[(size_t)key]);
		}

		/// <summary>
		/// Sets the previous key states to the current key states.
		/// </summary>
		virtual void NextFrame()
		{
			m_previousKeyState = m_keyState;
		}

		/// <summary>
		/// Create the keyboard.
		/// </summary>
		/// <returns>(std::unique_ptr<GenericKeyboard>) The keyboard that is created.</returns>
		static std::unique_ptr<GenericKeyboard> Create();

	protected:
		std::array<bool, (size_t)Code::kCodeMax> m_keyState;
		std::array<bool, (size_t)Code::kCodeMax> m_previousKeyState;
	};

	/// <summary>
	/// Exelius specific mouse, used as a mouse translator.
	/// </summary>
	class GenericMouse
	{
	public:
		/// <summary>
		/// Codes for every button on a mouse (At least the Dev's mouse).
		/// </summary>
		enum class Button
		{
			//Left and Right mouse buttons.
			kButtonLeft,
			kButtonRight,
			kButtonMiddle,

			//Maximum Button
			kButtonMax
		};

		/// <summary>
		/// Codes for wheel movement on a mouse (At least the Dev's mouse).
		/// </summary>
		enum class Direction
		{
			kWheelUp,
			kWheelDown,

			kWheelNone
		};

		/// <summary>
		/// Virtual Mouse Destructor.
		/// </summary>
		virtual ~GenericMouse() = default;

		/// <summary>
		/// Initializes the button states to "not-pressed".
		/// </summary>
		/// <returns>(bool) Always true.</returns>
		virtual bool Initialize()
		{
			m_buttonState = { false };
			return true;
		}

		/// <summary>
		/// Sets the input button to be up or down.
		/// </summary>
		/// <param name="key">(Button) Button to set.</param>
		/// <param name="down">(bool) True if down, false if up.</param>
		virtual void SetButtonState(Button button, bool down)
		{
			if (button != Button::kButtonMax)
			{
				m_buttonState[(size_t)button] = down;
			}
		}

		/// <summary>
		/// Sets the wheel movement to be up or down.
		/// </summary>
		/// <param name="key">(Direction) Direction to set.</param>
		/// <param name="down">(bool) True if in that direction, false if not.</param>
		virtual void SetWheelState(Direction direction)
		{
			if (direction == Direction::kWheelUp)
			{
				m_wheelState[(size_t)Direction::kWheelUp] = true;
				m_wheelState[(size_t)Direction::kWheelDown] = false;
			}
			else if (direction == Direction::kWheelDown)
			{
				m_wheelState[(size_t)Direction::kWheelUp] = false;
				m_wheelState[(size_t)Direction::kWheelDown] = true;
			}
			else
			{
				//For possible x directions...
			}
		}
		
		/// <summary>
		/// Check if input button is currently down.
		/// </summary>
		/// <param name="key">(Button) Button to check.</param>
		/// <returns>(bool) True if button is down, false if button is up.</returns>
		virtual bool IsButtonDown(Button button) { return m_buttonState[(size_t)button]; }

		/// <summary>
		/// Check if input button was pressed in this frame.
		/// </summary>
		/// <param name="key">(Code) Button to check.</param>
		/// <returns>(bool) True if button was pressed, false if button was not pressed.</returns>
		virtual bool IsButtonPressed(Button button)
		{
			return m_buttonState[(size_t)button] && !m_previousButtonState[(size_t)button];
		}

		/// <summary>
		/// Check if input button was released in this frame.
		/// </summary>
		/// <param name="key">(Code) Button to check.</param>
		/// <returns>(bool) True if button was released, false if button was not released.</returns>
		virtual bool IsButtonReleased(Button button)
		{
			return !m_buttonState[(size_t)button] && m_previousButtonState[(size_t)button];
		}

		/// <summary>
		/// Check if the mouse wheel scrolled in input direction this frame.
		/// </summary>
		/// <param name="direction">(Direction) Direction to check.</param>
		/// <returns>(bool) True if scrolled in that direction, false if not.</returns>
		virtual bool DidScrollInDirection(Direction direction)
		{
			return m_wheelState[(size_t)direction];
		}

		/// <summary>
		/// Sets the previous button to the current button states.
		/// Sets the wheels movement directions to none.
		/// </summary>
		virtual void NextFrame()
		{
			m_previousButtonState = m_buttonState;
			m_wheelState[(size_t)Direction::kWheelUp] = false;
			m_wheelState[(size_t)Direction::kWheelDown] = false;
		}

		/// <summary>
		/// Create the mouse.
		/// </summary>
		/// <returns>(std::unique_ptr<GenericMouse>) The mouse that is created.</returns>
		static std::unique_ptr<GenericMouse> Create();

	protected:
		std::array<bool, (size_t)Button::kButtonMax> m_buttonState;
		std::array<bool, (size_t)Button::kButtonMax> m_previousButtonState;
		std::array<bool, (size_t)Direction::kWheelNone> m_wheelState;
	};
}