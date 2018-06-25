// cppApp_laba1_2sem.cpp: определяет точку входа для консольного приложения.
/*Написать программу, содержащую следующие функции:
1. Функцию заполнения текстового файла N целыми случайными числами в
диапазоне от -M до M. Имя файла, N и M поступают на вход функции, файл
возвращается функцией как результат (использовать потоки ввода-вывода).
Реализовать два варианта генерации чисел — в цикле и с использованием
алгоритма std::generate.
2. Функцию, получающую файл на вход, и возвращающую последовательный
контейнер, заполненный числами из файла.
3. Функцию modify, выполняющую преобразование полученного контейнера. На вход
функции должен поступать контейнер, в качестве результата функция должна
возвращать модифицированный контейнер.
4. Перегруженную функцию modify, принимающую на вход итераторы начала и конца
обрабатываемой части контейнера.
5. Функцию, реализующую требуемое преобразование с использованием алгоритма
std::transform вместо функции modify.
6. Функцию, реализующую требуемое преобразование с использованием алгоритма
std::for_each вместо функции modify.
7. Функции, вычисляющие сумму и среднее арифметическое чисел, содержащихся в
контейнере.
8. Функцию вывода результата в файл и на экран (опционально).

Преобразование: Заменить все нули средним арифметическим чисел.
*/

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <iterator>
using namespace std;


//заполнение файла случайными N числами в цикле
fstream make_file_for(string filename, int count, int diapazone) {
	fstream file(filename, ios::out);
	if (!file.is_open())
	{
		throw "Не удалось открыть файл";
	}

	if (count < 1)
	{
		throw "Количество элементов должно быть положительным";
	}

	if (diapazone < 1)
	{
		throw "Число М должно быть положительным";
	}

	for(int i=0;i<count;i++) 
	{
		file << ' ' << rand() % (diapazone * 2 + 1) - diapazone;
	}
	return file;
}

//заполнение файла случайными N числами алгоритмом generate
fstream make_file_generate(string filename, int count, int diapazone) {
	fstream file(filename, std::ios::out);
	if (!file.is_open())
	{
		throw "Не удалось открыть файл";
	}

	if (count< 1)
	{
		throw "Количество элементов должно быть положительным";
	}

	if (diapazone < 1)
	{
		throw "Число М должно быть положительным";
	}

	vector<int> vect = vector<int>(count);
	generate(vect.begin(), vect.end(), [diapazone]() {return rand() % (diapazone * 2 + 1) - diapazone; });

	vector<int>::iterator iter = vect.begin();
	copy(vect.begin(),vect.end(), ostream_iterator<double>(file));

	return file;
}

//считывание последовательности из файла и формирование контейнера
vector<double> make_contnr(string filename)
{
	
	ifstream file(filename);
	if (file.fail())
	{
		throw "Не удалось открыть файл";
	}
	vector<double> vect;
	copy(istream_iterator<double>(file), istream_iterator<double>(), back_inserter(vect));
	return vect;
}


//сумма всех чисел в контейнере
double sum(vector<double>::iterator start, vector<double>::iterator end)
{
	vector<double> vect;

	vector<double>::iterator head = start;
	vector<double>::iterator tail = end;
	while (head != tail) {
		vect.push_back(*head);
		head++;
	}

	int res = 0;
	
	if (vect.empty())
	{
		throw "Невозможно вычислить сумму: контейнер пуст";
		
	}

	for each (double ex in vect)
	{
		res += ex;
	}
	return res;
}

//среднее арифметическое всех чисел контейнера
double average(vector<double>::iterator start, vector<double>::iterator end)
{
	if (start==end)
	{
		throw "Невозможно вычислить среднее арифметическое: контейнер пуст!";
	}
	return sum(start,end)/ (double)(end-start);
}

//минимальное число
double min_val(vector<double>::iterator start, vector<double>::iterator end) {
	if (start == end)
	{
		throw "Невозможно вычислить минимальное значение: контейнер пуст!";
	}
	int res = *start;
	vector<double> vect;
	vector<double>::iterator head = start;
	vector<double>::iterator tail = end;
	while (head != tail) {
		if(*head<res) res = *head;
		head++;
	}
	return res;
	

}

