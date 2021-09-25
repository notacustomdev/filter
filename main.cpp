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
    string indata, outdata, filterdata;

    of_filter of;
    int fdin, fdout, n, m;
    // Shut GetOpt error messages down (return '?'): 
    opterr = 0;

    // Retrieve the options:
    while ( (opt = getopt(argc, argv, "i:o:f:")) != -1 ) {  // for each option...
        switch ( opt ) {
            case 'i':
                    indata = optarg;
                    cout << "input: " << indata << endl;
                break;
            case 'o':
                    outdata = optarg;
                    cout << "output: " << outdata << endl;
                break;
            case 'f':
                    filterdata = optarg;
                    cout << "filterfile: " << filterdata << endl;
                break;
            case '?':  // unknown option...
                    cerr << "Unknown option: '" << char(optopt) << "'!" << endl;
                break;
        }
    }

    of = load_filter(filterdata.c_str());

    char buf[of.h.length];

    char *bptr = &buf[0];
    fdin = open(indata.c_str(), O_RDONLY);
    fdout = open(outdata.c_str(), O_WRONLY | O_CREAT);
    int req_b=of.h.length;

    while( (n = read(fdin, bptr, req_b) ) > 0){
        cout << "read: " << n << "bytes\n";
        m = write(fdout, bptr, n);
        cout << "wrote: " << m << "bytes\n";
        bptr = &buf[n];
        req_b = (of.h.length - n);
        if(req_b==0)
            req_b = of.h.length;
        if(bptr == &buf[of.h.length])
            bptr = &buf[0];
    }

    close(fdin);
    close(fdout);
    return 0;
}
