//Author: Smirnov Nikita (xsmirn02)

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h> 
#include <time.h>

#define SLEEP(time) usleep((rand() % (time)) * 1000)
#define SLEEP_RANGE(min_time, max_time) usleep((rand() % (max_time - min_time + 1) + min_time) * 1000)

//Debug
#define debug(s) printf("- %s\n", s)
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

typedef struct Data {
    int NZ;
    int NU;
    int TZ;
    int TU;
    int F;

    int *actionCounter;
    int *employeeCount;
    int *clientCount;
    int *postOpen;
    int *queueList;

    sem_t* mutex;
    sem_t* queue1;
    sem_t* queue2;
    sem_t* queue3;

    FILE* output_file;
} global_t;

int _is_number(char s[])
{
    for (int i = 0; s[i]!= '\0'; i++)
    {
        if (isdigit(s[i]) == 0 && s[i] != '-')
              return 1;
    }
    return 0;
}

int arguments_parse(int argc, char *argv[], global_t* data) {
    //check if user eneter right quantity of arguments
    if (argc != 6) {
        fprintf(stderr, "%s", "Error: Bad Input : bad arguments count!\n");
        return 0;
    }

    //check if every arg is a number with function _is_number
    for (int i = 1; i < argc; i++) {
        if (_is_number(argv[i])) {
            fprintf(stderr, "%s", "Error: Bad Input : all arguments must be integers!\n");
            return 0;
        }
    }

    data->NZ = atoi(argv[1]);
    data->NU = atoi(argv[2]);
    data->TZ = atoi(argv[3]);
    data->TU = atoi(argv[4]);
    data->F = atoi(argv[5]);
    
    if ((data->NZ < 0) || (data->NU <= 0)    ||
        (data->TZ < 0) || (data->TZ > 10000) ||
        (data->TU < 0) || (data->TU > 100)   ||
        (data->F < 0)  || (data->F > 10000)) {
            fprintf(stderr, "%s", "Error: Bad Input : wrong range of argument values!\n");
            return 0;
        }

    return 1;
}

