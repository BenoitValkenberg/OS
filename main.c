#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#define MYSYSCALL 349

int main(){
	
    char buf [10];
    memcpy(buf, "hi kernel", strlen("hi kernel") +1);
    printf("Return value : %d\n", syscall(MYSYSCALL, buf, 10));
	
	
	
	
	FILE* f = fopen("/dev/seqgen", "r+");
	// TODO Null check
	if(f == NULL){
		printf("SEQGEN Error : File pointer is null");
		return -1;
	}
		
	int fd = fileno(f);
	if(fd == -1){
		printf("SEQGEN Error : Stream number not valid");
		return -2;
	}
	
	if(ioctl(fd, 1) == -1){
		printf("SEQGEN Error : ioctl error");
		return -3;
	}
	
	char buf_ret[10];
	int amount = read(fd, buf, 9);
	buf[amount] = '\0';
	printf("Buffer size %d content : %s\n",amount, buf);
	fclose(f);
	
    return 0;
}
