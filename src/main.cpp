#include <iostream>
#include <string>
#include <cstdio>

#include "parse.h"

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
        BufferManager bufferManager;
        CatalogManager catlogManager;
        RecordManager recordManager(bufferManager);
        IndexManager indexManager(bufferManager);

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

            // call api according to plist
            for (PlanList::const_iterator plan = plist.begin();
                    plan != plist.end(); ++plan) {

                std::string tname = plan->tname;
                // seperate function call accoring to acti
                Action acti = plan->acti;
                const WrapperList &wlist = plan->wlist;
                try {
                    switch(acti) {
                        case DELV: api.dropTable(tname); break;
                        case SELV: if ( !wlist.size() ) {
                                       api.select(tname);
                                   }
                                   else {
                                       api.select(tname, wlist);
                                   }
                                   break;
                        case CTBL: api.createTable(tname, wlist); break;
                        case DTBL: api.dropTable(tname); break;
                        case CIDX: api.createIndex(tname, wlist.begin()->name,
                                           wlist.begin()->strv); break;
                        case DIDX: api.dropIndex(tname); break;
                        case LEAVE: break;
                    }
                }
                catch (...) {

                }
                //cout << str << endl;

                for (WrapperList::const_iterator j = plan->wlist.begin();
                        j != plan->wlist.end(); j++)
                    j->debug();
            }

        } while (str.length());

    }
    catch (exception const& e) {
        cerr << "err: " << e.what() << endl;
    }
    catch (...) {

        cout << fail << endl;
        return(-1);

    }

    return 0;
}

