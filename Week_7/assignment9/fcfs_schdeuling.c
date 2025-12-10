#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Process {
    int processId;
    char *processName;
    int burstTime;
    int remainingBurst;

    int ioStartTime;
    int ioDuration;
    int remainingIo;
    int executedCpu;

    int arrivalTime;
    int completionTime;
    int turnaroundTime;
    int waitingTime;

    int killed;

    char state[15];

    struct Process *next;
    struct Process *hashNext;
} Process;

typedef struct KillEvent {
    int processId;
    int time;
    struct KillEvent *next;
} KillEvent;

KillEvent *killEventList = NULL;

const int PROCESS_CONTROL_BLOCK_TABLE_SIZE = 10;
Process **processControlBlockTable;

int hashFunction(int processId) {
    return processId % PROCESS_CONTROL_BLOCK_TABLE_SIZE;
}

void insertProcess(Process *process) {
    int index = hashFunction(process->processId);
    process->hashNext = processControlBlockTable[index];
    processControlBlockTable[index] = process;
}

Process* findProcess(int processId) {
    int index = hashFunction(processId);
    Process *currentProcess = processControlBlockTable[index];
    while (currentProcess) {
        if (currentProcess->processId == processId) return currentProcess;
        currentProcess = currentProcess->hashNext;
    }
    return NULL;
}

Process *readyQueueFront = NULL, *readyQueueRear = NULL;
Process *waitingQueueFront = NULL, *waitingQueueRear = NULL;
Process *terminatedQueueFront = NULL, *terminatedQueueRear = NULL;

void enqueueToReadyQueue(Process *process) {
    process->next = NULL;
    strcpy(process->state, "Ready");
    if (!readyQueueRear) readyQueueFront = readyQueueRear = process;
    else readyQueueRear->next = process, readyQueueRear = process;
}

void enqueueToWaitingQueue(Process *process) {
    process->next = NULL;
    strcpy(process->state, "Waiting");
    if (!waitingQueueRear) waitingQueueFront = waitingQueueRear = process;
    else waitingQueueRear->next = process, waitingQueueRear = process;
}

void enqueueToTerminatedQueue(Process *process) {
    process->next = NULL;
    strcpy(process->state, "Terminated");
    if (!terminatedQueueRear) terminatedQueueFront = terminatedQueueRear = process;
    else terminatedQueueRear->next = process, terminatedQueueRear = process;
}

Process* dequeueFromReadyQueue() {
    if (!readyQueueFront) return NULL;
    Process *process = readyQueueFront;
    readyQueueFront = readyQueueFront->next;
    if (!readyQueueFront) readyQueueRear = NULL;
    return process;
}

Process* dequeueFromWaitingQueue() {
    if (!waitingQueueFront) return NULL;
    Process *process = waitingQueueFront;
    waitingQueueFront = waitingQueueFront->next;
    if (!waitingQueueFront) waitingQueueRear = NULL;
    return process;
}

void addKillEvent(int processId, int time) {
    KillEvent *newKillEvent = malloc(sizeof(KillEvent));
    if (!newKillEvent) {
        fprintf(stderr, "Memory allocation failed for kill event\n");
        exit(1);
    }
    newKillEvent->processId = processId;
    newKillEvent->time = time;
    newKillEvent->next = killEventList;
    killEventList = newKillEvent;
}

int isKilledAtTick(int processId, int tick) {
    KillEvent *currentKillEvent = killEventList;
    while (currentKillEvent) {
        if (currentKillEvent->processId == processId && currentKillEvent->time == tick) return 1;
        currentKillEvent = currentKillEvent->next;
    }
    return 0;
}

void removeFromReadyQueue(int processId) {
    Process *previous = NULL, *current = readyQueueFront;
    while (current) {
        if (current->processId == processId) {
            if (previous == NULL) {
                readyQueueFront = current->next;
                if (!readyQueueFront) readyQueueRear = NULL;
            } else {
                previous->next = current->next;
                if (current == readyQueueRear) readyQueueRear = previous;
            }
            return;
        }
        previous = current;
        current = current->next;
    }
}

void removeFromWaitingQueue(int processId) {
    Process *previous = NULL, *current = waitingQueueFront;
    while (current) {
        if (current->processId == processId) {
            if (previous == NULL) {
                waitingQueueFront = current->next;
                if (!waitingQueueFront) waitingQueueRear = NULL;
            } else {
                previous->next = current->next;
                if (current == waitingQueueRear) waitingQueueRear = previous;
            }
            return;
        }
        previous = current;
        current = current->next;
    }
}

