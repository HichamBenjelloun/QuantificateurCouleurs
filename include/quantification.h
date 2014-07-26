

#ifndef __QUANTIFICATION_H
#define __QUANTIFICATION_H

#include <stdio.h> 
#include <stdlib.h>
#include "image.h"
#include "histogram.h"

/*
typedef struct cell * cell;

static cell create_cell(int *color, int freq, cell next);

static cell insert_cell(cell head, int *color,int freq);

static void delete_list(cell list);
*/

void quantification(histo h,int *tab,int K);

void closest_color(int *tab,int K,int R,int G,int B,int *output);

void mapping(image input,image output,int *tab,int K);






#endif
