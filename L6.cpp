#include "pch.h"
#include <iostream>
#include <iomanip>
#include <conio.h>
using namespace std;

bool CheckAddress(char* ip_);
bool CheckMask(char* mask_);
unsigned long long CharToULong(char* ip_);
void ULongToChar(unsigned long long  n, unsigned  long long m);

void main()
{
	setlocale(0, "");

	unsigned long long ip, mask; 
	char *cIP = new char[16];
	char *cMask = new char[16];

	cout << setiosflags(ios::left) << setw(20) << "Введите IP:";
	cin >> cIP;
	cout << setiosflags(ios::left) << setw(20) << "Введите маску:";
	cin >> cMask;

	if (CheckAddress(cIP))
	{
		if (CheckMask(cMask))
		{
			ip = CharToULong(cIP);
			mask = CharToULong(cMask);

			ULongToChar(ip, mask);
		}
		else
			cout << "Неправильно введена маска\n";
	}
	else
		cout << "Неправильно введен IP\n";
}


bool CheckAddress(char* ip_)//Проверка IP-адреса на правильность
{
	int points = 0;
	int numbers = 0;
	char* buff = new char[3];

	for (int i = 0; ip_[i] != '\0'; i++)
	{
		if (ip_[i] <= '9' && ip_[i] >= '0')
		{
			if (numbers== 3)    
				return false;
			buff[numbers++] = ip_[i];
		}
		else if (ip_[i] == '.')
		{
			if (numbers == 0)////если числа нет - ошибка 
				return false;

			if (atoi(buff) > 255)//приведения(конвертации) строки в числовой вид.
				return false;

			numbers = 0;
			points++;
			delete buff;
			buff = new char[3];
		}
		else return false;
	}

	if (points != 3)// если количество точек в IP-адресе не 3 - ошиб
		return false;

	return true;//Если IP удовлетворяет всем условиям, то возвращаем true
}

//Затем необходимо выполнить преобразование строки IP - адреса(16 символов)
//в десятично - точечной нотации, содержащей 4 группы по 3 символа(октета),
//разделенных точками, в число unsigned long.Это можно выполнить,
//написав собственную функцию проверки и преобразования CharToULong :
unsigned long long CharToULong(char* ip_)//Возвращает IP в unsigned long long и без точек
{
	unsigned long long out = 0; //число для IP-адреса 
	char* buff = new char[3];//буфер для хранения одного октета

	for (int i = 0, j = 0; ip_[i] != '\0'; i++, j++)
	{
		if (ip_[i] != '.')
			buff[j] = ip_[i];

		if (ip_[i] == '.' || ip_[i + 1] == '\0')
		{
			out *= 1000;//сдвинуть число на 8 бит 
			out += (unsigned long long)atoi(buff);//преобразовать и добавить к числу IP-адреса 
			delete buff;
			buff = new char[3];
			j = -1;
		}
	}
	return out;
}

//Аналогично для маски следует проверить пределы допустимого диапазона октета
//	: от 0 до 255 и выполнить преобразование в число.
//После чего надо проверить корректность задания маски подсети.
bool CheckMask(char* mask_)//Проверка маски на правильность
{
	int points = 0;
	int numbers = 0;
	char* buff = new char[3];

	bool zero = false;
	int oct;
	int broadscast = 0;
	
	for (int i = 0; mask_[i] != '\0'; i++)
	{
		if ((mask_[i] <= '9' && mask_[i] >= '0') && (mask_[i + 1] != '\0'))
		{
			if (numbers ==3)
				return false;
			buff[numbers++] = mask_[i];
		}
		else if ((mask_[i] == '.') || (mask_[i + 1] == '\0'))
		{
			if (mask_[i + 1] == '\0')
			{
				if (numbers == 3)
					return false;
				buff[numbers++] = mask_[i];
			}

			oct = atoi(buff);//Преобразует строку в целое число
			if (oct > 255)
				return false;

			switch (oct)
			{
			case 0:
				zero = true;
				break;

			case 128: // 1 0 0 0 0 0 0 0
				if (zero == false)
					zero = true;
				else
					return false;
				break;

			case 192: // 1 1 0 0 0 0 0 0
				if (zero == false)
					zero = true;
				else
					return false;
				break;

			case 224: // 1 1 1 0 0 0 0 0
				if (zero == false)
					zero = true;
				else
					return false;
				break;

			case 240: // 1 1 1 1 0 0 0 0
				if (zero == false)
					zero = true;
				else
					return false;
				break;

			case 248: // 1 1 1 1 1 0 0 0
				if (zero == false)
					zero = true;
				else
					return false;
				break;

			case 252: // 1 1 1 1 1 1 0 0
				if (zero == false)
					zero = true;
				else
					return false;
				break;

			case 254: // 1 1 1 1 1 1 1 0
				if (zero == false)
					zero = true;
				else
					return false;
				break;

			case 255: // 1 1 1 1 1 1 1 1
				if (zero == false)
					broadscast++;
				else
					return false;
				break;

			default:
				return false;
			}

			delete buff;
			buff = new char[3];
			numbers = 0;
			points++;
			if (mask_[i + 1] == '\0')//если это был последний октет
				points--;//Уменьшаем счетчик точек
		}
		else return false;//Если рассматриваемый символ - не цифра и не точка, то ошибка
	}

	if (broadscast == 4)//если все цифры маски - единицы
		return false;

	if (points != 3) 
		return false;//Если в маске число точек больше 3, то ошибка

	return true;//Если маска удовлетворяет всем условиям, то возвращаем true
}


