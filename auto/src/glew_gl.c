/* ------------------------------------------------------------------------- */

/*
 * GLEW, just like OpenGL or GLU, does not rely on the standard C library.
 * These functions implement the functionality required in this file.
 */

static GLuint _glewStrLen (const GLubyte* s)
{
  GLuint i=0;
  while (s+i != NULL && s[i] != '\0') i++;
  return i;
}

static GLuint _glewStrCLen (const GLubyte* s, GLubyte c)
{
  GLuint i=0;
  while (s+i != NULL && s[i] != '\0' && s[i] != c) i++;
  return i;
}

static GLboolean _glewStrSame (const GLubyte* a, const GLubyte* b, GLuint n)
{
  GLuint i=0;
  while (i < n && a+i != NULL && b+i != NULL && a[i] == b[i]) i++;
  return i == n;
}

/* 
 * Search for name in the extensions string. Use of strstr()
 * is not sufficient because extension names can be prefixes of
 * other extension names. Could use strtok() but the constant
 * string returned by glGetString might be in read-only memory.
 */
GLboolean glewGetExtension (const char* name)
{    
  GLubyte* p;
  GLubyte* end;
  GLuint len = _glewStrLen((const GLubyte*)name);
  p = (GLubyte*)glGetString(GL_EXTENSIONS);
  if (0 == p) return GL_FALSE;
  end = p + _glewStrLen(p);
  while (p < end)
  {
    GLuint n = _glewStrCLen(p, ' ');
    if (len == n && _glewStrSame((const GLubyte*)name, p, n)) return GL_TRUE;
    p += n+1;
  }
  return GL_FALSE;
}

/* ------------------------------------------------------------------------- */

#ifndef GLEW_MX
static
#endif
GLenum glewContextInit (GLEW_CONTEXT_ARG_DEF_LIST)
{
  const GLubyte* s;
  GLuint major, minor;
  /* query opengl version */
  s = glGetString(GL_VERSION);
  if (!s) return GLEW_ERROR_NO_GL_VERSION;
  major = _glewStrCLen(s, '.')-1;
  minor = _glewStrCLen(s, '.')+1;
  
  if (s+major == NULL
      || s+minor-1 == NULL || s+minor == NULL
      || (s[major] == 1 && s[minor] < '1'))
  {
    return GLEW_ERROR_GL_VERSION_10_ONLY;
  }
  else
  {
    if (s[major] >= '2')
    {
      GLEW_VERSION_1_1 = GL_TRUE;
      GLEW_VERSION_1_2 = GL_TRUE;
      GLEW_VERSION_1_3 = GL_TRUE;
      GLEW_VERSION_1_4 = GL_TRUE;
      GLEW_VERSION_1_5 = GL_TRUE;
      GLEW_VERSION_2_0 = GL_TRUE;
    }
    else
    {
      if (s[minor] >= '5')
      {
        GLEW_VERSION_1_1 = GL_TRUE;
        GLEW_VERSION_1_2 = GL_TRUE;
        GLEW_VERSION_1_3 = GL_TRUE;
        GLEW_VERSION_1_4 = GL_TRUE;
        GLEW_VERSION_1_5 = GL_TRUE;
        GLEW_VERSION_2_0 = GL_FALSE;
      }
      if (s[minor] == '4')
      {
        GLEW_VERSION_1_1 = GL_TRUE;
        GLEW_VERSION_1_2 = GL_TRUE;
        GLEW_VERSION_1_3 = GL_TRUE;
        GLEW_VERSION_1_4 = GL_TRUE;
        GLEW_VERSION_1_5 = GL_FALSE;
        GLEW_VERSION_2_0 = GL_FALSE;
      }
      if (s[minor] == '3')
      {
        GLEW_VERSION_1_1 = GL_TRUE;
        GLEW_VERSION_1_2 = GL_TRUE;
        GLEW_VERSION_1_3 = GL_TRUE;
        GLEW_VERSION_1_4 = GL_FALSE;
        GLEW_VERSION_1_5 = GL_FALSE;
        GLEW_VERSION_2_0 = GL_FALSE;
      }
      if (s[minor] == '2')
      {
        GLEW_VERSION_1_1 = GL_TRUE;
        GLEW_VERSION_1_2 = GL_TRUE;
        GLEW_VERSION_1_3 = GL_FALSE;
        GLEW_VERSION_1_4 = GL_FALSE;
        GLEW_VERSION_1_5 = GL_FALSE;
        GLEW_VERSION_2_0 = GL_FALSE;
      }
      if (s[minor] < '2')
      {
        GLEW_VERSION_1_1 = GL_TRUE;
        GLEW_VERSION_1_2 = GL_FALSE;
        GLEW_VERSION_1_3 = GL_FALSE;
        GLEW_VERSION_1_4 = GL_FALSE;
        GLEW_VERSION_1_5 = GL_FALSE;
        GLEW_VERSION_2_0 = GL_FALSE;
      }
    }
  }
  /* initialize extensions */
