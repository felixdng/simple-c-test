#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <tcl.h>
#include <expect.h>
#include <expect_tcl.h>

#define HOST_IP     "192.168.3.134"
#define USR_NAME    "root"
#define USR_PWD     "Semptian1q2w3e"

#define SSH_ARG_NUM     2
#define SSH_ARG_LEN     50

static int _exp_init(Tcl_Interp *ptcl)
{
    exp_timeout = 100;
    ptcl = Tcl_CreateInterp();
    if (Expect_Init(ptcl) != TCL_OK)
    {
        perror("exp init failed!\n");
        return 1;
    }
    return 0;
}

static void _exp_exit(Tcl_Interp *tcl)
{
    if (tcl)
    {
        Tcl_DeleteInterp(tcl);
        tcl = NULL;
    }
}

static int _exp_ssh_login(const char *host, const char *name, const char *passwd)
{
    if (NULL == host || NULL == name || NULL == passwd)
        return -1;
    
    int i = 0;
    int fd = -1;
    char args[SSH_ARG_NUM][SSH_ARG_LEN] = {};
    
    for (i = 0; i < SSH_ARG_NUM; i++)
    {
        switch (i)
        {
        case 0:
            snprintf(args[i], sizeof(args[i]), "%s", "ssh");
            break;
            
        case 1:
            snprintf(args[i], sizeof(args[i]), "%s@%s", name, host);
            break;
        }
    }
    
    fd = exp_spawnl("ssh", args[0], args[1], (char *)0);
    if (fd < 0)
    {
        perror("failed to ssh\n");
        return 1;
    }
    
    fclose(stdout);
    
    char pwd[30] = {0};
    snprintf(pwd, sizeof(pwd), "%s\r", passwd);
    int loop = 1;
    while (loop)
    {
        int result = exp_expectl(fd, exp_glob, "password:", 1,
                                     exp_glob, "yes/no",    2,
                                     exp_end);
        switch (result)
        {
        case 1:
            write(fd, pwd, strlen(pwd));
            break;
            
        case 2:
            write(fd, "yes\r", strlen("yes\r"));
            break;
            
        case EXP_EOF:
            perror("eof\n");
            return 1;
        case EXP_TIMEOUT:
            perror("timeout\n");
            return 1;
            
        default:
            loop = 0;
            perror("ssh login success.\n");
            break;
        }
                                 
    }
}

int main(int argc, char *argv[])
{
    Tcl_Interp *tcl = NULL;
    if (0 != _exp_init(tcl))
        return -1;
        
    if (0 != _exp_ssh_login(HOST_IP, USR_NAME, USR_PWD))
        goto err_exit0;
        
    _exp_exit(tcl);
    return 0;
    
err_exit0:
    _exp_exit(tcl);
    return 1;
}

