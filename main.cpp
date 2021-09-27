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

void short_to_double(int16_t *in, double *out, size_t n){
    for(size_t i = 0; i < n; i++)
        out[i] = double(in[i])*20./65536.;
}

int main(int argc, char** argv) {
    int opt;
    string jsondata, filterdata;

    of_filter of;
    int n, m;
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

    int16_t rawbuff[of.h.length];
    double conv_buff[of.h.length];
    int16_t *bptr = &rawbuff[0];
    
    int req_b=of.h.length*sizeof(short);

    while( (n = read(STDIN_FILENO, (char*)bptr, req_b) ) > 0){
        bptr = &rawbuff[n % of.h.length];
        req_b = n % of.h.length*sizeof(int16_t);
        if(req_b==0){
            req_b = of.h.length*sizeof(int16_t);
            short_to_double(rawbuff, conv_buff, of.h.length);
            m = write(STDOUT_FILENO, (char*)conv_buff, of.h.length);
        }
    }

    return 0;
}


