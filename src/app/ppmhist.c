#include "image.h"
#include "histogram.h"

struct cell
{
   unsigned char B;
   cell next;
};

struct histo_iter
{
int R,G;
cell current;
};

int main(int argc, char **argv) 
{
   image img=FAIRE_image();   
   int i=0;
   /* Param a afficher */

   int R=0,G=0,B=0;
   int lum=0;
   int occ=0;
   
   histo histogram=create_histo();

   histo_iter iter;
   
   /* Chargement de l'image  */
   image_charger(img,argv[1]);
   
   /* Positionnement du pointeur courant au debut de l'image */
   image_debut(img);
   
   init_histo(histogram,img);
   
   /* Creation de l'iterateur */
   iter = create_histo_iter(histogram);

   /* Parcours de l'histogramme */
   do
      {
         R=iter->R;
         G=iter->G;
	 B=iter->current->B;
         lum=(R+G+B)/3;
	 occ=give_freq_histo(histogram,R,G,B);

         printf("(R = %d, G = %d, B = %d, LUM = %d, OCC = %d)\n",R,G,B,lum,occ);
	 i++;
      } while(next_histo_iter(iter,histogram));
   
   printf("\033[32mNombre de couleurs : %d\033[0m\n", i);

   return 0;
}
