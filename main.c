/*
Copyright (c) 2003-2006 by Juliusz Chroboczek
Copyright (c) 2017-2025 by Silas S. Brown

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "polipo2.h"

AtomPtr configFile = NULL;
AtomPtr pidFile = NULL;
int daemonise = 0;

static void
usage(char *argv0)
{
    fprintf(stderr, 
            "%s [ -h ] [ -v ] [ -x ] [ -c filename ] [ -- ] [ var=val... ]\n",
            argv0);
    fprintf(stderr, "  -h: display this message.\n");
    fprintf(stderr, "  -v: display the list of configuration variables.\n");
    fprintf(stderr, "  -x: perform expiry on the disk cache.\n");
    fprintf(stderr, "  -c: specify the configuration file to use.\n");
}

int
main(int argc, char **argv)
{
    FdEventHandlerPtr listener;
    int i;
    int rc;
    int expire = 0, printConfig = 0;

    /* VerboseDebug("polipo2 starting main()\n"); */

    initAtoms();
    CONFIG_VARIABLE(daemonise, CONFIG_BOOLEAN, "Run as a daemon");
    CONFIG_VARIABLE(pidFile, CONFIG_ATOM, "File with pid of running daemon.");

    preinitChunks();
    preinitLog();
    preinitObject();
    preinitIo();
    preinitDns();
    preinitServer();
    preinitHttp();
    preinitDiskcache();
    preinitLocal();
    preinitForbidden();
    preinitSocks();

    i = 1;
    while(i < argc) {
        if(argv[i][0] != '-')
            break;
        if(strcmp(argv[i], "--") == 0) {
            i++;
            break;
        } else if(strcmp(argv[i], "-h") == 0) {
            usage(argv[0]);
            exit(0);
        } else if(strcmp(argv[i], "-v") == 0) {
            printConfig = 1;
            i++;
        } else if(strcmp(argv[i], "-x") == 0) {
            expire = 1;
            i++;
        } else if(strcmp(argv[i], "-c") == 0) {
            i++;
            if(i >= argc) {
                usage(argv[0]);
                exit(1);
            }
            if(configFile)
                releaseAtom(configFile);
            configFile = internAtom(argv[i]);
            i++;
        } else {
            usage(argv[0]);
            exit(1);
        }
    }

    if(configFile)
        configFile = expandTilde(configFile);

    if(configFile == NULL) {
        configFile = expandTilde(internAtom("~/.polipo2"));
        if(configFile)
            if(access(configFile->string, F_OK) < 0) {
                releaseAtom(configFile);
                configFile = NULL;
            }
    }

    if(configFile == NULL) {
        if(access("/etc/polipo2/config", F_OK) >= 0)
            configFile = internAtom("/etc/polipo2/config");
        if(configFile && access(configFile->string, F_OK) < 0) {
            releaseAtom(configFile);
            configFile = NULL;
        }
    }

    rc = parseConfigFile(configFile);
    if(rc < 0)
        exit(1);

    while(i < argc) {
        rc = parseConfigLine(argv[i], "command line", 0, 0);
        if(rc < 0)
            exit(1);
        i++;
    }

    initChunks();
    initLog();
    initObject();
    if(!expire && !printConfig)
        initEvents();
    initIo();
    initDns();
    initHttp();
    initServer();
    initDiskcache();
    initForbidden();
    initSocks();

    if(printConfig) {
        printConfigVariables(stdout, 0);
        exit(0);
    }

    if(expire) {
        expireDiskObjects();
        exit(0);
    }

    if(daemonise)
        do_daemonise(loggingToStderr());

    if(pidFile) {
        pidFile = expandTilde(pidFile);
        writePid(pidFile->string);
    }

    listener = create_listener(proxyAddress->string, 
                               proxyPort, httpAccept, NULL);
    if(!listener) {
        if(pidFile) unlink(pidFile->string);
        exit(1);
    }

    VerboseDebug("polipo2 entering eventLoop\n");
    eventLoop();
    VerboseDebug("polipo2 left eventLoop\n");

    if(pidFile) unlink(pidFile->string);
    return 0;
}
