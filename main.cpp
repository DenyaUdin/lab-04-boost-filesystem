#include <boost/filesystem.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
using namespace boost::filesystem;
using namespace std;
class Count // Информация о счете
{
public:
	string broc_id; // Идентификатор брокера
	string numcount; // Номер счета в виде строки
	int NumFile; // Счетчик файлов с информацией о счете
	int day, mes, year; // Дата 
	// Перегрузка операции > 
	// Оператор-функция
	bool operator>(Count & c2)  // Сравнение дат
	{   
		if (year > c2.year) return true;
		else
		{
			if (year < c2.year) return false;
			if (mes > c2.mes) return true;// Попадаем сюда если год одинаковый
			else
			{
				if (mes < c2.mes) return false;
				if (day > c2.day) return true; // Попадаем сюда если месяц одинаковый
				else
				{
					return false;
				}

			}
		}
	}
	// Конструктор
	Count(string broc_id, string numcount, int day, int mes, int year)
	{
	   NumFile=1; // Счетчик файлов
	   this->broc_id=broc_id;
	   this->numcount = numcount;
	   this->day = day;
	   this->mes = mes;
	   this->year = year;
	  
	}
};

int main(int argc, char *argv[])
{
	vector <Count> VC; // Вектор разных счетов
	path mainpath; // Главный каталог
	if (argc < 2) //  Параметров командной строки нет (только имя запускаемого файла)
	{
		mainpath = current_path(); // Текущий каталог
	}
	else // Параметры командной строки есть
	{
		mainpath = path(argv[1]); // Имя каталога через параметр командной строки
	}
	setlocale(LC_ALL, "rus");
	bool flag2 = false; // Флаг если не найдем требуемых файл
	cout << mainpath << endl; // Печать имени каталога
	if (!is_directory(mainpath)) // Если это не каталог, то сообщение об ошибке
	{
		cout << "Error!! Directory not found!" << endl;
		system("pause");
		return 0;
	}
	for (const directory_entry& x : directory_iterator{ mainpath }) // Цикл просмотра содержимого главного каталога
	{ // x - элемент каталога (файл или каталог)
		if (is_directory(x.path())) //  Это каталог (х) - имя каталога - это имя брокера
		{
			for (const directory_entry& y : directory_iterator{ x.path() }) // Просматриваем подкаталог (имя подкаталога - имя брокера)
			{  // y - элемент каталога (файл или каталогог)
				if (is_regular_file(y.path())) // Это файл (не каталог)
				{
					string name = y.path().stem().string(); // Имя файла (без расширения)
					string ext = y.path().extension().string(); // Расширение файла
					int pos = name.find("balance_");  // Ищет строку "balance_"
					if (pos == 0 && ext==".txt") // Строка "balance_" начинается с начала и расширение .txt
					{
						if (name.length()==25) // Длина имени файла всегда такая
						if (name[16] == '_') //  На этом месте всегда символ '_'
						{
							//cout << "ok" << endl;
							string numcount = name.substr(8, 8); // Извлекаем в строку номер счета
							string buf; // Для чтения года, месяца и дня
							//cout << numcount << endl;
							istringstream sin1(numcount); // Поток для чтения из строки
							int Num, dd, mm, yy; // Номер счета, год, месяц, день как целые числа
							sin1 >> Num; // Читаем номер счета как целое число
							if (sin1.fail()) continue; // При ошибке чтения переходим к следующему файлу этот файле не имеет нужный формат
							buf = name.substr(17, 4);  // Читаем в виде строки год
							//cout << buf << endl;
							istringstream sin2(buf); // Поток для чтения из строки
							sin2 >> yy; // Читаем год из строки как целое число
							if (sin2.fail()) continue; // При ошибке чтения переходим к следующему файл
							if (yy < 1900) continue; // Проверка допустимости года
							buf = name.substr(21, 2); // Читаем в виде строки номер месяца
							//cout << buf << endl;
							istringstream sin3(buf); // Поток для чтения из строки
							sin3 >> mm; // Читаем месяц из строки
							if (sin3.fail()) continue; // При ошибке чтения переходим к следующему файл
							if (mm < 1 || mm>12) continue;
							buf = name.substr(23, 2); // Читаем в виде строки день
							//cout << buf << endl;
							istringstream sin4(buf); // Поток для чтения из строки
							sin4 >> dd; // Читаем день из строки
							if (sin4.fail()) continue; // При ошибке чтения переходим к следующему файл
							if (dd < 0 || dd>31) continue;
							// Печать о прочитанном
							flag2 = true; // Нашли хотя бы 1 элемент, удовлетворящий требованиям
							cout << x.path().filename().string() << ' ' << y.path().filename().string() << endl;
							bool flag = false; // Флаг показываем есть такой счет или нет
							for (auto & pos : VC) // Просматриваем сохраненные счета
							{
								if (pos.numcount == numcount) // Нашли такой счет
								{
									pos.NumFile++; // Счетчик файлов
									// Создаем новый объект (прочитанный счет)
									Count C2(x.path().filename().string(), numcount, dd, mm, yy);
									if (C2>pos) // Нашли более поздний счет (вызов перегруженной оператор-функции)
									{
									// Обновляем данные
									pos.broc_id=C2.broc_id;
									pos.day=dd;
									pos.mes=mm;
									pos.year=yy;
									}
									flag = true; // Такой счет есть
									break;

								}
							}
							if (!flag) // Такого счета не было, создаем новый, добаляем его в вектор
							{
							   Count C2(x.path().filename().string(), numcount, dd, mm, yy);
							  // cout <<"1) "<< x.path().filename().string() << ' ' << numcount << ' ' << dd << ' ' << mm << ' ' << yy<<endl;
							   VC.push_back(C2);
							}

						}
					}
				}
			}
		}
		//std::cout << x.path() << std::endl;
	}
    // Печать информации о найденных счетах
    for (auto  pos : VC) // Просматриваем сохраненные счета
    {
           //broker:ib account:00001234 files:10 lastdate:20181017
		cout << "broker: " << pos.broc_id << " account: " << pos.numcount << " files: " << pos.NumFile << " lastdate:" << pos.year << setfill('0') << setw(2) << pos.mes << setw(2) << pos.day << endl;
    }
	if (!flag2) // Нет элементов, удовлетворяющих требованиям
	{
		cout << "Files not found!!!" << endl;
	}
	system("pause");
	return 1;
}
