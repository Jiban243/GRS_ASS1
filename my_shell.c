#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>

#define BUF_SIZE 1024
#define ARG_SIZE 128

void welcome()
{
    printf("\n-- My Shell --\nType 'exit' to quit.\n");
}

int get_input(char *buf)
{
    char *line = readline("\nshell> ");
    if (line && strlen(line) > 0)
    {
        add_history(line);
        strcpy(buf, line);
        free(line);
        return 0;
    }
    else
    {
        if (line)
            free(line);
        return 1;
    }
}

void parse(char *buf, char **args, char **in_file, char **out_file)
{
    int idx = 0;
    char *tok = strtok(buf, " \t\n");
    *in_file = NULL;
    *out_file = NULL;

    while (tok != NULL && idx < ARG_SIZE - 1)
    {
        if (strcmp(tok, "<") == 0)
        {
            tok = strtok(NULL, " \t\n");
            if (tok != NULL)
                *in_file = tok;
        }
        else if (strcmp(tok, ">") == 0)
        {
            tok = strtok(NULL, " \t\n");
            if (tok != NULL)
                *out_file = tok;
        }
        else
        {
            args[idx++] = tok;
        }
        tok = strtok(NULL, " \t\n");
    }
    args[idx] = NULL;
}

void run_cmd(char **args, char *in_file, char *out_file)
{
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("err fork");
    }
    else if (pid == 0)
    {
        if (in_file)
        {
            int in_fd = open(in_file, O_RDONLY);
            if (in_fd < 0)
            {
                perror("err open in");
                exit(1);
            }
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);
        }
        if (out_file)
        {
            int out_fd = open(out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (out_fd < 0)
            {
                perror("err open out");
                exit(1);
            }
            dup2(out_fd, STDOUT_FILENO);
            close(out_fd);
        }
        if (execvp(args[0], args) == -1)
        {
            perror("err exec");
        }
        exit(1);
    }
    else
    {
        waitpid(pid, NULL, 0);
    }
}

void show_tree()
{
#if defined(__linux__)
    if (system("pstree -p $(ps -o ppid= -p $$ | tail -1) | grep -v '?'") < 0)
    {
        perror("err tree");
    }
#elif defined(__APPLE__)
    if (system("ps -ax -o pid,ppid,command | grep -E 'my_shell|fact'") < 0)
    {
        perror("err tree");
    }
#else
    printf("tree not supported\n");
#endif
}

int run_builtin(char **args)
{
    if (strcmp(args[0], "exit") == 0)
    {
        exit(0);
    }
    else if (strcmp(args[0], "cd") == 0)
    {
        if (args[1] == NULL)
        {
            fprintf(stderr, "cd needs dir\n");
        }
        else if (chdir(args[1]) != 0)
        {
            perror("err cd");
        }
        return 1;
    }
    else if (strcmp(args[0], "tree") == 0)
    {
        show_tree();
        return 1;
    }
    return 0;
}

int main()
{
    char buf[BUF_SIZE];
    char *args[ARG_SIZE];
    char *in_file, *out_file;

    welcome();

    while (1)
    {
        if (get_input(buf))
            continue;
        parse(buf, args, &in_file, &out_file);
        if (!run_builtin(args))
        {
            run_cmd(args, in_file, out_file);
        }
    }

    return 0;
}
