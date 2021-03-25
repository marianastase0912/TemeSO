#ifndef __UTILS_H_
#define __UTILS_H_

#include "hashmap.h"

#define MAX_LINE_SIZE 256
#define ENOMEM 12
#define ENOENT 2
#define NOLINES 300

void addDefine(HashMap *map, char *arg);
int hasOpenBracket(char *line, int index);
void replaceVariables(HashMap *map, char *line);
void chechIfDefinedBefore(HashMap *map, char *value);
void defineDirective(char *line, HashMap *hash, FILE *in);
void deleteDefine(char *line, HashMap *map);
int includeDirective(char *line, HashMap *map, char *buffer,
	char *pathIncludeFile, char **includeDirectories);
int processline(char *line, HashMap *hash, FILE *in, char *buffer,
	char *pathIncludeFile, char **includeDirectories);
int readFile(char *file, HashMap *hash, char *buffer,
	char *pathIncludeFile, char **includeDirectories);
void getPathfromInfile(char *includePath);
void addDirectory(char **includeDirectories, char *argv);
int command_line_arguments(HashMap *map, char **inFile, char **outFile,
	char **includeDirectories, int argc, char **argv);
void evaluateCondition(char *line, HashMap *map, FILE *in,
	int evaluateIf, char *buffer, char *path, char **directories);
int conditionDirective(char *line, HashMap *map);

#endif
