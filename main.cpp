// C Libraries:
#include <string>
#include <iostream>
#include <unistd.h>

// Namespaces:
using namespace std;

int main(int argc, char** argv) {
    int opt;
    string input = "";
    bool flagA = false;
    bool flagB = false;

    // Retrieve the (non-option) argument:
    if ( (argc <= 1) || (argv[argc-1] == NULL) || (argv[argc-1][0] == '-') ) {  // there is NO input...
        cerr << "No argument provided!" << endl;
        //return 1;
    }
    else {  // there is an input...
        input = argv[argc-1];
    }

    // Debug:
    cout << "input = " << input << endl;

    // Shut GetOpt error messages down (return '?'): 
    opterr = 0;

    // Retrieve the options:
    while ( (opt = getopt(argc, argv, "ab")) != -1 ) {  // for each option...
        switch ( opt ) {
            case 'a':
                    flagA = true;
                break;
            case 'b':
                    flagB = true;
                break;
            case '?':  // unknown option...
                    cerr << "Unknown option: '" << char(optopt) << "'!" << endl;
                break;
        }
    }

    // Debug:
    cout << "flagA = " << flagA << endl;
    cout << "flagB = " << flagB << endl;

    return 0;
}