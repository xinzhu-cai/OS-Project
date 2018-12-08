#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <readline/readline.h>

#define MAX_LINE 80
#define BUFFER_SIZE 50

char buffer[BUFFER_SIZE];
char *history[10];
int count = 0;

/*change path*/
int cd(char *path) {
    return chdir(path);
}

/*function to display the history of commands*/
void Displayhistory(){
    int cont = 1;
    int i = count - 1;
    if (i<0) i = i + 10;
    printf("Shell command history:\n");
    while(1){
        if( cont > 10 || history[i] == NULL)break;
        printf("%d\t%s\n",cont,history[i]);
        cont ++;
        i--;
        if (i<0) i = i + 10;
    }
}

/*The handle the interrupt from keyboard*/
void handle_SIGINT(){
    write(STDOUT_FILENO, buffer, strlen(buffer));
    fflush(stdout);
    if(count <= 0) printf("No command in history\n");
    else Displayhistory();
}

/*Fuction to get the command from shell, tokenize it and set the args parameter*/
void setup(char *inputBuffer, char *args[], int *background){
    int len = 0;
    char c;
    const char *d = " ";
    //initialize
    for(int i = 0; i< MAX_LINE/2; i++)
        args[i] = NULL;
    
    inputBuffer = readline("COMMAND->");
    if(inputBuffer == NULL) {
        printf("\n");
        exit(0);
    }
    if((len = strlen(inputBuffer)) == 0) return;
    //When command is ended with &, set background = 1
    if(inputBuffer[len - 1] == '&') {*background = 1; inputBuffer[len - 1] = '\0';}
    
    if(inputBuffer[0] == 'r'){
        //r
        int tmp = count;
        if(tmp == 0) tmp = 9;
        else tmp--;
        if(inputBuffer[1] == '\0'){
            if(history[tmp] == '\0'){
                printf("No Commands in the history\n");
                return;
            }
            //Copy the last command in history to inputBuffer
            strcpy(inputBuffer,history[tmp]);
        }
        //rx
        if(inputBuffer[1] == ' ' && inputBuffer[2] != '\0' && inputBuffer[3] == '\0'){
            int cont = 1;
            int i = count - 1;
            if (i < 0) i = i + 10;
            while(1){
                if( cont > 10 || history[i] == NULL){
                    printf("No such Command in the history\n");
                    return;
                }
                if(history[i][0] == inputBuffer[2]){
                    // Find the corresponding command and copy it to inputBuffer
                    strcpy(inputBuffer,history[i]);
                    break;
                }
                cont ++;
                i--;
                if (i<0) i = i + 10;
            }
        }
        
    }
    
    //push in history
    int tmp = count;
    if(tmp == 0) tmp = 9;
    else tmp--;
    //If this command is the same as the previous one, do not add to history 
    if(history[tmp] == NULL || strcmp(inputBuffer,history[tmp])){
        history[count] = (char*)malloc(strlen(inputBuffer));
        strcpy(history[count],inputBuffer);
        count = (count + 1)%10;
    }

    //Divide inputBuffer by space
    char *ch;
    int index = 0;
    ch = strtok(inputBuffer,d);
    args[index] = p;
    while ((ch = strtok(NULL,d))){
        index++;
        args[index] = ch;
    }
}

int main()
{
    char *inputBuffer;
    int background;
    char *args[MAX_LINE/2 + 1];
    int i;

    //initialize history array
    for(i = 0; i < 10; i++) history[i] = NULL;
    
    //capture the ctrl+C
    struct sigaction handler;
    handler.sa_handler = handle_SIGINT;
    signal(SIGINT, handle_SIGINT);
    
    while(1){
        background = 0;
        
        fflush(stdin); // clean the input buffer
        fflush(stdout); // clean the output buffer 
        setup(inputBuffer, args, &background);
        
        if(!args[0]) {  // Handle empty commands 
            continue;
        }
        
        // Skip the fork
        if(strcmp(args[0], "cd") == 0) { 
            
            if(cd(args[1]) < 0) {
                perror(args[1]);
            }
            continue;
        }
        
        if(args[0] != NULL){
            pid_t pid;
            pid = fork();
            if(pid < 0){ 
                //if pid is less than 0, forking fails
                perror("Fork failed!\n");
                exit(1);
            }
            else if(pid == 0){
                signal(SIGINT, SIG_DFL);
                // command not executed
                if(execvp(args[0],args) < 0) {
                    perror(args[0]);
                    exit(1);
                }
                background = 0;
            }
            else{
                if(background == 0) //wait for the child process
                    wait(NULL);
            }
        }
    }
    return 0;
}