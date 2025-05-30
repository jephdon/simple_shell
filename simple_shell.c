#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

extern char **environ;		/* Global environment variable */

/**
 * main - Simple Shell that runs single-word commands
 *
 * Return: Always 0
 */
int main(int argc, char **argv)
{
	char *line = NULL;	/* Buffer for the command */
	char *args[2];
	const char *err_msg = "./shell: No such file or directory\n";
	size_t len = 0;		/* Size for getline */
	ssize_t n_read;		/* Characters read by getline */
	pid_t pid;		/* Process ID for fork */
	(void)argc;
	(void)argv;

	while (1)
	{
		write(STDOUT_FILENO, "#cisfun$ ", 9); /* Show the prompt */
		/* Read the user's command */
		n_read = getline(&line, &len, stdin);
		if (n_read == -1)
		{
			/* Exit on EOF (Ctrl+D) */
			write(STDOUT_FILENO, "\n", 1);
			break;
		}
		/* Remove the newline from the command */
		line[strcspn(line, "\n")] = '\0';
		/* Fork a new process */
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			continue;
		}
		else if (pid == 0)
		{
			/* Child process: run the command */
			args[0] = line;
			args[1] = NULL;
			if (execve(line, args, environ) == -1)
			{
				/* Print the error message */
				write(STDERR_FILENO, err_msg, strlen(err_msg));
				exit(1);
			}
		}
		else
		{
			/* Parent process: wait for child */
			wait(NULL);
		}
	}
	free(line);
	return (0);
}
