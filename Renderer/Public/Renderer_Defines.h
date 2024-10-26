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

struct RENDERER_DLL RENDER_OBJECT
{
public: /* For.PlayerName */
	string		strName;
public: /* For.GlowFilter_Color */
	XMFLOAT4	vGlowColor;



public: /* For.Constructor*/

    /* For.Default */
    RENDER_OBJECT()
        : strName(""), vGlowColor({ 0.0f, 0.0f, 0.0f, 1.0f }) {}

    /* For.Player */
    RENDER_OBJECT(const string& name)
        : strName(name), vGlowColor({ 0.0f, 0.0f, 0.0f, 1.0f }) {}

    /* For.GlowObject */
    RENDER_OBJECT(const XMFLOAT4& glowColor)
        : strName(""), vGlowColor(glowColor) {}

    /* For.... юс╫ц */
    RENDER_OBJECT(const string& name, const XMFLOAT4& glowColor)
        : strName(name), vGlowColor(glowColor) {}

};