int init_memory(global_t* data) {
    data->actionCounter = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *data->actionCounter = 0;

    data->employeeCount = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *data->employeeCount = 0;

    data->clientCount = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *data->clientCount = 0;
    
    data->postOpen = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *data->postOpen = 1;

    data->queueList = mmap(NULL, 3 * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    data->queueList[0] = 0;
    data->queueList[1] = 0;
    data->queueList[2] = 0;

    if ((data->actionCounter == MAP_FAILED) ||
        (data->employeeCount == MAP_FAILED) ||
        (data->clientCount == MAP_FAILED)   ||
        (data->postOpen == MAP_FAILED)      ||
        (data->queueList == MAP_FAILED)) {

        fprintf(stderr, "%s", "Error: Memory Error : an error occurred while initializing the memory!\n");
        return 1;
    }

    return 0;
}

void destroy_memory(global_t* data) {
    munmap(data->actionCounter, sizeof(int));
    munmap(data->employeeCount, sizeof(int));
    munmap(data->clientCount, sizeof(int));
    munmap(data->postOpen, sizeof(int));
    munmap(data->queueList, 3 * sizeof(int));
}

int init_sem(global_t* data) {
    data->mutex = sem_open("/xsmirn02.ios.mutex", O_CREAT | O_EXCL, 0666, 1);
    data->queue1 = sem_open("/xsmirn02.ios.queue1", O_CREAT | O_EXCL, 0666, 0);
    data->queue2 = sem_open("/xsmirn02.ios.queue2", O_CREAT | O_EXCL, 0666, 0);
    data->queue3 = sem_open("/xsmirn02.ios.queue3", O_CREAT | O_EXCL, 0666, 0);

    if ((data->mutex == SEM_FAILED)        ||
        (data->queue1 == SEM_FAILED)       ||
        (data->queue2 == SEM_FAILED)       ||
        (data->queue3 == SEM_FAILED)) {
            
        fprintf(stderr, "%s", "Error: Semaphore Error : an error occurred while initializing the semaphore!\n");
        return 1;
    }

    return 0;
}

void destroy_sem(global_t* data) {
    sem_close(data->mutex);
    sem_unlink("xsmirn02.ios.mutex");
    sem_close(data->queue1);
    sem_unlink("xsmirn02.ios.queue1");
    sem_close(data->queue2);
    sem_unlink("xsmirn02.ios.queue2");
    sem_close(data->queue3);
    sem_unlink("xsmirn02.ios.queue3");
}

int choose_random_queue(global_t* data) {
    int nonZeroCount = 0;

    //count non zero queues
    if (data->queueList[0] != 0) nonZeroCount++;
    if (data->queueList[1] != 0) nonZeroCount++;
    if (data->queueList[2] != 0) nonZeroCount++;

    //if every queue is empty, employeer can take a break
    if (nonZeroCount == 0) 
        return 0;

    //choose random queue
    int nonZeroList[nonZeroCount];
    int i = 0;
    if (data->queueList[0] != 0) nonZeroList[i++] = 1;
    if (data->queueList[1] != 0) nonZeroList[i++] = 2;
    if (data->queueList[2] != 0) nonZeroList[i++] = 3;

    int r = rand() % nonZeroCount;
    data->queueList[nonZeroList[r]-1]--;

    return nonZeroList[r]; 
}

void employee_process(global_t* data) {
    //every employeer has an unique random sequence
    srand(time(NULL) ^ getpid());
    int employeeID = ++(*data->employeeCount);

    sem_wait(data->mutex);
    fprintf(data->output_file, "%d: U %d: started\n", ++(*data->actionCounter), employeeID);
    sem_post(data->mutex);

    while (1) {
        //choose random queue or let the employeer go home/rest
        int queueInd = choose_random_queue(data);
        if (queueInd == 0) {
            if (!(*data->postOpen)) {
                sem_wait(data->mutex);
                fprintf(data->output_file, "%d: U %d: going home\n", ++(*data->actionCounter), employeeID);
                sem_post(data->mutex);

                break;
            }

            sem_wait(data->mutex);
            fprintf(data->output_file, "%d: U %d: taking break\n", ++(*data->actionCounter), employeeID);
            sem_post(data->mutex);

            //imitate break
            SLEEP(data->TU);

            sem_wait(data->mutex);
            fprintf(data->output_file, "%d: U %d: break finished\n", ++(*data->actionCounter), employeeID);
            sem_post(data->mutex);

            continue;
        }

        sem_wait(data->mutex);
        fprintf(data->output_file, "%d: U %d: serving a service of type %d\n", ++(*data->actionCounter), employeeID, queueInd);

        //let the client to get served by employeer
        switch (queueInd) { //CHANGE LATER
            case 1:
                sem_post(data->queue1);
                break;
            case 2:
                sem_post(data->queue2);
                break;
            case 3:
                sem_post(data->queue3);
                break;
        }    
        sem_post(data->mutex);

        //imitate work
        SLEEP(10);

        sem_wait(data->mutex);
        fprintf(data->output_file, "%d: U %d: service finished\n", ++(*data->actionCounter), employeeID);
        sem_post(data->mutex);
    }
}

void client_process(global_t* data) {
    //every client has an unique random sequence
    srand(time(NULL) ^ getpid());
    int clientID = ++(*data->clientCount);
    int serviceID;

    sem_wait(data->mutex);
    fprintf(data->output_file, "%d: Z %d: started\n", ++(*data->actionCounter), clientID);
    sem_post(data->mutex);

    //wait before client enters to the post
    SLEEP(data->TZ);

    //if post is closed the client should go home
    if (!(*data->postOpen)) {
        sem_wait(data->mutex);
        fprintf(data->output_file, "%d: Z %d: going home\n", ++(*data->actionCounter), clientID);
        sem_post(data->mutex);
        return;
    }

    //choose service
    serviceID = (rand() % 3) + 1;
    sem_wait(data->mutex);
    fprintf(data->output_file, "%d: Z %d: entering office for a service %d\n", ++(*data->actionCounter), clientID, serviceID);
    data->queueList[serviceID-1]++;
    sem_post(data->mutex);

    //place client in queue
    switch (serviceID) { //CHANGE LATER
        case 1:
            sem_wait(data->queue1);
            break;
        case 2:
            sem_wait(data->queue2);
            break;
        case 3:
            sem_wait(data->queue3);
            break;
    } 

    sem_wait(data->mutex);
    fprintf(data->output_file, "%d: Z %d: called by office worker\n", ++(*data->actionCounter), clientID);
    sem_post(data->mutex);
    
    //imitate employeer working process (synchronizace s úředníkem na dokončení žádosti není vyžadována)
    SLEEP(10);

    sem_wait(data->mutex);
    fprintf(data->output_file, "%d: Z %d: going home\n", ++(*data->actionCounter), clientID);
    sem_post(data->mutex);
}

int main(int argc, char *argv[]) {

    global_t Data;
    int clientPID;
    int employeePID;
    int error = 1;
    
    if (!arguments_parse(argc, argv, &Data)) {
        return 1;
    }

    error = init_memory(&Data);
    if (error) {
        destroy_memory(&Data);
        return 1;
    }
    error = init_sem(&Data);
    if (error) {
        destroy_memory(&Data);
        destroy_sem(&Data);
        return 1;
    }

    //open the output file
    Data.output_file = fopen("proj2.out", "w");
    if (Data.output_file == NULL)
    {
        fprintf(stderr, "%s", "Error: File Initialization : an error occurred while opening the output file!\n");
        return 1;
    }
    setbuf(Data.output_file, 0);

    // init clients
    for (int i = 0; i < Data.NZ; i++) {
        clientPID = fork();

        if(clientPID == -1)
        {
            fprintf(stderr, "%s", "Error: Multiprocessing Error : problem with client child process!\n");
            return 1;
        }
        else if (clientPID == 0)
        {
            client_process(&Data);
            exit(0);
        }
    }

    // init employeers
    for (int i = 0; i < Data.NU; i++) {
        employeePID = fork();

        if(employeePID == -1)
        {
            fprintf(stderr, "%s", "Error: Multiprocessing Error : problem with employee child process!\n");
            return 1;
        }
        else if (employeePID == 0)
        {
            employee_process(&Data);
            exit(0);
        }
    }

    //wait untill post closes
    SLEEP_RANGE(Data.F/2, Data.F);

    sem_wait(Data.mutex);
    fprintf(Data.output_file, "%d: closing\n", ++(*Data.actionCounter));
    *Data.postOpen = 0;
    sem_post(Data.mutex);

    //wait until every process ends
    for (int i = 0; i < Data.NZ + Data.NU + 1; i++) {
        wait(NULL);
    }

    //cleanup memory
    destroy_memory(&Data);
    destroy_sem(&Data);
    fclose(Data.output_file);
}