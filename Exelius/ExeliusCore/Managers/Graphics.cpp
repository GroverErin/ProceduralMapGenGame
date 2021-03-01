#include "Graphics.h"
#include "ApplicationLayer.h"
#include "ResourceManagement/Resource.h"

#include <ThirdParty/Middleware/SDL2/include/SDL.h>
#include <ThirdParty/Middleware/SDL2_image/include/SDL_image.h>
#include <ThirdParty/Middleware/SDL2_ttf/include/SDL_ttf.h>

namespace Exelius
{
    static void CloseFont(TTF_Font* pFont)
    {
        TTF_CloseFont(pFont);
    }

    class SDLTexture
        : public ITexture
    {
    public:
        SDLTexture()
            : m_pTexture(nullptr, nullptr)
        {
            //
        }

        virtual ~SDLTexture() = default;

        bool Initialize(SDL_Renderer* pRenderer, SDL_Surface* pSurface)
        {
            m_pTexture = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>(
                SDL_CreateTextureFromSurface(pRenderer, pSurface), &SDL_DestroyTexture);

            if (!m_pTexture)
            {
                auto& logger = IApplicationLayer::GetInstance()->GetLogger();
                logger.LogFatal("Unable to create texture");
                logger.LogFatal(IMG_GetError());
                return false;
            }
            return true;
        }

        bool Initialize(SDL_Texture* pTexture)
        {
            m_pTexture = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>(
                pTexture, &SDL_DestroyTexture);

            if (!m_pTexture)
            {
                auto& logger = IApplicationLayer::GetInstance()->GetLogger();
                logger.LogFatal("Unable to create texture");
                logger.LogFatal(IMG_GetError());
                return false;
            }
            return true;
        }

        virtual void* GetNativeTexture() const final override { return m_pTexture.get(); }

        virtual void GetTextureDimensions(int& width, int& height) final override
        {
            SDL_QueryTexture(m_pTexture.get(), nullptr, nullptr, &width, &height);
        }

    private:
        std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> m_pTexture;
    };

    class SDLFont
        : public IFont
    {
    public:
        SDLFont()
            : m_pFont(nullptr, nullptr)
        {
            //
        }

        virtual ~SDLFont() = default;

        bool Initialize(std::shared_ptr<Resource>& pResource, int fontSize)
        {
            m_pFont = std::unique_ptr<TTF_Font, decltype(&CloseFont)>
                (TTF_OpenFontRW(
                    SDL_RWFromMem(pResource->GetData().data(), (int)pResource->GetData().size())
                    , 0, fontSize)
                    , &CloseFont);

            if (!m_pFont)
            {
                auto& logger = IApplicationLayer::GetInstance()->GetLogger();
                logger.LogFatal("Unable to create font");
                logger.LogFatal(TTF_GetError());
                return false;
            }
            return true;
        }

        virtual void* GetNativeFont() const final override { return m_pFont.get(); }

    private:
        std::unique_ptr<TTF_Font, decltype(&CloseFont)> m_pFont;
    };

