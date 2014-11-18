#ifndef PAGE_H
#define PAGE_H
#include <string>
struct pagedata{
	unsigned offset;
	unsigned datalen;
	char *data;
};
class Page{
public:
	char data[4096];//page 的4K数据
	std::string filename;//page的储存的表单的名称
	int call_time; //表示这块page被访问的次数，-1表示没有写进任何数据的page
	unsigned int pages_addr;//page的首地址

	void write_to_file();
	void read_from_file();
	void write_page(const pagedata info);//info.date的长度必须与info.len相等
	void read_page(pagedata & info); //info.date事先开好空间。
	Page()
	{
		call_time = -1;
		filename = "";
		pages_addr = 0;
	}
	~Page(){}
};

#endif