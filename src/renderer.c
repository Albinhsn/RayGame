#include "renderer.h"
#include "common.h"
#include "files.h"
#include "logging.h"
#include "vector.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <stdio.h>
#include <string.h>

void sta_updateWindowSize(SDL_Window* window, i32 width, i32 height)
{
  sta_updateWindowSizeSDL(window, width, height);
}

void sta_createLineVertexArray(GLuint* vertexArrayId, GLuint* vertexBufferId)
{
  GLfloat bufferData[4] = {0.0f, 0.0f, 0.0f, 0.0f};

  i32     indices[]     = {0, 1};

  sta_glGenVertexArrays(1, vertexArrayId);
  sta_glBindVertexArray(*vertexArrayId);

  sta_glGenBuffers(1, vertexBufferId);
  sta_glBindBuffer(GL_ARRAY_BUFFER, *vertexBufferId);

  sta_glEnableVertexAttribArray(0);

  sta_glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, 0);

  u32 indexBufferId;
  sta_glGenBuffers(1, &indexBufferId);
  sta_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
  sta_glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * 2, indices, GL_STATIC_DRAW);

  sta_glBindVertexArray(0);
}

void sta_create2DQuadVertexArray(GLuint* vertexArrayId, GLuint* vertexBufferId)
{
  GLfloat bufferData[20] = {
      -1.0f, -1.0f, 0.0f, 1.0f, //
      1.0f,  -1.0f, 1.0f, 1.0f, //
      -1.0f, 1.0f,  0.0f, 0.0f, //
      1.0f,  1.0f,  1.0f, 0.0f  //
  };
  int    indices[6] = {0, 1, 2, 1, 3, 2};

  GLuint indexBufferId;

  sta_glGenVertexArrays(1, vertexArrayId);
  sta_glBindVertexArray(*vertexArrayId);

  sta_glGenBuffers(1, vertexBufferId);
  sta_glBindBuffer(GL_ARRAY_BUFFER, *vertexBufferId);
  sta_glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(GLfloat), bufferData, GL_STATIC_DRAW);

  sta_glEnableVertexAttribArray(0);
  sta_glEnableVertexAttribArray(1);

  sta_glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, 0);
  sta_glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (signed char*)NULL + (2 * sizeof(GLfloat)));

  sta_glGenBuffers(1, &indexBufferId);
  sta_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
  sta_glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indices, GL_STATIC_DRAW);

  sta_glBindVertexArray(0);
}

bool sta_createAndCompileShader(GLuint* shaderId, GLenum glShaderMacro, const char* source)
{
  String buffer = {};
  Arena  arena  = {};
  arena.maxSize = 2048;
  arena.ptr     = 0;
  arena.memory  = (u64)malloc(arena.maxSize);
  bool res      = sta_readFile(&arena, &buffer, source);
  if (!res)
  {
    sta_logVar(&GlobalLogger, LOGGING_LEVEL_WARNING, "Failed to read shader from '%s'\n", source);
    return false;
  }
  *shaderId = sta_glCreateShader(glShaderMacro);
  sta_glShaderSource(*shaderId, 1, (const char**)&buffer.buffer, NULL);

  sta_glCompileShader(*shaderId);

  int result;
  sta_glGetShaderiv(*shaderId, GL_COMPILE_STATUS, &result);
  if (result != 1)
  {
    int logSize;

    sta_glGetShaderiv(*shaderId, GL_INFO_LOG_LENGTH, &logSize);
    logSize++;
    char infoLog[logSize];
    infoLog[logSize - 1] = '\0';

    sta_glGetShaderInfoLog(*shaderId, logSize, NULL, infoLog);
    printf("Failed to compile shader\n");
    printf("%s\n", infoLog);
    return false;
  }

  free((u8*)arena.memory);

  return true;
}

bool sta_createAndCompileVertexShader(GLuint* shaderId, const char* source)
{
  return sta_createAndCompileShader(shaderId, GL_VERTEX_SHADER, source);
}

