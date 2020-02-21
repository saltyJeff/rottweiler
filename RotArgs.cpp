#include "RotArgs.h"
#include <getopt.h>
const char* SHORT_OPTIONS = "a:e:s:t:T:o:O:vhVS";
struct option long_options[] = {
    {"az-file", required_argument, 0, 'a'},
    {"el-file", required_argument, 0, 'e'},
    {"serial-speed", required_argument, 0, 's'},
    {"port", required_argument, 0, 't'},
    {"listen-addr", required_argument, 0, 'T'},
    {"test", no_argument, 0, 'S'},
    {"set-azoffset", required_argument, 0, 'o'},
    {"set-eloffset", required_argument, 0, 'O'},
    {"verbose", no_argument, 0, 'v'},
    {"compat", no_argument, 0, 'C'},
    {"help", no_argument, 0, 'h'},
    {"version",no_argument, 0, 'V'},
    {0, 0, 0, 0}
};

RotArgs::RotArgs(int argc, char** argv) {
    int c;
    int optIndex = 0;
    // CS111
    while ( (c = getopt_long(argc, argv, SHORT_OPTIONS, long_options, &optIndex)) != -1) {
        switch (c) {
        case 'a':
	        azDevFile = optarg;
	        break;
        case 'e':
	        elDevFile = optarg;
	        break;
        case 'V':
	        //version();
	        exit(0);
        case 'm':
	        modelId = atoi(optarg);
	        break;
        case 's':
	        baudRate = atoi(optarg);
	        break;
        case 't':
	        portNum = atoi(optarg);
	        break;
        case 'T':
	        listenAddr = inet_addr(optarg);
	        break;
        case 'o':
	        azOffset = atof(optarg);
	        break;
        case 'O':
	        elOffset = atof(optarg);
        case 'v':
	        verbose = true;
	        break;
        case 'S':
	        test = true;
	        break;
        case 'C':
	        compatMode = true;
        	break;
        case '?':
	        exit(1);
        }
    }
}