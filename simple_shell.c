#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

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
	char *line = NULL;	/* Buffer for the command */
	char *env[1];
	char *args[2];
	char *err_msg;
	size_t len = 0;	/* Size for getline */
	ssize_t n_read;		/* Characters read by getline */
	pid_t pid;		/* Process ID for fork */

	while (1)
	{
		/* Display the prompt */
		write(STDOUT_FILENO, "#cisfun$ ", 9);
		/* Read the User's command */
		n_read = getline(&line, &len, stdin);
		if (n_read == -1)
		{
			/* Exit on EOF (Ctrl+D) */
			write(STDOUT_FILENO, "\n", 1);
			break;
		}
		/* Remove the newline from the command */
		line[strcspn(line, "\n")] = '\0';
		/* Trim leading and trainling spaces */
		trim_spaces(line);
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
			args[0] = line;
			args[1] = NULL;
			env[0] = NULL;
			if (execve(line, args, env) == -1)
			{
				err_msg = "./shell: No such file or directory\n";
				write(STDERR_FILENO, err_msg, 35);
				exit(1);
			}
		}
		else
		{
			/* Parent process: wait for the child*/
			wait(NULL);
		}
	}
	free(line);
	return (0);
}
