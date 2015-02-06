/*----------------------------------------------
Serveur à lancer avant le client
------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <linux/types.h> 	/* pour les sockets */
#include <sys/socket.h>
#include <netdb.h>          /* pour hostent, servent */
#include <string.h> 		/* pour bcopy, ... */  
#include <pthread.h>
#define TAILLE_MAX_NOM 256
#define TIME_SLEEP 15

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct hostent hostent;
typedef struct servent servent;

/*------------------------------------------------------*/
void renvoi (int sock, char[] mot, char[] reponse, int vie)
{

    char buffer[256];
    char rep[256];
    int longueur;
    bool trouver=false;
   
    //si client n'a rien envoyer
    if ((longueur = read(sock, buffer, sizeof(buffer))) <= 0) 
    	return;
    
    //si client a envoyer plusieur lettre
    if (longueur > 1)
    {
        printf("client n'a pas envoyer trop de lettre");
        rep='lettre deja trouve'
        write(sock,rep,strlen(rep)+1);
        return;
    }
    
    for (i=0; i<sizeof(mot); i++)
    {
        //si lettre existe dans le mot
        if (buffer[0]=mot[i])
        {
            trouver = true;
            //une lettre deja demander
            if(buffer[0]=reponse[i])
            {
                printf("lettre deja trouver");
                rep='lettre deja trouve';
            }
            //trouver en 1ere fois
            else
            {
                printf("trouve la lettre en 1ere");
                reponse[i]=buffer[0];
                //prendre son ip et compter
                //...
            }
        }
    }
    
    rep +=' '+'mot: '+ reponse;

    if (!trouver)
    {
        vie--;
        printf("pas trouve le bon lettre");
    }
    
    rep +=' '+'reste de vie: '+vie;
    
    printf("message apres traitement : %s \n", rep);
    
    printf("renvoi du message traite.\n");
 
    /* mise en attente du prgramme pour simuler un delai de transmission */
    sleep(TIME_SLEEP);
    
    //envoyer a client
    write(sock,rep,strlen(rep)+1);
    
    printf("message envoye. \n");
        
    return;
    
}

/*------------------------------------------------------*/
main(int argc, char **argv)
{
    char[] mot;
    char[] reponse;
    int vie=5;
    
    //cree le mot

    //cree la reponse
    for (i=0; i<siezof(mot); i++)
    {
        reponse[i]='_';
    }
    
    //contacter avec un client
    
        //envoyer le mot trouver et la vie. ce ip_trouver = 0
    
}

