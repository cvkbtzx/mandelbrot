// -*- coding: utf-8 -*-

#ifndef __TYPES_H_
#define __TYPES_H_


/**
 * \struct t_data
 * \brief  Contains data about Mandelbrot Set state
 */
typedef struct {
    long double xmin;           /**< Min x coord */
    long double xmax;           /**< Max x coord */
    long double ymin;           /**< Min y coord */
    long double ymax;           /**< Max y coord */
    unsigned int iter;          /**< Max iteration */
    int zoom;                   /**< Current zoom factor */
    const unsigned int DIM;     /**< Screen size in pixel */
    char seq[255];              /**< Keyboard sequence to get current image */
} t_data;


#endif

