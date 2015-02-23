#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#define MYSYSCALL 272

int main(){
	
    char *buf [10];
    memcpy(buf, "hi kernel", strlen("hi kernel") +1);
    syscall(MYSYSCALL, buf, 10);
	
	
	/*FILE* f = fopen("/dev/seqgen", "r+");
	// TODO Null check
	if(f == NULL){
		printf("SEQGEN Error : File pointer is null");
		return -1;
	}
		
	int fd = fileno(f);
	if(f == NULL){
		printf("SEQGEN Error : File pointer is null");
		return -2;
	}
	
	ioctl(fd, 0, 1);
	// TODO Return check
	
	char buf[21];
	int amount = read(fd, buf, 20);
	buf[amount] = '\0';
	printf("Buffer size %d content : %s\n",amount, buf);
	fclose(f);*/
	
    return 0;
}