bool sta_createAndCompileFragmentShader(GLuint* shaderId, const char* source)
{
  return sta_createAndCompileShader(shaderId, GL_FRAGMENT_SHADER, source);
}

bool sta_createAndAttachShaders(GLuint* programId, const char* vsLocation, const char* psLocation)
{
  GLuint vShader, fShader;
  bool   res = sta_createAndCompileVertexShader(&vShader, vsLocation);
  if (!res)
  {
    return false;
  }
  res = sta_createAndCompileFragmentShader(&fShader, psLocation);
  if (!res)
  {
    return false;
  }

  *programId = sta_glCreateProgram();
  sta_glAttachShader(*programId, vShader);
  sta_glAttachShader(*programId, fShader);

  return true;
}

void sta_createTextureShaderProgram(GLuint* programId, const char* vsLocation, const char* psLocation)
{

  sta_createAndAttachShaders(programId, vsLocation, psLocation);

  sta_glBindAttribLocation(*programId, 0, "inputPosition");
  sta_glBindAttribLocation(*programId, 1, "inputTexCoord");

  sta_glLinkProgram(*programId);
}

void sta_createTextShaderProgram(GLuint* programId, const char* vsLocation, const char* psLocation)
{

  sta_createAndAttachShaders(programId, vsLocation, psLocation);

  sta_glBindAttribLocation(*programId, 0, "inputPosition");
  sta_glBindAttribLocation(*programId, 1, "inputTexCoord");

  sta_glLinkProgram(*programId);
}

void sta_createLineShaderProgram(GLuint* programId, const char* vsLocation, const char* psLocation)
{
  sta_createAndAttachShaders(programId, vsLocation, psLocation);

  sta_glBindAttribLocation(*programId, 0, "inputPosition");

  sta_glLinkProgram(*programId);
}

void sta_createShaderProgram(GLuint* programId, const char* vsLocation, const char* psLocation, const char** attribLocations, u64 attribCount)
{
  sta_createAndAttachShaders(programId, vsLocation, psLocation);
  for (u64 i = 0; i < attribCount; i++)
  {
    sta_glBindAttribLocation(*programId, i, attribLocations[i]);
  }
  sta_glLinkProgram(*programId);
}

void sta_renderQuadTextureTransMatrix(GLuint programId, GLuint vertexArrayId, Matrix3x3* transMatrix, u32 textureIdx)
{
  sta_glUseProgram(programId);
  sta_glBindVertexArray(vertexArrayId);

  glBindTexture(GL_TEXTURE_2D, textureIdx);

  i32 location = sta_glGetUniformLocation(programId, "transMatrix");
  if (location == -1)
  {
    printf("failed to set transMatrix\n");
    exit(1);
  }
  sta_glUniformMatrix3fv(location, 1, true, (f32*)transMatrix);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  sta_glBindVertexArray(0);
}

static void sta_setLineShaderParams(GLuint programId, Color* color)
{
  sta_glUseProgram(programId);

  int location = sta_glGetUniformLocation(programId, "pixelColor");
  if (location == -1)
  {
    printf("Failed to get pixel color location\n");
    exit(1);
  }
  f32 c[4] = {color->r, color->g, color->b, color->a};
  sta_glUniform4fv(location, 1, &c[0]);
}

static void sta_setTextShaderParams(GLuint programId, Color* color)
{
  sta_glUseProgram(programId);

  i32 location = sta_glGetUniformLocation(programId, "fontTexture");
  if (location == -1)
  {
    printf("Failed to get fontTexture location\n");
    exit(1);
  }
  sta_glUniform1i(location, 0);

  location = sta_glGetUniformLocation(programId, "pixelColor");
  if (location == -1)
  {
    printf("Failed to get pixel color location\n");
    exit(1);
  }
  f32 c[4] = {color->r, color->g, color->b, color->a};
  sta_glUniform4fv(location, 1, &c[0]);
}

