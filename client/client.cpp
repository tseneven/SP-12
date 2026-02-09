#include <iostream>
#include <Windows.h>
#include <conio.h>

using namespace std;

HANDLE printer;

int main() {

	setlocale(0, "rus");

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

	printer = OpenMutex(MUTEX_ALL_ACCESS, NULL, L"printer");

	while (true) {
		DWORD wait = WaitForSingleObject(printer, 10);

		if (!printer) {
			cout << "Принтер занят или не запущен. Повтор попытки через 10 секунд";
			Sleep(10000);
		}
		else {
			if (wait == WAIT_OBJECT_0) {
				cout << "Принтер готов. Нажмите клавишу" << endl;
				_getch();
				cout << "Задание отправлено на печать" << endl;
			}
			else if (wait == WAIT_TIMEOUT) {
				cout << "Принтер занят" << endl;
			}
			ReleaseMutex(printer);
		}
	}

	return 0;
}

