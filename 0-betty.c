#include <unistd.h>

/**
 * main - Prints a message to the standard output using write.
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
	const char *msg = "Hello, Betty!\n";
	ssize_t len = 0;

	while (msg[len] != '\0')
		len++;
	if (write(STDOUT_FILENO, msg, len) == -1)
		return (1);
	return (0);
}
