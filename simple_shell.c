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
	char *line = NULL;	/* Buffer for the command input */
	char *args[2];		/* Arguments array for execve */
	size_t len = 0;		/* Length for getline */
	ssize_t n_read;		/* Characters read by getline */
	pid_t pid;		/* Process ID for fork */
	(void)argc;		/* Avoid unused parameter warning */

	while (1)
	{
		/* Display prompt */
		write(STDOUT_FILENO, "#cisfun$ ", 9);
		/* Read the user's command */
		n_read = getline(&line, &len, stdin);
		if (n_read == -1)
		{
			/* Handle EOF (Ctrl+D) */
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}
		/* Remove trailing newline from the command */
		line[strcspn(line, "\n")] = '\0';
		/* Skip is input is empty or only spaces */
		if (strlen(line) == 0 || strspn(line, " \t") == strlen(line))
			continue;
		/* Create child process */
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
			args[1] = NULL;	/* No arguments allowed */
			if (execve(line, args, environ) == -1)
			{
				/* Print the error message */
				write(STDERR_FILENO, argv[0], strlen(argv[0]));
				write(STDERR_FILENO, ": No such file or directory\n", 28);
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
