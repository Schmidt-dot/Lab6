#include "task2.h"

#include<iostream>

using namespace std;


//БЛОК 1


//ПЕРЕВОД ТЕКСТА В БАЙТЫ
vector<uint8_t> addPadding(const string& text){
    vector<uint8_t> bytes(text.begin(), text.end());

    int pkcs7 = 16 - (bytes.size() % 16);

    if (pkcs7 == 0){
        pkcs7 = 16;
    }

    for (int i = 0; i < pkcs7; ++i){
        bytes.push_back(uint8_t(pkcs7));
    }
    return bytes;
}


//УДАЛЕНИЕ PKCS7 ДЛЯ ДЕШИФРОВАНИЯ
vector<uint8_t> removePadding(vector<uint8_t> bytes){

    if (bytes.empty()){
        return bytes;
    }

    int pkcs7 = bytes.back();

    if (pkcs7 < 1 || pkcs7 > 16){
        return bytes;
    }

    for (int i = 0; i < pkcs7; ++i){
        bytes.pop_back();
    }
    return bytes;
}


//ВЫВОД HEX
void printHex(const vector<uint8_t>& data){

    for (int i = 0; i < data.size(); ++i){
        cout << hex << (int)data[i] << " ";
    }
    cout << dec << endl;
}


//ВЫВОД STATE
void printState(const vector<uint8_t>& block){

    for (int row = 0; row < 4; ++row){
        for (int col = 0; col < 4; ++col){
            cout << hex << (int)block[col * 4 + row] << " ";
        }
        cout << endl;
    }
    cout << dec << endl;
}



//БЛОК 2. ГЕНЕРАЦИЯ КЛЮЧЕЙ


//ГЕНЕРАЦИЯ КЛЮЧА
vector<uint8_t> ChiperKey(){

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(0, 255);

    vector<uint8_t> key(16);

    for (int i = 0; i < 16; ++i){
        key[i] = static_cast<uint8_t>(dis(gen));
    }
    return key;
}


//ГЕНЕРАЦИЯ IV
vector<uint8_t> GenerateIV(){

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(0, 255);
    vector<uint8_t> iv(16);

    for (int i = 0; i < 16; ++i){
        iv[i] = static_cast<uint8_t>(dis(gen));
    }
    return iv;
}


//ЦИКЛИЧЕСКИЙ СДВИГ СЛОВА
vector<uint8_t> RotWord(vector<uint8_t> word){

    uint8_t temp = word[0];

    word[0] = word[1];
    word[1] = word[2];
    word[2] = word[3];
    word[3] = temp;

    return word;
}


//ПОДСТАНОВКА WORD ЧЕРЕЗ SBOX
vector<uint8_t> SubWord(vector<uint8_t> word){
    for (int i = 0; i < 4; ++i){
        word[i] = Sbox[word[i]];
    }
    return word;
}


//ПРЕОБРАЗОВАНИЕ КЛЮЧА
vector<uint8_t> KeyExpransion(const vector<uint8_t>& cipherKey){
    return expandedKey;
}


//ПОЛУЧЕНИЕ КЛЮЧА РАУНДА
vector<uint8_t> GetRoundKey(const vector<uint8_t>& expandedKey, int round){
    vector<uint8_t> roundKey(16);

    for (int i = 0; i < 16; ++i){

        roundKey[i] = expandedKey[round * 16 + i];
    }
    return roundKey;
}


//ДОБАВЛЕНИЕ РАУНДОВОГО КЛЮЧА
void AddRoundKey(
    vector<uint8_t>& block,
    const vector<uint8_t>& roundKey){

    for (int i = 0; i < 16; ++i){
        block[i] ^= roundKey[i];
    }
}



//БЛОК 4. ПОДСТАНОВКА БАЙТОВ


//ПОДСТАНОВКА БАЙТОВ
void SubBytes(vector<uint8_t>& data){
    for (int i = 0; i < 16; ++i){
        data[i] = Sbox[data[i]];
    }
}


//ОБРАТНАЯ ПОДСТАНОВКА
void InvSubBytes(vector<uint8_t>& data){
    for (int i = 0; i < 16; ++i){
        data[i] = InvSbox[data[i]];
    }
}



// БЛОК 5. СДВИГ СТРОК


/*
0 4  8 12
1 5  9 13
2 6 10 14
3 7 11 15
*/


//СДВИГ СТРОК
void ShiftRows(vector<uint8_t>& data){

    vector<uint8_t> block = data;

    // 0 строка: сбвиг на 0 байт

    // 1 строка: сбвиг на 1 байт
    data[1] = block[5];
    data[5] = block[9];
    data[9] = block[13];
    data[13] = block[1];

    // 2 строка: сбвиг на 2 байт
    data[2] = block[10];
    data[6] = block[14];
    data[10] = block[2];
    data[14] = block[6];

    // 3 строка: сбвиг на 3 байт
    data[3] = block[15];
    data[7] = block[3];
    data[11] = block[7];
    data[15] = block[11];
}


