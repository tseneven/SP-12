#include <iostream> 
#include <Windows.h> 
#include <random> 
using namespace std;

int awaitTimeValue; 
int timerValue = 600; 

HANDLE printer; 

HANDLE hTimerThread; 
DWORD IDTimerThread; 

int randomInt(int a, int b) 
{ 
	static mt19937 gen(random_device{}()); 
	uniform_int_distribution<int> dis(a, b); 
	return dis(gen); 
} 
DWORD WINAPI timer(LPVOID) 
{ 
	while (timerValue != 0) 
	{ 
		Sleep(1000); 
		timerValue--; 
	} 
	cout << "Время ожидания вышло" << endl; 

	CloseHandle(hTimerThread);
	CloseHandle(printer);

	exit(0); // https://learn.microsoft.com/ru-ru/cpp/cpp/program-termination?view=msvc-170 чтоб не лукавить, вот дока 
} 


int main() 
{ 
	setlocale(0, "rus"); 
	
	LPCTSTR name = L"printer"; 
	hTimerThread = CreateThread(NULL, 0, timer, NULL, 0, &IDTimerThread); 

	printer = CreateMutex(NULL, FALSE, name); 

	cout << "Принтер готов." << endl;

	while (true) { 
		DWORD wait = WaitForSingleObject(printer, 1000);
		if (wait == WAIT_OBJECT_0) {
			ReleaseMutex(printer);
		}


		if (wait != WAIT_OBJECT_0)
		{
			WaitForSingleObject(printer, INFINITE);

			cout << "ПЕЧАТЬ" << endl;
			awaitTimeValue = randomInt(5000, 15000);
			Sleep(awaitTimeValue);
			if (awaitTimeValue >= 10000)
			{
				cout << "Ошибка печати" << endl;
			}
			timerValue = 600;
			ReleaseMutex(printer);


			cout << "Готово" << endl;
		}
		ReleaseMutex(printer);

	} 
	CloseHandle(hTimerThread); 
	CloseHandle(printer); 
	return 0; 
}