    class SDLGraphics
        : public IGraphics
    {
        SDL_Texture* m_pPixelMapTexture;

    public:
        SDLGraphics()
            : m_pRenderer(nullptr, nullptr)
            , m_pPixelMapTexture(nullptr)
        {
            m_cameraX = 0;
            m_cameraY = 0;
            m_cameraZoom = 1;
        }

        virtual ~SDLGraphics()
        {
            SDL_DestroyTexture(m_pPixelMapTexture);
            m_pPixelMapTexture = nullptr;
        }

        virtual bool Initialize(IWindow* pWindow) final override
        {
            auto& logger = IApplicationLayer::GetInstance()->GetLogger();

            SDL_Window* pSDLWindow = reinterpret_cast<SDL_Window*>
                (pWindow->GetNativeWindow());

            m_pRenderer = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>
                (SDL_CreateRenderer(pSDLWindow, -1
                    , SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)
                    , &SDL_DestroyRenderer);

            if (!m_pRenderer)
            {
                logger.LogFatal("SDL_CreateRenderer has failed: ", false);
                logger.LogFatal(SDL_GetError());
                return false;
            }

            SDL_SetRenderDrawBlendMode(m_pRenderer.get(), SDL_BLENDMODE_BLEND);

            return true;
        }

        virtual bool StartDrawing(uint8_t red, uint8_t green, uint8_t blue) final override
        {
            auto& logger = IApplicationLayer::GetInstance()->GetLogger();
            if (SDL_SetRenderDrawColor(m_pRenderer.get(), red, green, blue, SDL_ALPHA_OPAQUE) != 0)
            {
                logger.LogDebug("SDL_SetRenderDrawColor has failed: ", false);
                logger.LogDebug(SDL_GetError());
                return false;
            }


            if (SDL_RenderClear(m_pRenderer.get()) != 0)
            {
                logger.LogDebug("SDL_RenderClear has failed: ", false);
                logger.LogDebug(SDL_GetError());
                return false;
            }

            return true;
        }

        virtual void EndDrawing() final override
        {
            SDL_RenderPresent(m_pRenderer.get());
        }

        virtual std::shared_ptr<ITexture> LoadTexture(std::shared_ptr<Resource> pResource) final override
        {
            if (m_pRenderer == nullptr)
            {
                return nullptr;
            }

            auto& logger = IApplicationLayer::GetInstance()->GetLogger();

            std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>
                pSurface(IMG_Load_RW(SDL_RWFromMem(pResource->GetData().data(),
                    (int)pResource->GetData().size()), 0), &SDL_FreeSurface);

            if (pSurface == nullptr)
            {
                logger.LogDebug("IMG_Load has failed for File: ", false);
                logger.LogDebug(pResource->GetName().c_str());
                logger.LogDebug("IMG_Load Error: ", false);
                logger.LogDebug(IMG_GetError());
                return nullptr;
            }

            std::shared_ptr<ITexture> pTexture = std::make_shared<SDLTexture>();

            if (!static_cast<SDLTexture*>(pTexture.get())->Initialize(
                m_pRenderer.get(), pSurface.get()))
            {
                logger.LogDebug("Failed to initialize texture.");
                return nullptr;
            }

            return pTexture;
        }

        virtual std::shared_ptr<IFont> LoadFont(std::shared_ptr<Resource>& pResource, int fontSize) final override
        {
            if (m_pRenderer == nullptr)
            {
                return nullptr;
            }

            auto& logger = IApplicationLayer::GetInstance()->GetLogger();

            std::shared_ptr<IFont> pFont = std::make_shared<SDLFont>();

            if (!static_cast<SDLFont*>(pFont.get())->Initialize(pResource, fontSize))
            {
                logger.LogDebug("Failed to initialize Font.");
                return nullptr;
            }

            return pFont;
        }

        virtual std::shared_ptr<ITexture> LoadText(const char* pText, std::shared_ptr<IFont>& font
            , uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) final override
        {
            if (!m_pRenderer)
            {
                return nullptr;
            }

            auto& logger = IApplicationLayer::GetInstance()->GetLogger();

            SDL_Color color = { red, green, blue, alpha };
            TTF_Font* pTTFFont = reinterpret_cast<TTF_Font*>(font->GetNativeFont());
            
            std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>
                pSurface(TTF_RenderText_Solid(pTTFFont, pText, color), &SDL_FreeSurface);

            if (!pSurface)
            {
                logger.LogDebug("TTF_RenderText_Solid has failed: ", false);
                logger.LogDebug(TTF_GetError());
                return nullptr;
            }

            std::shared_ptr<ITexture> pTexture = std::make_shared<SDLTexture>();

            if (!static_cast<SDLTexture*>(pTexture.get())->Initialize(
                m_pRenderer.get(), pSurface.get()))
            {
                logger.LogDebug("Failed to initialize texture.");
                return nullptr;
            }

            return std::move(pTexture);
        }

        virtual bool DrawTexture(ITexture* pTexture
            , int32_t xPos, int32_t yPos
            , int32_t width, int32_t height
            , int32_t srcX, int32_t srcY
            , int32_t srcWidth, int32_t srcHeight) final override
        {
            if (!pTexture)
                return false;

            SDL_Texture* pSDLTexture =
                reinterpret_cast<SDL_Texture*>(pTexture->GetNativeTexture());

            SDL_SetTextureBlendMode(pSDLTexture, SDL_BLENDMODE_BLEND);

            auto& logger = IApplicationLayer::GetInstance()->GetLogger();

            SDL_Rect srcRect = { 0,0,0,0 };

            if (srcWidth != 0 && srcHeight != 0)
            {
                srcRect.x = srcX;
                srcRect.y = srcY;
                srcRect.w = srcWidth;
                srcRect.h = srcHeight;
            }

            if (width == 0 || height == 0)
            {
                if (SDL_RenderCopy(m_pRenderer.get(), pSDLTexture, nullptr, nullptr))
                {
                    logger.LogDebug("Unable to draw texture.", false);
                    logger.LogDebug(IMG_GetError());
                    return false;
                }
                return true;
            }

            SDL_Rect rect = { (xPos * (int32_t)m_cameraZoom) - (int32_t)m_cameraX, (yPos * (int32_t)m_cameraZoom) - (int32_t)m_cameraY, (width * (int32_t)m_cameraZoom), (height * (int32_t)m_cameraZoom) };

            if (srcRect.w != 0 && srcRect.h != 0)
            {
                if (SDL_RenderCopy(m_pRenderer.get(), pSDLTexture, &srcRect, &rect))
                {
                    logger.LogDebug("Unable to draw texture.", false);
                    logger.LogDebug(IMG_GetError());
                    return false;
                }
            }
            else
            {
                if (SDL_RenderCopy(m_pRenderer.get(), pSDLTexture, nullptr, &rect))
                {
                    logger.LogDebug("Unable to draw texture.", false);
                    logger.LogDebug(IMG_GetError());
                    return false;
                }
            }

            return true;
        }

        virtual bool DrawTextureEx(ITexture* pTexture
            , int32_t xPos, int32_t yPos
            , int32_t width, int32_t height
            , int32_t srcX, int32_t srcY
            , int32_t srcWidth, int32_t srcHeight
            , float angle) final override
        {
            if (!pTexture)
                return false;

            SDL_Texture* pSDLTexture =
                reinterpret_cast<SDL_Texture*>(pTexture->GetNativeTexture());

            SDL_SetTextureBlendMode(pSDLTexture, SDL_BLENDMODE_BLEND);

            auto& logger = IApplicationLayer::GetInstance()->GetLogger();

            SDL_Rect srcRect = { 0,0,0,0 };

            SDL_Point center = { width / 2, height / 2 };
            SDL_RendererFlip flip = SDL_FLIP_NONE;

            if (srcWidth != 0 && srcHeight != 0)
            {
                srcRect.x = srcX;
                srcRect.y = srcY;
                srcRect.w = srcWidth;
                srcRect.h = srcHeight;
            }

            if (width == 0 || height == 0)
            {
                if (SDL_RenderCopyEx(m_pRenderer.get(), pSDLTexture, &srcRect, nullptr, angle, &center, flip))
                {
                    logger.LogDebug("Unable to draw texture.", false);
                    logger.LogDebug(IMG_GetError());
                    return false;
                }
                return true;
            }

            SDL_Rect rect = { (xPos * (int32_t)m_cameraZoom) - (int32_t)m_cameraX, (yPos * (int32_t)m_cameraZoom) - (int32_t)m_cameraY, (width * (int32_t)m_cameraZoom), (height * (int32_t)m_cameraZoom) };

            if (srcRect.w != 0 && srcRect.h != 0)
            {
                if (SDL_RenderCopyEx(m_pRenderer.get(), pSDLTexture, &srcRect, &rect, angle, &center, flip))
                {
                    logger.LogDebug("Unable to draw texture.", false);
                    logger.LogDebug(IMG_GetError());
                    return false;
                }
            }
            else
            {
                if (SDL_RenderCopyEx(m_pRenderer.get(), pSDLTexture, nullptr, &rect, angle, &center, flip))
                {
                    logger.LogDebug("Unable to draw texture.", false);
                    logger.LogDebug(IMG_GetError());
                    return false;
                }
            }

            return true;
        }

        virtual bool DrawSolidRectangle(int32_t xPos, int32_t yPos
            , int32_t width, int32_t height
            , uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) final override
        {
            auto& logger = IApplicationLayer::GetInstance()->GetLogger();

            if (width == 0 || height == 0)
            {
                logger.LogDebug("Cannot draw a rect with 0 as the width or height.");
                return false;
            }

            SDL_Rect rect = { (xPos * (int32_t)m_cameraZoom) - (int32_t)m_cameraX, (yPos * (int32_t)m_cameraZoom) - (int32_t)m_cameraY, (width * (int32_t)m_cameraZoom), (height * (int32_t)m_cameraZoom) };

            if (SDL_SetRenderDrawColor(m_pRenderer.get(), red, green, blue, alpha))
            {
                logger.LogDebug("SDL_SetRenderDrawColor has failed: ", false);
                logger.LogDebug(SDL_GetError());
                return false;
            }
            if (SDL_RenderFillRect(m_pRenderer.get(), &rect))
            {
                logger.LogDebug("SDL_RenderFillRect has failed: ", false);
                logger.LogDebug(SDL_GetError());
                return false;
            }

            return true;
        }

        virtual bool DrawEmptyRectangle(int32_t xPos, int32_t yPos
            , int32_t width, int32_t height
            , uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) final override
        {
            auto& logger = IApplicationLayer::GetInstance()->GetLogger();

            if (width == 0 || height == 0)
            {
                logger.LogDebug("Cannot draw a rect with 0 as the width or height.");
                return false;
            }

            SDL_Rect rect = { (xPos * (int32_t)m_cameraZoom) - (int32_t)m_cameraX, (yPos * (int32_t)m_cameraZoom) - (int32_t)m_cameraY, (width * (int32_t)m_cameraZoom), (height * (int32_t)m_cameraZoom) };

            if (SDL_SetRenderDrawColor(m_pRenderer.get(), red, green, blue, alpha))
            {
                logger.LogDebug("SDL_SetRenderDrawColor has failed: ", false);
                logger.LogDebug(SDL_GetError());
                return false;
            }
            if (SDL_RenderDrawRect(m_pRenderer.get(), &rect))
            {
                logger.LogDebug("SDL_RenderDrawRect has failed: ", false);
                logger.LogDebug(SDL_GetError());
                return false;
            }

            return true;
        }

        virtual bool DrawPolygon(const std::vector<FPoint>& points, Color) final override
        {
            SDL_SetRenderDrawColor(m_pRenderer.get(), 255, 0, 0, SDL_ALPHA_OPAQUE);

            for (size_t i = 1; i < points.size(); ++i)
            {
                auto [x1, y1] = points[i - 1];
                auto [x2, y2] = points[i];

                if (SDL_RenderDrawLineF(m_pRenderer.get()
                    , x1 - (int32_t)m_cameraX  //Box2DPhysics::kPixelToMeterConversion
                    , y1 - (int32_t)m_cameraY  //Box2DPhysics::kPixelToMeterConversion
                    , x2 - (int32_t)m_cameraX //Box2DPhysics::kPixelToMeterConversion
                    , y2 - (int32_t)m_cameraY))//Box2DPhysics::kPixelToMeterConversion);
                {
                    return false;
                }
            }

            return true;
        }

        virtual std::shared_ptr<ITexture> GetTextureFromPixels(const std::vector<uint32_t>& pixelMap, unsigned int mapWidth, unsigned int mapHeight, unsigned int pitch) final override
        {
            auto& logger = IApplicationLayer::GetInstance()->GetLogger();
            std::shared_ptr<ITexture> pTexture = std::make_shared<SDLTexture>();

            auto* pSDLTexture = SDL_CreateTexture(m_pRenderer.get()
                , SDL_PIXELFORMAT_ABGR32
                , SDL_TEXTUREACCESS_STREAMING
                , mapWidth, mapHeight);

            if (!static_cast<SDLTexture*>(pTexture.get())->Initialize(
                pSDLTexture))
            {
                logger.LogDebug("Failed to initialize texture.");
                return nullptr;
            }

            if (SDL_UpdateTexture(pSDLTexture, nullptr, pixelMap.data(), pitch))
            {
                logger.LogDebug("SDL_UpdateTexture has failed: ", false);
                logger.LogDebug(SDL_GetError());
                return nullptr;
            }

            return pTexture;
        }

        virtual bool DrawPixelMap(const std::vector<uint32_t>& pixelMap, bool isInitialRender, unsigned int mapWidth, unsigned int mapHeight, unsigned int pitch) final override
        {
            auto& logger = IApplicationLayer::GetInstance()->GetLogger();

            if (isInitialRender)
            {
                m_pPixelMapTexture = SDL_CreateTexture(m_pRenderer.get()
                    , SDL_PIXELFORMAT_ABGR32
                    , SDL_TEXTUREACCESS_STREAMING
                    , mapWidth, mapHeight);
            }
            if (!m_pPixelMapTexture)
            {
                logger.LogDebug("SDL_CreateTexture has failed: ", false);
                logger.LogDebug(SDL_GetError());
                return false;
            }

            if (SDL_UpdateTexture(m_pPixelMapTexture, nullptr, pixelMap.data(), pitch))
            {
                logger.LogDebug("SDL_UpdateTexture has failed: ", false);
                logger.LogDebug(SDL_GetError());
                return false;
            }

            if (SDL_RenderCopy(m_pRenderer.get(), m_pPixelMapTexture, nullptr, nullptr))
            {
                logger.LogDebug("SDL_RenderCopy has failed: ", false);
                logger.LogDebug(SDL_GetError());
                return false;
            }

            return true;
        }

        virtual bool DrawLine(int32_t xPosOne, int32_t yPosOne
            , int32_t xPosTwo, int32_t yPosTwo
            , uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) final override
        {
            auto& logger = IApplicationLayer::GetInstance()->GetLogger();

            if ((xPosOne == xPosTwo) && (yPosOne == yPosTwo))
            {
                logger.LogDebug("Cannot draw a line with 0 length.");
                return false;
            }

            if (SDL_SetRenderDrawColor(m_pRenderer.get(), red, green, blue, alpha))
            {
                logger.LogDebug("SDL_SetRenderDrawColor has failed: ", false);
                logger.LogDebug(SDL_GetError());
                return false;
            }
            if (SDL_RenderDrawLine(m_pRenderer.get(), (xPosOne * (int32_t)m_cameraZoom) - (int32_t)m_cameraX
                , (yPosOne * (int32_t)m_cameraZoom) - (int32_t)m_cameraX
                , (xPosTwo * (int32_t)m_cameraZoom) - (int32_t)m_cameraX
                , (yPosTwo * (int32_t)m_cameraZoom) - (int32_t)m_cameraX))
            {
                logger.LogDebug("SDL_RenderDrawLine has failed: ", false);
                logger.LogDebug(SDL_GetError());
                return false;
            }

            return true;
        }

        virtual bool DrawText(ITexture* pTexture
            , int32_t xPos, int32_t yPos, int32_t width, int32_t height) final override
        {
            //auto& logger = IApplicationLayer::GetInstance()->GetLogger();

            SDL_Texture* pSDLTexture = reinterpret_cast<SDL_Texture*>(pTexture->GetNativeTexture());

            SDL_Rect messageRect;
            messageRect.x = xPos;
            messageRect.y = yPos;
            messageRect.w = width;
            messageRect.h = height;

            SDL_RenderCopy(m_pRenderer.get(), pSDLTexture, nullptr, &messageRect);

            return true;
        }

        virtual bool TintTexture(ITexture* pTexture, uint8_t red, uint8_t green, uint8_t blue) final override
        {
            SDL_Texture* pSDLTexture =
                reinterpret_cast<SDL_Texture*>(pTexture->GetNativeTexture());

            auto& logger = IApplicationLayer::GetInstance()->GetLogger();

            if (SDL_SetTextureColorMod(pSDLTexture, red, green, blue))
            {
                logger.LogDebug("SDL_SetTextureColorMod has failed: ", false);
                logger.LogDebug(SDL_GetError());
                return false;
            }

            return true;
        }

        virtual bool ChangeTextureAlpha(ITexture* pTexture, uint8_t alpha) final override
        {
            SDL_Texture* pSDLTexture =
                reinterpret_cast<SDL_Texture*>(pTexture->GetNativeTexture());

            auto& logger = IApplicationLayer::GetInstance()->GetLogger();

            if (SDL_SetTextureAlphaMod(pSDLTexture, alpha))
            {
                logger.LogDebug("SDL_SetTextureAlphaMod has failed: ", false);
                logger.LogDebug(SDL_GetError());
                return false;
            }

            return true;
        }

    private:
        std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> m_pRenderer;
    };

    std::unique_ptr<IGraphics> IGraphics::Create()
    {
        return std::make_unique<SDLGraphics>();
    }
}