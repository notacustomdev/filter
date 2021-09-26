// C Libraries:
#include <string>
#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <fcntl.h>
#include <sys/stat.h>
// Namespaces:
using namespace std;

#include "filter.h"

int main(int argc, char** argv) {
    int opt;
    string jsondata, filterdata;

    of_filter of;
    int fdin, fdout, n, m;
    // Shut GetOpt error messages down (return '?'): 
    opterr = 0;

    // Retrieve the options:
    while ( (opt = getopt(argc, argv, "j:f:")) != -1 ) {  // for each option...
        switch ( opt ) {
            case 'j':
                    jsondata = optarg;
                break;
            case 'f':
                    filterdata = optarg;
                break;
            case '?':  // unknown option...
                    cerr << "Unknown option: '" << char(optopt) << "'!" << endl;
                return 0;
        }
    }

    of = load_filter(filterdata.c_str());

    char buf[2*of.h.length];

    char *bptr = &buf[0];
    
    int req_b=of.h.length;

    while( (n = read(STDIN_FILENO, bptr, req_b) ) > 0){
        m = write(STDOUT_FILENO, bptr, n);
        bptr = &buf[n];
        req_b = (of.h.length - n);
        if(req_b==0)
            req_b = of.h.length;
        if(bptr == &buf[of.h.length])
            bptr = &buf[0];
    }

    return 0;
}
