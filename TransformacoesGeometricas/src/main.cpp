#include <iostream>
#include "Exercises.h"


int main(void) {
	std::string input;
	
	while (true) {
		std::cout << "\nDigite o exercicio desejado (1, 2, 3, 4 ou 5) ou 'x' para sair" << std::endl;
		std::cin >> input;

		switch (input.c_str()[0]) {
		case '1':
			Ex1::Exercise_1();
			break;
		case '2':
			Ex2::Exercise_2();
			break;
		case '3':
			Ex3::Exercise_3();
			break;
		case '4':
			Ex4::Exercise_4();
			break;
		case '5':
			Ex5::Exercise_5();
			break;
		case 'x':
			return EXIT_SUCCESS;
		default:
			std::cout << "\n\nInput invalido\n\n";
		}
	}
}