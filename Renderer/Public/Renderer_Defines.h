
#ifdef RENDERER_EXPORTS
#define RENDERER_DLL _declspec(dllexport)

#else
#define RENDERER_DLL _declspec(dllimport)

#endif


namespace Renderer
{
	typedef struct
	{
		float fBlurValue = {3.2f};
	}RENDER_EFFECT_DESC;
}

using namespace Renderer;
