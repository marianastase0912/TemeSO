#include "utils.h"

/*
 *adds in map the value given as argument
 * checks for the format -Dkey=val -D key=val
 * tokenizes the string and stores it in map
 */
void addDefine(HashMap *map, char *arg)
{
	char *key = NULL;
	char *value = NULL;
	char *token = NULL;

	if (strncmp(arg, "-D", 2) == 0)
		arg += 2;

	token = strtok(arg, "=");
	key = token;
	token = strtok(NULL, "=");

	if (token == NULL)
		token = "";

	value = token;
	insert(map, key, value);
}

/*
 * function to check if a string contains a bracket
 * returns the index if true
 */
int hasOpenBracket(char *line, int index)
{
	int i = 0;
	int size = 0;

	size = strlen(line);

	for (i = 0; i < size; i++) {
		if (line[i] == ')') {
			index = i;
			return index;
		}
	}
	return -1;
}

// function to replace the defined variables in file
void replaceVariables(HashMap *map, char *line)
{
	int lineSize = 0;
	int indexBracket = 0;
	int isReplaced = 0;
	char *token = NULL;
	char newLine[MAX_LINE_SIZE] = {0};
	int indexToAppendBracket = 0;

	lineSize = strlen(line);
	// get the initial index for closing bracket
	indexBracket = hasOpenBracket(line, indexBracket);

	// append tab to the processed line
	if (line[0] == ' ')
		strncat(newLine, "    ", MAX_LINE_SIZE - 1);

	// tokenize for variables
	token = strtok(line, " )");

	/*
	 * Check if token is a key in the map
	 * Replace the variable with the value from the map in the new line
	 * Add spaces missing from the tokenizing process
	 * Append to the new line
	 */
	while (token != NULL) {
		if (findInMap(map, token) != NULL) {
			isReplaced = 1;
			strncat(newLine, getValue(map, token), MAX_LINE_SIZE);
			strncat(newLine, " ", MAX_LINE_SIZE - 1);
		} else {
			strncat(newLine, token, MAX_LINE_SIZE);
			if (indexBracket != -1 && isReplaced == 0)
				newLine[indexBracket] = ')';


			if (token[strlen(token) - 1] != '\n')
				strncat(newLine, " ", MAX_LINE_SIZE - 1);
		}
		token = strtok(NULL, " )");
	}
	// add the bracket lost in the tokenizing process at the new index
	if (isReplaced == 1 && indexBracket != -1) {
		indexToAppendBracket = indexBracket -
				(lineSize - strlen(newLine));
		newLine[indexToAppendBracket] = ')';
	}
	strncpy(line, newLine, MAX_LINE_SIZE);
}

/*
 * function to check if the value already exists in map as key
 * if there is update the value
 */
void chechIfDefinedBefore(HashMap *map, char *value)
{
	char copyValue[SIZEVALUE] = {0};
	char *tokenizeValue = NULL;
	int isInMap = 0;

	tokenizeValue = strtok(value, " ");

	while (tokenizeValue != NULL) {
		// if value is in map, copy
		if (findInMap(map, tokenizeValue) != NULL) {
			isInMap = 1;
			strncat(copyValue, getValue(map, tokenizeValue), MAX_LINE_SIZE);
			strncat(copyValue, " ", MAX_LINE_SIZE - 1);
		} else {
			strncat(copyValue, tokenizeValue, MAX_LINE_SIZE);
			if (isInMap == 1)
				strncat(copyValue, " ", MAX_LINE_SIZE - 1);
		}
		tokenizeValue = strtok(NULL, " ");
	}

	strncpy(value, copyValue, MAX_LINE_SIZE);
}

/*
 * fuction to handle the define directive
 * tokenize the lines if there is a multiline define
 * check if the value is already a key in the map
 * if yes, update it, then insert the new pair in map
 */
