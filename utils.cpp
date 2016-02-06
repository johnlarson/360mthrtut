#include "utils.h"

#define MAX_MSG_SZ 1024

int stat(string path, struct stat &filestat) {
	return stat(path.c_str(), &filestat);
}

void errorOut(string msg) {
	perror(msg.c_str());
	exit(0);
}

static inline int isWhitespace(char c)
{
    switch (c) 
    {   
        case '\r':
        case '\n':
        case ' ':
        case '\0':
            return 1;
        default:
            return 0;
    }   
}

void chomp(char *line)
{
    int len = strlen(line);
    while (isWhitespace(line[len]))
    {   
        line[len--] = '\0';
    }   
}

string GetLine(int fds)
{
    char tline[MAX_MSG_SZ];
    char *line;
    
    int messagesize = 0;
    int amtread = 0;
    while((amtread = read(fds, tline + messagesize, 1)) < MAX_MSG_SZ)
    {   

        if (amtread > 0)
        {   
            messagesize += amtread;
        }   
        else if( amtread == 0 ) 
        {   
            break;
        }   
        else
        {   
            perror("Socket Error is:");
            fprintf(stderr, "Read Failed on file descriptor %d messagesize = %d\n", fds, messagesize);
            exit(2);
        }   
        //fprintf(stderr,"%d[%c]", messagesize,message[messagesize-1]);
        if (tline[messagesize - 1] == '\n')
        {   
            break;
        }   
    }   
    tline[messagesize] = '\0';
    chomp(tline);
    line = (char *)malloc((strlen(tline) + 1) * sizeof(char));
    strcpy(line, tline);
    //fprintf(stderr, "GetLine: [%s]\n", line);
    string returnable = line;
    free(line);
    return returnable;
}

