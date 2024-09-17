#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


#define MSH_BUFFERSIZE 1024
#define	MSH_TOK_BUFFERSIZE 64
#define MSH_TOK_DELIM " \t\r\n\a"



//built in declarations
int msh_cd(char **args);
int msh_help(char **args);
int msh_exit(char **args);


char *builtin_str[] ={
	"cd",
	"help",
	"exit"
};

int (*builtin_func[]) (char **) ={
	&msh_cd,
	&msh_help,
	&msh_exit
};
    
//built in implementations
int msh_launch(char ** args){

	pid_t pid, wpid;
	int status;

	pid = fork();

	if (pid == 0) {
		//child process
		if (execvp(args[0], args) == -1) {
			perror("msh");
		}	
		exit(EXIT_FAILURE);
	}else if (pid < 0) {
		perror("msh");	
	}else{
		do{
			wpid = waitpid(pid,&status,WUNTRACED);
		}while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return 1;
}



int msh_cd(char ** args){
	if(args[1] == NULL){
		fprintf(stderr, "msh: expected argument to \"cd\"\n");
	}else{
		if (chdir(args[1])!= 0) {
			perror("msh");	
		}
	}
	return 1;
}

int msh_help(char **args){
	int i;
	printf("My Own Shell!!\n");
	printf("Type program names and arguments, and hit enter.\n");
	printf("The following are built - in: \n");

	for (i=0; i<msh_num_builtins(); i++) {
		printf("   %s\n",builtin_str[i]);	
	}

	return 1;
}


int msh_exit(char **args){
	return 0;
}


int msh_num_builtins(){

	return sizeof(builtin_str)/sizeof(char*);
}

//non built in implementations
char * msh_read_line(){
	int buffer_size = MSH_BUFFERSIZE;
	int position = 0;
	char * buffer = malloc(buffer_size);
	int c;

	if (!buffer) {
		fprintf(stderr," msh: alloc error\n");
		exit(EXIT_FAILURE); }

	while (1) {
		c = getchar();
		// if hits EOF or newline, replace it with null and return
		if (c==EOF || c == '\n') {
			buffer[position] = '\0';
			return buffer;
		}else{
			buffer[position] = c;
		}
		position++;

		//if we get to te buffer limit, realloc
		if (position >=buffer_size) {
			buffer_size += MSH_BUFFERSIZE;	
			buffer = realloc(buffer,MSH_BUFFERSIZE);
			if (!buffer) {
				fprintf(stderr, "msh: allocation error\n");	
				exit(EXIT_FAILURE);
			}
		}

	}
	return buffer;
}

char ** msh_split_line(char * line){
	int args_count =0;
	char * copy =line;


	// fancy way to return the number of spaces in the string
	for (args_count = 0; copy[args_count]; copy[args_count] == ' ' ? args_count++ : *copy++);

	/*the total number of arguments is the whitespaces + the last word
	so we gotta add one more*/
	args_count++;


	char ** args= malloc(sizeof(char*) * args_count);

	if (!args) {
		fprintf(stderr,"msh: allocation error\n");	
		exit(EXIT_FAILURE);
	}

	char * token;
	token= strtok(line, MSH_TOK_DELIM);
	int i=0;
	while (token != NULL) {
		args[i] = token;
		token = strtok(NULL, MSH_TOK_DELIM);
		i++;
	}
	args[i] = NULL;

	return args;
}

int msh_execute(char **args){

	if (args[0] == NULL ) {
		// An empty command was entered.
		return 1;
	}
	for (int i =0; i < msh_num_builtins(); i++) {
		if (strcmp(args[0],builtin_str[i])==0) {
			return (*builtin_func[i])(args);	
		}	
	}
	return msh_launch(args);
}

int main(){
	char* line;
	char** args;
	int status;

	do{
		printf("> ");
		//read
		line = msh_read_line();
		//parse
		args = msh_split_line(line);
		//execute
		status = msh_execute(args);

		free(line);
		free(args);
	}while(status);
}
