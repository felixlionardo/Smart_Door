#ifndef _CAPTURE_H_
#define _CAPTURE_H_

// Camera Streaming
// Source: https://opencoursehub.cs.sfu.ca/bfraser/grav-cms/cmpt433/links/files/2022-student-howtos/StreamingWebcamFromBeagleBoneToNodeJSServer.pdf

#include <stdint.h>

void openConnectionT(void);
int sendResponseT(const void *str, int size);
void closeConnectionT(void);
void errno_exit(const char *s);
int xioctl(int fh, int request, void *arg);
void process_image(const void *p, int size);
int read_frame(void);
void mainloop(void);
void stop_capturing(void);
void start_capturing(void);
void uninit_device(void);
void init_device(void);
void close_device(void);
void open_device(void);

#endif