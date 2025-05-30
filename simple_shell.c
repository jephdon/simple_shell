#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define PROMPT "#cisfun$ "
#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGS 64

/**
 * trim_spaces - Removes leading and trailing spaces from a string
 * @str: The sring to trim
 */

void trim_spaces(char *str)
{
	char *end;

	/* Trim leading spaces */
	while (*str == ' ' || *str == '\t')
		str++;
	/* Trim trailing spaces */
	end = str + strlen(str) - 1;
	while (end > str && (*end == ' ' || *end == '\t'))
		end--;
	/* Null-terminate the trimmed string */
	*(end + 1) = '\0';
}

/**
 * main - Simple Shell that reads and executes commands
 *
 * Return: Always 0
 */
int main(void)
{
	char command[MAX_COMMAND_LENGTH];
	pid_t pid;
	int status, i;
	char *args[MAX_ARGS];
	char *token;
	char *envp[1];

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
		/* Trim leading and trainling spaces */
		trim_spaces(command);
		/* Skip empty or space-only commands */
		if (strlen(command) == 0)
			continue;
		/* Create a child process to run the command */
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			continue;
		}
		else if (pid == 0)
		{
			/* Child process: parse and run the command */
			i = 0;
			token = strtok(command, " ");
			while (token != NULL && i < MAX_ARGS - 1)
			{
				args[i++] = token;
				token = strtok(NULL, " ");
			}
			args[i] = NULL; /* Null-terminate the argument list */
			/* Set up environment and execute the command */
			envp[0] = NULL;
			if (execve(args[0], args, envp) == -1)
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
