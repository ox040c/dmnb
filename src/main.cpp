#include <iostream>
#include <string>
#include <cstdio>

#include "Interpreter/parse.h"

using namespace std;

int main() {

    char dmnb[] =
        "       _   __  __   _  _    _      \n"
        "    __| | |  \\/  | | \\| |  | |__   \n"
        "   / _` | | |\\/| | | .` |  | '_ \\  \n"
        "   \\__,_| |_|__|_| |_|\\_|  |_.__/  \n"
        "  _|\"\"\"\"\"|_|\"\"\"\"\"|_|\"\"\"\"\"|_|\"\"\"\"\"| \n"
        "  \"`-0-0-'\"`-0-0-'\"`-0-0-'\"`-0-0-' \n"
        ;

    char sepr[] =
        " ---------------------------------\n"
        ;

    char welcm[] = 
        " Welcome to DminiB system(exclaimation), enter command below\n"
        ;

    cout << dmnb << sepr << welcm << endl;

    string str;
    do {

        str.clear();
        cout << "dMNb>";
        getline(cin, str);
	parse(str);
        //cout << str << endl;
	
	    
    } while (str.length()); 

    return 0;
}

