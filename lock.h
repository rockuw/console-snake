#ifndef _LOCK_H_
#define _LOCK_H_

class Mutex
{
  public:
    Mutex()
    {
        pthread_mutex_init(&lock_, NULL);
    }

    void Lock()
    {
        pthread_mutex_lock(&lock_);
    }
    
    void UnLock()
    {
        pthread_mutex_unlock(&lock_);
    }

  private:
    pthread_mutex_t lock_;
};

class ScopedLock
{
  public:
    ScopedLock(Mutex* mutex): mutex_(mutex)
    {
        mutex_->Lock();
    }
    
    ~ScopedLock()
    {
        mutex_->UnLock();
    }

  private:
    Mutex* mutex_;
};


#endif /* _LOCK_H_ */
