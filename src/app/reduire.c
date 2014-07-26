#include "image.h"
#include "histogram.h"
#include "quantification.h"

int main(int argc, char **argv)
{

   int K = atoi(argv[1]);
   char *fichier = argv[2];

   histo histogram;

   int *tab=(int *)malloc(3*K*sizeof(int));

   image input = FAIRE_image();
   image output = FAIRE_image();

   /* Chargement de l'image d'entree */
   image_charger(input,fichier);

   /* Creation de l'histogramme */
   histogram = create_histo();
   init_histo(histogram,input);

   /* Quantification */
   quantification(histogram,tab,K);
   
   /* Mapping */
   mapping(input,output,tab,K);

   /* Enregistrement de l'image */
   image_sauvegarder(output,"resultat.ppm");

   /* Liberation memoire */
   DEFAIRE_image(input);
   DEFAIRE_image(output);

   /* Suppression de l'histogramme */
   delete_histo(histogram);
   /* Suppression du tableau des couleurs de plus haute frequence */
   free(tab);

   return 0;
}
