#include "main.h"

#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

#define BOLDBLUE "\033[1m\033[34m" 
#define BOLDRED "\033[1m\033[31m"
#define BOLDGREEN "\033[1m\033[32m"
#define RESET "\033[0m"

void ascii_vis(float *beam_dir);
