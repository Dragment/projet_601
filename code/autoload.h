#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/stat.h>

#include "ncurses.h"
#include "map.h"
#include "worldMap.h"
#include "requete.h"
#include "artefact.h"