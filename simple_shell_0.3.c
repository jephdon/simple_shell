#include "main.h"

/**
 * main - Simple Shell that runs single-word commands
 *
 * Return: Always 0
 */
int main(int argc, char **argv)
{
	char *line = NULL;	/* Buffer for the command input */
	char *full_path;
	char *args[20];		/* Arguments array for execve */
	size_t len = 0;		/* Length for getline */
	ssize_t n_read;		/* Characters read by getline */
	pid_t pid;		/* Process ID for fork */
	int i;
	struct stat st;
	(void)argc;		/* Avoid unused parameter warning */

	while (1)	/* Keep shell running */
	{
		if (isatty(STDIN_FILENO))/* Display prompt if interactive */
			write(STDOUT_FILENO, "$ ", 2);
		n_read = getline(&line, &len, stdin); /* Read user input */
		if (n_read == -1)	/* End of input (Ctrl+D) */
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}
		line[strcspn(line, "\n")] = '\0'; /* Remove newline */
		if (strlen(line) == 0) /* Skip empty lines */
			continue;
		/* Check if the user typed "exit" */
		if (strcmp(line, "exit") == 0)
		{
			free(line);
			return (0);
		}
		/* Check if the user typed "env" */
		if (strcmp(line, "env") == 0)
		{
			print_env();	/* Print the env variables */
			continue;	/* Go back to the prompt */
		}
		/* Split input into command and arguments */
		i = 0;
		args[i] = strtok(line, " ");
		while (args[i] != NULL && i < 19)
		{
			i++;
			args[i] = strtok(NULL, " ");
		}
		/* Check if command exists before forking */
		if (strchr(args[0], '/') != NULL) /* if a full path */
		{
			if (stat(args[0], &st) == 0 && (st.st_mode & S_IXUSR))
				full_path = args[0]; /* Use it directly */
			else
				full_path = NULL; /* Doesn't exist */
		}
		else /* No /, search PATH */
		{
			full_path = find_command(args[0]);
		}
		if (full_path == NULL) /* Command not found */
		{
			write(STDERR_FILENO, argv[0], strlen(argv[0]));
			write(STDERR_FILENO, ": 1: ", 5);
			write(STDERR_FILENO, args[0], strlen(args[0]));
			write(STDERR_FILENO, ": not found\n", 12);
			continue; /* Skip fork */
		}
		/* Fork and run the command */
		pid = fork();
		if (pid == 0) /* Child process */
		{
			if (execve(full_path, args, environ) == -1)
			{
				perror("execve"); /* Print error if execve fails */
				exit(1);
			}
		}
		else if (pid > 0) /* Parent process */
		{
			wait(NULL); /* Wait for child to finish */
		}
		else /* Fork failed */
		{
			perror("fork");
		}
		if (full_path != args[0])
			free(full_path);
	}
	free(line);
	return (0);
}
