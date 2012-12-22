#ifdef HAVE_GLES

int gl_init(void *display, void *window, int *quirks);
int gl_flip(const void *fb, int w, int h);
void gl_finish(void);

#else

static __inline int gl_init(void *display, void *window, int *quirks)
{
  return -1;
}
static __inline int gl_flip(const void *fb, int w, int h)
{
  return -1;
}
static __inline void gl_finish(void)
{
}

#endif

#define GL_QUIRK_ACTIVATE_RECREATE 1
