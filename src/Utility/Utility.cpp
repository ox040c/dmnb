#include "Utility.hpp"

extern utls::DataType getDataType(
        const TableDefinition& td, const std::string attName);


utls::DataType getDataType(
        const TableDefinition& td, const std::string attName) {
    for (TableDefinition::const_iterator i = td.begin();
            i != td.end(); ++i) {
        if (i->name == attName)
            return i->dataType;
    }
    return (utls::DataType)utls::TOTAL_TYPE;
}