//ОБРАТНЫЙ СДВИГ СТРОК
void InvShiftRows(vector<uint8_t>& data){

    vector<uint8_t> block = data;

    // 0 строка: сбвиг на 0 байт

    // 1 строка: сбвиг на 1 байт
    data[1] = block[13];
    data[5] = block[1];
    data[9] = block[5];
    data[13] = block[9];

    // 2 строка: сбвиг на 2 байт
    data[2] = block[10];
    data[6] = block[14];
    data[10] = block[2];
    data[14] = block[6];

    // 3 строка: сбвиг на 3 байт
    data[3] = block[7];
    data[7] = block[11];
    data[11] = block[15];
    data[15] = block[3];
}



//БЛОК 6. СМЕШИВАНИЕ СТОЛБЦОВ

/*Смешение
2 3 1 1
1 2 3 1
1 1 2 3
3 1 1 2
*/

//УМНОЖЕНИЕ НА 2
uint8_t MultGaloisField_2(uint8_t value){
    bool highBit = value & 0x80;

    value <<= 1;

    if (highBit){
        value ^= 0x1b;
    }
    return value;
}


//УМНОЖЕНИЕ НА 3
uint8_t MultGaloisField_3(uint8_t value){
    return MultGaloisField_2(value)^value;
}

/*Обратное смешение
14 11 13 9
9 14 11 13
13 9 14 11
11 13 9 14
*/

//УМНОЖЕНИЕ НА 9
uint8_t MultGaloisField_9(uint8_t value){
    return MultGaloisField_2(MultGaloisField_2(MultGaloisField_2(value)))^value;
}


//УМНОЖЕНИЕ НА 11
uint8_t MultGaloisField_11(uint8_t value){
    return MultGaloisField_2(MultGaloisField_2(MultGaloisField_2(value)))^MultGaloisField_2(value)^value;
}


//УМНОЖЕНИЕ НА 13
uint8_t MultGaloisField_13(uint8_t value){
    return MultGaloisField_2(MultGaloisField_2(MultGaloisField_2(value)))^MultGaloisField_2(MultGaloisField_2(value))^value;
}


// УМНОЖЕНИЕ НА 14
uint8_t MultGaloisField_14(uint8_t value){
    return MultGaloisField_2(MultGaloisField_2(MultGaloisField_2(value)))^MultGaloisField_2(MultGaloisField_2(value))^MultGaloisField_2(value);
}


/*
b0=2⋅a0+3⋅a1+1⋅a2+1⋅a3
b1=1⋅a0+2⋅a1+3⋅a2+1⋅a3
b2=1⋅a0+1⋅a1+2⋅a2+3⋅a3
b3=3⋅a0+1⋅a1+1⋅a2+2⋅a3
*/


// СМЕШИВАНИЕ СТОЛБЦОВ
void MixColumns(vector<uint8_t>& data){

    for (int i = 0; i < 16; i += 4){

        uint8_t a0 = data[i];
        uint8_t a1 = data[i + 1];
        uint8_t a2 = data[i + 2];
        uint8_t a3 = data[i + 3];

        //1 строка
        data[i] = MultGaloisField_2(a0)^MultGaloisField_3(a1)^a2^a3;

        //2 строка
        data[i + 1] = a0^MultGaloisField_2(a1)^MultGaloisField_3(a2)^a3;

        //3 строка
        data[i + 2] = a0^a1^MultGaloisField_2(a2)^MultGaloisField_3(a3);

        //4 строка
        data[i + 3] = MultGaloisField_3(a0)^a1^a2^MultGaloisField_2(a3);
    }
}


//ОБРАТНОЕ СМЕШИВАНИЕ
void InvMixColumns(vector<uint8_t>& block){

    for (int i = 0; i < 16; i += 4){

        uint8_t a0 = block[i];
        uint8_t a1 = block[i + 1];
        uint8_t a2 = block[i + 2];
        uint8_t a3 = block[i + 3];

        //1 строка
        block[i] = MultGaloisField_14(a0)^MultGaloisField_11(a1)^MultGaloisField_13(a2)^MultGaloisField_9(a3);
          
        //2 строка
        block[i + 1] = MultGaloisField_9(a0)^MultGaloisField_14(a1)^MultGaloisField_11(a2)^MultGaloisField_13(a3);

        //3 строка
        block[i + 2] = MultGaloisField_13(a0)^MultGaloisField_9(a1)^MultGaloisField_14(a2)^MultGaloisField_11(a3);

        //4 строка
        block[i + 3] = MultGaloisField_11(a0)^MultGaloisField_13(a1)^MultGaloisField_9(a2)^MultGaloisField_14(a3);
    }
}



// БЛОК 7. ШИФРОВАНИЕ


//ШИФРОВАНИЕ
vector<uint8_t> Encryption(){}



//БЛОК 8. ДЕШИФРОВАНИЕ


//ДЕШИФРОВАНИЕ
vector<uint8_t> Decryption(){}