void defineDirective(char *line, HashMap *hash, FILE *in)
{
	int lineSize = strlen(line);
	char *tokMultiDefine = NULL;
	char *key = NULL;
	char value[SIZEVALUE] = {0};
	int sizeNewLine = 0;
	char newLine[MAX_LINE_SIZE];
	char *token = NULL;

	token = strtok(line, " ");
	token = strtok(NULL, " ");
	key = token;

	if (token[strlen(token) - 1] != '\n')
		token = strtok(NULL, "\n");

	/*
	 * check for multiline define (line ending with '\')
	 * if there is a multiline define, read from file
	 * the file descriptor will change
	 * in is passed as argument to the function
	 * insert the key and value from define in map
	 */
	if (token[strlen(token) - 1] == '\\')
		strncpy(value, token, strlen(token) - 1);
	else {
		// printf("yu there?%s\n", line);
	// printf("yu there?%s====%s==\n", key, value);

		strncpy(value, token, strlen(token));
	}
	if (line[lineSize - 2] == '\\') {
		while (1) {
			fgets(newLine, MAX_LINE_SIZE, in);
			sizeNewLine = strlen(newLine);
			if (newLine[sizeNewLine - 2] != '\\') {
				tokMultiDefine = strtok(
					newLine, "\t\n");
				strncat(value,
					tokMultiDefine,
					MAX_LINE_SIZE);
				break;
			}
			tokMultiDefine = strtok(
				newLine, "\t\\");
			strncat(value,
			tokMultiDefine,
			MAX_LINE_SIZE);
			strncat(value, " \0",
				MAX_LINE_SIZE - 1);
		}
	}
	chechIfDefinedBefore(hash, value);
	insert(hash, key, value);
}

// delete entry from define helper function for #undef
void deleteDefine(char *line, HashMap *map)
{
	char *token = NULL;

	token = strtok(line, " ");
	token = strtok(NULL, " \n");
	if (findInMap(map, token) != NULL)
		delete(map, token);
}

/*
 * function to handle the include directive
 * tokenize the path from the infile if there is one
 * attempt to open the file
 * if the file can't be opened, search through the directories array
 * if the file can opened, stop looking
 * send the path to the include file to the readFile function for processing
 * if none of the files can be opened return ENOENT signal
 */
int includeDirective(char *line, HashMap *map, char *buffer,
		char *pathIncludeFile, char **includeDirectories)
{
	FILE *inFromInclude = NULL;
	char *token = NULL;
	char fileToInclude[MAX_LINE_SIZE] = {0};
	int i = 0;

	// tokenize path
	token = strtok(line, "\"");
	token = strtok(NULL, "\"");

	strncpy(fileToInclude, pathIncludeFile, MAX_LINE_SIZE);
	strncat(fileToInclude, token, MAX_LINE_SIZE);

	// try to open the file
	inFromInclude = fopen(fileToInclude, "r");

	// if the file can't be open, attempt to open one from -I directive
	if (inFromInclude == NULL) {
		while (1) {
			if (strlen(includeDirectories[i]) == 0)
				break;
			strncpy(fileToInclude, includeDirectories[i], MAX_LINE_SIZE);
			strncat(fileToInclude, "/", MAX_LINE_SIZE - 1);
			strncat(fileToInclude, token, MAX_LINE_SIZE);

			inFromInclude = fopen(fileToInclude, "r");
			if (inFromInclude != NULL)
				break;
			i++;
		}
	}
	if (inFromInclude != NULL) {
		readFile(fileToInclude, map, buffer, NULL, includeDirectories);
		fclose(inFromInclude);
	} else {
		return ENOENT;
	}

	return 0;
}

/*
 * helper function for the if else directive
 * check if the value from the condition evaluates to true or false
 * check the type of condition
 * look if the condition is an entry in map
 * used atoi to convert the string to int
 * return 1 if it evaluates to true and 0 if false
 */
