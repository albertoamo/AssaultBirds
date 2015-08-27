/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This allows to render the scene to a texture instead of the screen, it is used to do special FX.
*/

#ifndef RENDER_TO_TEXTURE
#define RENDER_TO_TEXTURE

#include <string>
#include <map>
#include "includes.h"
#include "framework.h"
#include "texture.h"

class RenderToTexture : public Texture
{
public:

	GLuint fbo;
	GLuint depthbuffer;
	bool generate_mipmaps;

	static bool init();

	RenderToTexture();
	~RenderToTexture();

	bool createMirror(int width, int height);
	bool createMirrorHDR(int width, int height);
	bool createShadowMap(int width, int height);
	bool create(int width, int height, bool generate_mipmaps = false);

	void enable();
	void disable();


};

#endif