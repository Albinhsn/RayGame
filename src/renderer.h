#ifndef RENDERER_H
#define RENDERER_H

#include "common.h"
#include "sdl.h"
#include "vector.h"

#define TEXT_MAX_LENGTH 32

typedef struct Renderer Renderer;

enum TextIndentation
{
  TEXT_INDENTATION_START,
  TEXT_INDENTATION_CENTERED,
  TEXT_INDENTATION_END,
};
typedef enum TextIndentation TextIndentation;

struct FontType
{
  float left, right;
  int   size;
};
typedef struct FontType FontType;

struct Font
{
  GLuint   programId;
  GLuint   vertexArrayId;
  GLuint   vertexBufferId;
  GLuint   textureId;
  FontType type[256];
};
typedef struct Font Font;

struct Renderer
{
  SDL_Window*   window;
  SDL_GLContext context;
  GLuint        textureProgramId;
  GLuint        textureVertexId;
  GLuint        textureBufferId;
  GLuint        lineProgramId;
  GLuint        lineVertexId;
  GLuint        lineBufferId;
  Font*         font;
  u32          textures[32];
};

void               sta_initRenderer(Renderer* renderer, Font* font, const i32 screenWidth, const i32 screenHeight);
void               sta_renderUnfilledQuad(GLuint programId, GLuint vertexArrayId, GLuint vertexBufferId, Vec2f32 start, Vec2f32 end, u32 width, Color* color);
void               sta_renderLine(GLuint programId, GLuint vertexArrayId, GLuint vertexBufferId, Vec2f32 start, Vec2f32 end, u32 width, Color* color);
void               sta_updateWindowSize(SDL_Window* window, i32 width, i32 height);
void               sta_renderTexture(Matrix3x3* transMatrix, u32 textureIdx);
void               sta_createLineVertexArray(GLuint* vertexArrayId, GLuint* vertexBufferId);
void               sta_renderTextCentered(Font* font, const char* text, Color* color, f32 x, f32 y, f32 fontSize, f32 spaceSize);
void               sta_renderTextStartsAt(Font* font, const char* text, Color* color, f32 x, f32 y, f32 fontSize, f32 spaceSize);
void               sta_renderTextEndsAt(Font* font, const char* text, Color* color, f32 x, f32 y, f32 fontSize, f32 spaceSize);
void               sta_renderQuadTexture(f32 x, f32 y, f32 width, f32 height, u32 textureIdx);
void               sta_renderTiledTexture(f32 x, f32 y, f32 width, f32 height, u32 tiledTextureIdx, u32 textureIdx);
void               sta_initFont(Font* font, const char* fontDataLocation);
void               sta_buildUpdatedTextVertexArray(Font* font, f32* vertices, u32 vertexCount, const char* text, f32 x, f32 y, f32 spaceSize, f32 fontSize, TextIndentation indentation);
void               sta_updateText(Font* font, const char* text, f32 x, f32 y, f32 fontSize, f32 spaceSize, TextIndentation indentation);

static inline void sta_initNewFrame(Color color)
{
  glClearColor(color.x, color.y, color.z, color.a);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

#endif
