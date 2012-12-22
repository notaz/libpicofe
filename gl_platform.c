#include <stdio.h>
#include <stdlib.h>
#include <EGL/egl.h>
#include <GLES/gl.h>

#include "gl.h"
#include "gl_platform.h"

#ifdef VCOS_VERSION

/*
 * hacks for Broadcom VideoCore / Raspberry Pi..
 * Why do I have to do this proprietary API stuff,
 * couldn't they implement EGL properly? D:
 */
#include <bcm_host.h>
#include <X11/Xlib.h>

static Display *x11display;
static Window x11window;
static DISPMANX_DISPLAY_HANDLE_T m_dispmanDisplay;
static EGL_DISPMANX_WINDOW_T m_nativeWindow;

static void get_window_rect(VC_RECT_T *rect)
{
	XWindowAttributes xattrs_root;
	uint32_t disp_w = 0, disp_h = 0;
	int dx = 0, dy = 0;
	unsigned int dw = 0, dh = 0, dummy;
	Window root, dummyw;

	graphics_get_display_size(0, &disp_w, &disp_h);
	if (disp_w == 0 || disp_h == 0)
		fprintf(stderr, "ERROR: graphics_get_display_size is broken\n");

	// default to fullscreen
	rect->x = rect->y = 0;
	rect->width = disp_w;
	rect->height = disp_h;

	if (x11display == NULL || x11window == 0)
		return; // use fullscreen

	XGetGeometry(x11display, x11window, &root, &dx, &dy, &dw, &dh,
		&dummy, &dummy);
	XGetWindowAttributes(x11display, root, &xattrs_root);

	if (dw == xattrs_root.width && dh == xattrs_root.height)
		return; // use fullscreen

	XTranslateCoordinates(x11display, x11window, root,
		dx, dy, &dx, &dy, &dummyw);

	// how to deal with that weird centering thing?
	// this is not quite right..
	dx += (disp_w - xattrs_root.width) / 2;
	dy += (disp_h - xattrs_root.height) / 2;

	rect->x = dx;
	rect->y = dy;
	rect->width = dw;
	rect->height = dh;
}

static void submit_rect(void)
{
	DISPMANX_UPDATE_HANDLE_T m_dispmanUpdate;
	DISPMANX_ELEMENT_HANDLE_T m_dispmanElement;
	VC_RECT_T srcRect = { 0, }; // unused, but we segfault without passing it??
	VC_RECT_T dstRect;

	get_window_rect(&dstRect);

	m_dispmanDisplay = vc_dispmanx_display_open(0);
	m_dispmanUpdate = vc_dispmanx_update_start(0);

	m_dispmanElement = vc_dispmanx_element_add(m_dispmanUpdate,
		m_dispmanDisplay, 0, &dstRect, 0, &srcRect,
		DISPMANX_PROTECTION_NONE, 0, 0, DISPMANX_NO_ROTATE);

	m_nativeWindow.element = m_dispmanElement;
	m_nativeWindow.width = dstRect.width;
	m_nativeWindow.height = dstRect.height;

	vc_dispmanx_update_submit_sync(m_dispmanUpdate);
}

int gl_platform_init(void **display, void **window, int *quirks)
{
	x11display = *display;
	x11window = (Window)*window;

	bcm_host_init();
	submit_rect();

	*display = EGL_DEFAULT_DISPLAY;
	*window = &m_nativeWindow;
	*quirks |= GL_QUIRK_ACTIVATE_RECREATE;

	return 0;
}

void gl_platform_finish(void)
{
	vc_dispmanx_display_close(m_dispmanDisplay);
	bcm_host_deinit();
}

#else

int gl_platform_init(void **display, void **window, int *quirks)
{
	return 0;
}

void gl_platform_finish(void)
{
}

#endif
