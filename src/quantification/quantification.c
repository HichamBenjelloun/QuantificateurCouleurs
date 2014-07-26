#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "image.h"
#include "histogram.h"
#include "quantification.h"


struct cell{
  int *color;
  int freq;
  cell next;
};



static cell create_cell(int *color, int freq, cell next)
{
  /* Allocation memoire d'une nouvelle cellule */
  cell new_cell=(cell)malloc(sizeof(struct cell));
  new_cell->color = (int *)malloc(3*sizeof(int));
  
  /* Initialisation de la cellule */
  new_cell->freq=freq;
  new_cell->next=next;

  new_cell->color[0]=color[0];
  new_cell->color[1]=color[1];
  new_cell->color[2]=color[2];

  return new_cell;
}

static cell insert_cell(int m, cell head, int *color,int freq)
{
  cell new_cell;
  cell current_cell;
  cell previous_cell;

  if(!head)
    {
      new_cell = create_cell(color,freq,NULL);
      return new_cell;
    }
  else
    {
      /* Comparaison a FK */
      if(freq > head->freq)
	{
	  /* Positionnement */
	  current_cell = head->next;
	  previous_cell = head;

	  while(current_cell && freq > current_cell->freq)
	    {
	      previous_cell=current_cell;
	      current_cell=current_cell->next;		  
	    }

	  /* Insertion */
	  new_cell = create_cell(color,freq,current_cell);
	  previous_cell->next = new_cell;
	  if(m) return head->next; else return head;
	}
    }

  return head;
}


static void delete_list(cell list)
{
  cell current_cell = list;
  cell to_delete = current_cell;

  while(current_cell)
    {
      to_delete=current_cell;
      current_cell=current_cell->next;
      free(to_delete->color);
      free(to_delete);
    }
}

/* Quantification */
void quantification(histo h,int *tab,int K)
{
  int i;
  histo_iter iter=create_histo_iter(h);

  cell list=(cell)calloc(1,sizeof(struct cell));
  cell current_cell;
  int *current_color = (int *)malloc(3*sizeof(int));

  /* Initialisation de la K-liste de couleurs */
  for(i=0;i<K;i++)
    {
      give_color_histo_iter(iter,current_color);
      list=insert_cell(0,list,current_color,give_freq_histo(h,current_color[0],current_color[1],current_color[2]));
      next_histo_iter(iter,h);
    }

  /* Mise a jour de la K-liste des couleurs les plus frequentes.
   * Par comparaison des frequences des couleurs rencontrees
   * Par rapport a fk
   *
   */
  do
    {
      give_color_histo_iter(iter,current_color);

      if(give_freq_histo(h,current_color[0],current_color[1],current_color[2]) > list->freq)
	{
	  list=insert_cell(1,list,current_color,give_freq_histo(h,current_color[0],current_color[1],current_color[2]));
	}
	 
    } while(next_histo_iter(iter,h));
   
  /* Conversion de la liste en tableau */
  current_cell=list;

  for(i=0;i<3*K-2;i+=3)
    {
      if(current_cell)
	{
	  tab[i] = current_cell->color[0];
	  tab[i+1] = current_cell->color[1];
	  tab[i+2] = current_cell->color[2];
	  current_cell=current_cell->next;
	}

    }

  /* Suppression de la liste */
  delete_list(list);

  /* Suppression de l'itérateur */
  delete_histo_iter(iter);

}

double calcul_distance(int a1,int b1,int c1,int a2,int b2,int c2)
{
  return (abs(a1-a2)+abs(b1-b2)+abs(c1-c2));
}

double calcul_distance2(int a1,int b1,int c1,int a2,int b2,int c2)
{
  return (pow(a1-a2,2)+pow(b1-b2,2)+pow(c1-c2,2));
}

void closest_color(int *tab,int K,int R,int G,int B,int *output)
{
  int i;
  double current_distance = 0.0;
  double min_distance = 196608.0;

  for(i=0;i<3*K-2;i+=3)
    {
      current_distance = calcul_distance2(tab[i],tab[i+1],tab[i+2],R,G,B);

      if(current_distance < min_distance)
	{
	  /* Maj de la distance minimale */
	  min_distance = current_distance;

	  /* Copie de la couleur la plus proche */
	  output[0]=tab[i];
	  output[1]=tab[i+1];
	  output[2]=tab[i+2];
	}
    }
}


void mapping(image input,image output,int *tab,int K)
{
  /* Ancienne et nouvelle couleurs */
  int *color;
  int *updated_color = (int *)malloc(3*sizeof(int));

  /* Initialisation de l'image destination */
  image_initialize(output,image_give_dim(input),image_give_largeur(input),image_give_hauteur(input));

  /* Positionnement au debut de l'image */
  image_debut(input);
  image_debut(output);

  /* Creation de l'image destination */
  do
    {
      color = image_lire_pixel(input);
      closest_color(tab,K,color[0],color[1],color[2],updated_color);
      image_ecrire_pixel(output,updated_color);
	 
    } while(image_pixel_suivant(input) && image_pixel_suivant(output));

  /* Free */
  free(updated_color);

}
