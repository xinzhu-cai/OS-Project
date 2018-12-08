#include <linux/linkage.h>
#include <linux/kernel.h>
#include "define_system_call.h"

asmlinkage long sys_helloworld(char * msg)
{
    char buf[256];
    long copied = strncpy_from_user(buf,msg,sizeof(buf));
    if(copied<0 || copied == sizeof(buf))
       return -EFAULT;
    printk(KERN_EMERG "hello world\n");
    printk(KERN_EMERG "hello %s \n",buf);
    return 1;
}