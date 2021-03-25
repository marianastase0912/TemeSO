#include "utils.h"


int main(int argc, char **argv)
{
	HashMap *hash = NULL;
	char buffer[NOLINES * MAX_LINE_SIZE] = {0};
	char pathFile[MAX_LINE_SIZE] = {0};
	FILE *out = NULL;
	char *inFile = NULL;
	char *outFile = NULL;
	int errCommandLine = -1;
	int ret = 0;
	int i = 0;
	char **includeDirectories = (char **)malloc(30 * sizeof(char *));

	for (i = 0; i < 30; i++)
		includeDirectories[i] = (char *)calloc(MAX_LINE_SIZE, sizeof(char));


	// initialize the hashmap, check if malloc successful
	hash  = initMap();

	if (hash == NULL)
		return ENOMEM;

	errCommandLine = command_line_arguments(hash, &inFile, &outFile,
			includeDirectories, argc, argv);

	if (errCommandLine != 0)
		return ENOENT;

	// Sends file to read and process, if there is no file, wait for stdin
	if (inFile != NULL) {
		strncpy(pathFile, inFile, MAX_LINE_SIZE);
		getPathfromInfile(pathFile);

		if (strlen(inFile) == (strlen(pathFile) - 1)) {
			ret = readFile(inFile, hash, buffer, "./",
				includeDirectories);
		} else {
			ret = readFile(inFile, hash, buffer, pathFile,
				includeDirectories);
		}
	} else {
		ret = readFile(NULL, hash, buffer, "./", includeDirectories);
	}

	// write in file, if there is no output file specified, write to stdout
	if (outFile != NULL && ret != ENOENT) {
		out = fopen(outFile, "w");
		if (out == NULL) {
			ret = ENOENT;
		} else {
			fprintf(out, "%s", buffer);
			fclose(out);
		}
	} else if (ret != ENOENT) {
		fprintf(stdout, "%s", buffer);
	}

	// free allocated memory
	for (i = 0; i < 30; i++)
		free(includeDirectories[i]);

	free(includeDirectories);
	freeMem(hash);
	return ret;
}
