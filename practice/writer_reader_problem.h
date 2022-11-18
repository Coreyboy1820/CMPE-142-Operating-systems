// Givens 

class Queue{
    public:
    void enqueue(Process sem){}
    Process dequeue(){}
    bool empty(){
        return size ? false : true;
    }

    private:
    int size = 0;
};


class Process{
    public:
    Process getCurrentProcess(){
        return *this;
    }
};

Queue rq;

// ===============================================================================================================

class CriticalLock {
    public:
    int Lock() {
        int temp = lock_;
        lock_ = 1;
        return temp;
    }

    int Unlock() {
        lock_ = 0;
    }
    private:
    int lock_ = 0;
};

// ===============================================================================================================

class BusySemaphore {
    public:
    void signal(){ // this should increment the count (freeing up resources)
        while(lock_.Lock() == 1);
        count_++;
        lock_.Unlock();
    }

    void wait(){ // this should take up resources by decrementing the count
        while(lock_.Lock() == 1);
        count_--;
        lock_.Unlock();
    }

    private:    
    int count_;
    CriticalLock lock_;
};

// ===============================================================================================================

class NonBusySemaphore {
    public:

    NonBusySemaphore(int count) : count_(count) {}

// this should increment the count (freeing up resources)
    void signal(){ 
    while(lock_.Lock() == 1);
        if(!queue_.empty()) { // if queue is not empty
            Process p;
            p = queue_.dequeue();
            rq.enqueue(p);
        }
        else {
            count_++;
        }
        lock_.Unlock();
    }

// this should take up resources by decrementing the count
    void wait(){ 
        Process p; // this is the current process
        while(lock_.Lock() == 1);
        if(count_ == 0) {
            queue_.enqueue(p.getCurrentProcess());
            lock_.Unlock();
            yield(); // DNE in windows
        }
        else {
            count_--;
            lock_.Unlock();
        }
    }

    private:    
    int count_;
    Queue queue_;
    CriticalLock lock_;
};

// ===============================================================================================================

// the reader writer problem is where you have one writer (sometimes more) and multiple readers, but you want to be able to have as many readers as you want reading
// but not allow a writer to write as we are reading and not allow readers to read while you are writing.

class ReaderWriterProblem {
public:

    void Writer(int write) {
        sem->wait();
        value_ = write; // write
        sem->signal();
    }

    void Reader() {
        while(lock_.Lock() == 1);
        if(reader_count_ == 0) {
            sem->wait();
        }
        reader_count_++;
        lock_.Unlock();
        printf("%d", value_); // read (printf DNE in windows)
        while(lock_.Lock() == 1);
        if(--reader_count_ == 0) {
            sem->signal();
        }
        lock_.Unlock();
    }

private:
    NonBusySemaphore *sem = &NonBusySemaphore(1); // binary nonbusy semaphore
    int value_;
    int reader_count_ = 0;
    CriticalLock lock_;
};