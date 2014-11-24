#include <iostream>
#include <string>
#include <cstdio>
#include <stdexcept>

#include "parse.h"

#include "API.hpp"
#include "CatalogManager.hpp"
#include "IndexManager.hpp"
#include "RecordManager.hpp"
#include "BufferManager.hpp"


using namespace std;
void printList (const Entries& rlist, const TableDefinition& tdef);

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

    char byee[] =
        " BBBByyyyeeee ~~~~\n"
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

        string str, temp;
        do {

            str.clear(); temp.clear();
            if (!cin) { cout << endl; break; } // check if ^D
            cout << "dMNb> ";

            do {

                getline(cin, temp);
                str += " " + temp;

            } while (temp.find(';') == string::npos && cin);

            if (str == " exit;" || str == " exit") break;
            if (str == " " ) break;

            PlanList& plist = parse(str);

            // call api according to plist
            for (PlanList::iterator plan = plist.begin();
                    plan != plist.end(); ++plan) {

                std::string tname = plan->tname;
                // seperate function call accoring to acti
                Action acti = plan->acti;
                WrapperList &wlist = plan->wlist;

                try {
                    switch(acti) {
                        case DELV: if ( !wlist.size() ) {
                                       cout << "del item num: " << api.remove(tname) << endl;
                                   }
                                   else {
                                       cout << "del item num: " << api.remove(tname, wlist) << endl;
                                   }
                                   break;

                        case SELV: if ( !wlist.size() ) {
                                       const Entries rlist = api.select(tname);
                                       printList(rlist, catlogManager.getTableDef(tname));
                                   }
                                   else {
                                       const Entries rlist = api.select(tname, wlist);
                                       printList(rlist, catlogManager.getTableDef(tname));
                                   }
                                   break;
                        case CTBL: api.createTable(tname, wlist); break;
                        case DTBL: api.dropTable(tname); break;
                        case CIDX: api.createIndex(tname, wlist.begin()->name,
                                           wlist.begin()->strv); break;
                        case INSV:
                                   //cout << "[main] api.insertEntry calling..." << endl;
                                   cout.flush();

                                   api.insertEntry(tname, wlist); break;
                        case DIDX: api.dropIndex(tname); break;
                        default: cerr << "unhandled action\n"; break;
                    }
                }
                catch (exception const& e) {
                    cerr << "err: " << e.what() << endl;
                }
                //cout << str << endl;
                /*
                   for (WrapperList::const_iterator j = plan->wlist.begin();
                   j != plan->wlist.end(); j++)
                   j->debug();
                   */
            }

        } while (1);

    }
    catch (exception const& e) {
        cerr << "err: " << e.what() << endl;
        return(-1);
    }
    catch (...) {

        cout << fail << endl;
        return(-1);

    }

    cout << byee;
    return 0;
}

void printList (const Entries& rlist, const TableDefinition& tdef) {

    if ( rlist.size() ) {
        for (TableDefinition::const_iterator i = tdef.begin();
                i != tdef.end(); i++) {
            //if ( i->type == utls::CHAR) cout.width(i->intv);
            //else cout.width(10);
            cout << i->name << "    ";
        }

        cout << endl;
    }

    for (Entries::const_iterator i = rlist.begin();
            i != rlist.end(); i++) {
        for (Entry::const_iterator j = i->begin();
                j != i->end(); j++) {
            j->debug();
        }
        cout << endl;
    }
    cout << rlist.size() << " rows selected" << endl;
}
