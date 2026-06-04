#include "task2.h"

#include<iostream>

using namespace std;


//БЛОК 1


//ПЕРЕВОД ТЕКСТА В БАЙТЫ
vector<uint8_t> addPadding(const string& text){
    vector<uint8_t> bytes(text.begin(), text.end()); //строка в символы uint8_t

    int pkcs7 = N - (bytes.size() % N); //сколько до кратности 

    if (pkcs7 == 0){ //если кратно добавить ещё блок (правила pksc7)
        pkcs7 = N;
    }

    //Добавление
    for (int i = 0; i < pkcs7; ++i){
        bytes.push_back(uint8_t(pkcs7));
    }
    return bytes;
}


//УДАЛЕНИЕ PKCS7 ДЛЯ ДЕШИФРОВАНИЯ
vector<uint8_t> removePadding(vector<uint8_t> bytes){

    if (bytes.empty()){ //не пустой ли массив
        return bytes;
    }

    int pkcs7 = bytes.back(); //последний байт = число добавленных

    if (pkcs7 < 1 || pkcs7 > N){ //послдений байт 1 < pkcs7 < 16
        return bytes;
    }

    if (bytes.size() < static_cast<size_t>(pkcs7)){
        return bytes;
    }

    for (int i = 0; i < pkcs7; ++i){
        if (bytes[bytes.size() - 1 - i] != pkcs7){
            return bytes;
        }
    }

    //Удаление
    for (int i = 0; i < pkcs7; ++i){
        bytes.pop_back();
    }
    return bytes;
}


//ВЫВОД HEX
void printHex(const vector<uint8_t>& data){

    for (size_t i = 0; i < data.size(); ++i){
        cout << hex << setw(2) << setfill('0') << (int)data[i] << " ";
    }
    cout << dec << endl;
}
/*
0, 1, 2, 3, 4, 5,  6, 7, 8, 9, 10, 11, 12, 13, 14, 16

0 4  8 12       
1 5  9 13
2 6 10 14
3 7 11 15      
*/
//ВЫВОД STATE
void printState(const vector<uint8_t>& block){

    for (int row = 0; row < 4; ++row){
        for (int col = 0; col < 4; ++col){
            cout << hex << setw(2) << setfill('0') << (int)block[col * 4 + row] << " ";
        }
        cout << endl;
    }
    cout << dec << endl;
}



//БЛОК 2. ГЕНЕРАЦИЯ КЛЮЧЕЙ


//ГЕНЕРАЦИЯ КЛЮЧА
vector<uint8_t> CipherKey(){

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(0, 255); //2^8 значений 

    vector<uint8_t> key(N);

    for (int i = 0; i < N; ++i){
        key[i] = static_cast<uint8_t>(dis(gen));
    }
    return key;
}


//ГЕНЕРАЦИЯ IV (вектор инициализации)
vector<uint8_t> GenerateIV(){

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(0, 255);
    vector<uint8_t> iv(N);

    for (int i = 0; i < N; ++i){
        iv[i] = static_cast<uint8_t>(dis(gen));
    }
    return iv;
}


//ЦИКЛИЧЕСКИЙ СДВИГ СЛОВА (байты сдвигаются влево)
vector<uint8_t> RotWord(vector<uint8_t> word){ 

    uint8_t temp = word[0];

    word[0] = word[1];
    word[1] = word[2];
    word[2] = word[3];
    word[3] = temp;

    return word;
}


//ПОДСТАНОВКА WORD ЧЕРЕЗ SBOX (табличная замена)
vector<uint8_t> SubWord(vector<uint8_t> word){
    for (int i = 0; i < 4; ++i){
        word[i] = Sbox[word[i]];
    }
    return word;
}


//ПРЕОБРАЗОВАНИЕ КЛЮЧА
vector<uint8_t> KeyExpansion(const vector<uint8_t>& CipherTextherKey, const int& N, const int& r){
    
    vector<uint8_t> expandedKey(N*r + N); //массив для всех раундовых ключей

    for (int i = 0; i < N; ++i){ 
        expandedKey[i] = CipherTextherKey[i];
    }

    int bytesGenerated = N; 
    int rconIndex = 0;
    vector<uint8_t> temp(4); //временное слово (строка из 4 байт)

    while (bytesGenerated < (N*r + N)){

        for (int i = 0; i < 4; ++i){ //ключ делится на слова
            temp[i] = expandedKey[bytesGenerated - 4 + i]; //по 4 байта
        }

        // КАЖДЫЕ 16 БАЙТ
        if (bytesGenerated % N == 0){

            temp = RotWord(temp);

            temp = SubWord(temp);

            temp[0] ^= Rcon[rconIndex];

            ++rconIndex;
        }

        //
        for (int i = 0; i < 4; ++i){

            expandedKey[bytesGenerated] = expandedKey[bytesGenerated - N]^temp[i];

            ++bytesGenerated;
        }
    }

    return expandedKey;
}


//ПОЛУЧЕНИЕ КЛЮЧА РАУНДА
vector<uint8_t> GetRoundKey(const vector<uint8_t>& expandedKey, int round){
    vector<uint8_t> roundKey(N);

    for (int i = 0; i < N; ++i){

        roundKey[i] = expandedKey[round * N + i];
    }
    return roundKey;
}


