#pragma once
#include "Utilities/Shapes/Shapes.h"
#include "Utilities/Color.h"

#include <memory>
#include <vector>

//Exelius Engine namespace. Used for all Engine related code.
namespace Exelius
{
	class IWindow;
	class Resource;

	/// <summary>
	/// Abstract texture for mapping non platform-specific native textures to Exelius specific.
	/// </summary>
	class ITexture
	{
	public:
		/// <summary>
		/// Virtual Texture Destructor.
		/// </summary>
		virtual ~ITexture() = default;

		/// <summary>
		/// Get the texture in it's native format. (SDL, SFML, etc)
		/// </summary>
		/// <returns>(void*) Pointer to the texture in its native format.</returns>
		virtual void* GetNativeTexture() const = 0;

		/// <summary>
		/// Get the texture's width and height. (SDL, SFML, etc)
		/// </summary>
		virtual void GetTextureDimensions(int& width, int& height) = 0;
	};

	class IFont
	{
	public:
		virtual ~IFont() = default;

		virtual void* GetNativeFont() const = 0;
	};

	/// <summary>
	/// Graphics interface for rendering with non-specific graphics implementations.
	/// </summary>
	class IGraphics
	{
	public:
		/// <summary>
		/// Virtual Graphics Destructor.
		/// </summary>
		virtual ~IGraphics() = default;

		/// <summary>
		/// Initializes the Graphics implementation using the input window.
		/// </summary>
		/// <param name="pWindow">(IWindow*) Window to attach graphics to.</param>
		/// <returns></returns>
		virtual bool Initialize(IWindow* pWindow) = 0;

		/// <summary>
		/// Begins the drawing to window process.
		/// In SDL this is the screen background.
		/// </summary>
		/// <param name="red">(uint8_t) The level of red to draw.</param>
		/// <param name="green">(uint8_t) The level of green to draw.</param>
		/// <param name="blue">(uint8_t) The level of blue to draw.</param>
		/// <returns>(bool) True if successful, false if an error occured. Errors will be logged.</returns>
		virtual bool StartDrawing(uint8_t red, uint8_t green, uint8_t blue) = 0;

		/// <summary>
		/// Ends the drawing to window process.
		/// In SDL this will present the background.
		/// </summary>
		virtual void EndDrawing() = 0;

		/// <summary>
		/// Loads a texture from a file.
		/// </summary>
		/// <param name="pFilename">(const char*) Path (from output directory) to - and filename of the texture to load.</param>
		/// <returns>(std::shared_ptr<ITexture>) The loaded texture. Nullptr if unsuccessful.</returns>
		virtual std::shared_ptr<ITexture> LoadTexture(std::shared_ptr<Resource> pResource) = 0;

		virtual std::shared_ptr<IFont> LoadFont(std::shared_ptr<Resource>& pResource, int fontSize) = 0;

		virtual std::shared_ptr<ITexture> LoadText(const char* pText, std::shared_ptr<IFont>& font
			, uint8_t red = 255, uint8_t green = 255, uint8_t blue = 255, uint8_t alpha = 255) = 0;
		
		/// <summary>
		/// 
		/// </summary>
		virtual bool DrawTexture(ITexture* pTexture
			, int32_t xPos, int32_t yPos
			, int32_t width, int32_t height
			, int32_t srcX = 0, int32_t srcY = 0
			, int32_t srcWidth = 0, int32_t srcHeight = 0) = 0;

		/// <summary>
		/// 
		/// </summary>
		virtual bool DrawTextureEx(ITexture* pTexture
			, int32_t xPos, int32_t yPos
			, int32_t width, int32_t height
			, int32_t srcX = 0, int32_t srcY = 0
			, int32_t srcWidth = 0, int32_t srcHeight = 0
			, float angle = 0.0) = 0;

		/// <summary>
		/// Draws a solid rectangle to the window.
		/// </summary>
		/// <param name="xPos">(int32_t) X position to draw to.</param>
		/// <param name="yPos">(int32_t) Y position to draw to.</param>
		/// <param name="width">(int32_t) Width of the rectangle.</param>
		/// <param name="height">(int32_t) Height of the rectangle.</param>
		/// <param name="red">(uint8_t) The level of red in the rectangle.</param>
		/// <param name="green">(uint8_t) The level of green in the rectangle.</param>
		/// <param name="blue">(uint8_t) The level of blue in the rectangle.</param>
		/// <param name="alpha">(uint8_t) The level of alpha in the rectangle.</param>
		/// <returns>(bool) True if successful, false if not. Logs and errors.</returns>
		virtual bool DrawSolidRectangle(int32_t xPos, int32_t yPos
			, int32_t width, int32_t height
			, uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0, uint8_t alpha = 0) = 0;