void ULongToChar(unsigned long long n, unsigned long long m)//Подсчет и вывод на экран Network ID и HOST ID
{
	int oneN = n / 1000000000;
	int twoN = (n / 1000000) - (oneN * 1000);
	int threeN = (n / 1000) - (oneN * 1000000) - (twoN * 1000);
	int fourN = n - (oneN * 1000000000) - (twoN * 1000000) - (threeN * 1000);
	//cout << oneN << "oneN" << twoN << "twoN" << threeN << " threeN" << fourN << "fourN" << endl;

	int oneM = m / 1000000000;
	int twoM = (m / 1000000) - (oneM * 1000);
	int threeM = (m / 1000) - (oneM * 1000000) - (twoM * 1000);
	int fourM = m - (oneM * 1000000000) - (twoM * 1000000) - (threeM * 1000);
	//cout << oneM << "oneM" << twoM << "twoM" << threeM << " threeM" << fourM << "fourM" << endl;

	//ID подсети
	int oneS = oneN & oneM;
	int twoS = twoN & twoM;
	int threeS = threeN & threeM;
	int fourS = fourN & fourM;
	//cout <<"NetworkID"<< oneS << "oneS" << twoS << "twoS" << threeS << " threeS" << fourS << "fourS" << endl;
	//ID хоста
	int oneH = oneN & ~oneM;
	int twoH = twoN & ~twoM;
	int threeH = threeN & ~threeM;
	int fourH = fourN & ~fourM;

	//cout << "HOSTID" << oneH << "oneH" << twoH << "twoH" << threeH << " threeH" << fourH << "fourH" << endl;
	//"Broadcast
	int oneB = oneN & oneM;
	int twoB = twoN & twoM;
	int threeB = threeN & threeM;
	int fourB = fourN & fourM;
	if (oneM<255) {
		oneB = (255)& ~oneM;
	}
	if (twoM<255) {
		twoB = (255) & ~twoM;
	}
	if (threeM<255) {
		threeB = (255) & ~threeM;
	}
	if (fourM<255) {
		fourB = (255) & ~fourM;
	}
	cout << setiosflags(ios::left) << setw(20) << "ID хоста:";
	cout << oneH << "." << twoH << "." << threeH << "." << fourH << endl;
	cout << setiosflags(ios::left) << setw(20) << "Broadcast:";
	cout << oneB << "." << twoB << "." << threeB << "." << fourB << endl;
	
	_getch();
}















//Маской сети называется битовая маска, определяющая, какая часть IP-адреса узла сети относится к адресу сети, а какая — к адресу самого узла в этой сети.
//network ID - идентификатор сети (часть IP - адреса, которая определяет группу компьютеров и устройств одной логической сети)
//host ID -	идентификатор хоста (часть IP - адреса, определяющая принадлежность компьютера к определенной сети)
//broadCast - широковещательный адрес сети //последний адрес в подсети
//калькулятор нужен, чтобы узнавать какие-то данные, например, BroadCast
//Формат IP-адреса достаточно удобен для человека: четыре цифры, разделенные точками. Однако такой формат очень не удобен при программировании, 
//поэтому IP-адреса преобразуют в обычное число для удобства