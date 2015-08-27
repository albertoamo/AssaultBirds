#include "rendertotexture.h"
#include <iostream>

//typedef void (APIENTRY * glGenFramebuffers_func)(GLsizei n, GLuint *framebuffers); glGenFramebuffers_func glGenFramebuffersEXT = NULL;
#ifndef USE_GLEW
	REGISTER_GLEXT( void, glGenFramebuffersEXT, GLsizei n, GLuint *framebuffers )

	REGISTER_GLEXT( void, glDeleteFramebuffersEXT, GLsizei n, const GLuint *framebuffers );
	REGISTER_GLEXT( void, glDeleteRenderbuffersEXT, GLsizei n, const GLuint *renderbuffers );
	REGISTER_GLEXT( void, glBindFramebufferEXT, GLenum target, GLuint framebuffer);
	REGISTER_GLEXT( void, glGenRenderbuffersEXT, GLsizei n, GLuint *renderbuffers);
	REGISTER_GLEXT( void, glBindRenderbufferEXT, GLenum target, GLuint renderbuffer);
	REGISTER_GLEXT( void, glRenderbufferStorageEXT, GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
	REGISTER_GLEXT( void, glFramebufferRenderbufferEXT, GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);

	REGISTER_GLEXT( void, glFramebufferTexture2DEXT, GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level );
	REGISTER_GLEXT( GLenum, glCheckFramebufferStatusEXT, GLenum target);
#endif

bool RenderToTexture::init()
{
	static bool firsttime = true;

	//avoid doing this more than once
	if(firsttime)
	{
		#ifndef USE_GLEW
			IMPORT_GLEXT( glGenFramebuffersEXT );
			IMPORT_GLEXT( glDeleteFramebuffersEXT );
			IMPORT_GLEXT( glDeleteRenderbuffersEXT );
			IMPORT_GLEXT( glBindFramebufferEXT );
			IMPORT_GLEXT( glGenRenderbuffersEXT );
			IMPORT_GLEXT( glBindRenderbufferEXT );
			IMPORT_GLEXT( glRenderbufferStorageEXT );
			IMPORT_GLEXT( glFramebufferRenderbufferEXT );
			IMPORT_GLEXT( glFramebufferTexture2DEXT );
			IMPORT_GLEXT( glCheckFramebufferStatusEXT );
		#endif
	}
	
	firsttime = false;
	return true;
}

RenderToTexture::RenderToTexture()
{
	width = 0;
	height = 0;
	fbo = 0;
	depthbuffer = 0;
	texture_id = 0;
	generate_mipmaps = false;

	RenderToTexture::init();
}

RenderToTexture::~RenderToTexture()
{
	if (fbo)
	{
		glDeleteFramebuffersEXT(1, &fbo);
		glDeleteRenderbuffersEXT(1, &depthbuffer);
	}
}

bool RenderToTexture::create(int width, int height, bool generate_mipmaps)
{
	this->width = width;
	this->height = height;
	this->generate_mipmaps = generate_mipmaps;

	glGenFramebuffersEXT(1, &fbo);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

	glGenRenderbuffersEXT(1, &depthbuffer);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthbuffer);

	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthbuffer);

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (generate_mipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR));
	if (generate_mipmaps)
		this->generateMipmaps(); //glGenerateMipmapEXT(GL_TEXTURE_2D);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texture_id, 0);

	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		std::cout << "Error: Framebuffer object is not completed" << std::endl;
		return false;
	}
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	return true;
}

void RenderToTexture::enable()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void RenderToTexture::disable()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool RenderToTexture::createShadowMap(int width, int height)
{
	this->width = width;
	this->height = height;

	glGenFramebuffers(1, &fbo);

	// Create the depth buffer
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_EQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture_id, 0);


	// Disable writes to the color buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", Status);
		return false;
	}

	return true;
}

bool RenderToTexture::createMirror(int width, int height)
{
	this->width = width;
	this->height = height;
	// Create the FBO
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// Create the depth buffer
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenRenderbuffers(1, &depthbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);

	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);

	return true;
}

bool RenderToTexture::createMirrorHDR(int width, int height)
{
	this->width = width;
	this->height = height;
	// Create the FBO
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	// Create the depth buffer
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenRenderbuffers(1, &depthbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);

	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);

	return true;
}