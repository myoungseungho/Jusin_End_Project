#pragma once

#ifdef RENDERER_EXPORTS
#define RENDERER_DLL _declspec(dllexport)

#else
#define RENDERER_DLL _declspec(dllimport)

#endif


namespace Renderer
{

}

using namespace Renderer;

struct GLOW_DESC
{
    XMFLOAT4 vGlowColor;
    float fGlowFactor;

    // 기본 생성자
    GLOW_DESC() : vGlowColor{ 0.0f, 0.0f, 0.0f, 1.0f }, fGlowFactor(1.0f) {}

    // 매개변수 있는 생성자
    GLOW_DESC(const XMFLOAT4& glowColor, float glowFactor)
        : vGlowColor(glowColor), fGlowFactor(glowFactor) {}
};

struct RENDERER_DLL RENDER_OBJECT
{
public: /* For.PlayerName */
    string strName;

public: /* For.GlowFilter_Color */
    GLOW_DESC tGlowDesc;

public: /* For.Constructor */

    /* For.Default */
    RENDER_OBJECT()
        : strName(""), tGlowDesc() {}

    /* For.Player */
    RENDER_OBJECT(const string& name)
        : strName(name), tGlowDesc() {}

    /* For.GlowObject */
    RENDER_OBJECT(const XMFLOAT4& glowColor)
        : strName(""), tGlowDesc(glowColor, 1.0f) {}

    /* For.... 임시 */
    RENDER_OBJECT(const string& name, const XMFLOAT4& glowColor)
        : strName(name), tGlowDesc(glowColor, 1.0f) {} 

    /* For.... 임시 */
    RENDER_OBJECT(const string& name, const XMFLOAT4& glowColor, float glowFactor)
        : strName(name), tGlowDesc(glowColor, glowFactor) {}
};

