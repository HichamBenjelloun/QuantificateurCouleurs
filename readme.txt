
		Fondements de l'Informatique - Projet

---------------------------------------------------------------------

Hicham Benjelloun 					le 16/01/2013

Encadrant de TP : Luc Brun
---------------------------------------------------------------------



Voir le fichier ./doc/rapport.pdf pour plus de détails.

I - Arborescence du répertoire de travail

.
├── bin
├── doc
├── images
│   ├── arbre.ppm
│   └── fleurs.ppm
├── include
│   ├── classe.h
│   ├── exit_if.h
│   ├── histogram.h
│   ├── image.h
│   ├── move_type.h
│   ├── objet.h
│   ├── point.h
│   ├── quantification.h
│   ├── type_liste_bf.h
│   ├── type_liste_rgbf.h
│   └── type_obj.h
├── lib
│   └── libimage.a
├── makefile
├── obj
├── readme.txt
└── src
    ├── app
    │   ├── ppmhist.c
    │   └── reduire.c
    ├── histo
    │   └── histogram.c
    ├── image
    │   └── image.c
    └── quantification
        └── quantification.c

11 directories, 21 files


II - Compilation du projet

	Pour créer l'application PPMHIST : 

	make ppmhist

	Pour créer l'application de réduction de couleurs REDUIRE : 

	make all

	Pour recréer la librairie statique libimage.a si besoin est : 

	make libimage

III - Application

	PPMHIST : 

	./bin/ppmhist

	REDUIRE :

	./bin/reduire K fichier.ppm

	où K est le nombre de couleurs d'affichage. 

IV - Bugs détéctés

	Aucun bug détecté dans le travail réalisé.

V - À faire

	Questions optionnelles et optimisation du code.



