#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
	int valeur;
	struct Node* fg;
	struct Node* fd;
}Node;

Node* creer_noeud(int val)
{
	Node* noeud = malloc(sizeof(Node));
	if(noeud == NULL)
	{
		fprintf(stderr, "Erreur malloc nouveau noeud\n");
		exit(EXIT_FAILURE);
	}
	
	noeud->valeur = val;
	noeud->fg = NULL;
	noeud->fd = NULL;
	
	return noeud;	
}

void ajouter(Node** racine, int val)
{
	Node* noeud = creer_noeud(val);
	Node* prec = NULL;
	Node* actuel = *racine;
	
	if(*racine == NULL) //si l'arbre est vide
	{
		*racine = noeud;
		return;
	}
	
	do
	{
		prec = actuel;
		
		//si la valeur du noeud est plus petite que la valeur du noeud actuel alors on se déplace à gauche
		if(actuel->valeur > val) 
		{
			actuel = actuel->fg;
			if(actuel == NULL)
				prec->fg = noeud;
		}
		//sinon on se déplace à droite
		else	
		{
			actuel = actuel->fd;
			if(actuel == NULL)
				prec->fd = noeud;
		}
	}while(actuel != NULL);
}

int rechercher(Node* racine, int val)
{
	while(racine != NULL)
	{
		if(racine->valeur == val)
			return 1;
		if(racine->valeur < val)
			racine = racine->fd;
		else
			racine = racine->fg;
	}
	
	return 0;
}

void affichage(Node* arbre)
{
	/*affichage trié*/
	/*if(arbre == NULL) 
		return;
	if(arbre->fg != NULL) 
		affichage(arbre->fg);
	printf("%d\n", arbre->valeur);
	if(arbre->fd != NULL)
		affichage(arbre->fd);*/
	if(arbre != NULL)
	{
		printf("%d\n", arbre->valeur);
		affichage(arbre->fg);
		affichage(arbre->fd);
	}
}

void supprimer(Node** racine, int val)
{
	Node* actuel = *racine;
	Node* prec = NULL;
	Node* remplacant = NULL; //seulement utile pour le cas des 2 fils
	int left;
	
	if(!rechercher(*racine, val)) //si le noeud qu'on veut supprimer n'existe pas
		return;
	
	while(actuel->valeur != val)
	{
		prec = actuel;
		if(actuel->valeur < val)
		{
			left = 0;
			actuel = actuel->fd;
		}
		else if(actuel->valeur > val)
		{
			left = 1;
			actuel = actuel->fg;
		}
	}
	
	/*3 cas pour le noeud que l'on veut supprimer :
		- n'a pas de fils (c'est une feuille)
		- a 1 fils (2 cas : soit fils gauche, soit fils droit)
		- a 2 fils (on remplace alors sa valeur par la plus petite valeur parmi les plus grandes valeurs,
		  c'est à dire que l'on va 1 fois à droite puis tout le temps à gauche)
	*/
	if(actuel->fg == NULL && actuel->fd == NULL) //pas de fils
	{
		if(left) //le noeud à supprimer est le fils gauche de prec
			prec->fg = NULL; //annule le lien qui unit le père à son fils (rt si t triste)
		else //le noeud à supprimer est le fils droit de prec
			prec->fd = NULL;
		free(actuel);
		actuel = NULL;
		printf("pas de fils\n");
	}
	else if(actuel->fg != NULL && actuel->fd == NULL) //1 fg, pas de fd
	{
		if(left)
			prec->fg = actuel->fg;
		else
			prec->fd = actuel->fg;
		actuel->fg = NULL;
		free(actuel);
		actuel = NULL;
		printf("fg\n");
	}
	else if(actuel->fg == NULL && actuel->fd != NULL)//1 fd, pas de fg
	{
		if(left)
			prec->fg = actuel->fd;
		else
			prec->fd = actuel->fd;
		actuel->fd = NULL;
		free(actuel);
		actuel = NULL;
		printf("fd\n");
	}
	else //2 fils
	{
		remplacant = actuel->fd;
		
		if(remplacant->fg != NULL) //si le fils droit a un fils gauche
		{
			while(remplacant->fg != NULL)
			{
				prec = remplacant;
				remplacant = remplacant->fg;
			}
			prec->fg = remplacant->fd;
		}
		else //sinon c'est le fils droit qui est le plus petit des plus grands
			actuel->fd = remplacant->fd;
		
		actuel->valeur = remplacant->valeur;
		remplacant->fd = NULL;
		free(remplacant);
		remplacant = NULL;
		printf("2 fils\n");
	}
}

void detruire_arbre(Node** arbre)
{
    Node* tmp = *arbre;

    if(arbre==NULL)
		return;
    if(tmp->fg != NULL)
		detruire_arbre(&tmp->fg);
    if(tmp->fd != NULL)
        detruire_arbre(&tmp->fd);
    free(tmp);

    *arbre = NULL;
}

int main(int argc, int** argv)
{
	Node* arbre = NULL;
	ajouter(&arbre, 10);
	ajouter(&arbre, 2);
	ajouter(&arbre, 20);
	ajouter(&arbre, 21);
	
	affichage(arbre);
	printf("\n");
	supprimer(&arbre, 20);
	affichage(arbre);
	
	detruire_arbre(&arbre);
	return 0;
}
