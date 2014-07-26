#ifndef __LISTES_H
#define __LISTES_H

#include <stdio.h>
#include <stdlib.h>

/* Structures */
typedef struct cell * cell;
struct cell{
   int *color;
   int freq;
   cell next;
};

static cell create_cell(int *color, int freq, cell next);
static cell insert_cell(cell head, int *color,int freq);
static void delete_list(cell list);

#endif
