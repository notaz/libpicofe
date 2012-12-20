#ifdef HAVE_GLES

int gl_init(void *display, void *window);
int gl_flip(const void *fb, int w, int h);
void gl_finish(void);

#else

static __inline int gl_init(void *display, void *window) { return -1; }
static __inline int gl_flip(const void *fb, int w, int h) { return -1; }
static __inline void gl_finish(void) {}

#endif
