/*Record Manager
**
Created by gaotao*/

class RecordManager {
private:
    
public:
    RecordManager();
    ~RecordManager();
    
// TODO:
    int getNext(const std::string tableName); 
    // 给定表名，顺序迭代，返回硬盘中位置
    
    void *getAttValue(const std::string &tableName,
                      const int &pos,
                      const std::string &colName);
    // 给定表名，硬盘上位置，列名，返回一个存着值的void *
    
    int insertEntry(const std::string &tableName,
                    const Entry &entry);
    // 给定表名，一条待插入的数据，插入后返回硬盘位置
    
};
