//b+tree node with m = 5
class Tnode{

private:
	int isleaf;
	int num;
	keyType *k;
	filePoint *p;

public:
	Tnode(){
		p = new filePoint[5];
		k = new keyType[4];
	}
	~Tnode(){
		delete[] p;
		delete[] k;
	}
}