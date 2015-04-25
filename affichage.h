// -*- coding: utf-8 -*-

#ifndef __AFFICHAGE_H_
#define __AFFICHAGE_H_


#include "SDL.h"


extern SDL_Surface * surface;

extern Uint32 COLOR_BLACK;
extern Uint32 COLORS_SCALE[48];


/**
 * \fn    void setPixel(const unsigned int x, const unsigned int y, const Uint32 c)
 * \brief Draw a pixel on the screen
 * \param x Coord x
 * \param y Coord y
 * \param c Color
 */
static inline void setPixel(const unsigned int x, const unsigned int y, const Uint32 c)
{
    *((Uint32*)(surface->pixels) + x + y * (surface->w)) = c;
}


extern void initSDL(t_data * const);
extern void refresh(void);
extern int waitKeyPressed(t_data * const);
extern void stopSDL(t_data * const);


#endif

