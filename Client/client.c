/*-----------------------------------------------------------
Client a lancer apres le serveur avec la commande :
client <adresse-serveur> <message-a-transmettre>
------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <linux/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

typedef struct sockaddr 	sockaddr;
typedef struct sockaddr_in 	sockaddr_in;
typedef struct hostent 		hostent;
typedef struct servent 		servent;

int main(int argc, char **argv) {
  
    int 	socket_descriptor, 	/* descripteur de socket */
			longueur; 		/* longueur d'un buffer utilisé */
    sockaddr_in adresse_locale; 	/* adresse de socket local */
    hostent *	ptr_host; 		/* info sur une machine hote */
    servent *	ptr_service; 		/* info sur service */
    char 	buffer[256];
    char *	prog; 			/* nom du programme */
    char *	host; 			/* nom de la machine distante */
    char *	mesg;// = ""; 			/* message envoyé */
    
    int i;
    for (i = 0; i<256; i++)
    	buffer[i]='\0';  
    
    
    
    if (argc != 3) {
		perror("usage : client <adresse-serveur> <numero-port>");
		exit(1);
    }
   
    prog = argv[0];
    host = argv[1];
    
    if ((ptr_host = gethostbyname(host)) == NULL) {
		perror("erreur : impossible de trouver le serveur a partir de son adresse.");
		exit(1);
    }
    
    /* copie caractere par caractere des infos de ptr_host vers adresse_locale */
    bcopy((char*)ptr_host->h_addr, (char*)&adresse_locale.sin_addr, ptr_host->h_length);
    adresse_locale.sin_family = AF_INET;

    adresse_locale.sin_port = htons(atoi(argv[2]));//htons(5000);
    
    /* creation de la socket */
    if ((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	perror("erreur : impossible de creer la socket de connexion avec le serveur.");
	exit(1);
    }
    
    /* tentative de connexion au serveur dont les infos sont dans adresse_locale */
    if ((connect(socket_descriptor, (sockaddr*)(&adresse_locale), sizeof(adresse_locale))) < 0) {
	perror("erreur : impossible de se connecter au serveur.");
	exit(1);
    }
    
    printf("connexion etablie avec le serveur. \n");

    if (longueur = read(socket_descriptor, buffer, sizeof(buffer)) > 0) {
		printf("Jeu en cours: \n");
		printf("%s\n",buffer);
	}
	
    write(socket_descriptor, "OK", 2);
    strcpy(mesg,"");
    while (1)
    {
    	if((longueur = read(socket_descriptor, buffer, sizeof(buffer))) > 0) {
			buffer[longueur] = '\0';
			printf("%s\n",buffer);
			if (longueur == 28) 
				exit(0);
		}
    	
		while (strlen(mesg) != 1) 
		{
			printf("lettre? \n");
			scanf("%s", mesg);
		}
		  
		/* envoi du message vers le serveur */
		if ((write(socket_descriptor, mesg, strlen(mesg))) < 0) {
			perror("erreur : impossible d'envoyer la lettre.");
			exit(1);
		}
		 
		printf("lettre envoye au serveur. \n");
		
		sleep(1);
		         
		for (i = 0; i<256; i++)
    		buffer[i]='\0'; 
		            
		/* lecture de la reponse en provenance du serveur */
		if((longueur = read(socket_descriptor, buffer, sizeof(buffer))) > 0) {
			printf("reponse : \n");
			buffer[longueur] = '\0';
			printf("%s\n",buffer);
		}
		write(socket_descriptor, "OK", 2);
		//printf("before print pendu (l=%d)\n",longueur);
		/*if(read(socket_descriptor, buffer, sizeof(buffer)) > 0) {
			printf("test\n");
			int vie = atoi(buffer);
			switch (vie)
			{
				case 5: printf("\n");
						break;
				case 4: printf(" | \n");
						break;
				case 3: printf(" | \n 0 \n");
						break;
				case 2: printf(" | \n 0 \n ^ \n");
						break;
				case 1:	printf(" | \n 0 \n ^ \n | \n");
						break;
				case 0: printf(" | \n 0 \n ^ \n | \n ^ \n Perdu! \n");
						break;
				default:printf("Erreur vie!\n");
						break;
			}
		}*/
		
		strcpy(mesg,"");
    }
    
    close(socket_descriptor);
    
    printf("connexion avec le serveur fermee, fin du programme.\n");
    
    exit(0);
    
}
