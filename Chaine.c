#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Chaine.h"
#include "SVGwriter.h"

Chaines* lectureChaines(FILE *f){
	Chaines* ch = malloc(sizeof(Chaines));
	ch->chaines = NULL;
	
	int nb_ch,gamma;
	double x,y;
	char buffer[256];
	
	fgets(buffer,256,f); //on stocke le contenu de la ligne dans buffer
	assert(strncmp(buffer,"NbChain: ",9)==0); //on verifie que le model de fichier est comme le modele qu'on a ecrit (il commence par NbChain:  etc.)
	sscanf(buffer,"NbChain: %d",&nb_ch);
	ch->nbChaines=nb_ch;
	
	fgets(buffer,256,f); //on stocke le contenu de la ligne dans buffer
	assert(strncmp(buffer,"Gamma: ",7)==0); //on verifie que le model de fichier est comme le modele qu'on a ecrit (il commence par Gamma:   etc.)
	sscanf(buffer,"Gamma: %d",&gamma);
	ch->gamma=gamma;
	
	fgets(buffer,256,f); //on avance
	int i=0;
	int j;
	int nb_pairs;
	//on fait cette boucle jusqu'on fini la liste de points
	while (i<nb_ch) {
		CellChaine *cell_chaine = malloc(sizeof(CellChaine));
		cell_chaine->numero = i;
		cell_chaine->suiv = NULL;
		CellChaine *tmp = ch->chaines;
		ch->chaines = cell_chaine;
		cell_chaine->suiv = tmp;
		//on scan le contenu du buffer
		sscanf(buffer,"%*d %d",&nb_pairs);
		//printf("%c\n",buffer[4]);
		char subbuff[256-4];
		memcpy(subbuff,&buffer[4],256-4);
		//subbuff[256-4]='\0';
		//printf("%c\n",subbuff[4]);
		j=0;
		int index = 0;
		while(j<nb_pairs){
			sscanf(subbuff+index,"%lf %lf ",&x,&y);
			//printf("%c\n",subbuff[index]);
			CellPoint *point = malloc(sizeof(CellPoint));
			point->x = x;
			point->y =y;
			point->suiv = NULL;
			CellPoint *tmp2 = ch->chaines->points;
			ch->chaines->points = point;
			point->suiv = tmp2;
			
			index += 11;
			if (subbuff[index]==' '){
				index++;
			} else if (subbuff[index-1]!=' '){
				index--;
			}
				
			j++;
			
		}
		i++;
		fgets(buffer,256,f);	
	}
	return ch;
	
}

void ecrireChaines(Chaines *C, FILE *f){
	fprintf(f,"NbChain: %d\n",C->nbChaines);
	fprintf(f,"Gamma: %d\n",C->gamma);
	CellChaine *cell_ch =C->chaines;
	while (cell_ch){
		fprintf(f,"%d ",cell_ch->numero);
		CellPoint *cell_p = cell_ch->points;
		int cpt = 0;
		while (cell_p){
			cpt++;
			cell_p = cell_p->suiv;
		}
		fprintf(f,"%d ",cpt);
		cell_p = cell_ch->points;
		while (cell_p){
			fprintf(f,"%.2f %.2f ",cell_p->x,cell_p->y);
			cell_p = cell_p->suiv;	
		}
		fprintf(f,"\n");
		cell_ch = cell_ch->suiv;
	}
}

void afficheChainesSVG(Chaines *C, char* nomInstance){
    int i;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;
    CellChaine *ccour;
    CellPoint *pcour;
    double precx,precy;
    SVGwriter svg;
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        while (pcour!=NULL){
            if (maxx<pcour->x) maxx=pcour->x;
            if (maxy<pcour->y) maxy=pcour->y;
            if (minx>pcour->x) minx=pcour->x;
            if (miny>pcour->y) miny=pcour->y;  
            pcour=pcour->suiv;
        }
    ccour=ccour->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        SVGlineRandColor(&svg);
        SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny)); 
        precx=pcour->x;
        precy=pcour->y;  
        pcour=pcour->suiv;
        while (pcour!=NULL){
            SVGline(&svg,500*(precx-minx)/(maxx-minx),500*(precy-miny)/(maxy-miny),500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            precx=pcour->x;
            precy=pcour->y;    
            pcour=pcour->suiv;
        }
        ccour=ccour->suiv;
    }
    SVGfinalize(&svg);
}




double longueurChaine(CellChaine *c){
	CellPoint *point = c->points;
	CellPoint *tmp;
	double d = 0;
	while(point->suiv!=NULL){
		tmp = point->suiv;
		d += sqrt((tmp->x-point->x)*(tmp->x-point->x)+(tmp->y-point->y)*(tmp->y-point->y));
	}
	return d;
}

double longueurTotale(Chaines *C){
	double d = 0;
	CellChaine *chaine = C->chaines;
	while(chaine){
		d+= longueurChaine(chaine);
		chaine = chaine->suiv;
	}
	return d;
}

int comptePointsTotal(Chaines *C){
	int cpt = 0;
	CellChaine *chaine = C->chaines;
	while(chaine){
		CellPoint *point = chaine->points;
		while (point) {
			cpt++;
			point = point->suiv;
		}
		chaine = chaine->suiv;
	}
	return cpt;
}

