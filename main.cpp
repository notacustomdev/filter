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
    int fd, n;
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

    fd = open(indata.c_str(), O_RDONLY);

    while( (n = read(fd, buf, of.h.length) ) > 0)
        cout << "read: " << n << "bytes\n";

    return 0;
}
