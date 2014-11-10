/// Interpreter

enum Operator {EQUAL, UNEQUAL, LESS, MORE, NO_MORE, NO_LESS, TOTAL_OPERATOR};

class Interpreter {
private:
    API &api;
public:
    void parse(const std::string &statement);
};

/// API

typedef std::vector < Condition > Conditions;

class Condition {
private:
    std::string key;
    Operator op;
    void *valuePtr;
    
public:
    void setKey(const std::string &key);
    void setOp(const Operator &op);
    void setValue(const void *valuePtr);
    
    std::string getKey();
    Operator getOp();
    void *getValuePtr();
};
    

class API {
private:
    RecordManager &recordManager;
    IndexManager &indexManager;
    CatalogManger &catalogManager;
    
public:
    void createTable(const std::string &tableName, const std::vector < Scheme > &data);
    
    void dropTable(const std::string &tableName);   // indexManager.dropAddress() should be called
    void dropIndex(const std::string &tableName);
    void select(const std::string &tableName,
                const Conditions &conditions);      // no "or" in statement
    ConstItemIterator itemsBegin();
    ConstItemIterator itemsEnd();
        
};

/// Catalog Manager

enum DataType {CHAR, INT, FLOAT, TOTAL_TYPE};

class Scheme {
private:
    std::string name;
    DataType dataType;
    byte charLength;
    
public:
    void setName(const std::string &name);
    void setDataType(const DataType &dataType);
    void setCharLength(const byte &charLength);
    
    std::string getName();
    DataType getDataType();
    byte getCharLength();
};

typedef std::vector < Scheme > TableDefinition;
typedef std::map < std::string, std::vector < Scheme > > TablesDefinition;
typedef std::map < std::string, bool > IndexFlags;

class CatalogManager {
private:
    TablesDefinition tables;
    IndexFlags indexFlags;
public:
    void createTable(const std::string &tableName, const std::TableDefinition &table);
    void dropTable(const std::string &tableName);
    void setTableIndexed(const std::string &tableName);
    bool isTableIndexed(const std::string &talbeName);
};

/// Record Manager

typedef std::vector < void * > ItemType;
typedef std::vector < ItemType > ItemArray;
typedef ItemArray::const_iterator ConstItemIterator; 

class RecordManage {

private:
    ItemArray items;

public:
    ConstItemIterator itemsBegin() const;
    ConstItemIterator itemsEnd() const;
    
    void select();
    void select(const Condition &condition);
    
};

/// Index Manager

typedef std::list < unsigned int > TableId;

class TableIndex {
    // B+-Tree
};

class IndexManager {
private:
    std::map < std::string, TableIndex > indices;
    std::map < std::string, TableId > ids;
    
    BufferManager &bufferManager;
    
public:
    void createTableIndex(const std::string &tableName);
    void createTableAddress(const std::string &tableName);
    void dropTableIndex(const std::string &table);
    void dropTableAddress(const std::string &tableName); // dropTableIndex() should be called

};

/// Buffer Manager

typedef unsigned char DBData;

typedef struct BlockType {
    DBData data[4096];  // 4KB
    unsigned int address;
    unsigned int modifyTimes;
} Block;

typedef std::list < Block >  Buffer;
typedef std::queue < unsigned int > FreePage;

class BufferManager {
private:
    static const unsigned int MAX_BUFFER = 65536;
    
    Buffer wBuffer, rBuffer;
    std::map < std::string, FreePage > freePages;
    std::map < std::string, unsigned int > tailPointer;
    
public:
    unsigned int insert(const DBData data[],
                        const unsigned int &sizePerTable);
    void remove(const unsigned int &index,
                const unsigned int );
    void writeBuffer(const DBData data[],
                     const std::string tableName,
                     const unsigned int &blockId,
                     const std:string &indexOn);
    const Block &readBuffer(const std::string tableName,
                            const unsigned int address);
};
