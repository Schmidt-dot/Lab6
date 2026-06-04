#include "task2.h"

#include <iostream>
#include <fstream>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;


int main(){
//Локаль
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif

    string text;
    //Ввод
    cout << "Введите текст: ";
    getline(cin, text);

    //Генерация и (де)шифрование
    vector<uint8_t> cipherKey = CipherKey(); //Генерация ключа

    vector<uint8_t> iv = GenerateIV(); //Генерация вектора инициализации

    vector<uint8_t> expandedKey = KeyExpansion(cipherKey, N, r); //Генерация расширенных (раундовых) ключей

    vector<uint8_t> CipherText = Encryption(text, cipherKey, iv); //Шифрование текста

    string CipherResult(CipherText.begin(), CipherText.end());

    vector<uint8_t> PlainText = Decryption(CipherText, cipherKey, iv); //Дешифрование текста

    string PlainResult(PlainText.begin(), PlainText.end());


    ofstream keyFile("key.txt"); //Запись ключа в файл
    for (uint8_t byte : cipherKey){
        keyFile << hex << setw(2) << setfill('0') << (int)byte << " ";
    }   
    keyFile.close();

    //Вывод
    cout << "\nКлюч:\n";
    printHex(cipherKey);

    cout << "\nIV:\n";
    printHex(iv);
    
    cout << "\nРаундовые ключи:\n";
    printRoundKeys(expandedKey);

    cout << "\nШифротекст:\n";
    cout << "Байты: ";
    printHex(CipherText);
    cout << "Набор симовлов: " << CipherResult << endl;

    cout << "\nРасшифрованный текст:\n";
    cout << PlainResult << endl;

    cout << "\nКлюч расположен в файле key.txt\n";

    return 0;
}