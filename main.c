#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#define MYSYSCALL 272

int main(){
    char *buf [10];
    memcpy(buf, "hi kernel", strlen("hi kernel") +1);
    syscall(MYSYSCALL, buf, 10);
    return 0;
}