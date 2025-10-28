#include <memory>
#include <iostream>

struct Data
{
	Data(int x, int y) : a(x), b(y) {}
	~Data() { std::cout << "Destory" << std::endl; }
	int a;
	int b;
};

int main()
{
	{
		std::shared_ptr<Data> s1(new Data(1, 1));
		{
			std::shared_ptr<Data> s2 = s1;
			std::shared_ptr<Data> s3 = s1;
		}
		int a = 10;
	}

	{

	}

	
	int a = 0;
	return 0;
}