int conditionDirective(char *line, HashMap *map)
{
	char *token = NULL;
	int evaluate = -1;

	token = strtok(line, " ");

	if (strncmp(token, "#ifdef", 6) == 0) {
		token = strtok(NULL, " \n");
		if (findInMap(map, token) != NULL) {
			evaluate = 1;
			return evaluate;
		}
		evaluate = 0;
		return evaluate;
	} else if (strncmp(token, "#ifndef", 7) == 0) {
		token = strtok(NULL, " \n");
		if (findInMap(map, token) != NULL) {
			evaluate = 0;
			return evaluate;
		}
		evaluate = 1;
		return evaluate;
	}
		token = strtok(NULL, " \n");

		if (findInMap(map, token) != NULL)
			evaluate = atoi(getValue(map, token));
		else
			evaluate = atoi(token);

	return evaluate;
}

/*
 * If the condition evaluates to true, process line until endif
 * If the condition evaluates to false, process the lines after endif
 * If there is an elif clause, get the condition
 * Check what it evaluaets to, reapeat the same steps
 * If any clause is evaluated, check for else clause
 * Process the lines after that
 */
void evaluateCondition(char *line, HashMap *map, FILE *in, int evaluateIf,
		char *buffer, char *path, char **directories)
{
	int endIf = 0;

	// if condition evaluates to true, process lines
	if (evaluateIf != 0) {
		while (fgets(line, MAX_LINE_SIZE, in)) {
			if (strncmp(line, "#endif", 6) == 0)
				break;

			if (strncmp(line, "#else", 5) == 0) {
				while (fgets(line, MAX_LINE_SIZE, in)) {
					if (strncmp(line, "#endif", 6) == 0)
						break;
				}
				break;
			}

			processline(line, map, in, buffer, path, directories);
		}
	} else if (evaluateIf == 0) {
		// if condition evaluates to false, look for elif, else, process lines
		while (fgets(line, MAX_LINE_SIZE, in)) {

			if (strncmp(line, "#elif", 5) == 0) {
				evaluateIf = conditionDirective(line, map);
				evaluateCondition(line, map, in, evaluateIf, buffer, path, directories);
			}
			if (strncmp(line, "#else", 5) == 0) {
				while (fgets(line, MAX_LINE_SIZE, in)) {
					if (strncmp(line, "#endif", 6) == 0)
						break;
					processline(line, map, in, buffer, path, directories);
				}
			}
			if (strncmp(line, "#endif", 6) == 0)
				endIf = 1;
			if (endIf == 1) {
				if (strncmp(line, "#endif", 6) != 0)
					processline(line, map, in, buffer, path, directories);
			}
		}
	}
}


// Function to process lines, look for define, insert, conditions in file
int processline(char *line, HashMap *hash, FILE *in, char *buffer,
		char *pathIncludeFile, char **includeDirectories)
{
	char lineCopy[MAX_LINE_SIZE] = {0};
	int ret = 0;
	int evaluateIf = -1;

	strncpy(lineCopy, line, MAX_LINE_SIZE);

	if (strncmp(line, "#define", 7) == 0) {
		defineDirective(line, hash, in);
	} else if (strncmp(line, "#undef", 6) == 0) {
		deleteDefine(line, hash);
	} else if (strncmp(line, "#include", 8) == 0) {
		ret = includeDirective(line, hash, buffer, pathIncludeFile,
			includeDirectories);
	} else if (strncmp(line, "#if", 3) == 0) {
		evaluateIf = conditionDirective(line, hash);
		evaluateCondition(line, hash, in, evaluateIf, buffer,
			pathIncludeFile, includeDirectories);
	} else if (strncmp(line, "\n", 1) != 0) {
		replaceVariables(hash, lineCopy);
		strncat(buffer, lineCopy, strlen(lineCopy));
	}
	return ret;
}

/*
 * Function to handle the file and read it, sends it to process
 * Attempts to open the file, if successful, send line by line to process
 * If there is no file given as input, wait for stdin
 * If error signal received from processing an include file
 * Stop and send the error signal to end the execution
 */