void sta_renderTexture(Renderer* renderer, Matrix3x3* transMatrix, u32 textureIdx)
{
  sta_glUseProgram(renderer->textureProgramId);
  sta_glBindVertexArray(renderer->textureVertexId);

  glBindTexture(GL_TEXTURE_2D, textureIdx);

  i32 location = sta_glGetUniformLocation(renderer->textureProgramId, "transMatrix");
  if (location == -1)
  {
    printf("failed to set transMatrix\n");
    exit(1);
  }
  sta_glUniformMatrix3fv(location, 1, true, (f32*)transMatrix);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  sta_glBindVertexArray(0);
}

void sta_renderTextureTile(Renderer* renderer, f32 x, f32 y, f32 width, f32 height, u32 tiledTextureIdx, u32 textureIdx)
{
  Matrix3x3 transMatrix = {};
  getTransformationMatrix(&transMatrix, x, y, width, height);

  TextureTiled texture = renderer->tiledTextures[tiledTextureIdx];
  u32          maxCol  = texture.texture->width / texture.dim;
  u32          maxRow  = texture.texture->height / texture.dim;
  u32          row     = textureIdx / maxCol;
  u32          col     = textureIdx % maxCol;

  if (row >= maxRow)
  {
    printf("SEVERE: Trying to access outside of texture, tiledTexture %d, %d vs %d, textureIdx: %d, mr %d, mc %d\n", tiledTextureIdx, row, maxRow, textureIdx, maxRow, maxCol);
    return;
  }

  f32     uvHeight       = 1.0f / (f32)maxRow;
  f32     uvWidth        = 1.0f / (f32)maxCol;

  f32     uvY            = uvHeight * row;
  f32     uvX            = uvWidth * col;

  GLfloat bufferData[20] = {
      -1.0f, -1.0f, uvX,           uvY + uvHeight, //
      1.0f,  -1.0f, uvX + uvWidth, uvY + uvHeight, //
      -1.0f, 1.0f,  uvX,           uvY,            //
      1.0f,  1.0f,  uvX + uvWidth, uvY             //
  };

  sta_glBindVertexArray(renderer->textureVertexId);
  sta_glBindBuffer(GL_ARRAY_BUFFER, renderer->textureBufferId);
  sta_glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(GLfloat), bufferData, GL_STATIC_DRAW);

  sta_renderTexture(renderer, &transMatrix, texture.texture->textureId);
}

// ToDo hoist this out to file
void sta_initTiledTextures(Renderer* renderer)
{
  const u32    numberOfTiles = 2;
  TextureModel tiles[]       = {TEXTURE_WALLS, TEXTURE_MONSTERS};
  u32          tileCount[]   = {6, 4};
  for (u32 i = 0; i < numberOfTiles; i++)
  {
    TextureTiled* tile = &renderer->tiledTextures[tiles[i]];
    tile->texture      = &renderer->textures[tiles[i]];
    tile->dim          = tile->texture->width / tileCount[i];
    tile->count        = tileCount[i];
  }
}

void sta_renderUnfilledQuad(GLuint programId, GLuint vertexArrayId, GLuint vertexBufferId, Vec2f32 start, Vec2f32 end, u32 width, Color* color)
{
  sta_renderLine(programId, vertexArrayId, vertexBufferId, CREATE_VEC2f32(start.x, start.y), CREATE_VEC2f32(end.x, start.y), width, color);
  sta_renderLine(programId, vertexArrayId, vertexBufferId, CREATE_VEC2f32(start.x, start.y), CREATE_VEC2f32(start.x, end.y), width, color);
  sta_renderLine(programId, vertexArrayId, vertexBufferId, CREATE_VEC2f32(end.x, start.y), CREATE_VEC2f32(end.x, end.y), width, color);
  sta_renderLine(programId, vertexArrayId, vertexBufferId, CREATE_VEC2f32(start.x, end.y), CREATE_VEC2f32(end.x, end.y), width, color);
}

