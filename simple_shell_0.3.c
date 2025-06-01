#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

extern char **environ;		/* Global environment variable */

/**
 * find_command - Searches for the command in PATH directories
 * @command: The command to find
 *
 * Return: A copy of the full path, or NULL
 */
char *find_command(char *command)
{
	char *path = getenv("PATH");	/* Get the PATH variable */
	char *path_copy;
	char *dir;
	char full_path[1024];	/* Big enough to hold a path */
	struct stat st;		/* Used to check if a file exists */

	if (path == NULL)
		return (NULL);
	path_copy = strdup(path); /* Make a copy to avoid messing up PATH */
	dir = strtok(path_copy, ":"); /* Split PATH by ":" */
	while (dir != NULL)
	{
		/* Build path like /bin/ls */
		snprintf(full_path, sizeof(full_path), "%s/%s",dir, command);
		if (stat(full_path, &st) == 0 && (st.st_mode & S_IXUSR))
		{
			free(path_copy);
			return (strdup(full_path)); /* Found */
		}
		dir = strtok(NULL, ":"); /* Move to next directory */
	}
	free(path_copy);
	return (NULL); /* Command not found */
}

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
			write(STDOUT_FILENO, ":) ", 3);
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
