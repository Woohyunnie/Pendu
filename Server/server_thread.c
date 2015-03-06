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

typedef struct _data data;
struct _data{
	int socket;
	char mot[256];
	char reponse[256];
    int vie;
};

typedef int boolean;
#define true 1
#define false 0

/*------------------------------------------------------*/
void* renvoi (void* d)
{
    char buffer[256] = "";
    struct _data* dd = (struct _data*)d;
    char *rep;//= (char *)malloc(sizeof(char)*257);
    int longueur;
	boolean fin;

    int i;
    
	/* Initilisation du jeu */
	printf("init >%s<(%d)\n",dd->mot,(int)strlen(dd->mot));
	for (i=0; i<strlen(dd->mot); i++)
    {
		dd->reponse[i] = '_';
	}
	dd->reponse[i] = '\0';
   	write(dd->socket,dd->reponse,strlen(dd->mot));
   
   	read(dd->socket, rep, 2);
    while(1)
    {
		//test si mot fini
		fin = true;
		for(i=0; i<strlen(dd->mot); i++)
		{
			if(dd->reponse[i] != dd->mot[i])
			{
				fin = false;
				break;
			}	
		}
		if (fin)
		{
			rep = "Mot trouvé : fin de partie\n";
			write(dd->socket, rep, strlen(rep));
			return NULL;
		}
		else
		{
			rep = "\n";
			write(dd->socket, rep, strlen(rep));
		}
		

		//lecture de l'envoi du client
		while((longueur = read(dd->socket, buffer, sizeof(buffer))) != 1)
			{}
	   buffer[longueur] = '\0';
	   printf("message lu : >%s< %d \n", buffer, longueur);
	   
		//si client envoit plusieurs lettres
		/*if (longueur > 1)
		{
		    printf("le client a envoyé trop de lettre \n");
		    rep = "vous avez fourni trop de lettres";
		    write(dd->socket,rep,strlen(rep));
		    return NULL;
		}*/
		printf("mot = >%s<(%d)<\n",dd->mot,(int)strlen(dd->mot));
		for (i=0; i<strlen(dd->mot); i++)
		{
			if (buffer[0] == dd->mot[i])
			{
				//nouvelle découverte de lettre
				if (buffer[0] != dd->reponse[i])
				{
					printf("lettre trouvée\n");	
					dd->reponse[i] = buffer[0];
				}
				//lettre déjà trouvée
				else
				{
					printf("lettre déjà trouvée\n");	
				}
			}
			else
			{
			//	dd->reponse[i] = '_';
				dd->vie--;
			}
			//printf("%c\n",dd->reponse[i]);
		}
		buffer[0] = '\0';
		buffer[1] = '\0';
		printf("renvoi du message traite.\n");
		
		sleep(1);
		
		//envoyer a client
		printf(">>>>%s\n",dd->reponse);
		write(dd->socket,dd->reponse,strlen(dd->mot));
		read(dd->socket, rep, 2);
		//char vieT[20]="";
		//sprintf(vieT,"%d",dd->vie);
		//write(dd->socket,vieT,sizeof(vieT));
    }
    return NULL;
    
}


/*------------------------------------------------------*/
int main (int argc, char **argv)
{
    int socket_descriptor, 		/* descripteur de socket */
    	nouv_socket_descriptor, 	/* [nouveau] descripteur de socket */
    	longueur_adresse_courante; 	/* longueur d'adresse courante d'un client */
    	sockaddr_in 
    	adresse_locale, 		/* structure d'adresse locale*/
    	adresse_client_courant; 	/* adresse client courant */
    hostent* ptr_hote; 			/* les infos recuperees sur la machine hote */
    servent* ptr_service; 			/* les infos recuperees sur le service de la machine */
    char machine[TAILLE_MAX_NOM+1]; 	/* nom de la machine locale */
    
    gethostname(machine,TAILLE_MAX_NOM);		/* recuperation du nom de la machine */
    
    
    static char *listMot[4];
    //cree mot
    listMot[0]="jesappellegroot";
    listMot[1]="orchidee";
    listMot[2]="anticonstitutionnellement";
    listMot[3]="trucmachinchose";
    
    srand((unsigned)time(NULL));
    struct _data threadData;
    char* tmp = listMot[rand()%4];
    printf("mot >%s<",tmp);
    strcpy(threadData.mot,tmp);
    threadData.vie = 5;
    
    
    //cree la reponse
    int i;
    for (i=0; i<strlen(threadData.mot); i++)
    {
        threadData.reponse[i]='_';
    }

	/* Test du nombre d'argument */
	if (argc != 2) {
    	perror("usage : client <numero-port>");
		exit(1);
	}

	/* recuperation de la structure d'adresse en utilisant le nom */
    if ((ptr_hote = gethostbyname(machine)) == NULL) {
		perror("erreur : impossible de trouver le serveur a partir de son nom.");
		exit(1);
    }

	/* copie de ptr_hote vers adresse_locale */
    bcopy((char*)ptr_hote->h_addr, (char*)&adresse_locale.sin_addr, ptr_hote->h_length);
    adresse_locale.sin_family		= ptr_hote->h_addrtype; 
    adresse_locale.sin_addr.s_addr	= INADDR_ANY; 
    
    /* utilisation d'un nouveau numero de port */
    adresse_locale.sin_port = htons(atoi(argv[1]));//htons(5000);


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
        
        pthread_t nouveauThread;
        threadData.socket=nouv_socket_descriptor;
        if(pthread_create(&nouveauThread, NULL, renvoi, (void* )&threadData)){
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

