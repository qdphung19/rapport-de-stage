#include <stdio.h>
#include <stdlib.h>
#include <math.h>

class DISQUE //un élément de la pile
{	private:
	int _valeur;
	DISQUE* _suivant;
	~DISQUE(){}
	DISQUE(int n=0){ _valeur=n; _suivant= 0;}

	public:
	static void Empiler (DISQUE ** tete, int n);
	static void Depiler (DISQUE ** tete, int * n);
	static void Depiler (DISQUE ** tete); //dépiler tout
	//
	int GetValeur() { return(_valeur); }
};

void DISQUE::Empiler (DISQUE ** tete, int n)
{	DISQUE * nouveau = new DISQUE(n);
	nouveau->_suivant=*tete;
    *tete=nouveau;
}

void DISQUE::Depiler (DISQUE ** tete, int * n)
{   if (!*tete) {*n = 0; return; } //erreur
	DISQUE * adetruire = *tete;
	*n = adetruire->_valeur;
	*tete=adetruire->_suivant;
	delete(adetruire);
}

void DISQUE::Depiler (DISQUE ** tete)
{	int n;
	while (*tete) Depiler(tete,&n);
}

//Cette fonction résoud une formule polonaise, en cas d'erreur elle renvoie 0
int Polonaise (char* formule, int longueur)
{	DISQUE * pile = 0; int i, m, n; char caractere;
	for (i=0;i<longueur;i++)
	{	caractere=formule[i];
		if (caractere=='1') DISQUE::Empiler(&pile,1);
		else
		{	DISQUE::Depiler(&pile,&m); DISQUE::Depiler(&pile,&n);
			if(!m||!n) return(0); //erreur
			if (caractere=='+') DISQUE::Empiler(&pile,m+n);
			if (caractere=='*') DISQUE::Empiler(&pile,m*n);
		}
	}
	return((*pile).GetValeur());
	DISQUE::Depiler(&pile);
}

//La classe liste pour les résultats
class LISTE
{	private:
	int * _liste;
	int _nbtotal;
	int _nbpresent;

	public:
	LISTE(int n=1000)
	{	_nbtotal=n;
		_nbpresent=0;
		_liste = new int[_nbtotal];
	}

	~LISTE(){delete[]_liste;}

	bool DansListe (int n)
	{	for (int i=0;i<_nbpresent;i++)
			if (n==_liste[i]) return(true);
		return(false);
	}

	void Ajout (int n)
	{	if (_nbpresent >= _nbtotal)
		{	int * nouvelle;
		    _nbtotal+=1000;
		    nouvelle = new int[_nbtotal];
			for(int i=0;i<_nbpresent;i++)	nouvelle[i]=_liste[i];
			delete[]_liste;
			_liste=nouvelle;
		}
        int i=0,j;
		while((_liste[i]<n)&&(i<_nbpresent)) i++;
		for (j=0;j<_nbpresent-i;j++)
            _liste[_nbpresent-j]=_liste[_nbpresent-j-1];
		_liste[i]=n;
		_nbpresent++;
	}

	void AfficherListe (FILE * resultats)
	{	for(int i=0;i<_nbpresent;i++)
			fprintf(resultats,"%d, ",_liste[i]);
        fprintf(resultats,"\n");
	}
};

long long boucles = 0;//elle compte le nombre de boucles que fait l'algorithme

//Cette fonction place les opérateurs de différentes manières dans la formule et retient les nouveaux nombres formés ainsi
void Combinaisons(int i,int n,char * formule,char * operateurs,char * positions,LISTE * liste)
{	int j, min, resultat;boucles++;
	if (i==n-2)
	{	for (j=0;j<2*n-2;j++) formule[j]='1';
        formule[n*2-2]=operateurs[n-2];
		for (j=0;j<n-2;j++) formule[positions[j]+2]=operateurs[j];
		resultat=Polonaise(formule,2*n-1);
		if (resultat && !liste->DansListe(resultat)) liste->Ajout(resultat);

	}
	else
	{	if (i==0) min=0; else min=positions[i-1]+1;
		for (positions[i]=min;positions[i]<2*n-4;positions[i]++)
			    Combinaisons(i+1,n,formule,operateurs,positions,liste);//Chaque passage par la fonction donne la position d'un opérateur
	}
}

int main()
{	int n, i, j, compte, temps=0;
	printf("Entrez le nombre de 1 : ");
	scanf("%d", &n);
	char * formule = new char[2*n-1];
	char * operateurs = new char[n-1];
	char * positions = new char[n-2];//la position des opérateurs dans la formule
	LISTE * liste = new LISTE(1000);
	for(compte=0;compte<pow(2,n-1);compte++) //parcours de toutes les combinaisons d'opérateurs possibles
	{	i=(int(pow(2,n-1))-compte)*10/int(pow(2,n-1));
        if(i != temps) { temps=i; printf("%d\n",temps);}
        for(i=0;i<n-1;i++)
		{	j=compte/pow(2,i);
			if (j%2==0) operateurs[i]='+';
			else operateurs[i]='*';
		}
		Combinaisons(0,n,formule,operateurs,positions,liste);
	}
	delete[]formule;
	delete[]operateurs;
	delete[]positions;
	FILE *resultats = fopen("resultats.txt", "w");
	fprintf(resultats,"Voici la liste des resultats possibles avec %d uns :\n",n);
	liste->AfficherListe(resultats);
	delete liste;
	fprintf(resultats,"Nombre de boucles : %lld\n",boucles);
    fclose(resultats);
	return(0);
}