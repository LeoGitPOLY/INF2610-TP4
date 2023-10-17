// Ajouter les directives d'inclusion nécessaires
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    int fd1[2];
    int fd2[2];
    
    pipe(fd1);
    pipe(fd2);

    
    //Creation premier processus fils faisant la premiere commande
    if(fork() == 0){
        close(fd2[0]); close(fd2[1]);
        close(fd1[0]);
        dup2(fd1[1], 1);
        close(fd1[1]);

        execlp("rev", "rev", "In.txt", NULL);
    }

    //Creation deuxieme processus fils faisant juste rev
    else if(fork() == 0){
        close(fd1[1]);
        dup2(fd1[0], 0);
        close(fd1[0]);

        close(fd2[0]);
        dup2(fd2[1], 1);
        close(fd2[1]);

        execlp("rev", "rev", NULL);
    }

    //Creation troisieme processus fils faisant derniere 
    else if(fork() == 0){
        close(fd1[0]); close(fd1[1]);
        close(fd2[1]);
        dup2(fd2[0], 0);
        close(fd2[0]);

        execlp("diff", "diff", "-", "In.txt", "-s", NULL);
    }

    else{
        close(fd2[0]);
        close(fd2[1]);
        close(fd1[0]);
        close(fd1[0]);

        wait(NULL);
    }

    return 0;

}
