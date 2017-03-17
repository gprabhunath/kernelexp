#define _GNU_SOURCE
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sched.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/mount.h>

#define DEVFILE	"/dev/newns"
#define STACK_SIZE (8 * 1024)
#define NS_CLONE_FLAGS (CLONE_NEWUTS | CLONE_NEWNS | CLONE_NEWIPC | \
				CLONE_NEWPID | CLONE_NEWNET | SIGCHLD)
#define UNSHARE_FLAGS (CLONE_NEWUTS | CLONE_NEWNS | CLONE_NEWIPC | \
				CLONE_NEWPID | CLONE_NEWNET)
unsigned char stack[STACK_SIZE];
int fd;

void openPseudo_cdev(void)
{
	fd = open(DEVFILE, O_RDWR);
	if (fd < 0)
	   perror("open failed");

	return;
}

void ioctlPseudo_cdev(int n)
{
	int ret;
	ret = ioctl (fd, getpid(), n);
	if (ret < 0)
		perror ("ioctl failed");
	
	return;
	
}

void do_mounts(void)
{
	int status;

	status = mount ("", "/", "", MS_REC|MS_PRIVATE, NULL);
	if (status < 0) {
		perror ("mount PVT failed");
		exit (1);
	}
#if 0
	status = mount ("/opt/lxc1", "/", NULL, MS_REC|MS_BIND, NULL);
	if (status < 0) {
		perror ("mount BIND failed");
		exit (1);
	}
#endif
	status = mount ("proc", "/proc", "proc", 
					MS_NOSUID|MS_NODEV|MS_NOEXEC, NULL);
	if (status < 0) {
		perror ("mount proc failed");
		exit (1);
	}

	status = mount ("/dev/sdb1", "/media/usb", "vfat", 0, NULL);
	if (status < 0) {
		perror ("mount failed");
		exit (1);
	}
}
int child_start_fn(void *p)
{
	int status;
	
	sethostname ("my_ns", 8);
	do_mounts();	
	ioctlPseudo_cdev(1);
	execlp("bash", "bash");
//	system("ls /media/usb");

	getchar();	
	return 0;	
}

void nextgen(void)
{
	int status;

	status = clone (child_start_fn, stack+STACK_SIZE, NS_CLONE_FLAGS, NULL);
	if (status < 0) {
		perror ("clone failed");
		exit (1);
	}

	return;
}

int child_detach (void *p)
{	
	int status;
	status = unshare (UNSHARE_FLAGS);
	if (status < 0) {
		perror ("unshare failed");
		exit (1);
	}
	ioctlPseudo_cdev(1);

	return 0;	
}
void detach(void)
{
	int status;
	status = clone (child_detach, stack+STACK_SIZE, SIGCHLD, NULL);
	if (status < 0) {
		perror ("clone failed");
		exit (1);
	}
}

int main(void)
{

	openPseudo_cdev();
	ioctlPseudo_cdev(0);
//	nextgen();
	detach();

	wait(NULL);	

	return 0;
}
