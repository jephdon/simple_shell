#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define PROMPT "#cisfun$ "
#define MAX_COMMAND_LENGTH 1024

/**
 * main - Simple Shell that reads and executes commands
 *
 * Return: Always 0
 */
int main(void)
{
	char command[MAX_COMMAND_LENGTH];
	pid_t pid;
	int status;
	char *argv[2], *envp[1];

	while (1)
	{
		/* Display the prompt */
		write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
		/* Read the command from the user */
		if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL)
		{
			/* Handle Ctrl+D (end of file) */
			write(STDOUT_FILENO, "\n", 1);
			break;
		}
		/* Remove the newline from the command */
		command[strcspn(command, "\n")] = '\0';
		/* Create a child process to run the command */
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			continue;
		}
		else if (pid == 0)
		{
			/* Child process: run the command */
			argv[0] = command;
			argv[1] = NULL;
			envp[0] = NULL;
			if (execve(command, argv, envp) == -1)
			{
				write(STDERR_FILENO, "./shell: No such file or directory\n", 35);
				exit(1);
			}
		}
		else
		{

			/* Parent process: wait for the child to finish */
			waitpid(pid, &status, 0);
		}
	}
	return (0);
}
