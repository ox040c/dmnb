#include <iostream>
#include <string>
#include <cstdio>

#include "Interpreter/parse.h"

#include "API.hpp"
#include "CatalogManager.hpp"
#include "IndexManager.hpp"
#include "RecordManager.hpp"
#include "BufferManager.hpp"


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

    char fail[] =
        "    ___ _____ ____\n"
        "   / _ \\_   _|_  /\n"
        "  | (_) || |  / / \n"
        "   \\___/ |_| /___|\n"
        "   init failed :( \n"
        ;

    char welcm[] =
        " Welcome to DminiB system(exclaimation),\n"
        " enter command below\n"
        ;

    cout << dmnb << sepr;

    try {
        CatalogManager catlogManager;
        RecordManager recordManager;
        IndexManager indexManager;

        API api(catlogManager,
                recordManager,
                indexManager);

        cout << welcm << endl;

        string str;
        do {

            str.clear();
            cout << "dMNb> ";
            getline(cin, str);
            const PlanList& plist = parse(str);
            
            for (PlanList::const_iterator i = plist.begin();
            i != plist.end(); ++i) {
                for (WrapperList::const_iterator j = i->wlist.begin();
                j != i->wlist.end(); j++)
                j->debug();
            }
            int acti = 0;
            try {
                switch(acti) {
                   case DELV:  break;
                   case SELV:
                   case CTBL:
                   case DTBL:
                   case CIDX:
                   case DIDX:
                   case LEAVE: break;
                }
            }
            catch (...) {
                
            }
            //cout << str << endl;


        } while (str.length());

    }
    catch (...) {

        cout << fail << endl;
        return(-1);

    }
    
    return 0;
}