void simulate() {

    int currentTime = 0;

    while (readyQueueFront != NULL || waitingQueueFront != NULL) {

        KillEvent *currentKillEvent = killEventList;
        while (currentKillEvent) {
            if (currentKillEvent->time == currentTime) {
                Process *processToKill = findProcess(currentKillEvent->processId);
                if (processToKill && !processToKill->killed && strcmp(processToKill->state, "Terminated") != 0) {
                    processToKill->killed = 1;
                    processToKill->completionTime = currentTime;

                    removeFromReadyQueue(processToKill->processId);
                    removeFromWaitingQueue(processToKill->processId);

                    strcpy(processToKill->state, "Killed");
                    enqueueToTerminatedQueue(processToKill);
                }
            }
            currentKillEvent = currentKillEvent->next;
        }

        int waitingIterationLimit = 1000;
        while (waitingIterationLimit-- && waitingQueueFront != NULL) {
            Process *waitingProcess = dequeueFromWaitingQueue();
            waitingProcess->remainingIo--;
            if (waitingProcess->remainingIo <= 0) {
                enqueueToReadyQueue(waitingProcess);
            } else {
                enqueueToWaitingQueue(waitingProcess);
            }
        }

        Process *runningProcess = dequeueFromReadyQueue();

        if (!runningProcess) {
            currentTime++;
            continue;
        }

        if (runningProcess->killed) {
            currentTime++;
            continue;
        }

        strcpy(runningProcess->state, "Running");

        runningProcess->remainingBurst--;
        runningProcess->executedCpu++;

        if (runningProcess->ioStartTime == runningProcess->executedCpu && runningProcess->ioDuration > 0) {
            runningProcess->remainingIo = runningProcess->ioDuration;
            enqueueToWaitingQueue(runningProcess);
        }

        else if (runningProcess->remainingBurst == 0) {

            runningProcess->completionTime = currentTime + 1; /* end of this tick */
            runningProcess->turnaroundTime = runningProcess->completionTime - runningProcess->arrivalTime;
            runningProcess->waitingTime = runningProcess->turnaroundTime - runningProcess->burstTime - runningProcess->ioDuration;
            if (runningProcess->waitingTime < 0) runningProcess->waitingTime = 0;

            enqueueToTerminatedQueue(runningProcess);
        }

        else {
            enqueueToReadyQueue(runningProcess);
        }

        currentTime++;
    }
}

void initialize() {
    processControlBlockTable = malloc(PROCESS_CONTROL_BLOCK_TABLE_SIZE * sizeof(Process *));
    if (!processControlBlockTable) {
        printf("Memory allocation failed for process control block table\n");
        exit(1);
    }
    for (int i = 0; i < PROCESS_CONTROL_BLOCK_TABLE_SIZE; i++) processControlBlockTable[i] = NULL;

    int numberOfProcesses;
    printf("Enter number of processes: ");
    if (scanf("%d", &numberOfProcesses) != 1) {
        printf("Failed to read number of processes\n");
        exit(1);
    }

    printf("Enter processName pid burst ioStart ioDuration\n");
    for (int i = 0; i < numberOfProcesses; i++) {

        Process *process = malloc(sizeof(Process));
        if (!process) {
            printf("Memory allocation failed for process\n");
            exit(1);
        }
        process->processName = malloc(64);
        if (!process->processName) {
            printf("Memory allocation failed for process name\n");
            free(process);
            exit(1);
        }

        if (scanf("%63s %d %d %d %d",
                  process->processName,
                  &process->processId,
                  &process->burstTime,
                  &process->ioStartTime,
                  &process->ioDuration) != 5) {
            printf("Invalid process input format\n");
            free(process->processName);
            free(process);
            exit(1);
        }

        process->remainingBurst = process->burstTime;
        process->remainingIo = process->ioDuration;
        process->arrivalTime = 0;
        process->executedCpu = 0;
        process->completionTime = 0;
        process->killed = 0;
        process->turnaroundTime = 0;
        process->waitingTime = 0;
        process->next = process->hashNext = NULL;

        insertProcess(process);
        enqueueToReadyQueue(process);
    }

    int numberOfKillEvents;
    printf("Enter number of KILL events: ");
    if (scanf("%d", &numberOfKillEvents) != 1) {
        printf("Failed to read number of KILL events\n");
        exit(1);
    }

    printf("Enter: KILL pid time\n");
    for (int i = 0; i < numberOfKillEvents; i++) {
        char killWord[16];
        int killProcessId, killTime;
        if (scanf("%15s %d %d", killWord, &killProcessId, &killTime) != 3) {
            printf("Invalid KILL event format\n");
            exit(1);
        }
        addKillEvent(killProcessId, killTime);
    }
}

void printResults() {
    printf("\nPID\tName\tCPU\tIO\tStatus\tTurn\tWait\n");

    while (terminatedQueueFront) {
        Process *process = terminatedQueueFront;
        terminatedQueueFront = terminatedQueueFront->next;

        if (process->killed) {
            printf("%d\t%s\t%d\t%d\tKILLED\t-\t-\n",
                   process->processId, process->processName,
                   process->burstTime, process->ioDuration);
        } else {
            printf("%d\t%s\t%d\t%d\tOK\t%d\t%d\n",
                   process->processId, process->processName,
                   process->burstTime, process->ioDuration,
                   process->turnaroundTime, process->waitingTime);
        }

        free(process->processName);
        free(process);
    }
}

int main() {
    initialize();
    simulate();
    printResults();
    free(processControlBlockTable);
    return 0;
}
