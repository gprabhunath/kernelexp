#include "k_ns.h"

void check_fdarray(struct file **f)
{

	printk ("fd[0] = %p \n fd[1] = %p \n fd[2] = %p \n",
				f[0], f[1], f[2]);	

	return;
}

void check_rootfs (struct fs_struct *fs)
{	
	printk ("Root dentry = %p \n", fs->root.dentry);

	return;	
}

long parse_vfs (struct task_struct *tsk)
{
	pid_t pid = cmd;
	struct pid *spid;
	char *who[2] = {"parent", "child"};


	check_fdarray(tsk->files->fdt->fd);
	check_rootfs(tsk->fs);

#if 0
	printk ("Logs if pid = %d \n", pid);
	sb = tsk->fs->root.mnt->mnt_sb;

	printk (KERN_INFO "Super Block address of %s is %p \n", 
          (u_data ? who[0] : who[1]), sb);
#endif

	return 0;
}

long parse_mnt_namespace(struct mnt_namespace *mnt_ns)
{
	struct mount *mnt;
	char *s1 = NULL;
	char *s2 = NULL;

	printk ("Device \t\t mountpoint");	
	list_for_each_entry (mnt, &mnt_ns->list, mnt_list)
	{
		s1 = mnt->mnt_mountpoint ? mnt->mnt_mountpoint->d_iname : NULL;
//		s2 = mnt->mnt_mp ? mnt->mnt_mp->m_dentry->d_iname : NULL;
		printk (KERN_INFO "%-12s %s \n", mnt->mnt_devname, s1);
	}
	
	return 0;
} 
