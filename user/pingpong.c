#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int p1[2]; // child -> parent
    int p2[2]; // parent -> child
    pipe(p1);
    pipe(p2);

    int pid = fork();

    if (pid < 0)
    {
        fprintf(2, "fork failed\n");
        exit(1);
    }
    else if (pid == 0)
    {                 // child
        close(p1[0]); // child doesn't read from p1
        close(p2[1]); // child doesn't write to p2

        char buf[1];
        read(p2[0], buf, 1);
        printf("%d: received ping\n", getpid());
        write(p1[1], buf, 1);

        close(p2[0]);
        close(p1[1]);
        exit(0);
    }
    else
    {                 // parent
        close(p1[1]); // parent doesn't write to p1
        close(p2[0]); // parent doesn't read from p2

        char buf[1] = {'a'};
        write(p2[1], buf, 1);
        read(p1[0], buf, 1);
        printf("%d: received pong\n", getpid());

        close(p2[1]);
        close(p1[0]);
        wait(0);
        exit(0);
    }
}