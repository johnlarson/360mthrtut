#ifndef UTILS_H
#define UTILS_H

#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <string>

using namespace std;

void errorOut(string msg);

int stat(string directory, struct stat &filestat);

static inline int isWhitespace(char c);

void chomp(char *line);

string GetLine(int fds);

#endif
