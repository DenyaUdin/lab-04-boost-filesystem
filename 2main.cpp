#include <stdlib.h>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <thread>
#include <chrono>
#include <iostream>
using namespace boost;
using namespace std;
void writeToSocket(asio::ip::tcp::socket& sock, std::string& buf) {  // Запись данных в сокет
	std::size_t total_bytes_written = 0;
	while (total_bytes_written != buf.length()) {
		total_bytes_written += sock.write_some(
			asio::buffer(buf.c_str() +
			total_bytes_written,
			buf.length() - total_bytes_written));
	}
}

bool flag = true; // Флаг работы клиента

// Потоковая функция клиент
void ClientThread(string clientName) // Параметр функции - имя клиента
{
	std::string raw_ip_address = "127.0.0.1"; // Адрес сервера - локальеный компьютер (localhost)
	unsigned short port_num = 3333; // Номер порта
	asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address),	port_num); // Создаем точку доступа
	asio::io_service ios;
	asio::ip::tcp::socket sock(ios, ep.protocol()); // Создаем сокет клиента
	sock.connect(ep); // подключение сокета к точке доступа

	// Отправляем данные серверу
	string strToServ = clientName;  // Первый раз отправляем имя клиент
	cout << "To server: " << strToServ << endl; // Печать данных отправляемых на сервер
	writeToSocket(sock, strToServ); // Отвправка - запись в сокет
	// Получаем ответ сервера
	boost::array<char, 256> buf; // Буфер для приема сообщения от сервера
	boost::system::error_code error; // Суда записываем код ошибки (не используем)
	size_t len = sock.read_some(boost::asio::buffer(buf), error); // Читаем данные от сервера
	buf[len] = 0; // Строка заканчивается 0
	cout  << "From server: " << buf.data(); // Печать данных от сервера
	while (flag) // В цикле посылаем запрос, получаем ответ
	{
		switch (rand() % 3 + 1) // Случайно выбирает тип запроса
		{
		case 1:	strToServ = "ping\n"; break;
		case 2:	strToServ = "clients\n"; break;
		case 3:	strToServ = "Hernауауцya\n"; break;
		}
		cout << "To server: " << strToServ ; // Печать данных отправляемых на сервер
		writeToSocket(sock, strToServ); // Отвправка - запись в сокет
		// Получаем ответ сервера
		size_t len = sock.read_some(boost::asio::buffer(buf), error); // Читаем данные от сервера
		buf[len] = 0; // Строка заканчивается 0
		cout << "From server: " << buf.data() ; // Печать данных от сервера
		this_thread::sleep_for(chrono::seconds(1)); // Задержка 1 сек
	}

	// Завершение приема передачи, закрытие сокета
	sock.shutdown(asio::socket_base::shutdown_send);
	// Close socket (is done by dtor)
	sock.close();
	
}

// Приложение клиент
void main()
{
	
	// Создаем потоки динамически
	cout << "For start thread, press Enter!\n";
	cout << "For exit, press Enter!\n";
	cin.get(); // Ждем нажатия Enter
	thread t1(ClientThread, "MyClient145");
	cout << "Thread1 Start Ok!!!!!\n";
	this_thread::sleep_for(chrono::seconds(5)); // Задерка 5 сек
	thread t2(ClientThread, "FghghhhMyClient");
	cout << "Thread2 Start Ok!!!!!\n";
	cin.get(); // Ждем нажатия Enter
	flag = false; // завершение потоки
	t1.join();
	t2.join();
	cout << "Client is end!\n";
	::system("pause");

}