static void sta_createTextBuffers(Font* font)
{
  GLuint indexBufferId;

  int    vertexCount = TEXT_MAX_LENGTH * 4;
  GLuint indices[TEXT_MAX_LENGTH * 6];
  for (int i = 0, idx = 0; idx < TEXT_MAX_LENGTH * 6; idx += 6, i += 4)
  {
    indices[idx + 0] = i + 0;
    indices[idx + 1] = i + 1;
    indices[idx + 2] = i + 2;
    indices[idx + 3] = i + 0;
    indices[idx + 4] = i + 3;
    indices[idx + 5] = i + 1;
  }

  GLfloat vertices[vertexCount];
  for (int i = 0; i < vertexCount; i++)
  {
    vertices[i] = 0;
  }

  sta_glGenVertexArrays(1, &font->vertexArrayId);
  sta_glBindVertexArray(font->vertexArrayId);

  sta_glGenBuffers(1, &font->vertexBufferId);
  sta_glBindBuffer(GL_ARRAY_BUFFER, font->vertexBufferId);
  sta_glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(GLfloat), vertices, GL_DYNAMIC_DRAW);

  sta_glEnableVertexAttribArray(0);
  sta_glEnableVertexAttribArray(1);
  sta_glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(GLfloat) * 5, 0);
  sta_glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(GLfloat) * 5, (unsigned char*)NULL + (3 * sizeof(float)));

  sta_glGenBuffers(1, &indexBufferId);

  sta_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
  sta_glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

  sta_glBindVertexArray(0);
}

static void sta_parseFontTypes(struct Font* font, const char* fileLocation)
{

  FILE* file;
  file = fopen(fileLocation, "r");
  char line[256];
  char number[32];
  for (int i = 0; i < 95; i++)
  {
    memset(number, 0, 32);
    fgets(line, sizeof(line), file);
    u8 pos = 0;
    u8 inc = 0;
    // remove number
    i32 idx;
    parseIntFromString(&idx, line, &inc);
    pos = inc;

    // remove whitespace, number and another whitespace
    pos += 3;
    parseFloatFromString(&font->type[idx].left, &line[pos], &inc);
    pos += inc;

    // This should always be the start of the number
    // parse whitespace again
    while (line[pos] == ' ')
    {
      pos++;
    }

    parseFloatFromString(&font->type[idx].right, &line[pos], &inc);
    pos += inc;
    while (line[pos] == ' ')
    {
      pos++;
    }
    parseIntFromString(&font->type[idx].size, &line[pos], &inc);
  }
}

void sta_initFont(Font* font, const char* fontDataLocation)
{
  sta_parseFontTypes(font, fontDataLocation);
  sta_createTextBuffers(font);
}