//частичное преобразование контейнера
vector<double> modify(vector<double>::iterator start, vector<double>::iterator end) {
	//deque<double> part_deq(start,end);

	vector<double> vect;
	double min = min_val(start,end);
	
	for (vector<double>::iterator iter = start; iter != end;iter++) {
		vect.push_back(*iter*min) ;
	}
	return vect;
}

//полное преобразование контейнера
vector<double> modify(vector<double> &vect)
{
	return modify(vect.begin(),vect.end());
}

//преобразование контейнера алгоритмом transform
vector<double> transform_cntnr(vector<double> vect)
{
	vector<double> vectNew = vector<double>(vect.size());
	double min = min_val(vectNew.begin(),vectNew.end());
	if (vect.size() == 0)
	{
		throw "Контейнер пуст!";
	}
	transform(vect.begin(), vect.end(), vectNew.begin(), [min](double value) { return value*min; }); 
	return vectNew;
}

//преобразование с помощщью алгоритма for_each
vector<double> for_each_cntnr(vector<double> vect)
{
	if (vect.size() == 0)
	{
		throw "Контейнер пуст!";
	}
	double min = min_val(vect.begin(),vect.end());
	for_each(vect.begin(), vect.end(), [min](double value) { return value*min; });
	return vect;
}

//вывод всех элементов на экран
void print(vector<double> &vect)
{
	if (vect.empty())
	{
		throw "Контейнер пуст!";
	}
	
	vector<double>::iterator iter = vect.begin();
		while(iter != vect.end())
		{
			cout << *iter << ' ';
			iter++;
		}
		cout << endl;
	
}

//вывод всех элементов контейнера в файл
void print_file(string filename, vector<double> &vect)
{
	fstream file;
	file.open(filename, ios::out);
	if (!file.is_open())
	{
		throw "Не удалось открыть файл!";
	}
	
	if (vect.empty())
	{
		file << "Контейнер пуст!" << endl;
	}
	else
	{
		vector<double>::iterator iter = vect.begin();
		while(iter!=vect.end())
		{
			file << *iter << ' ';
			iter++;
		}
		file << endl;
	}
}

//получение итератора по числу
vector<double>::iterator get_iterator(vector<double> &vect, int n)
{
	if ((n < 1) || (n > vect.size() + 1))
	{
		throw "Невозможно найти элемент с данным номером";
	}
	vector<double>::iterator iter = vect.begin();
	for(int i=1;i<n;i++)
	{
		++iter;
	}
	return iter;
}

string filename;
int N, M;
fstream file;
vector<double> contnr;
vector<double>::iterator head, tail;


//ввод имени файла, числа, диапазона
void input_filename(string &filename, int &N, int &M) {
	cout << "Введите имя файла" << endl;
	cin.clear();
	cin >> filename;
	cout << "Введите количество случайных чисел" << endl;
	while (!(cin >> N))
	{
		cin.clear();
		while (cin.get() != '\n');
		cout << "Введите число" << endl;
	}
	cout << "Введите границу диапазона случайных чисел [-M; M]" << endl;
	while (!(cin >> M))
	{
		cin.clear();
		while (cin.get() != '\n');
		cout << "Введите число" << endl;
	}
}

//ввод диапазона модификации контейнера
void input_diapazone(int &N, int &M) {
	cout << "Введите нижнюю из границ, в пределах которых будет преобразование" << endl;
	while (!(cin >> N))
	{
		cin.clear();
		while (cin.get() != '\n');
		cout << "Введите число" << endl;
	}
	cout << "Введите верхнюю из границ, в пределах которых будет преобразование" << endl;
	while (!(cin >> M))
	{
		cin.clear();
		while (cin.get() != '\n');
		cout << "Введите число" << endl;
	}
	while ((N <= 0) || (M <= 0) || (N > (int)(contnr.size() + 1)) || (M > (int)(contnr.size() + 1)))
	{
		cout << "Неверно введены номера(допустимы значения от 1 до " << contnr.size() + 1 << "). Повторите ввод!" << endl;
		try
		{
			cout << "Введите нижнюю из границ, в пределах которых будет преобразование" << endl;
			while (!(cin >> N))
			{
				cin.clear();
				while (cin.get() != '\n');
				cout << "Введите число" << endl;
			}
			cout << "Введите верхнюю из границ, в пределах которых будет преобразование" << endl;
			while (!(cin >> M))
			{
				cin.clear();
				while (cin.get() != '\n');
				cout << "Введите число" << endl;
			}
		}
		catch (char *s)
		{
			cout << s << endl;
		}
	}
	if (N > M)
	{
		N = N + M;
		M = N - M;
		N = N - M;
	}
}

