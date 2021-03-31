#include <stdlib.h>

struct queue
{
    int size, front, rear;
    int *q;
    queue(int size) : size(size), front(0), rear(0) 
    {
        q = (int *)malloc(sizeof(int) * size);
    }

    ~queue() 
    {
        if(q != nullptr)
            free(q);
    }

    void push(int num)
    {
        q[rear] = num;
        rear = (rear + 1) % size;
    }

    int pop()
    {
        int res = q[front];
        front = (front + 1) % size;
        return res;
    }

    bool empty() {
        return front == rear;
    }
};