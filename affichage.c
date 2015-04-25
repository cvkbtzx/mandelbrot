// -*- coding: utf-8 -*-

/**
 * \file  affichage.c
 * \brief Mandelbrot - display functions
 */


#include <stdlib.h>   /* atoi */
#include <stdio.h>    /* printf, sprintf, fgets */
#include <string.h>   /* strcat */
#include "SDL.h"
#include "types.h"
#include "affichage.h"


SDL_Surface * surface;

Uint32 COLOR_BLACK;
Uint32 COLORS_SCALE[48];

static const unsigned short COLORS_COEFFS[] = {
    255,255,255,255,255,255,255,255,
    255,255,223,191,159,127,95 ,63 ,
    31 ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
    0  ,0  ,0  ,0  ,0  ,0  ,0  ,0  ,
    0  ,0  ,31 ,63 ,95 ,127,159,191,
    223,255,255,255,255,255,255,255
};


/*---------------------------------------------------------------------------*/
/**
 * \fn    void initSDL(t_data *d)
 * \brief Initialize SDL
 * \param d Data object
 */
void initSDL(t_data * const d)
{
    unsigned short i, R, G, B;
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL initialization: error %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    atexit(SDL_Quit);
    
    surface = SDL_SetVideoMode(d->DIM, d->DIM, 32, SDL_SWSURFACE);
    if (surface == NULL) {
        fprintf(stderr, "Can't set video mode: error %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    
    SDL_WM_SetCaption("Mandelbrot", NULL);
    
    for (i=0; i<48; i++) {
        R = COLORS_COEFFS[(i+16) % 48];
        G = COLORS_COEFFS[(i+32) % 48];
        B = COLORS_COEFFS[(i)    % 48];
        COLORS_SCALE[i] = SDL_MapRGB(surface->format, R, G, B);
    }
    COLOR_BLACK = SDL_MapRGB(surface->format, 0, 0, 0);
}


/*---------------------------------------------------------------------------*/
/**
 * \fn    void drawHLine(const int x, const int y, const int l, const Uint32 c)
 * \brief Draw a horizontal line
 * \param x Coord x (left end)
 * \param y Coord y (left end)
 * \param l Line length
 * \param c Color
 */
void drawHLine(const int x, const int y, const int l, const Uint32 c)
{
    SDL_Rect r;
    r.x = x; r.y = y;
    r.w = l; r.h = 1;
    SDL_FillRect(surface, &r, c);
}


/*---------------------------------------------------------------------------*/
/**
 * \fn    void drawVLine(const int x, const int y, const int l, const Uint32 c)
 * \brief Draw a vertical line
 * \param x Coord x (top end)
 * \param y Coord y (top end)
 * \param l Line length
 * \param c Color
 */
void drawVLine(const int x, const int y, const int l, const Uint32 c)
{
    SDL_Rect r;
    r.x = x; r.y = y;
    r.w = 1; r.h = l;
    SDL_FillRect(surface, &r, c);
}


/*---------------------------------------------------------------------------*/
/**
 * \fn    void drawSquare(const int x, const int y, const int s)
 * \brief Draw a white square
 * \param x Coord x (top left corner)
 * \param y Coord y (top left corner)
 * \param s Side length
 */
void drawSquare(const int x, const int y, const int s)
{
    Uint32 c = SDL_MapRGB(surface->format, 255, 255, 255);
    drawHLine(x, y, s, c);
    drawHLine(x, y+s-1, s, c);
    drawVLine(x, y, s, c);
    drawVLine(x+s-1, y, s, c);
}


/*---------------------------------------------------------------------------*/
/**
 * \fn    void refresh(void)
 * \brief Update the display
 */
void refresh(void)
{
    SDL_UpdateRect(surface, 0, 0, 0, 0);
}


/*---------------------------------------------------------------------------*/
/**
 * \fn     int waitKeyPressed(t_data * const d)
 * \brief  Wait for a keyboard event and compute new param values
 * \param  d Data object
 * \return Return what to do
 */
int waitKeyPressed(t_data * const d)
{
    SDL_Event event;
    int r = -1;
    long double x = ((d->xmax) + (d->xmin)) / 2.0;
    long double y = ((d->ymax) + (d->ymin)) / 2.0;
    long double x0, x1, y0, y1;
    char s[15];
    char filename[FILENAME_MAX];
    int rectx, recty;
    int rectL = ((d->DIM) + 1) / 2;
    int rect2 = ((d->DIM) - 1) / 2;
    int rect4 = ((d->DIM) + 1) / 4;
    
    do {
        SDL_WaitEvent(&event);
        switch(event.type) {
            case SDL_KEYDOWN:
                
                switch(event.key.keysym.sym) {
                    case SDLK_w:
                        d->xmax = x; d->ymin = y;
                        rectx = 0; recty = rect2;
                        d->zoom = (d->zoom)+1; r = 1;
                        strcat(d->seq, "W"); break;
                    case SDLK_x:
                        d->xmax = (x+(d->xmax))/2.0; d->xmin = ((d->xmin)+x)/2.0; d->ymin = y;
                        rectx = rect4; recty = rect2;
                        d->zoom = (d->zoom)+1; r = 1;
                        strcat(d->seq, "X"); break;
                    case SDLK_c:
                        d->xmin = x; d->ymin = y;
                        rectx = rect2; recty = rect2;
                        d->zoom = (d->zoom)+1; r = 1;
                        strcat(d->seq, "C"); break;
                    case SDLK_q:
                        d->xmax = x; d->ymax = (y+(d->ymax))/2.0; d->ymin = ((d->ymin)+y)/2.0;
                        rectx = 0; recty = rect4;
                        d->zoom = (d->zoom)+1; r = 1;
                        strcat(d->seq, "Q"); break;
                    case SDLK_s:
                        d->xmax = (x+(d->xmax))/2.0; d->xmin = ((d->xmin)+x)/2.0;
                        d->ymax = (y+(d->ymax))/2.0; d->ymin = ((d->ymin)+y)/2.0;
                        rectx = rect4; recty = rect4;
                        d->zoom = (d->zoom)+1; r = 1;
                        strcat(d->seq, "S"); break;
                    case SDLK_d:
                        d->xmin = x; d->ymax = (y+(d->ymax))/2.0; d->ymin = ((d->ymin)+y)/2.0;
                        rectx = rect2; recty = rect4;
                        d->zoom = (d->zoom)+1; r = 1;
                        strcat(d->seq, "D"); break;
                    case SDLK_a:
                        d->xmax = x; d->ymax = y;
                        rectx = 0; recty = 0;
                        d->zoom = (d->zoom)+1; r = 1;
                        strcat(d->seq, "A"); break;
                    case SDLK_z:
                        d->xmax = (x+(d->xmax))/2.0; d->xmin = ((d->xmin)+x)/2.0; d->ymax = y;
                        rectx = rect4; recty = 0;
                        d->zoom = (d->zoom)+1; r = 1;
                        strcat(d->seq, "Z"); break;
                    case SDLK_e:
                        d->xmin = x; d->ymax = y;
                        rectx = rect2; recty = 0;
                        d->zoom = (d->zoom)+1; r = 1;
                        strcat(d->seq, "E"); break;
                    case SDLK_o:
                        x0 = ((d->xmin)*3.0 - (d->xmax))/2.0; x1 = ((d->xmax)*3.0 - (d->xmin))/2.0;
                        y0 = ((d->ymin)*3.0 - (d->ymax))/2.0; y1 = ((d->ymax)*3.0 - (d->ymin))/2.0;
                        d->xmin = x0; d->xmax = x1; d->ymin = y0; d->ymax = y1;
                        d->zoom = (d->zoom)-1; r = 2; strcat(d->seq, "O"); break;
                    case SDLK_i:
                        printf("Iterations ? ");
                        if (fgets(s, sizeof(s), stdin) != NULL) {d->iter = atoi(s);}
                        r = 3; break;
                    case SDLK_UP:
                        d->iter += 1;
                        r = 3; break;
                    case SDLK_DOWN:
                        if ((d->iter) > 1) {d->iter -= 1;}
                        r = 3; break;
                    case SDLK_m:
                        sprintf(filename, "%s_%d.bmp", d->seq, d->iter);
                        if (SDL_SaveBMP(surface, filename) == 0) {
                            printf("File '%s' saved\n", filename);
                        } else {
                            printf("Can't save '%s'\n", filename);
                        }
                        r = -1; break;
                    case SDLK_ESCAPE:
                        r = 0; break;
                    default:
                        r = -1; break;
                }
                if (r == 1) {
                    drawSquare(rectx, recty, rectL);
                    refresh();
                }
                break;
            case SDL_QUIT:
                r = 0; break;
            default:
                r = -1; break;
        }
    } while (r < 0);
    
    if ((r > 0) && (r < 3))  printf("Zoom = %d\t%.50Le , %.50Le\n", d->zoom, x, y);
    
    return r;
}


/*---------------------------------------------------------------------------*/
/**
 * \fn    void stopSDL(t_data * const d)
 * \brief Stop SDL
 * \param d Data object
 */
void stopSDL(t_data * const d)
{
    printf("Sequence = %s\n", d->seq);
    SDL_FreeSurface(surface);
    SDL_Quit();
}


/*---------------------------------------------------------------------------*/

