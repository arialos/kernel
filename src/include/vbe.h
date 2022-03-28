#ifndef VBE_H
#define VBE_H

struct vbe_info_t;

#define vbeColor(r, g, b)  ((r << 16)|(g << 8)|(b << 0))
bool initVBE(struct vbe_info_t *vbe);


#endif