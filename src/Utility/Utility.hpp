#ifndef DATA_TYPE_HPP
#define DATA_TYPE_HPP

#include <string>
#include <list>

// applying namespace to avoid naming conflicts, thanks
// NOTE: please add following line if you are using these types
/*
   using namespace utls;
   */
namespace utls {

    enum Operator {EQUAL, UNEQUAL, LESS, MORE, NO_MORE, NO_LESS, TOTAL_OP};

    enum DataType {INT, FLOAT, CHAR, TOTAL_TYPE};

}

struct DataAddr {
    std::string filename;
    unsigned int dataaddr;
    unsigned int datalen;
};

class Wrapper {
    public:
        std::string name;
        utls::DataType type;
        utls::Operator op;
        void *valuep;
        bool isIndex;
        //int charLength;
        // consult *valuep for charlen when defining a table
        // TODO: overload more for convenient usage
        Wrapper (std::string name,
                utls::DataType type = utls::TOTAL_TYPE,
                utls::Operator op = utls::TOTAL_OP,
                void *valuep = 0,
                bool isIndex = false):
            name(name), type(type), op(op), isIndex(isIndex) {}
        ~Wrapper() {
            if ( valuep ) {
                switch (op) {
                    case utls::INT: break;
                    case utls::FLOAT: break;
                    case utls::CHAR: break;
                }
            }
        }
};

typedef std::list < Wrapper > WrapperList;

typedef Wrapper Condition;
typedef Wrapper Scheme;
typedef WrapperList TableDefinition;
typedef WrapperList Entry;
typedef std::list < Entry > Entries;


utls::DataType getDataType(
        const TableDefinition& td, const std::string attName);

#endif