//ВЫВОД КЛЮЧА НА КАЖДОМ РАУНДЕ
void printRoundKeys(const vector<uint8_t>& expandedKey){

    for (int round = 0; round <= r; ++round){

        cout << "\nКлюч раунда " << round << ":\n";

        printHex(
            GetRoundKey(
                expandedKey,
                round
            )
        );
    }
}


//ДОБАВЛЕНИЕ РАУНДОВОГО КЛЮЧА
void AddRoundKey(vector<uint8_t>& block, const vector<uint8_t>& roundKey){
    for (int i = 0; i < N; ++i){
        block[i] ^= roundKey[i];
    }
}



//БЛОК 4. ПОДСТАНОВКА БАЙТОВ


//ПОДСТАНОВКА БАЙТОВ
void SubBytes(vector<uint8_t>& data){
    for (int i = 0; i < N; ++i){
        data[i] = Sbox[data[i]]; //байт = индекс 
    }
}


//ОБРАТНАЯ ПОДСТАНОВКА
void InvSubBytes(vector<uint8_t>& data){
    for (int i = 0; i < N; ++i){
        data[i] = InvSbox[data[i]]; //байт = индекс 
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

//УМНОЖЕНИЕ В ПОЛЕ ГАЛУА   

//УМНОЖЕНИЕ НА 2
uint8_t MultGaloisField_2(uint8_t value){
    bool highBit = value & 0x80; //потерялся ли бит

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

    for (int i = 0; i < N; i += 4){

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

    for (int i = 0; i < N; i += 4){

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

//ШИФРОВАНИЕ ОДНОГО БЛОКА ДАННЫХ
vector<uint8_t> EncryptBlock(vector<uint8_t> PlainText, const vector<uint8_t>& expandedKey){

    vector<uint8_t> CipherText = PlainText;

    AddRoundKey(CipherText, GetRoundKey(expandedKey, 0)); 

    cout << "\nСостояние после раунда 0:\n";
    printState(CipherText); //состояние текста после 0 раунда

    for (int round = 1; round < r; ++round){

        SubBytes(CipherText);

        ShiftRows(CipherText);

        MixColumns(CipherText);

        AddRoundKey(CipherText, GetRoundKey(expandedKey, round));

        cout << "\nСостояние после раунда " << round << ":\n";
        printState(CipherText); //состояние текста после каждого раунда
    }

    SubBytes(CipherText);

    ShiftRows(CipherText);

    AddRoundKey(CipherText, GetRoundKey(expandedKey, r));

    cout << "\nСостояние после раунда " << r << ":\n";
    printState(CipherText);

    return CipherText;
}

//ШИФРОВАНИЕ
vector<uint8_t> Encryption(const string& text, const vector<uint8_t>& CipherTextherKey, const vector<uint8_t>& iv){
    
    vector<uint8_t> CipherText;

    vector<uint8_t> bytes = addPadding(text); //добавление до кратности
    vector<uint8_t> expandedKey = KeyExpansion(CipherTextherKey, N, r); //расширенный ключ
    vector<uint8_t> previousBlock = iv; //предыдущий блок

    for (size_t i = 0; i < bytes.size(); i += N){ //длина по N байт
        vector<uint8_t> block(bytes.begin() + i, bytes.begin() + i + N); //создание блока

        for (int j = 0; j < N; ++j){ //AES-CBC
            block[j] ^= previousBlock[j];
        }

        block = EncryptBlock(block, expandedKey);

        CipherText.insert(CipherText.end(), block.begin(), block.end());

        previousBlock = block;
    }

    return CipherText; //зашифрованный текст
}



//БЛОК 8. ДЕШИФРОВАНИЕ

vector<uint8_t> DecryptBlock(vector<uint8_t> CipherText, const vector<uint8_t>& expandedKey){

    vector<uint8_t> PlainText = CipherText;

    AddRoundKey(PlainText, GetRoundKey(expandedKey, r)); 

    for (int round = r - 1; round > 0; --round){

        InvShiftRows(PlainText);

        InvSubBytes(PlainText);

        AddRoundKey(PlainText, GetRoundKey(expandedKey, round));

        InvMixColumns(PlainText);
    }

    InvShiftRows(PlainText);

    InvSubBytes(PlainText);

    AddRoundKey(PlainText, GetRoundKey(expandedKey, 0));

    return PlainText;
}


//ДЕШИФРОВАНИЕ
vector<uint8_t> Decryption(const vector<uint8_t>& CipherText, const vector<uint8_t>& CipherTextherKey, const vector<uint8_t>& iv){

    vector<uint8_t> PlainText;

    if (CipherText.empty() || CipherText.size() % N != 0){ 
        return PlainText;
    }

    vector<uint8_t> expandedKey = KeyExpansion(CipherTextherKey, N, r); //расширенный ключ
    vector<uint8_t> previousBlock = iv; //предыдущий блок

    for (size_t i = 0; i < CipherText.size(); i += N){ //длина по N байт
        vector<uint8_t> block(CipherText.begin() + i, CipherText.begin() + i + N); //создание блока
        vector<uint8_t> decryptedBlock = DecryptBlock(block, expandedKey); //получение блока без iv

        for (int j = 0; j < N; ++j){ //CBC-AES
            decryptedBlock[j] ^= previousBlock[j];
        }

        PlainText.insert(PlainText.end(), decryptedBlock.begin(), decryptedBlock.end());

        previousBlock = block;
    }

    PlainText = removePadding(PlainText); //удаление pksc7

    return PlainText; //отркрытый текст
}
