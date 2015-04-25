// -*- coding: utf-8 -*-

/**
 * \file  fractal.c
 * \brief Mandelbrot - math functions
 */


#include "types.h"
#include "affichage.h"
#include "fractal.h"


/*---------------------------------------------------------------------------*/
/**
 * \fn     unsigned int mandelbrot_pt(const long double a, const long double b, const unsigned int n)
 * \brief  Compute escape time for given point
 * \param  a Coord x
 * \param  b Coord y
 * \param  n Max iteration
 * \return Iteration escape number
 */
static inline unsigned int mandelbrot_pt(const long double a, const long double b, const unsigned int n)
{
    unsigned int i;
    long double x1 = a;
    long double y1 = b;
    long double x2, y2;
    
    for (i=1; i<n; i++) {
        #ifdef JULIA
        x2 = x1*x1 - y1*y1 + Jx;   y2 = 2.0*x1*y1 + Jy;
        x1 = x2*x2 - y2*y2 + Jx;   y1 = 2.0*x2*y2 + Jy;
        #else
        x2 = x1*x1 - y1*y1 + a;   y2 = 2.0*x1*y1 + b;
        x1 = x2*x2 - y2*y2 + a;   y1 = 2.0*x2*y2 + b;
        #endif
        if (x1*x1 + y1*y1 > 4.0) {
            break;
        }
    }
    
    return i;
}


/*---------------------------------------------------------------------------*/
/**
 * \fn    void mandelbrot_img(t_data * const d)
 * \brief Draw the whole Mandelbrot Set
 * \param d Data object
 */
void mandelbrot_img(t_data * const d)
{
    unsigned int i, j, k;
    const unsigned int n = d->DIM;
    long double x;
    long double y[n];
    
    for (j=0; j<n; j++) {
        y[j] = (d->ymin) + (j*((d->ymax)-(d->ymin))) / (n-1);
    }
    
    #pragma omp parallel for private(j,k,x) schedule(dynamic)
    for (i=0; i<n; i++) {
        x = (d->xmin) + (i*((d->xmax)-(d->xmin))) / (n-1);
        for (j=0; j<n; j++) {
            k = mandelbrot_pt(x, y[j], d->iter);
            if (k == (d->iter)) {
                setPixel(i, j, COLOR_BLACK);
            } else {
                setPixel(i, j, COLORS_SCALE[((d->iter)-k) % 48]);
            }
        }
    }
}


/*---------------------------------------------------------------------------*/

