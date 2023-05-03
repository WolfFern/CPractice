#ifndef INC__QUEUE__H
#define INC__QUEUE__H
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include "Base.h"

template <typename T>
class Queue{
    public:
    const char *name;
    Queue(const char *set_name);
    Queue(const char *set_name, int set_fix_size);
    ~Queue();

    // function for entry add
    int Insert(T entry, int id);
    int PushFront(T entry) {Insert(entry, 0); return 0;}
    int PushBack(T entry) {Insert(entry, use_size); return (use_size - 1);}
    // function for entry del
    T Fetch(int id);
    T PopFront() {return Fetch(0);};
    T PopBack() {return Fetch(use_size - 1);};
    int Delete(int id) {Fetch(id); return 0;};
    // function for entry modify
    int Set(T entry, int id) {Fetch(id); return Insert(entry, id);};
    // function for entry lookup
    T Get(int id);
    int Find(T entry, int start_id, int end_id);
    int Find(T entry, int start_id) {return Find(entry, start_id, use_size);};
    int Find(T entry){return Find(entry, 0, use_size);};
    int Size(){return use_size;};
    // misc
    void (*entry_disp)(T entry);
    void Display(int id);
    void Display();
    int (*entry_cmp)(T entry0, T entry1);

    private:
    int fix_size;
    int use_size;
    T *queue_entry;
};

template<typename T>
Queue<T>::Queue(const char *set_name){
    name = set_name;
    fix_size = 0;
    use_size = 0;
    queue_entry = NULL;
    entry_disp = NULL;
    entry_cmp = NULL;
};
template<typename T>
Queue<T>::Queue(const char *set_name, int set_fix_size){
    name = set_name;
    fix_size = set_fix_size;
    use_size = 0;
    queue_entry = new T[fix_size];
    entry_disp = NULL;
    entry_cmp = NULL;
};
template<typename T>
Queue<T>::~Queue(){
    fix_size = 0;
    use_size = 0;
    delete []queue_entry;
    queue_entry = NULL;
    entry_disp =  NULL;
    entry_cmp = NULL;
}
template<typename T>
int Queue<T>::Insert(T entry, int id) {
    if (fix_size > 0) {
        if (0 <= id && id < fix_size) {
            for (int i = use_size; i > id; i--) {
                memcpy(&queue_entry[i], &queue_entry[i-1], sizeof(T));
            }
            memcpy(&queue_entry[id], &entry, sizeof(T));
        } else {
            HWPRINT(PL_FATAL, "%s access overflow, fix_size=%0d, use_size=%0d, insert_id=%0d.", 
              name, fix_size, use_size, id);
        }
    } else {
        HWPRINT(PL_WARNING, "waiting for add dynamic queue imp...");
    }
    use_size += 1;
    return id;
};
template<typename T>
T Queue<T>::Fetch(int id) {
    T fetch_entry;
    if (fix_size > 0) {
        if (0 <= id && id < fix_size) {
            memcpy(&fetch_entry, &queue_entry[id], sizeof(T));
            for (int i = id; i < use_size - 1; i++) {
                memcpy(&queue_entry[i], &queue_entry[i+1], sizeof(T));
            }
        } else {
            HWPRINT(PL_FATAL, "%s access overflow, fix_size=%0d, use_size=%0d, fetch_id=%0d.",
              name, fix_size, use_size, id);
        }
    } else {
        HWPRINT(PL_WARNING, "waiting for add dynamic queue imp...");
    }
    use_size -= 1;
    return fetch_entry;
};
template<typename T>
T Queue<T>::Get(int id) {
    T get_entry;
    if (0 <= id && id < use_size) {
        memcpy(&get_entry, &queue_entry[id], sizeof(T));
    } else {
        HWPRINT(PL_FATAL, "%s acceess overflow, use_size=%0d, get_id=%0d", name, use_size, id);
    }
    return get_entry;
};
template<typename T>
int Queue<T>::Find(T entry, int start_id, int end_id) {
    int id = -1, res = -1;
    for (int i = start_id; i < end_id; i++) {
        if (NULL == entry_cmp) {
            res = memcmp(&entry, &queue_entry[i], sizeof(T));
        } else {
            res = entry_cmp(entry, queue_entry[i]);
        }
        if (0 == res) {
            id = i;
            break;
        }
    }
    return id;
};
template<typename T>
void Queue<T>::Display(int id) {
    assert(entry_disp != NULL);
    entry_disp(Get(id));
};
template<typename T>
void Queue<T>::Display() {
    assert(entry_disp != NULL);
    for (int id = 0; id < use_size; id++) {
        printf("%s entry[%0d]:\n", name, id);
        entry_disp(Get(id));
    }
};

#endif /*INC__QUEUE__H*/