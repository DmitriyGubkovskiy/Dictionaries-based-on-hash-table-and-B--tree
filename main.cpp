#include<vector>
#include<string>

#include<iostream>
#include <iomanip> 
#include <windows.h> 
#include "Hash_Table.h"
#include "Bplustree.h"

using namespace std;




int main()
{
	setlocale(LC_ALL, "russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

    
	
	int y = 1;
	while (y != 0)
	{
		cout << '\n' << "Словарь на основе дерева хэш-таблицы - 1" << '\n';
		cout << "Словарь на основе дерева - 2" << '\n';
		cout << "Выход - 0" << '\n' << '\n';
		cin >> y;
		Hash_Table table;
		vector<pair<int, string>> key_words;
		Bplus* bplus = new Bplus();
		string s = " ";
		int x = 1;
		switch (y)
		{
		case(0):
			break;
		case(1):
			
			while (true)
			{
				bool fl = false;

				cout << '\n' << "Добавить элемент в словарь - 1" << '\n';
				cout << "Удалить элемент из словаря - 2" << '\n';
				cout << "Вывести словарь - 3" << '\n';
				cout << "Найти элемент в словаре - 4" << '\n';
				cout << "Загрузить словарь из файла(исходный словарь будет очищен) - 5" << '\n';
				cout << "Дозаписать словарь из файла - 6" << '\n';
				cout << "Очистить словарь - 7" << '\n';
				cout << "Выход - 0" << '\n' << '\n';
				cin >> x;
				string inp;
				switch (x)
				{
				case(1):
					cout << "Введите элемент: " << '\n';
					cin >> inp;
					table.Add(inp);
					break;
				case(2):
					cout << "Введите элемент: " << '\n';
					cin >> inp;
					table.Delete(inp);
					break;
				case(3):
					table.Print();
					break;
				case(4):
					cout << "Введите элемент: " << '\n';
					cin >> inp;
					table.Searc(inp);
					break;
				case(5):
					table.From_txt();
					break;
				case(6):
					table.Clear();
					table.From_txt();
					break;
				case(7):
					table.Clear();
					break;
				case(0):
					fl = true;
					break;
				default:
					cout << "Ошибка." << '\n';
					break;
				}
				if (fl)
				{
					break;
				}
			}
			break;

		case(2):
			
			

			while (0 != x)
			{
				cout << '\n' << "Добавить элемент в словарь - 1" << '\n';
				cout << "Удалить элемент из словаря - 2" << '\n';
				cout << "Вывести словарь - 3" << '\n';
				cout << "Найти элемент в словаре - 4" << '\n';
				cout << "Загрузить словарь из файла(исходный словарь будет очищен) - 5" << '\n';
				cout << "Дозаписать словарь из файла - 6" << '\n';
				cout << "Очистить словарь - 7" << '\n';
				cout << "Выход - 0" << '\n' << '\n';
				cin >> x;
				switch (x)
				{
				case 1:
					cout << "Введите элемент: " << '\n';
					cin >> s;
					Add(bplus, s, key_words);
					break;
				case 2:
					cout << "Введите элемент: " << '\n';
					cin >> s;
					Del(bplus, s);
					break;
				case 3:
					Print_tree(bplus, key_words);
					break;
				case 4:
					cout << "Введите элемент: " << '\n';
					cin >> s;
					Search(bplus, s);
					break;
				case 5:
					delete bplus;
					bplus = new Bplus();
					From_txt(bplus, key_words);
					break;
				case 6:
					From_txt(bplus, key_words);
					break;
				case 7:
					delete bplus;
					bplus = new Bplus();
					cout << "Словарь очищен. " << '\n';
				case 0:
					break;
				default:
					cout << "Ошибка." << '\n';
					break;
				}
			}
			break;
		default:
			cout << "Ошибка" << '\n';
			break;
		}
		delete bplus;
	}
	/*Hash_Table table;
	while (true)
	{
		for (unsigned long long i = 624269248252; i < 624269248252 + 1000; i++)
		{
			string s = to_string(i);
			table.Add(s);
			
		}
		
	}
	table.Print();*/
    return 0;
}











