#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>


//start time
int count_time = 0;

//mutex to lock queue while it is updated
pthread_mutex_t lockQueue;

//mutex to lock track while crossing
pthread_mutex_t lockTrack;

//signal for when crossing is done
pthread_cond_t finishedCrossSignal;

//variable to signal to start loading
int loading = 0;

//array of individual variables to signal train crossing
pthread_cond_t cross[200];
pthread_mutex_t crossMutexes[200];

//struct to carry info for each train
struct trainNode {
    pthread_t train_id;
	int number;
    char * direction;
    int status; //set to 1 when done crossing
    int priority; //either high or low, so will use 1 for high and 0 for low
    double loadingTime;
    double crossingTime;
    struct trainNode * next;
};

//station queues (linked implementation)
struct trainNode * eastHead = NULL;
struct trainNode * westHead = NULL;
struct trainNode * eastHeadHigh = NULL;
struct trainNode * westHeadHigh = NULL;


void add_to_list(struct trainNode ** head, struct trainNode * new){

        new->next = NULL;
    if(*head == NULL){
        *head = new;
        
        return;
    }else{
        struct trainNode * temp = *head;

        while(temp->next != NULL){
            temp = temp->next;
        }
        temp->next = new;
        return;

    }
}

struct trainNode * remove_from_list(struct trainNode ** head){

        struct trainNode * temp = *head;
        *head = (*head)->next;
        return temp;
    
}



void * train(void * cur){ //code to be executed by train threads
    struct trainNode * t = (struct trainNode*)cur;
   

    //wait for all trains to be created to start loading
    while(loading != 1);


    usleep(t->loadingTime*100000); //sleep to simulate loading time

        struct timespec loadtime;
        if(clock_gettime(CLOCK_REALTIME, &loadtime)!=0){
            printf("Error getting time.\n");
            exit(0);
        }
        
        //get on time in dsecs
        int load_done = (loadtime.tv_sec * 10) + (loadtime.tv_nsec / 100000000) - count_time;
        int hours = (load_done / (60 * 60 * 10));
        int mins = (load_done / (10 * 60));
        int secs = (load_done / 10);
        int dsecs = (load_done % 10);

      
    //lock mutex to add to queue
    if(pthread_mutex_lock(&lockQueue)!=0){
        printf("%s", "Failed to lock mutex.\n");
        exit(0);
}
        
    printf("%02d:%02d:%02d.%d Train %2d is ready to go %4s\n", hours, mins, secs, dsecs, t->number, t->direction); //print when loading is complete

    //add to station queues
    if(strcmp(t->direction, "East")==0){
        if(t->priority == 1){
             add_to_list(&eastHeadHigh, t);
        }else{
            add_to_list(&eastHead, t);
        }

    }else{
        if(t->priority == 1){
            add_to_list(&westHeadHigh, t);
        }else{
            add_to_list(&westHead, t);
        }

    }
       
       //unlock queue when done adding to it
        if(pthread_mutex_unlock(&lockQueue)!=0){
        printf("%s", "Failed to unlock mutex.\n");
        exit(0);
    }
           
       

        //wait for signal to cross
        if(pthread_mutex_lock(&crossMutexes[t->number])!=0){
            printf("%s", "Failed to lock mutex.\n");
            exit(0);
}
               
if(pthread_cond_wait(&cross[t->number], &crossMutexes[t->number])!=0){
            printf("%s", "Failed to lock mutex.\n");
            exit(0);
}
        
                struct timespec curtime;
        if(clock_gettime(CLOCK_REALTIME, &curtime)!=0){
            printf("Error getting time.\n");
            exit(0);
        }
        
        //get on time in dsecs
        int on_time = (curtime.tv_sec * 10) + (curtime.tv_nsec / 100000000) - count_time;

        hours = (on_time / (60 * 60 * 10));
         mins = (on_time / (10 * 60));
        secs = (on_time / 10);
        dsecs = (on_time % 10);
       
        //cross when signal is recieved
        printf("%02d:%02d:%02d.%d Train %2d is ON the main track going %4s\n", hours, mins, secs, dsecs, t->number, t->direction);

        if(pthread_mutex_unlock(&crossMutexes[t->number])!=0){
                printf("%s", "Failed to unlock mutex.\n");
                exit(0);
}
                
        usleep(t->crossingTime*100000); //crossing time

        struct timespec offtime;
        if(clock_gettime(CLOCK_REALTIME, &offtime)!=0){
            printf("Error getting time.\n");
            exit(0);
        }
        
        //get on time in dsecs
        int off_time = (offtime.tv_sec * 10) + (offtime.tv_nsec / 100000000) - count_time;

        hours = (off_time / (60 * 60 * 10));
         mins = (off_time / (10 * 60));
        secs = (off_time / 10);
        dsecs = (off_time % 10);
    
        printf("%02d:%02d:%02d.%d Train %2d is OFF the main track after going %4s\n", hours, mins, secs, dsecs, t->number, t->direction);
        
        //signal done crossing, and give mutex back to dispatcher
        if(pthread_cond_signal(&finishedCrossSignal)!=0){
            printf("Failed to signal done crossing\n");
            exit(0);
} 
       
        //destroy mutex and convar
    if(pthread_mutex_destroy(&crossMutexes[t->number])!=0){
            printf("Failed to destroy mutex.\n");
            exit(0);
        }
        if(pthread_cond_destroy(&cross[t->number])!=0){
            printf("Failed to destroy convar.\n");
            exit(0);
}
        
        pthread_exit("Done\n");
    
}