//печать меню
void cases()
{
	cout << endl;
	cout << "Введите номер команды:" << endl;
	cout << "1 - заполнить файл случайными числами с помощью цикла" << endl;
	cout << "2 - заполнить файл случайными числами с помощью алгоритма std::generate" << endl;
	cout << "3 - сформировать контейнер на основе файла" << endl;
	cout << "4 - преобразовать контейнер" << endl;
	cout << "5 - преобразовать часть контейнера" << endl;
	cout << "6 - преобразовать контейнер с помощью алгоритма transform" << endl;
	cout << "7 - преобразовать контейнер с помощью алгоритма for_each" << endl;
	cout << "8 - вычислить сумму всех элементов контейнера" << endl;
	cout << "9 - вычислить среднее арифметическое всех элементов контейнера" << endl;
	cout << "10 - вывести все элементы контейнера на экран" << endl;
	cout << "11 - вывести все элементы контейнера в файл" << endl;
	cout << "0 - завершить работу программы" << endl;
	cout << endl;
}



//меню
void menu(int key)
{
	
	switch (key)
	{
	case 1:
		input_filename(filename, N, M);
		try
		{
			file = make_file_for(filename, N, M);
			file.close();
		}
		catch (char *s)
		{
			cout << s << endl;
		}
		break;

	case 2:
		input_filename(filename, N, M);
		try
		{
			file = make_file_generate(filename, N, M);
			file.close();
		}
		catch (char *s)
		{
			cout << s << endl;
		}
		break;

	case 3:
		try
		{
			string filename;
			cout << "Введите имя файла, из которого будет считана последовательность" << endl;
			cin >> filename;
			contnr = make_contnr(filename);
		}
		catch (char *s)
		{
			cout << s << endl;
		}
		break;
	case 4:
		if (!contnr.empty())
		{
			contnr = modify(contnr);
		}
		else
		{
			cout << "Контейнер пуст" << endl;
		}
		break;
	case 5:
		input_diapazone(N, M);
		try
		{
			head = get_iterator(contnr, N);
			tail = get_iterator(contnr, M);
			modify(head, tail);
		}
		catch (char *s)
		{
			cout << s << endl;
		}
		break;
	case 6:
		try
		{
			contnr = transform_cntnr(contnr);
		}
		catch (char *s)
		{
			cout << s << endl;
		}
		break;
	case 7:
		try
		{
			contnr = for_each_cntnr(contnr);
		}
		catch (char *s)
		{
			cout << s << endl;
		}
		break;
	case 8:
		try
		{
			cout << "Сумма всех чисел контейнера равна " << sum(contnr.begin(),contnr.end()) << endl;
		}
		catch (char *s)
		{
			cout << s << endl;
		}
		break;
	case 9:
		try
		{
			cout << "Среднее арифметическое всех элементов контейнера равно " << average(contnr.begin(), contnr.end()) << endl;
		}
		catch (char *s)
		{
			cout << s << endl;
		}
		break;
	case 10:
		try
		{
			print(contnr);
		}
		catch (char *s)
		{
			cout << s << endl;
		}
		break;
	case 11:
		cout << "Введите имя файла" << endl;
		cin.clear();
		cin >> filename;
		try
		{
			print_file(filename, contnr);
		}
		catch (char *s)
		{
			cout << s << endl;
		}
		break;
	case 0:
		break;
	default:
		cout << "Такой команды не существует! Повторите ввод" << endl;
		break;
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");

	int key;
	do
	{
		cases();
		while (!(cin >> key))
		{
			cin.clear();
			while (cin.get() != '\n');
			cout << "Команда вводится в форме числа" << endl;
		}
		menu(key);
	} while (key != 0);

	return 0;
	
}

