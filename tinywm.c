/* TinyWM is written by Nick Welch <nick@incise.org> in 2005 & 2011.
 *
 * This software is in the public domain
 * and is provided AS IS, with NO WARRANTY. */

#include <stdio.h>
#include <stdlib.h>


#include <X11/Xlib.h>


#define MAX(a, b) ((a) > (b) ? (a) : (b))

int main(void)
{
    Display * dpy;
    XWindowAttributes attr;
    XButtonEvent start;
    XEvent ev;

    if(!(dpy = XOpenDisplay(0x0))) return 1;

    XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("1")), Mod1Mask,
            DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync);
    XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("2")), Mod1Mask,
            DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync);
    XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("3")), Mod1Mask,
            DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync);
    XGrabButton(dpy, 1, Mod1Mask, DefaultRootWindow(dpy), True,
            ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);
    XGrabButton(dpy, 3, Mod1Mask, DefaultRootWindow(dpy), True,
            ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);

    start.subwindow = None;
    for(;;) {
        XNextEvent(dpy, &ev);

        if(ev.type == KeyPress) {

            if(ev.xkey.subwindow != None) {
            	if(ev.xkey.keycode==XKeysymToKeycode(dpy, XStringToKeysym("1"))) { 
    	            XRaiseWindow(dpy, ev.xkey.subwindow);
            	} else if(ev.xkey.keycode==XKeysymToKeycode(dpy, XStringToKeysym("2"))) {
      	            XLowerWindow(dpy, ev.xkey.subwindow);
      	        }
        	}

        	if(ev.xkey.keycode==XKeysymToKeycode(dpy, XStringToKeysym("3"))) {
        		system("xterm &");	
            }
            
        } else if(ev.type == ButtonPress && ev.xbutton.subwindow != None) {
            XGetWindowAttributes(dpy, ev.xbutton.subwindow, &attr);
            start = ev.xbutton;
        } else if(ev.type == MotionNotify && start.subwindow != None) {
            int xdiff = ev.xbutton.x_root - start.x_root;
            int ydiff = ev.xbutton.y_root - start.y_root;
            XMoveResizeWindow(dpy, start.subwindow,
                attr.x + (start.button==1 ? xdiff : 0),
                attr.y + (start.button==1 ? ydiff : 0),
                MAX(1, attr.width + (start.button==3 ? xdiff : 0)),
                MAX(1, attr.height + (start.button==3 ? ydiff : 0)));
        }
        else if(ev.type == ButtonRelease)
            start.subwindow = None;
    }
}

