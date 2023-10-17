/*
 * processlab - TubesNommes.c
 *
 * Ecole polytechnique de Montreal, GIGL, Automne  2022
 * vos noms, prénoms et matricules
 * Aurelie Nichols - 2142404
 * Leonard Pouliot - 2150965
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main() {
    mkfifo("fdRevToRev", 0600);
    mkfifo("fdRevToDiff", 0600);

    
    //Creation premier processus fils faisant rev
    if(fork() == 0){
        int fd;
        //Ouverture du tube nomme
        fd = open("fdRevToRev", O_WRONLY);
        //Connexion entre le premier tube et STDOUT
        dup2(fd, 1);
        close(fd);
        //Execution de la commande demandee
        execlp("rev", "rev", "In.txt", NULL);
    }

    //Creation deuxieme processus fils faisant rev aussi
    else if(fork() == 0){
        int fd1;
        //Ouverture du tube nomme
        fd1 = open("fdRevToRev", O_RDONLY);
        //Connexion entre le premier tube et STDIN
        dup2(fd1, 0);
        close(fd1);

        int fd2;
        //Ouverture du tube nomme
        fd2 = open("fdRevToDiff", O_WRONLY);
        //Connexion entre le deuxieme tube et STDOUT
        dup2(fd2, 1);
        close(fd2);

        //Execution de la commande demandee
        execlp("rev", "rev", NULL);
    }

    //Creation troisieme processus fils faisant diff
    else if(fork() == 0){
        int fd;
        //Ouverture du tube nomme
        fd = open("fdRevToDiff", O_RDONLY);
        //Connexion entre le deuxieme tube et STDIN
        dup2(fd, 0);
        close(fd);

        //Execution de la commande demandee
        execlp("diff", "diff", "-", "In.txt", "-s", NULL);
    }

    //Processus parent
    else{
        //Attente de la fin de ses fils avant de se terminer
        wait(NULL);
    }

    return 0;

}

