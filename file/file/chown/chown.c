#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	uid_t uid;
	struct passwd *pwd;
	char *endptr;

	if (argc != 3 || argv[1][0] == '\0') {
		fprintf(stderr, "%s <owner> <file>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	uid = strtol(argv[1], &endptr, 10); /* allow a numeric string */

	if (*endptr != '\0') {		/* was not pure numeric string */
		pwd = getpwnam(argv[1]); /* try get UID for username */
		if (pwd == NULL) {
			perror("getpwnam");
			exit(EXIT_FAILURE);
		}

		uid = pwd->pw_uid;
	}

	if (chown(argv[2], uid, -1) == -1) {
		perror("chown");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
