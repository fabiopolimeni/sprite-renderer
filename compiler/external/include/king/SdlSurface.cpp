#include "SdlSurface.h"

#include <stdexcept>
#include <string>
#include <glew/glew.h>
#include <sdl/SDL.h>
#include <sdl/SDL_image.h>

namespace King {
	SdlSurface::SdlSurface(const char* filename)
		: mSurface(IMG_Load(filename), SDL_FreeSurface)
		, mTextureId(new unsigned int, &DeleteTexture) {
		if (mSurface == nullptr) {
			throw std::runtime_error(std::string("Unable to load texture ") + filename);
		}
		glGenTextures(1, mTextureId.get());
		glBindTexture(GL_TEXTURE_2D, *mTextureId);
		int internal, external;
		switch (mSurface->format->BytesPerPixel) {
		case 4:
			internal = GL_RGBA8;
			external = GL_RGBA;
			break;
		case 3:
			internal = GL_RGB8;
			external = GL_RGB;
			break;
		case 1:
			internal = GL_R8;
			external = GL_RED;
			break;
		default:
			throw std::runtime_error("Image with unknown channel profile");
			break;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, internal, mSurface->w, mSurface->h, 0, external, GL_UNSIGNED_BYTE, mSurface->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	void SdlSurface::DeleteTexture(unsigned int* textureId) {
		glDeleteTextures(1, textureId);
	}

	void SdlSurface::Bind() {
		glBindTexture(GL_TEXTURE_2D, *mTextureId);
	}

	int SdlSurface::Width() const {
		return mSurface->w;
	}

	int SdlSurface::Height() const {
		return mSurface->h;
	}

	SdlSurface::operator SDL_Surface*() {
		return mSurface.get();
	}
}