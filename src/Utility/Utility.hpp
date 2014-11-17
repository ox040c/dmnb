#ifndef DATA_TYPE_HPP
#define DATA_TYPE_HPP

#include <string>
#include <list>
#include <iostream>

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
        bool isIndex;
        int intv;
        float floatv;
        std::string strv;
        //int charLength;
        // consult *valuep for charlen when defining a table
        // TODO: overload more for convenient usage

        // Condition
        Wrapper (std::string name,
                utls::Operator op,
                int intv,
                utls::DataType type = utls::INT,
                float floatv = 0.0,
                std::string strv = "",
                bool isIndex = false):
            name(name), type(type), op(op), isIndex(isIndex),
            intv(intv), floatv(floatv), strv(strv) { debug(); }
        Wrapper (std::string name,
                utls::Operator op,
                std::string strv,
                utls::DataType type = utls::CHAR,
                int intv = 0,
                float floatv = 0.0,
                bool isIndex = false):
            name(name), type(type), op(op), isIndex(isIndex),
            intv(intv), floatv(floatv), strv(strv) { debug(); }
        Wrapper (std::string name,
                utls::Operator op,
                float floatv,
                utls::DataType type = utls::FLOAT,
                int intv = 0,
                std::string strv = "",
                bool isIndex = false):
            name(name), type(type), op(op), isIndex(isIndex),
            intv(intv), floatv(floatv), strv(strv) { debug(); }

        // Attribute
        Wrapper (std::string name,
                utls::DataType type,
                int intv = 0,
                bool isIndex = false,
                utls::Operator op = utls::TOTAL_OP,
                float floatv = 0.0,
                std::string strv = ""
                ):
            name(name), type(type), op(op), isIndex(isIndex),
            intv(intv), floatv(floatv), strv(strv) { debug(); }

        // Values (insert into values (..)
        Wrapper (
                utls::DataType type,
                float floatv,
                std::string name = "",
                int intv = 0,
                std::string strv = "",
                bool isIndex = false,
                utls::Operator op = utls::TOTAL_OP
                ):
            name(name), type(type), op(op), isIndex(isIndex),
            intv(intv), floatv(floatv), strv(strv) { debug(); }
        Wrapper (
                utls::DataType type,
                int intv,
                std::string name = "",
                std::string strv = "",
                bool isIndex = false,
                float floatv = 0.0,
                utls::Operator op = utls::TOTAL_OP
                ):
            name(name), type(type), op(op), isIndex(isIndex),
            intv(intv), floatv(floatv), strv(strv) { debug(); }
        Wrapper (
                utls::DataType type,
                std::string strv,
                std::string name = "",
                int intv = 0,
                bool isIndex = false,
                float floatv = 0.0,
                utls::Operator op = utls::TOTAL_OP
                ):
            name(name), type(type), op(op), isIndex(isIndex),
            intv(intv), floatv(floatv), strv(strv) { debug(); }

        /*~Wrapper() {
          if ( valuep ) {
          switch (op) {
          case utls::INT: break;
          case utls::FLOAT: break;
          case utls::CHAR: delete (char*) valuepbreak;
          }
          }
          }*/
    private:
        void debug() {
            std::cout << name;
            switch (op) {
                case utls::EQUAL:   std::cout << " == "; break;
                case utls::UNEQUAL: std::cout << " <> "; break;
                case utls::LESS:    std::cout << " <  "; break;
                case utls::MORE:    std::cout << " >  "; break;
                case utls::NO_MORE: std::cout << " <= "; break;
                case utls::NO_LESS: std::cout << " >= "; break;
                case utls::TOTAL_OP:std::cout << ""; break;
            }

            switch (type) {
                case utls::INT:   std::cout << " int " << intv; break;
                case utls::FLOAT: std::cout << " float "<< floatv; break;
                case utls::CHAR:  std::cout << " char "<< strv; break;
                case utls::TOTAL_TYPE:  std::cout << ""<< strv; break;
            }
            std::cout << std::endl;
        }
};

typedef Wrapper Condition;
typedef Wrapper Scheme;

typedef std::list < Wrapper > WrapperList;
typedef WrapperList TableDefinition;
typedef WrapperList Entry;
typedef WrapperList Values;
typedef WrapperList Conditions;

typedef std::list < Entry > Entries;

typedef utls::DataType dt_t;
typedef utls::Operator op_t;

utls::DataType getDataType(
        const TableDefinition& td, const std::string attName);

#endif