int readFile(char *file, HashMap *hash, char *buffer, char *pathIncludeFile,
		char **includeDirectories)
{
	FILE *in;
	char line[MAX_LINE_SIZE] = {0};
	int ret = 0;

	// If there is no given file, read from stdin, else open file
	if (file == NULL) {
		while (fgets(line, MAX_LINE_SIZE, stdin)) {
			ret = processline(line, hash, stdin, buffer, pathIncludeFile,
				includeDirectories);
			if (ret == ENOENT)
				return ret;
		}
	} else {
		in = fopen(file, "r");
		if (in == NULL)
			return ENOENT;

		while (fgets(line, MAX_LINE_SIZE, in)) {
			ret = processline(line, hash, in, buffer, pathIncludeFile,
				includeDirectories);
			if (ret == ENOENT)
				return ret;
		}
		fclose(in);
	}

	return ret;
}

/*
 * helper function to extract the path given with the input file t
 * to use it in the include directive
 */
void getPathfromInfile(char *includePath)
{
	char *token = NULL;
	char newPath[MAX_LINE_SIZE] = {0};
	int sizeToken = 0;
	int terminator = 3;
	char *fileTerminator = NULL;

	// tokenize the path
	token = strtok(includePath, "/");

	while (token != NULL) {
		sizeToken = strlen(token);
		fileTerminator = &token[sizeToken - terminator];

		if (strncmp(fileTerminator, ".in", terminator) == 0)
			break;

		// copy path and add '/' lost in the tokenizing process
		strncat(newPath, token, MAX_LINE_SIZE);
		strncat(newPath, "/", MAX_LINE_SIZE - 1);
		token = strtok(NULL, "/");
	}

	strncpy(includePath, newPath, MAX_LINE_SIZE);
}

// helper function to add the directories from -I directive in array
void addDirectory(char **includeDirectories, char *argv)
{
	int i = 0;
	char *token = NULL;

	if (strncmp(argv, "-I", 2) == 0) {
		token = strtok(argv, "-I");
		argv = token;
	}

	for (i = 0; i < 30; i++) {
		if (strlen(includeDirectories[i]) == 0) {
			strncpy(includeDirectories[i], argv, MAX_LINE_SIZE);
			break;
		}
	}
}

/*
 * Function to handle the command lines arguments
 * If any flag is found, send it to process the values
 * Look for input or output files
 */
int command_line_arguments(HashMap *map, char **inFile, char **outFile,
		char **includeDirectories, int argc, char **argv)
{
	int isInFile = 0;
	int isOutFile = 0;
	char *token = NULL;
	int i = 0;

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-D") == 0) {
			i = i + 1;
			addDefine(map, argv[i]);
		} else if ((strcmp(argv[i], "-D") != 0) &&
				(strncmp(argv[i], "-D", 2) == 0)) {
			addDefine(map, argv[i]);
		} else if (strcmp(argv[i], "-I") == 0) {
			i = i + 1;
			addDirectory(includeDirectories, argv[i]);
		} else if ((strcmp(argv[i], "-I") != 0) &&
				(strncmp(argv[i], "-I", 2) == 0)) {
			addDirectory(includeDirectories, argv[i]);
		} else if (strcmp(argv[i], "-o") == 0) {
			isOutFile = 1;
			i = i + 1;
			*outFile = argv[i];
		} else if ((strcmp(argv[i], "-o") != 0) &&
				(strncmp(argv[i], "-o", 2) == 0)) {
			isOutFile = 1;
			token = strtok(argv[i], "-o");
			*outFile = token;
		} else {
			if (isInFile == 0) {
				isInFile = 1;
				*inFile = argv[i];
			} else if (isOutFile == 0) {
				isOutFile = 1;
				isInFile = 0;
				*outFile = argv[i];
			}
		}
	}

	return 0;
}
