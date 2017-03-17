#include "ns.h"

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
#if 0
	status = mount ("/dev/sdb1", "/media/usb", "vfat", 0, NULL);
	if (status < 0) {
		perror ("mount failed");
		exit (1);
	}
#endif
	
	return;
}
