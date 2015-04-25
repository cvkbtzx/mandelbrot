// -*- coding: utf-8 -*-

/**
 * \file    main.c
 * \brief   Mandelbrot - main function
 */


#include <stdlib.h>   /* atof */
#include "types.h"
#include "affichage.h"
#include "fractal.h"


#ifdef JULIA
long double Jx, Jy;
#endif


int main(int argc, char **argv)
{
    int k = 1;
    t_data d = {
        .xmin = -1.5,
        .xmax = 0.5,
        .ymin = -1.0,
        .ymax = 1.0,
        .iter = 753,
        .zoom = 0,
        .DIM  = 801
    };
    
    #ifdef JULIA
    if (argc == 3) {
        Jx = atof(argv[1]); Jy = atof(argv[2]);
    }
    d.xmin = -2.0; d.xmax = 2.0;
    d.ymin = -2.0; d.ymax = 2.0;
    #endif
    
    initSDL(&d);
    while (k > 0) {
        mandelbrot_img(&d);
        refresh();
        k = waitKeyPressed(&d);
    }
    stopSDL(&d);
    
    return 0;
}

