#ifndef STA_LIB_SDL_H
#define STA_LIB_SDL_H
#include "common.h"
#include "files.h"
#include <GL/gl.h>
#include <SDL2/SDL.h>

#define INPUT_STATE_LENGTH 127

struct InputState
{
  i32  mouseX;
  i32  mouseY;
  i32  mouseXRel;
  i32  mouseYRel;
  bool md_1;
  bool mr_1;
  bool md_3;
  bool mr_3;
  bool keyboardStateRelease[INPUT_STATE_LENGTH];
  bool keyboardStateDown[INPUT_STATE_LENGTH];
};
typedef struct InputState InputState;

typedef GLuint(APIENTRY* PFNGLCREATESHADERPROC)(GLenum type);
typedef void(APIENTRY* PFNGLCOMPILESHADERPROC)(GLuint shader);
typedef void(APIENTRY* PFNGLGETSHADERIVPROC)(GLuint shader, GLenum pname, GLint* params);
typedef void(APIENTRY* PFNGLGETSHADERINFOLOGPROC)(GLuint shader, GLsizei bufSize, GLsizei* length, char* infoLog);
typedef GLuint(APIENTRY* PFNGLCREATEPROGRAMPROC)(void);
typedef void(APIENTRY* PFNGLATTACHSHADERPROC)(GLuint program, GLuint shader);
typedef void(APIENTRY* PFNGLBINDATTRIBLOCATIONPROC)(GLuint program, GLuint index, const char* name);
typedef void(APIENTRY* PFNGLLINKPROGRAMPROC)(GLuint program);
typedef void(APIENTRY* PFNGLGETPROGRAMIVPROC)(GLuint program, GLenum pname, GLint* params);
typedef void(APIENTRY* PFNGLGETPROGRAMINFOLOGPROC)(GLuint program, GLsizei bufSize, GLsizei* length, char* infoLog);
typedef void(APIENTRY* PFNGLDETACHSHADERPROC)(GLuint program, GLuint shader);
typedef void(APIENTRY* PFNGLDELETESHADERPROC)(GLuint shader);
typedef void(APIENTRY* PFNGLDELETEPROGRAMPROC)(GLuint program);
typedef void(APIENTRY* PFNGLUSEPROGRAMPROC)(GLuint program);
typedef GLint(APIENTRY* PFNGLGETUNIFORMLOCATIONPROC)(GLuint program, const char* name);
typedef void(APIENTRY* PFNGLUNIFORMMATRIX4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void(APIENTRY* PFNGLGENVERTEXARRAYSPROC)(GLsizei n, GLuint* arrays);
typedef void(APIENTRY* PFNGLBINDVERTEXARRAYPROC)(GLuint array);
typedef void(APIENTRY* PFNGLGENBUFFERSPROC)(GLsizei n, GLuint* buffers);
typedef void(APIENTRY* PFNGLBINDBUFFERPROC)(GLenum target, GLuint buffer);
typedef void(APIENTRY* PFNGLBUFFERDATAPROC)(GLenum target, ptrdiff_t size, const GLvoid* data, GLenum usage);
typedef void(APIENTRY* PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint index);
typedef void(APIENTRY* PFNGLVERTEXATTRIBPOINTERPROC)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
typedef void(APIENTRY* PFNGLDISABLEVERTEXATTRIBARRAYPROC)(GLuint index);
typedef void(APIENTRY* PFNGLDELETEBUFFERSPROC)(GLsizei n, const GLuint* buffers);
typedef void(APIENTRY* PFNGLDELETEVERTEXARRAYSPROC)(GLsizei n, const GLuint* arrays);
typedef void(APIENTRY* PFNGLUNIFORM1IPROC)(GLint location, GLint v0);
typedef void(APIENTRY* PFNGLGENERATEMIPMAPPROC)(GLenum target);
typedef void(APIENTRY* PFNGLUNIFORM2FVPROC)(GLint location, GLsizei count, const GLfloat* value);
typedef void(APIENTRY* PFNGLUNIFORM3FVPROC)(GLint location, GLsizei count, const GLfloat* value);
typedef void(APIENTRY* PFNGLUNIFORM4FVPROC)(GLint location, GLsizei count, const GLfloat* value);
typedef void*(APIENTRY* PFNGLMAPBUFFERPROC)(GLenum target, GLenum access);
typedef GLboolean(APIENTRY* PFNGLUNMAPBUFFERPROC)(GLenum target);
typedef void(APIENTRY* PFNGLUNIFORM1FPROC)(GLint location, GLfloat v0);
typedef void(APIENTRY* PFNGLGENFRAMEBUFFERSPROC)(GLsizei n, GLuint* framebuffers);
typedef void(APIENTRY* PFNGLDELETEFRAMEBUFFERSPROC)(GLsizei n, const GLuint* framebuffers);
typedef void(APIENTRY* PFNGLBINDFRAMEBUFFERPROC)(GLenum target, GLuint framebuffer);
typedef void(APIENTRY* PFNGLFRAMEBUFFERTEXTURE2DPROC)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void(APIENTRY* PFNGLGENRENDERBUFFERSPROC)(GLsizei n, GLuint* renderbuffers);
typedef void(APIENTRY* PFNGLBINDRENDERBUFFERPROC)(GLenum target, GLuint renderbuffer);
typedef void(APIENTRY* PFNGLRENDERBUFFERSTORAGEPROC)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
typedef void(APIENTRY* PFNGLFRAMEBUFFERRENDERBUFFERPROC)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void(APIENTRY* PFNGLDELETERENDERBUFFERSPROC)(GLsizei n, const GLuint* renderbuffers);
typedef void(APIENTRY* PFNGLBLENDFUNCSEPARATEPROC)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
typedef void(APIENTRY* PFNGLSHADERSOURCEPROC)(GLuint shader, GLsizei count, const char* const* string, const GLint* length);
typedef void(APIENTRY* PFNGLGENERATEMIPMAPPROC)(GLenum target);
typedef void*(APIENTRY* PFNGLMAPNAMEDBUFFERPROC)(GLuint buffer, GLenum access);
typedef void(APIENTRY* PFNGLFRAMEBUFFERTEXTURE2DPROC)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void(APIENTRY* PFNGLDRAWBUFFERSPROC)(GLint n, const GLenum* bufs);
typedef void(APIENTRY* PFNGLDELETERENDERBUFFERSPROC)(GLsizei n, const GLuint* renderbuffers);
typedef void(APIENTRY* PFNGLGENERATEMIPMAPPROC)(GLuint n);

void        sta_updateWindowSizeSDL(SDL_Window* window, i32 width, i32 height);
SDL_Window* sta_initSDLWindow(SDL_GLContext* context, i32 screenWidth, i32 screenHeight);
GLuint      sta_glCreateShader(GLenum type);
void        sta_glCompileShader(GLuint shader);
void        sta_glGetShaderiv(GLuint shader, GLenum pname, GLint* params);
void        sta_glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei* length, char* infoLog);
GLuint      sta_glCreateProgram();
void        sta_glAttachShader(GLuint program, GLuint shader);
void        sta_glBindAttribLocation(GLuint program, GLuint index, const char* name);
void        sta_glLinkProgram(GLuint program);
void        sta_glGetProgramiv(GLuint program, GLenum pname, GLint* params);
void        sta_glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei* length, char* infoLog);
void        sta_glDetachShader(GLuint program, GLuint shader);
void        sta_glDeleteProgram(GLuint program);
void        sta_glUseProgram(GLuint program);
GLint       sta_glGetUniformLocation(GLuint program, const char* name);
void        sta_glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void        sta_glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void        sta_glGenVertexArrays(GLsizei n, GLuint* arrays);
void        sta_glGenTextures();
void        sta_glBindVertexArray(GLuint array);
void        sta_glGenBuffers(GLsizei n, GLuint* buffers);
void        sta_glBindBuffer(GLenum target, GLuint buffer);
void        sta_glBufferData(GLenum target, ptrdiff_t size, const GLvoid* data, GLenum usage);
void        sta_glEnableVertexAttribArray(GLuint index);
void        sta_glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
void        sta_glDisableVertexAttribArray(GLuint index);
void        sta_glDeleteBuffers(GLsizei n, const GLuint* buffers);
void        sta_glDeleteVertexArrays(GLsizei n, const GLuint* arrays);
void        sta_glUniform1i(GLint location, GLint v0);
void        sta_glGenerateMipmap(GLenum target);
void        sta_glUniform2fv(GLint location, GLsizei count, const GLfloat* value);
void        sta_glUniform3fv(GLint location, GLsizei count, const GLfloat* value);
void        sta_glUniform4fv(GLint location, GLsizei count, const GLfloat* value);
void*       sta_glMapBuffer(GLenum target, GLenum access);
GLboolean   sta_glUnmapBuffer(GLenum target);
void        sta_glUniform1f(GLint location, GLfloat v0);
void        sta_glGenFramebuffers(GLsizei n, GLuint* framebuffers);
void        sta_glDeleteFramebuffers(GLsizei n, const GLuint* framebuffers);
void        sta_glBindFramebuffer(GLenum target, GLuint framebuffer);
void        sta_glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
void        sta_glGenRenderbuffers(GLsizei n, GLuint* renderbuffers);
void        sta_glBindRenderbuffer(GLenum target, GLuint renderbuffer);
void        sta_glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
void        sta_glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
void        sta_glDeleteRenderbuffers(GLsizei n, const GLuint* renderbuffers);
void        sta_glBlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
void        sta_glShaderSource(GLuint shader, GLsizei count, const char* const* string, const GLint* length);
void        sta_glMapNamedBuffer(GLuint buffer, GLenum access);
void        sta_glDrawBuffers(GLint n, const GLenum* bufs);

inline void initInputState(InputState* inputState)
{
  for (u32 i = 0; i < INPUT_STATE_LENGTH; i++)
  {
    inputState->keyboardStateRelease[i] = 0;
    inputState->keyboardStateDown[i]    = 0;
  }
}

bool queryInputEvents(struct InputState* inputState);
void resetInputState(InputState* inputState);
void debugInputState(InputState* inputState);
void getKeyboardInputCharacters(struct InputState* inputState, struct String* string);

#endif
