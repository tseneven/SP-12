#include <iostream>
#include <Windows.h>
#include <conio.h>

using namespace std;

HANDLE printer;

int main() {

	setlocale(0, "rus");
	printer = OpenMutex(MUTEX_ALL_ACCESS, NULL, L"printer");


	cout << "Выберите приоритет:" << endl;
	cout << "1 - ФОНОВЫЙ ПРОЦЕСС" << endl;
	cout << "2 - НОРМАЛЬНЫЙ ПРИОРИТЕТ" << endl;
	cout << "3 - ВЫСОКИЙ ПРИОРИТЕТ" << endl;

	short action;

	cin >> action;

	switch (action) {
	case 1: 
		SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS);
		break;
	case 2:
		SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
		break;
	case 3:
		SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
		break;
	}

	while (true) {
		DWORD wait = WaitForSingleObject(printer, 1000);

		if (!printer) {
			cout << "Принтер занят или не запущен. Повтор попытки через 10 секунд";
			Sleep(10000);
		}
		else {
			if (wait == WAIT_OBJECT_0) {
				cout << "Принтер готов. Нажмите клавишу\n";
				_getch();
				cout << "Задание отправлено на печать\n" << endl;
				ReleaseMutex(printer);
			}
			else if (wait == WAIT_TIMEOUT) {
				cout << "Принтер занят" << endl;
			}
		}
	}

	return 0;
}

// Из-за повышенного приоритета процесса, мьютекс захватывается чаще, и наоборот, чем ниже приоритет процесса, тем реже он захватывает мьютекс