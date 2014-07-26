#ifndef __HISTOGRAM_C
#define __HISTOGRAM_C

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "image.h"
#include "histogram.h"


/* Structures privées */
struct cell
{
  unsigned char B;
  int freq;
  cell next;
};

struct histo_iter
{
  int R,G;
  cell current;
};

/* Fonctions */
static cell create_cell(int B, cell next)
{
  /* Allocation memoire d'une nouvelle cellule */
  cell new_cell=(cell)malloc(sizeof(struct cell));
  
  /* Initialisation de la cellule */
  new_cell->B=(unsigned char)B;
  new_cell->freq=1;
  new_cell->next=next;

  return new_cell;
}

static cell insert_cell(cell head, int B)
{
  if(!head)
    {
      cell new_cell = create_cell(B,NULL);
      return new_cell;
    }
  else
    {
      /* Creation des pointeurs de parcours */
      cell current_cell = head;
      cell previous_cell = current_cell;

      /* Positionnement */  
      while(current_cell && current_cell->B < B)
	{
	  previous_cell=current_cell;
	  current_cell=current_cell->next;
	}
   
      if(current_cell)
	{
	  /* Application des modifications */
	  if(current_cell->B == B)
	    {
	      /* Incrementation de la frequence de la cellule courante. */
	      current_cell->freq++;
	      return head;
	    }
	  else
	    {
	      if(current_cell == head)
		{
		  /* Insertion en tete de liste */
		  cell new_cell = create_cell(B,head);
		  return new_cell;
		}
	      else
		{
		  /* Insertion entre deux cellules */
		  cell new_cell = create_cell(B,current_cell);
		  previous_cell->next=new_cell;
		  return head;
		}
	    }
	}
      else
	{
	  /* Insertion en fin de liste */
	  cell new_cell = create_cell(B,NULL);
	  previous_cell->next=new_cell;
	  return head;
	}
    }
}

static void delete_list(cell list)
{
  cell current_cell = list;
  cell to_delete = current_cell;

  while(current_cell)
    {
      to_delete=current_cell;
      current_cell=current_cell->next;
      free(to_delete);
    }
}

histo create_histo()
{
  int i;
  histo new_histo = (histo)malloc(256*sizeof(cell *));

  for(i=0;i<256;i++)
    {
      new_histo[i] = (cell *)calloc(256,sizeof(cell));
    }

  return new_histo;
}

void init_histo(histo h, image img)
{

  int *c;
  image_debut(img);

  do
    {
      /* Lecture du pixel courant */
      c = image_lire_pixel(img);

      /* Insertion de la cellule correspondante dans l'histogramme */
      h[c[0]][c[1]] = insert_cell(h[c[0]][c[1]],c[2]);
	 
    } while(image_pixel_suivant(img));
}

void delete_histo(histo h)
{
  int i,j;
  for(i=0;i<256;i++)
    {
      for(j=0;j<256;j++)
	{
	  delete_list(h[i][j]);
	}
      free(h[i]);
    }
  free(h);
}

int give_freq_histo(histo h, int R, int G, int B)
{
  cell current_cell = h[R][G];

  while(current_cell && current_cell->B != B)
    {
      current_cell=current_cell->next;
    }

  return (current_cell)?current_cell->freq:0;
}


histo_iter create_histo_iter(histo h)
{
  int i,j;
   
  histo_iter iter = (histo_iter)malloc(sizeof(struct histo_iter));

  for(i=0;i<256;i++)
    {
      for(j=0;j<256;j++)
	{
	  if(h[i][j])
	    {
	      iter->R = i;
	      iter->G = j;
	      iter->current=h[i][j];
	      return iter;
	    }
	}
    }

  /* Si toutes les cellules sont nulles */
  assert(false);

  return iter;
}

histo_iter start_histo_iter(histo_iter iter,histo h)
{
  int i,j;

  for(i=0;i<256;i++)
    {
      for(j=0;j<256;j++)
	{
	  if(h[i][j])
	    {
	      iter->R = i;
	      iter->G = j;
	      iter->current=h[i][j];
	      return iter;
	    }
	}
    }

  return iter;
}

boolean next_histo_iter(histo_iter iter, histo h)
{
  int i,j;

  i=iter->R;
  j=iter->G;

  if(iter->current->next)
    {
      iter->current = iter->current->next;
      return true;
    }
  else
    {
      do
	{
	  /* Calcul de la case suivante */
	  j = (1+j)%256;
	  i = j?i:i+1;
	       
	  if(i > 255)
	    {
	      return false;
	    }

	} while(!h[i][j]);

	 
      /* Maj de l'iterateur */
      iter->current=h[i][j];
      iter->R = i;
      iter->G=j;
      return true;
    }
}

void give_color_histo_iter(histo_iter iter, int *c)
{
  c[0]=iter->R;
  c[1]=iter->G;
  c[2]=iter->current->B;
}

void delete_histo_iter(histo_iter iter)
{
  free(iter);
}

#endif

