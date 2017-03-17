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
