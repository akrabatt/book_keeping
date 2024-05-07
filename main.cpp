#include <iostream>

extern int menu();
extern int menu_ch;


int main()
{
	std::cout << "hello!" << std::endl;	
	menu_ch = menu();
	std::cout << "result: " << menu_ch << std::endl;
}
