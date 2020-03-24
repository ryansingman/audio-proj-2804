#include "main.h"

#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

#define BOLDBLUE "\033[1;34m" 
#define RESET "\033[0m"

void ascii_vis(float *beam_dir);
