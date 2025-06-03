#include "main.h"

/**
 * find_command - Searches for the command in PATH directories
 * @command: The command to find
 *
 * Return: A copy of the full path, or NULL
 */
char *find_command(char *command)
{
	char *path = getenv("PATH");    /* Get the PATH variable */
	char *path_copy;
	char *dir;
	char full_path[1024];   /* Big enough to hold a path */
	struct stat st;         /* Used to check if a file exists */

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
