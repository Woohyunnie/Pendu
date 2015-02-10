/*----------------------------------------------
Serveur à lancer avant le client
------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
//#include <linux/types.h> 	/* pour les sockets */
#include <sys/socket.h>
#include <netdb.h>          /* pour hostent, servent */
#include <string.h> 		/* pour bcopy, ... */  
#include <pthread.h>
#include <unistd.h>//pour mac
#include <time.h>
#define TAILLE_MAX_NOM 256
#define TIME_SLEEP 15

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct hostent hostent;
typedef struct servent servent;

/*------------------------------------------------------*/
void renvoi (int sock, char mot[], char reponse[], int vie)
{

    char buffer[256];
    char *rep;//= (char *)malloc(sizeof(char)*257);
    int longueur;
    int trouver=0;
    int i;
   
    //si client n'a rien envoyer
    if ((longueur = read(sock, buffer, sizeof(buffer))) <= 0)
    	return;
   
    //si client a envoyer plusieur lettre
    if (longueur > 1)
    {
        printf("client n'a pas envoyer trop de lettre");
        //rep="lettre deja trouve";
        strcpy(rep,"lettre deja trouve");
        write(sock,rep,strlen(rep)+1);
        return;
    }
    
    for (i=0; i<strlen(mot); i++)
    {
        //si lettre existe dans le mot
        if (buffer[0] == mot[i])
        {
            trouver = 1;
            //une lettre deja demander
            if(buffer[0]==reponse[i])
            {
                printf("lettre deja trouver");
                rep="lettre deja trouve";
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
    
    strcpy(rep, " mot: ");
    for(i=0;i<strlen(reponse);i++)
        rep = rep + reponse[i];

    if (trouver==0)
    {
        vie--;
        printf("pas trouve le bon lettre");
    }
    
    strcpy(rep," rest de vie: ");
    rep = rep + vie ;
    
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
int main (int argc, char **argv)
{
    int 		socket_descriptor, 		/* descripteur de socket */
    nouv_socket_descriptor, 	/* [nouveau] descripteur de socket */
    longueur_adresse_courante; 	/* longueur d'adresse courante d'un client */
    sockaddr_in 	adresse_locale, 		/* structure d'adresse locale*/
    adresse_client_courant; 	/* adresse client courant */
    hostent*		ptr_hote; 			/* les infos recuperees sur la machine hote */
    servent*		ptr_service; 			/* les infos recuperees sur le service de la machine */
    char 		machine[TAILLE_MAX_NOM+1]; 	/* nom de la machine locale */
    
    gethostname(machine,TAILLE_MAX_NOM);		/* recuperation du nom de la machine */
    
    
    static char *listMot[3];
    char mot[256];
    char reponse[256];
    int vie=5;
    int i;
    
    //cree le mot
    listMot[0]="aabb";
    listMot[1]="abba";
    listMot[2]="abab";
    
    srand((unsigned)time(NULL));
    strcpy(mot,listMot[rand()%2]);
    
    //cree la reponse
    for (i=0; i<strlen(mot); i++)
    {
        reponse[i]='_';
    }
    
    /* creation de la socket */
    if ((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("erreur : impossible de creer la socket de connexion avec le client.");
        exit(1);
    }
    
    /* association du socket socket_descriptor à la structure d'adresse adresse_locale */
    if ((bind(socket_descriptor, (sockaddr*)(&adresse_locale), sizeof(adresse_locale))) < 0) {
        perror("erreur : impossible de lier la socket a l'adresse de connexion.");
        exit(1);
    }
    
    /* initialisation de la file d'ecoute */
    listen(socket_descriptor,5);
    
    /* attente des connexions et traitement des donnees recues */
    for(;;) {
        
        longueur_adresse_courante = sizeof(adresse_client_courant);
        
        /* adresse_client_courant sera renseigné par accept via les infos du connect */
        if ((nouv_socket_descriptor =
             accept(socket_descriptor,
                    (sockaddr*)(&adresse_client_courant),
                    &longueur_adresse_courante))
            < 0) {
            perror("erreur : impossible d'accepter la connexion avec le client.");
            exit(1);
        }
        
        pthread_t nouveauClient;
        
        if(pthread_create(&nouveauClient, NULL, traitement, (int*)&nouv_socket_descriptor)){
            perror(">> Erreur lors de la creation du thread");
            return(1);
        }
        
    }
    close(socket_descriptor);
    
    
    
    
    //envoyer le mot au client
   

    
    //contacter avec un client
    
        //envoyer le mot trouver et la vie. ce ip_trouver = 0
    
    
    return 0;
}

