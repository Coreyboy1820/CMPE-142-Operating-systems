void signalHandler() {
}

int main(int argc, char *argv[]) {
    int c = fork();
    signal(SIGINT, signalHandler);
    if(c == 0) {
        // child
        // execute binary
        exit(0);
    }
    else if(c > 0) {
        // parent
        int c1 = fork();
        if(c1 == 0) {
            // child
            // do binary
            exit(0);
        }
        else if(c1 > 0) {
            // parent
            sleep(60);
            kill(0, SIGINT);
            if(wait() < 0) {
                fprintf(stderr, "failed when waiting");
            }
        }
        else{
            fprintf(stderr, "failed when forking");
        }
    }
    else {
        fprintf(stderr, "failed when forking");
    }
}
/*
14.-COW stands for:  copy on write
 
15.-In the context of CPU registers, PC stands for: program counter
 
16.-PCB stands for: process control block
 
17.-TCB stands for:  thread control block
 
18.-API stands for:  application program interface
 
19.-DMA stands for:  direct memory access
 
20.-GUI stands for  graphical user interface
 
Concepts Section (1 point each/3 points total) 
 
21.-A trap is:  a software interrupt
 
22.-The interrupt vector is:  a vector of pointers to interupt handlers
 
23.-A thread is:  I dont fucking know
*/