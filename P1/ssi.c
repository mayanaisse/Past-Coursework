#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <sys/wait.h>

struct bgp_node{ //struct to store background process list

	pid_t pid;
	char * cmd;
	struct bgp_node * next;
};

void listofArgs(char * input, char * args[]){ //function to seperate command and get list of args in an array

	char * cmd = strtok(input, " "); // strip off command, will be the new value of 'reply'
	int i = 1;
	args[0] = cmd;
	while(cmd != NULL){ //loop through string, update args array
		cmd = strtok(NULL, " ");
		args[i] = cmd;
		i++;
		
	}
	
}

void update_args(char * args[]){

	int i = 0;
	while(args[i] != NULL){
		args[i] = args[i+1];
		i++;
	}
}

void run_command(char * cmd, char * args[]){

	
	pid_t pid = fork(); //fork command to run child program
	if(pid == 0){
		execvp(cmd, args); //run child program
		exit(0);
	}else{
		waitpid(pid, NULL, 0); //wait for specific child process to finish
	}

}

void run_command_bg(char * cmd, char * args[], int num, struct bgp_node ** root){

	pid_t pid = fork(); //fork command to run child program
	pid_t child;
	if(pid == 0){
		execvp(cmd, args); //run child program
		exit(0);
		
	}else{
		struct bgp_node * temp = (struct bgp_node *) malloc(sizeof(struct bgp_node));
		if(num == 0){
			
			temp->pid = pid;
			temp->cmd = strdup(cmd);
			temp->next = NULL;

			(*root) = temp;
			
		}else{
			
			struct bgp_node * temp2 = *root;

			while(temp2->next != NULL){
				temp2 = temp2->next;
			}

			temp->pid = pid;
			temp->cmd = strdup(cmd);
			temp->next = NULL;

			temp2->next = temp; //append to back of list
			
		}

	}
}

void update_prompt(char * prompt){ //function to update the prompt depending on home directory
	

	char * user = getlogin();
	char hostname[200];
	char buffer[200];
	int host = gethostname(hostname, 200);
	char * cwd = getcwd(buffer, 200);

	//create prompt based on users name, host, and current directory
	strcat(prompt, user);
	strcat(prompt, "@");
	strcat(prompt, hostname);
	strcat(prompt, ": ");
	strcat(prompt, cwd);
	strcat(prompt, "> ");
}


int check_child_status(int num, struct bgp_node ** root){

	if(num > 0){


			pid_t ter = waitpid(0, NULL, WNOHANG);

			while(ter > 0){
			
				
				struct bgp_node * temp = (*root);
				if(temp != NULL && temp->pid == ter){ //check if first process is terminated
						num--;
					printf("%d: %s has terminated.\n", temp->pid, temp->cmd); //print pid and command termination

					(*root) = temp->next;
					
					free(temp);
					

				}else{
					
					struct bgp_node* prev = (*root);

					while(temp != NULL){ //loop until terminated process is found

						if(temp->next->pid == ter){
							num--;
							printf("%d: %s has terminated.\n", temp->next->pid, temp->next->cmd); //print pid and command termination

							struct bgp_node * temp2 = temp->next; //set pointer to node to be removed
							prev->next = temp->next->next; //update list and free finished node
							free(temp2);
							break;

						}

						prev = temp;
						temp = temp->next; 
					} 

				
			}
			ter = waitpid(0, NULL, WNOHANG);
			}
		
		
	}
		return num;

}


int
main()
{
	char * args[300];
	int num_bgp = 0;

	struct bgp_node * head = NULL; //initialize head for bg process list

	int bailout = 0;


	while (!bailout) {
		char* prompt = malloc(300);
		//allocate memory for prompt, which will show current directory
		
		update_prompt(prompt);

		char* reply = readline(prompt);
		

		if (!strcmp(reply, "exit")) { //type exit to leave shell
			bailout = 1;
		} else {
			
			listofArgs(reply, args); //separate args from first command
			
			
						
				if(strcmp(reply, "cd")==0){ //check if cd was entered as command
							
					if(args[1] == NULL || strcmp(args[1], "~")==0){ //use case if home directory entered
							const char * path = getenv("HOME");
							chdir(path);
						
					}else if(strcmp(args[1], "..")==0){ //use case if .. is entered
						chdir("..");
						

					}else{ //other case where path is used
							chdir(args[1]);
						
					}


				}else if(strcmp(reply, "bg")==0){ //if bg command is called

					char * temp = args[1]; //get command from array, after bg
					update_args(args); //shift array over by 1, to get updated args array for new command

					run_command_bg(temp, args, num_bgp, &head);
					num_bgp++;

				

					}else if(strcmp(reply, "bglist")==0){ //display bg list
						
						if(num_bgp > 0){
						struct bgp_node * temp = head;
						
						while(temp != NULL){

							printf("%d: %s currently executing\n", temp->pid, temp->cmd);

							temp = temp->next;
						}
						}
						printf("Number of Jobs Running: %d\n", num_bgp);
						
					}else{

					run_command(reply, args); //run command if entered normally
				}
				
				num_bgp = check_child_status(num_bgp, &head); //check if bg process has terminated
			
			
			
		}
	
		free(reply);
		free(prompt);
	}
	printf("Successfully Exited. Thank you!\n");
	

}

