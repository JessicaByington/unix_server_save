/******************************************************************************
 *  Author:         Jessica Maxey
 *  Filename:       device_driver.c 
 *  Date:           5/22/16
 *
 * ***************************************************************************/

#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/errno.h>
#include "myioctl.h"

//Module defines
#define BUFF_LEN 512
#define FIRST_MINOR 0
#define MINOR_CNT 1

static int MyCharOpen(struct inode * i, struct file * f);
static int MyCharClose(struct inode * i, struct file * f);
static ssize_t MyCharRead(struct file * f, char __user * buff, size_t len, loff_t * offset);
static ssize_t MyCharWrite(struct file * f, const char __user * buff, size_t len, loff_t * offset);
static int __init MyCharInit(void);
static void __exit MyCharExit(void);


//module defs
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jessica Maxey <jessica.maxey@oit.edu>");
MODULE_DESCRIPTION("CHAR driver and ioctl lab");

//vars
static dev_t major_num;
static struct cdev CharDevice;
static struct class * CharClass;
static char my_buff[BUFF_LEN] = {0};
static int full = 1;
static int empty = 0;

static int MyCharOpen(struct inode * i, struct file * f)
{
    printk(KERN_INFO "CHAR driver lab: Open Function\n");
    return 0;
}

static int MyCharClose(struct inode * i, struct file * f)
{
    printk(KERN_INFO "CHAR driver lab: Close Function\n");
    return 0;
}

static long my_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
    query_arg_t q; 

    printk(KERN_INFO "myioctl driver: ioctl function called\n"); 

    switch(cmd)
    {
        case QUERY_GET_VARIABLES:
            {
                //get status, 1 if data, 0 if empty and NULL
                if(my_buff[sizeof(my_buff) - 1] != '\0')
                    q.full = full;
                else
                    q.full = empty;
            }
            if (copy_to_user((query_arg_t *)arg, &q, sizeof(q)))
            {
                return -EACCES;
            }
            break;
        case QUERY_CLR_VARIABLES:
            memset(my_buff, '\0', sizeof(my_buff));
            q.full = empty;
            break;
        case QUERY_SET_VARIABLES:
            if (copy_from_user(my_buff, (query_arg_t *)arg, sizeof(my_buff)))
            {
                return -EACCES;
            }
            //fill with A's
            memset(my_buff, 'A', sizeof(my_buff));
            q.full = full;
            break;
        default:
            return -EINVAL;
    }
    return 0; 
}

static struct file_operations fops = 
{
    .owner = THIS_MODULE,
    .open = MyCharOpen,
    .release = MyCharClose,
    .read = MyCharRead, 
    .write = MyCharWrite,
    .unlocked_ioctl = my_ioctl
};

static ssize_t MyCharRead(struct file * f, char __user * buff, size_t len, loff_t * offset)
{
    printk(KERN_INFO "CHAR driver lab: Read Function\n");


    if(*offset == 0)
    {
        if(copy_to_user(buff, my_buff, sizeof(my_buff)) != 0)
            return -EFAULT;
        else 
        {        
            (*offset)++;
            return len;
        }
    }
    else
        return 0;
}

static ssize_t MyCharWrite(struct file * f, const char __user * buff, size_t len, loff_t * offset)
{
    printk(KERN_INFO "CHAR driver lab: Write Function\n");

    if(copy_from_user(my_buff, buff, sizeof(my_buff)) != 0)
        return -EFAULT;
    else  
        return len;
}


static int __init MyCharInit(void)
{
    printk(KERN_INFO "mychar Driver: Registered\n");

    if(alloc_chrdev_region(&major_num, FIRST_MINOR, MINOR_CNT, "Jessica"))
    {
        return -1;
    }

    if((CharClass = class_create(THIS_MODULE, "chardrv")) == NULL)
    {
        cdev_del(&CharDevice);
        unregister_chrdev_region(major_num, MINOR_CNT);

        return -1;
    }

    if(device_create(CharClass, NULL, major_num, NULL, "mychar") == NULL)
    {
        class_destroy(CharClass);
        cdev_del(&CharDevice);
        unregister_chrdev_region(major_num, MINOR_CNT);

        return -1;
    }

    cdev_init(&CharDevice, &fops);

    if(cdev_add(&CharDevice, major_num, MINOR_CNT) == -1)
    {
        device_destroy(CharClass, major_num);
        class_destroy(CharClass);
        unregister_chrdev_region(major_num, MINOR_CNT);

        return -1;
    }

    return 0;

}

static void __exit MyCharExit(void)
{
    cdev_del(&CharDevice);
    device_destroy(CharClass, major_num);
    class_destroy(CharClass);
    unregister_chrdev_region(major_num, MINOR_CNT);

    printk(KERN_INFO "CHAR driver lab: Unregistered\n");
}

module_init(MyCharInit);
module_exit(MyCharExit);
