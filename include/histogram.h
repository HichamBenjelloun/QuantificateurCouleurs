/* -*- c-basic-offset: 3 -*- 
 *
 * ENSICAEN
 * 6 Boulevard Marechal Juin 
 * F-14050 Caen Cedex 
 * 
 * This file is owned by ENSICAEN students.
 * No portion of this document may be reproduced, copied
 * or revised without written permission of the authors.
 */ 

/**
 * @author Hicham Benjelloun <benjelloun@ecole.ensicaen.fr> 
 * @version     1.0 - Jan 01, 2013
 * 
 * @todo
 * @bug
 */

#ifndef __HISTOGRAM_H
#define __HISTOGRAM_H


/**
 * @file histogram.h
 *  
 *
 *
 */

#include <stdio.h>

#include "image.h"

#define RED 0
#define GREEN 1
#define BLUE 2

/* Structures */

typedef struct histo_iter * histo_iter;
typedef struct cell * cell;
typedef cell ** histo;
typedef enum {false=0,true=1} boolean;

histo create_histo();
void init_histo(histo h, image img);
void delete_histo(histo h);
int give_freq_histo(histo h, int R, int G, int B);
histo_iter create_histo_iter(histo h);
histo_iter start_histo_iter(histo_iter iter,histo h);
boolean next_histo_iter(histo_iter iter,histo h);
void give_color_histo_iter(histo_iter iter, int *c);
void delete_histo_iter(histo_iter iter);

#endif
