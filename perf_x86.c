#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char **argv)
{
        uint32_t cycle_start = 0;
        uint32_t cycle_end   = 0;
        uint32_t event_start = 0;
        uint32_t event_end   = 0;

        if (argc <= 2) {
                printf("Usage: %s event_num_dec(check perf_lib.h) program\n", argv[0]);
                exit(0);
        };
        int event_num = atoi(argv[1]);
	printf("Event number = 0x%x\n", event_num);
        char  *program_argv[64] = {0};
        printf("Load program : ");
        for (int i = 2; i < argc; i++) {
                program_argv[i-2] = argv[i];
                printf("%s ", program_argv[i-2]);
        }
        printf("\n");

        /*Spawn a child to run the program.*/
        pid_t pid=fork();
        if (pid==0) { /* child process */
                execvp(argv[2], program_argv);
                exit(127); /* only if execv fails */
        }
        else { /* pid!=0; parent process */
                waitpid(pid,0,0); /* wait for child to exit */
        }

        /* Print PMU counter */
        printf("Cycle Count: %u\n", cycle_end - cycle_start);
        printf("Event %x Count: %u\n", event_num, event_end - event_start);

        return 0;
}