void sta_buildUpdatedTextVertexArray(Font* font, f32* vertices, u32 vertexCount, const char* text, f32 x, f32 y, f32 spaceSize, f32 fontSize, TextIndentation indentation)
{
  f32 drawX        = x * 0.01f;
  f32 drawY        = (y + fontSize) * 0.01f;
  f32 height       = fontSize * 0.04f;

  f32 sizeModifier = 0.5f;

  i32 numLetters   = strlen(text);
  if (numLetters > TEXT_MAX_LENGTH)
  {
    printf("WARNING: Trying to write text with %d characters, %d is max", numLetters, TEXT_MAX_LENGTH);
    numLetters = TEXT_MAX_LENGTH;
  }

  f32 totalSize = 0;
  for (u32 i = 0; i < numLetters; i++)
  {
    char letter    = text[i];
    f32  addedSize = font->type[(u8)letter].size * 0.01f * sizeModifier;
    totalSize += addedSize != 0 ? addedSize : spaceSize * 0.01f;
  }
  if (indentation == TEXT_INDENTATION_CENTERED)
  {
    drawX -= totalSize / 2.0f;
  }
  else if (indentation == TEXT_INDENTATION_END)
  {
    drawX -= totalSize;
  }

  for (i32 letterIdx = 0, vertexIdx = 0; letterIdx < numLetters; letterIdx++)
  {
    char letter = text[letterIdx];
    if (letter == 32)
    {
      drawX += spaceSize * 0.01f;
    }
    else
    {
      FontType type           = font->type[(u8)letter];
      f32      size           = type.size * 0.01f * sizeModifier;

      vertices[vertexIdx + 0] = drawX;
      vertices[vertexIdx + 1] = drawY;
      vertices[vertexIdx + 2] = 0.0f;
      vertices[vertexIdx + 3] = type.left;
      vertices[vertexIdx + 4] = 0.0f;
      vertexIdx += 5;

      vertices[vertexIdx + 0] = drawX + size;
      vertices[vertexIdx + 1] = drawY - height;
      vertices[vertexIdx + 2] = 0.0f;
      vertices[vertexIdx + 3] = type.right;
      vertices[vertexIdx + 4] = 1.0f;
      vertexIdx += 5;

      vertices[vertexIdx + 0] = drawX;
      vertices[vertexIdx + 1] = drawY - height;
      vertices[vertexIdx + 2] = 0.0f;
      vertices[vertexIdx + 3] = type.left;
      vertices[vertexIdx + 4] = 1.0f;
      vertexIdx += 5;

      vertices[vertexIdx + 0] = drawX + size;
      vertices[vertexIdx + 1] = drawY;
      vertices[vertexIdx + 2] = 0.0f;
      vertices[vertexIdx + 3] = type.right;
      vertices[vertexIdx + 4] = 0.0f;
      vertexIdx += 5;

      drawX += size + 0.002f;
    }
  }
}

void sta_generateTextures(Renderer* renderer, const char* textureLocations)
{
  FILE* filePtr = fopen(textureLocations, "rb");
  if (filePtr == 0)
  {
    sta_logVar(&GlobalLogger, LOGGING_LEVEL_WARNING, "Failed to open texture location '%s'\n", textureLocations);
    return;
  }
  char buffer[256];
  memset(buffer, 0, 256);
  u8 idx = 0;
  while (fgets(buffer, 256, filePtr))
  {
    Texture* texture = &renderer->textures[idx];
    glGenTextures(1, &texture->textureId);
    glBindTexture(GL_TEXTURE_2D, texture->textureId);

    buffer[strlen(buffer) - 1] = '\0';
    if (!sta_loadPNG(&texture->data, &texture->width, &texture->height, buffer))
    {
      sta_logVar(&GlobalLogger, LOGGING_LEVEL_WARNING, "Couldn't parse '%s'\n", buffer);
      continue;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    sta_glGenerateMipmap(GL_TEXTURE_2D);
    free(texture->data);
    idx++;
  }
}

void sta_updateText(Font* font, const char* text, f32 x, f32 y, f32 fontSize, f32 spaceSize, TextIndentation indentation)
{
  u32 vertexCount = TEXT_MAX_LENGTH * 4 * 5;
  f32 vertices[vertexCount];
  for (int i = 0; i < vertexCount; i++)
  {
    vertices[i] = 0.0f;
  }

  sta_glBindVertexArray(font->vertexArrayId);

  sta_buildUpdatedTextVertexArray(font, vertices, vertexCount, text, x, y, spaceSize, fontSize, indentation);

  sta_glBindBuffer(GL_ARRAY_BUFFER, font->vertexArrayId);
  sta_glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(f32), vertices, GL_DYNAMIC_DRAW);

  sta_glBindVertexArray(0);
}