int
main(int argc, char *argv[])
{



    if(argc != 2){ //handle error if no filename is given, or multiple arguments given
        printf("%s", "Incorrect input. Please type 1 file name");
    }else{
        
        
        char buffer[200]; //declare buffer variable to hold input
        struct trainNode * head = NULL; //declare head node for temporary list read from file

        FILE * fptr = fopen(argv[1], "r"); //open file in read mode
        if(fptr == NULL){
            printf("%s", "Could not read file.");
            return 0;
        }

        int numTrains = 0; //start counter for number of trains


        while(fgets(buffer, 200, fptr)){ //read line by line
            struct trainNode * temp = (struct trainNode*)malloc(sizeof(struct trainNode));
            
            char * token = strtok(buffer, " "); //get first token, which will tell us direction and priority
            //sort according to direction and priority
            
            if(strcmp(token, "W")==0){
                temp->direction = "West";
                temp->priority = 1;
            }else if(strcmp(token, "E")==0){
                temp->direction = "East";
                temp->priority = 1;
            }else if(strcmp(token, "e")==0){
                temp->direction = "East";
                temp->priority = 0;
            }else{
                temp->direction = "West";
                temp->priority = 0;
            }
            
            token = strtok(NULL, " "); //get second token, which will be loading time
            temp->loadingTime = atoi(token);
            

            token = strtok(NULL, " "); //get last token, which will be crossing time
            temp->crossingTime = atoi(token);
            
            temp->number = numTrains;        

            if(numTrains == 0){
                    head = temp;
                    //set struct to be head of list
            }else{

                    struct trainNode * temp2 = head;

                    while(temp2->next != NULL){ //loop to last node in list
                        temp2= temp2->next;
                    }
                // append to list
                temp2->next = temp;
            }
            
            numTrains++; //increase number of trains as file is read, 1 line = 1 train

        } 
            
        if(fclose(fptr)!=0){
        printf("Error closing file.\n");
            exit(0);
        }


        //initialize first mutex and convar (to signal to start loading) and mutex and convar for the track
        
        if(pthread_mutex_init(&lockTrack, NULL)!=0){
                printf("%s", "Failed to create mutex.\n");
                exit(0);
            
        }
        if(pthread_cond_init(&finishedCrossSignal, NULL)!=0){
            printf("%s", "Failed to create convar.\n");
                exit(0);
        }
        
                      
        struct trainNode * iter = head;
        while(iter != NULL){ //loop through each train node and create a thread for each train
            //create mutex and convar for each train
            if(pthread_mutex_init(&crossMutexes[iter->number], NULL) != 0){
                printf("%s", "Failed to create mutex.\n");
                exit(0);
            }
            if(pthread_cond_init(&cross[iter->number], NULL)!=0){
                    printf("%s", "Failed to create convar.\n");
                exit(0);
}
                        
            int test = pthread_create(&iter->train_id, NULL, train, (void*)iter);

            if(test != 0){ //handle error if pthread create fails
                printf("%s", "Failed to create train.\n");
                exit(0);
            }
            
          
            iter = iter->next;
            
        }
        
       
        struct timespec start;
        if(clock_gettime(CLOCK_REALTIME, &start)!=0){
            printf("Error with clock function.\n");
            exit(0);
        }
        
        //get starting time in dsecs
        count_time = (start.tv_sec * 10) + (start.tv_nsec / 100000000);

        //signal trains to begin loading
        loading = 1;

        //variable to track number of trains dispatched
        int i = numTrains;

        //variables for starvation case
        int westCount = 0;
        int eastCount = 0;
        //variable for opposite direction case
        int lastTrain = 0;
      
        while(i > 0){

          
            struct trainNode * ready = NULL;
            if(westCount != 3 && westHeadHigh != NULL && (lastTrain == 0 || eastHeadHigh == NULL)){

                if(westHeadHigh->next != NULL){ //if there are at least 2 trains ready

                struct trainNode * temp = westHeadHigh;
                
                struct trainNode * iter = westHeadHigh; //start with second node as we loop through
                struct trainNode * smallest = westHeadHigh;
                while(iter != NULL){
                        //if other node has smaller loading time
                        if(smallest->loadingTime > iter->loadingTime){ 
                                smallest = iter;
                            //if loading times are the same, pick smaller train number
                        }else if(smallest->loadingTime == iter->loadingTime){

                            if(smallest->number > iter->number){
                                smallest = iter;
                            }
                        }
                        iter = iter->next;
                }
                //swap nodes so smallest is at the front of queue

                //if they are beside eachother
                if(westHeadHigh->next == smallest){
                        westHeadHigh = smallest;
                        westHeadHigh->next = temp;
                        temp->next = NULL;
                }else{ //if they are not beside eachother

                    westHeadHigh = smallest;
                    struct trainNode* temp3 = smallest->next;
                    smallest->next = temp->next;
                    temp->next = temp3;
                }
                
                }
                    ready = remove_from_list(&westHeadHigh);
                    westCount++;
                    eastCount = 0;
                    lastTrain = 1;
            }else if(eastCount != 3 && eastHeadHigh != NULL && (lastTrain == 1 || westHeadHigh == NULL)){
                
                if(eastHeadHigh->next != NULL){ //if there are at least 2 trains ready

                struct trainNode * temp = eastHeadHigh;
                
                struct trainNode * iter = eastHeadHigh; //start with second node as we loop through
                struct trainNode * smallest = eastHeadHigh;
                while(iter != NULL){
                        //if other node has smaller loading time
                        if(smallest->loadingTime > iter->loadingTime){ 
                                smallest = iter;
                            //if loading times are the same, pick smaller train number
                        }else if(smallest->loadingTime == iter->loadingTime){

                            if(smallest->number > iter->number){
                                smallest = iter;
                            }
                        }
                        iter = iter->next;
                }
                //swap nodes so smallest is at the front of queue

                //if they are beside eachother
                if(eastHeadHigh->next == smallest){
                        eastHeadHigh = smallest;
                        eastHeadHigh->next = temp;
                        temp->next = NULL;
                }else{ //if they are not beside eachother

                    eastHeadHigh = smallest;
                    struct trainNode* temp3 = smallest->next;
                    smallest->next = temp->next;
                    temp->next = temp3;
                }
                
                }
                
                ready = remove_from_list(&eastHeadHigh);
               
                    eastCount++;
                   westCount = 0;
                   lastTrain = 0;
                
            }else if(westCount != 3 && westHead != NULL && (lastTrain == 0 || eastHead == NULL)){
               
               if(westHead->next != NULL){ //if there are at least 2 trains ready

                struct trainNode * temp = westHead;
                
                struct trainNode * iter = westHead; //start with second node as we loop through
                struct trainNode * smallest = westHead;
                while(iter != NULL){
                        //if other node has smaller loading time
                        if(smallest->loadingTime > iter->loadingTime){ 
                                smallest = iter;
                            //if loading times are the same, pick smaller train number
                        }else if(smallest->loadingTime == iter->loadingTime){

                            if(smallest->number > iter->number){
                                smallest = iter;
                            }
                        }
                        iter = iter->next;
                }
                //swap nodes so smallest is at the front of queue

                //if they are beside eachother
                if(westHead->next == smallest){
                        westHead = smallest;
                        westHead->next = temp;
                        temp->next = NULL;
                }else{ //if they are not beside eachother

                    westHead = smallest;
                    struct trainNode* temp3 = smallest->next;
                    smallest->next = temp->next;
                    temp->next = temp3;
                }
                
                }
                ready = remove_from_list(&westHead);
                westCount++;
                eastCount = 0;
                lastTrain = 1;
         
                
            }else if(eastCount != 3 && eastHead != NULL && (lastTrain == 1 || westHead == NULL)){
                
                if(eastHead->next != NULL){ //if there are at least 2 trains ready

                struct trainNode * temp = eastHead;
                
                struct trainNode * iter = eastHead; //start with second node as we loop through
                struct trainNode * smallest = eastHead;
                while(iter != NULL){
                        //if other node has smaller loading time
                        if(smallest->loadingTime > iter->loadingTime){ 
                                smallest = iter;
                            //if loading times are the same, pick smaller train number
                        }else if(smallest->loadingTime == iter->loadingTime){

                            if(smallest->number > iter->number){
                                smallest = iter;
                            }
                        }
                        iter = iter->next;
                }
                //swap nodes so smallest is at the front of queue

                //if they are beside eachother
                if(eastHead->next == smallest){
                        eastHead = smallest;
                        eastHead->next = temp;
                        temp->next = NULL;
                }else{ //if they are not beside eachother

                    eastHead = smallest;
                    struct trainNode* temp3 = smallest->next;
                    smallest->next = temp->next;
                    temp->next = temp3;
                }
                
                }
                ready = remove_from_list(&eastHead);
                eastCount++;
                westCount = 0;
                lastTrain = 0;
             
            }else if(eastCount == 3 || westCount == 3){ //if no opposite trains went (starvation case)
                    eastCount = 0;
                    westCount = 0;
                    continue;
            } else if(ready == NULL){ //if no trains ready
                usleep(50000); //tiny break between cycles so that trains have a chance to be ready
                continue;
            }
            
            //lock track and signal train to cross
            
            if(pthread_mutex_lock(&lockTrack)!=0){
                printf("%s", "Failed to lock mutex.\n");
                exit(0);
}
            if(pthread_cond_signal(&cross[ready->number])!=0){
                printf("%s", "Failed to signal train.\n");
                exit(0);
}
                       
            //wait for train to signal done crossing
            if(pthread_cond_wait(&finishedCrossSignal, &lockTrack)!=0){
                printf("%s", "Failed to lock track again.\n");
                exit(0);
}
                        
            if(pthread_mutex_unlock(&lockTrack)!=0){
                printf("%s", "Failed to unlock track.\n");
                exit(0);
}
            //free train node when we no longer need it
            free(ready);        
            i--;
           
            
            
        }
           
           //destroy global mutexes and convar
            if(pthread_mutex_destroy(&lockTrack)!=0){
                printf("%s", "Failed to destroy mutex.\n");
                exit(0);
            }
            if(pthread_mutex_destroy(&lockQueue)!=0){
                printf("%s", "Failed to destroy mutex.\n");
                exit(0);
            }

            if(pthread_cond_destroy(&finishedCrossSignal)!=0){
                printf("%s", "Failed to destroy convar.\n");
                exit(0);
            }
            
            
    }

           

        
       

        
    }
    

