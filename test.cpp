#ifndef INC__TEST__CPP
#define INC__TEST__CPP

#include "Queue.h"

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/
typedef struct _test_t{
    char *name;
    int  id;
    double val;
} test_t;
void test_disp(test_t a){
    char info[100] = {'\0'};
    printf("  => name: %s\n"
           "  => id  : %0d\n"
           "  => val : %.2f\n",
    a.name, a.id, a.val);
};
int test_cmp(test_t a, test_t b) {
    return (a.id - b.id);
};
int main(){
    HWPRINT(PL_NONE, "Start main.\n");
    Queue<test_t> test_q("test_q", 10);
    test_q.entry_disp = test_disp;
    test_q.entry_cmp = test_cmp;
    for (int i = 0; i < 10; i++) {
        test_q.PushBack({(char *)"t0", i, i*1.4});
    } 
    test_q.Display();
    for (int i = 0; i < 10; i++) {
        int id = rand() % 10;
        int get_id = test_q.Find((test_t){NULL, id, 0.0});
        test_t a = test_q.Get(get_id);
        HWPRINT(PL_NONE, "fetch %0d", a.id);

    } 

    return 0;
}

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*INC__TEST__CPP*/