void sta_createTextureVertexArray(Renderer* renderer)
{
  GLfloat bufferData[20] = {
      -1.0f, -1.0f, 0.0f, 1.0f, //
      1.0f,  -1.0f, 1.0f, 1.0f, //
      -1.0f, 1.0f,  0.0f, 0.0f, //
      1.0f,  1.0f,  1.0f, 0.0f  //
  };
  int    indices[6] = {0, 1, 2, 1, 3, 2};

  GLuint indexBufferId;

  sta_glGenVertexArrays(1, &renderer->textureVertexId);
  sta_glBindVertexArray(renderer->textureVertexId);

  sta_glGenBuffers(1, &renderer->textureBufferId);
  sta_glBindBuffer(GL_ARRAY_BUFFER, renderer->textureBufferId);
  sta_glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(GLfloat), bufferData, GL_STATIC_DRAW);

  sta_glEnableVertexAttribArray(0);
  sta_glEnableVertexAttribArray(1);

  sta_glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, 0);
  sta_glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (signed char*)NULL + (2 * sizeof(GLfloat)));

  sta_glGenBuffers(1, &indexBufferId);
  sta_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
  sta_glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indices, GL_STATIC_DRAW);

  sta_glBindVertexArray(0);
}

void sta_initRenderer(Renderer* renderer, Font* font, const i32 screenWidth, const i32 screenHeight)
{
  renderer->window = sta_initSDLWindow(&renderer->context, screenWidth, screenHeight);
  renderer->font   = font;

  sta_generateTextures(renderer, TEXTURE_LOCATION);
  sta_createTextShaderProgram(&font->programId, "./resources/shaders/font.vs", "./resources/shaders/font.ps");
  sta_initFont(font, FONT_DATA_LOCATION);
  font->textureId = renderer->textures[2].textureId;

  sta_createTextureShaderProgram(&renderer->textureProgramId, "./resources/shaders/texture.vs", "./resources/shaders/texture.ps");
  sta_createTextureVertexArray(renderer);

  sta_createLineShaderProgram(&renderer->lineProgramId, "./resources/shaders/line.vs", "./resources/shaders/line.ps");
  sta_createLineVertexArray(&renderer->lineVertexId, &renderer->lineBufferId);

  sta_initTiledTextures(renderer);
}

void sta_renderLine(GLuint programId, GLuint vertexArrayId, GLuint vertexBufferId, Vec2f32 start, Vec2f32 end, u32 width, Color* color)
{
  sta_setLineShaderParams(programId, color);
  sta_glBindVertexArray(vertexArrayId);

  sta_glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
  f32 bufferData[4] = {
      start.x * 0.01f, start.y * 0.01f, //
      end.x * 0.01f, end.y * 0.01f      //
  };

  sta_glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4, bufferData, GL_STATIC_DRAW);
  glLineWidth(width);
  glDrawElements(GL_LINE_STRIP, 2, GL_UNSIGNED_INT, 0);
  glLineWidth(1);
}

static void sta_renderText(Font* font, Color* color)
{
  sta_setTextShaderParams(font->programId, color);
  sta_glBindVertexArray(font->vertexArrayId);
  glBindTexture(GL_TEXTURE_2D, font->textureId);

  glDrawElements(GL_TRIANGLES, TEXT_MAX_LENGTH * 4, GL_UNSIGNED_INT, 0);
}
void sta_renderTextCentered(Font* font, const char* text, Color* color, f32 x, f32 y, f32 fontSize, f32 spaceSize)
{
  sta_updateText(font, text, x, y, fontSize, spaceSize, TEXT_INDENTATION_CENTERED);
  sta_renderText(font, color);
}

void sta_renderTextStartsAt(Font* font, const char* text, Color* color, f32 x, f32 y, f32 fontSize, f32 spaceSize)
{
  sta_updateText(font, text, x, y, fontSize, spaceSize, TEXT_INDENTATION_START);
  sta_renderText(font, color);
}

void sta_renderTextEndsAt(Font* font, const char* text, Color* color, f32 x, f32 y, f32 fontSize, f32 spaceSize)
{
  sta_updateText(font, text, x, y, fontSize, spaceSize, TEXT_INDENTATION_END);
  sta_renderText(font, color);
}
