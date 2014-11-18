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
	char data[4096];//page ��4K����
	std::string filename;//page�Ĵ���ı�������
	int call_time; //��ʾ���page�����ʵĴ�����-1��ʾû��д���κ����ݵ�page
	unsigned int pages_addr;//page���׵�ַ

	void write_to_file();
	void read_from_file();
	void write_page(const pagedata info);//info.date�ĳ��ȱ�����info.len���
	void read_page(pagedata & info); //info.date���ȿ��ÿռ䡣
	Page()
	{
		call_time = -1;
		filename = "";
		pages_addr = 0;
	}
	~Page(){}
};

#endif