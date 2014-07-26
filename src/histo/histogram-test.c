#ifndef __HISTOGRAM_C
#define __HISTOGRAM_C

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "image.h"
#include "histogram.h"


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

/* FONCTIONS LISTES */

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


/* FONCTIONS HISTOGRAMME */

histo create_histo()
{
   int i,j;
   histo new_histo = (histo)calloc(256,sizeof(cell *));

      for(i=0;i<256;i++)
	 {
	    new_histo[i] = (cell *)calloc(256,sizeof(cell));
	    for(j=0;j<256;j++)
	       {
		  /* Initialisation de chaque cellule du tableau */
		  new_histo[i][j] = NULL;
	       }
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

   if(current_cell)
      {
	 return current_cell->freq;
      }
   else
      {
	 return 0;
      }
}


histo_iter create_histo_iter(histo h)
{
   int i,j;
   
   histo_iter iter = (histo_iter)calloc(1,sizeof(struct histo_iter));

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

   /* Si toutes les cellules sont nulles */
   assert(1);

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
	       j = (1+j)%256;
	       i = j?i:i+1;
	       
	       if(i > 255 || j > 255)
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

int print_list(int R,int G,cell list)
{
   int k=0;
   cell current_cell = list;

   while(current_cell)
      {
	 printf("%d %d %d\n",R,G,current_cell->B);
	 current_cell=current_cell->next;
	 k++;
      }
   return k;
}


int main(int argc, char **argv)
{
   int i,j,k;
   k=0;

   /* Chargement de l'image */
   image img = FAIRE_image();
   image_charger(img,argv[1]);

   /* Insertion dans les listes */

   /* Allocation memoire de l'histogramme */
   puts("Allocation memoire de l'histogramme...");
   histo h = create_histo();
   puts("Succès !");

   /* Initialisation de l'hitogramme */
   puts("Initisalisation de l'histogramme...");
   init_histo(h,img);
   puts("Succès !");

   /* Affichage de l'histogramme et du nombre de couleurs */
   int m = 10;
   
   //puts("Affichage de l'histogramme");
   /*
   for(i=0;i<256;i++)
      {
	 for(j=0;j<256;j++)
	    {
	       if(h[i][j]){
		  k+=print_list(i,j,h[i][j]);
	       }	  
	    }
      }
      
   printf("Card{Couples(R,G)} : %d\n",k);
   
   */
   
   histo_iter iter = create_histo_iter(h);

   k=0;
   m=10;
do
   {
      printf("%d %d %d\n",iter->R,iter->G,iter->current->B);
   } while(next_histo_iter(iter,h));
   
// printf("Card{Triplets(R,G,B)} : %d\n",k);
   

   return 0;
}
   

#endif

