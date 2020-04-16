
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <tcl.h>
#include <expect.h>
#include <expect_tcl.h>

#define HOSTNAME "192.168.3.209"
#define PASSWORD "Semptian1q2w3e\r"


int main(int argc, char *argv[])
{
    Tcl_Interp *tcl;
    tcl = Tcl_CreateInterp();
    if (Expect_Init(tcl) != TCL_OK)
    {
        puts("failure");
        return 1;
    }
    exp_timeout = 3;

    int fd = exp_spawnl("ssh", "ssh", "root@192.168.3.134", 0);
    if(fd < 0)
    {
        puts("exp_spawnl");
        return 1;
    }
    
    fclose(stdout);

    int match, loop = 1, index = 1;
    while (loop)
    {
        switch (exp_expectl(fd, exp_glob,"*assword:", 1,
                                exp_glob, "yes/no", 2,
                                exp_end))
        {
        case 1:
            write(fd, "Semptian1q2w3e\r", 15);
            loop = 0;
            break;

        case 2:
            write(fd, "yes\r", 4);
            break;

        case EXP_EOF:
        case EXP_TIMEOUT:
            loop = 0;
            break;

        default:
            loop = 0;
            break;
        }
    }

    write(fd, "touch hello.txt\r", 16);
/*
    loop = 1;
    while (loop && loop < 2)
    {
        switch (exp_expectl(fd, exp_glob, "*#", 1, exp_end))
        {
        case 1:
            write(fd, "ls /home\r", 8);
            break;
            
        case EXP_EOF:
            loop = 0;
            break;
            
        default:
            loop = 0;
            break;
        }
        loop++;
    }

    switch (exp_expectl(fd, exp_exact, "Password:", 1, exp_exact, "*#", 2, exp_end))
    {
    case 1:
        write(fd, PASSWORD, sizeof(PASSWORD) - 1);
        break;
        
    case 2:
        write(fd, "exit\r", 5);
        break;
        
    case EXP_EOF:
        loop = 0;
        break;
        
    default:
        match = 0;
        break;
    }
 */
    match = exp_buffer_end - exp_buffer;
    FILE *outfd = fopen("./outlog", "w");
    if (outfd)
    {
        char tmp[20];
        snprintf(tmp, sizeof(tmp), "match=%u\n", match);
        fwrite(tmp, 1, strlen(tmp), outfd);
        fwrite(exp_buffer, 1, match, outfd);
        fflush(outfd);
        fclose(outfd);
    }

    Tcl_DeleteInterp(tcl);
    return 0;
}