		/// <summary>
		/// Draws a empty rectangle to the window.
		/// </summary>
		/// <param name="xPos">(int32_t) X position to draw to.</param>
		/// <param name="yPos">(int32_t) Y position to draw to.</param>
		/// <param name="width">(int32_t) Width of the rectangle.</param>
		/// <param name="height">(int32_t) Height of the rectangle.</param>
		/// <param name="red">(uint8_t) The level of red in the rectangle.</param>
		/// <param name="green">(uint8_t) The level of green in the rectangle.</param>
		/// <param name="blue">(uint8_t) The level of blue in the rectangle.</param>
		/// <param name="alpha">(uint8_t) The level of alpha in the rectangle.</param>
		/// <returns>(bool) True if successful, false if not. Logs and errors.</returns>
		virtual bool DrawEmptyRectangle(int32_t xPos, int32_t yPos
			, int32_t width, int32_t height
			, uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0, uint8_t alpha = 0) = 0;


		virtual bool DrawPolygon(const std::vector<FPoint>& points, Color color) = 0;

		virtual std::shared_ptr<ITexture> GetTextureFromPixels(const std::vector<uint32_t>& pixelMap, unsigned int mapWidth, unsigned int mapHeight, unsigned int pitch) = 0;

		virtual bool DrawPixelMap(const std::vector<uint32_t>& pixelMap, bool isInitialRender = true, unsigned int mapWidth = 0, unsigned int mapHeight = 0, unsigned int pitch = 64 * 4) = 0;

		/// <summary>
		/// Draws a line to the window.
		/// </summary>
		/// <param name="xPosOne">(int32_t) X1 position to draw to.</param>
		/// <param name="yPosOne">(int32_t) Y1 position to draw to.</param>
		/// <param name="xPosTwo">(int32_t) X2 position to draw to.</param>
		/// <param name="yPosTwo">(int32_t) Y2 position to draw to.</param>
		/// <param name="red">(uint8_t) The level of red in the rectangle.</param>
		/// <param name="green">(uint8_t) The level of green in the rectangle.</param>
		/// <param name="blue">(uint8_t) The level of blue in the rectangle.</param>
		/// <param name="alpha">(uint8_t) The level of alpha in the rectangle.</param>
		/// <returns>(bool) True if successful, false if not. Logs and errors.</returns>
		virtual bool DrawLine(int32_t xPosOne, int32_t yPosOne
			, int32_t xPosTwo, int32_t yPosTwo
			, uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0, uint8_t alpha = 0) = 0;

		virtual bool DrawText(ITexture * pText
			, int32_t xPos, int32_t yPos, int32_t width, int32_t height) = 0;

		/// <summary>
		/// Tints a texture to the input color.
		/// </summary>
		/// <param name="pTexture">(ITexture*) Texture to tint.</param>
		/// <param name="red">(uint8_t) The level of red in the rectangle.</param>
		/// <param name="green">(uint8_t) The level of red in the rectangle.</param>
		/// <param name="blue">(uint8_t) The level of red in the rectangle.</param>
		/// <returns>(bool) True if successful, false if not. Logs and errors.</returns>
		virtual bool TintTexture(ITexture* pTexture, uint8_t red, uint8_t green, uint8_t blue) = 0;

		/// <summary>
		/// Changes a textures level of transparency/opacity.
		/// </summary>
		/// <param name="pTexture">(ITexture*) Texture to change.</param>
		/// <param name="alpha">(uint8_t) The level of alpha to change the rectangle.</param>
		/// <returns>(bool) True if successful, false if not. Logs and errors.</returns>
		virtual bool ChangeTextureAlpha(ITexture* pTexture, uint8_t alpha) = 0;

		/// <summary>
		/// Creates the graphics system.
		/// </summary>
		/// <returns>(std::unique_ptr<IGraphics>) The created graphics.</returns>
		static std::unique_ptr<IGraphics> Create();

		//TEMP!
		void SetCamera(float x, float y) { m_cameraX = x; m_cameraY = y; }
		float GetCameraX() { return m_cameraX; }
		float GetCameraY() { return m_cameraY; }

		void SetCameraZoom(float zoom) { m_cameraZoom = zoom; }
		float GetCameraZoom() { return m_cameraZoom; }
	protected:
		float m_cameraX;
		float m_cameraY;
		float m_cameraZoom;
	};
}