#include <vector>
#include <iostream>
#include <map>
#include <gmp.h>
#include <gmpxx.h>
#include <fstream>
#include <cmath>
#include "math.h"

using namespace std;

// Зашифрование (ElGamal)
vector<pair<mpz_class, mpz_class>> encrypt(const string& message, const mpz_class& p, const mpz_class& k, const mpz_class& g, const mpz_class& y) {
    vector<pair<mpz_class, mpz_class>> encrypted;
    for (char c : message) {
        mpz_class m = static_cast<unsigned char>(c);
        mpz_class a = fast_pow(g, k, p);
        mpz_class b = (fast_pow(y, k, p) * m) % p;
        encrypted.emplace_back(a, b);
    }
    return encrypted;
}

// Дешифрование (ElGamal)
string decrypt(const vector<pair<mpz_class, mpz_class>>& encrypted, const mpz_class& p, const mpz_class& x) {
    string decrypted;
    for (const auto& pair : encrypted) {
        mpz_class a = pair.first;
        mpz_class b = pair.second;
        mpz_class exp = p - 1 - x;
        mpz_class a_pow = fast_pow(a, exp, p);
        mpz_class m = (b * a_pow) % p;
        decrypted += static_cast<char>(m.get_ui()); // преобразование в unsigned long int
    }
    return decrypted;
}

// Обратный по модулю 
mpz_class mod_inverse(const mpz_class& a, const mpz_class& m) {
    mpz_class inv;
    if (mpz_invert(inv.get_mpz_t(), a.get_mpz_t(), m.get_mpz_t()) == 0) //  x^-1modm = 1
        throw runtime_error("Обратный элемент не существует");
    return inv;
}

mpz_class find_k(const mpz_class& g, const mpz_class& a, const mpz_class& p) {
    mpz_class n = sqrt(p.get_d()) + 1;

    // g^(1 * i) % p
    map<mpz_class, mpz_class> values;
    mpz_class current = 1;
    for (mpz_class i = 0; i < n; ++i) {
        values[current] = i;
        current = (current * g) % p;
    }

    // g^(-n) % p
    mpz_class factor = fast_pow(g, p - 1 - n, p);
    current = a;

    for (mpz_class j = 0; j < n; ++j) {
        if (values.count(current)) {
            return j * n + values[current];
        }
        current = (current * factor) % p;
    }
    return -1;
}

// Атака
string attack(const vector<pair<mpz_class, mpz_class>>& encrypted, const mpz_class& p, const mpz_class& g, const mpz_class& y) {
    if (encrypted.empty()) return "";
    mpz_class a = encrypted[0].first;
    for (const auto& pair : encrypted) {
        if (pair.first != a) {
            cout << "Атака невозможна: разные a.\n";
            return "";
        }
    }
    mpz_class k = find_k(g, a, p);
    if (k == -1) {
        cout << "k не найден.\n";
        return "";
    }
    mpz_class s = fast_pow(y, k, p);
    mpz_class s_inv = mod_inverse(s, p);
    string result;
    for (const auto& pair : encrypted) {
        mpz_class m = (pair.second * s_inv) % p;
        result += static_cast<char>(m.get_ui());
    }
    return result;
}

string readMessageFromFile(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        throw runtime_error("Не удалось открыть файл " + filename);
    }
    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    return content;
}

// Запись зашифрованных данных в файл
void writeEncryptedToFile(const string& filename, const vector<pair<mpz_class, mpz_class>>& encrypted) {
    ofstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Не удалось создать файл " + filename);
    }
    for (const auto& pair : encrypted) {
        file << pair.first.get_str() << " " << pair.second.get_str() << " ";
    }
    if (!file.good()) {
        throw runtime_error("Ошибка записи в файл " + filename);
    }
    file.close();
}

string getMessageFromUser() {
    int sourceChoice;
    string message;
    cout << "\nВыберите источник сообщения:\n";
    cout << "1 - Ввод с клавиатуры\n";
    cout << "2 - Чтение из файла\n";
    cout << "Ваш выбор: ";
    if (!(cin >> sourceChoice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        throw runtime_error("Ошибка ввода выбора источника");
    }
    cin.ignore();
    if (sourceChoice == 1) {
        cout << "Введите сообщение для шифрования: ";
        getline(cin, message);
    } else if (sourceChoice == 2) {
        string filename;
        cout << "Введите имя файла с сообщением: ";
        getline(cin, filename);
        message = readMessageFromFile(filename);
    } else {
        throw runtime_error("Неверный выбор источника сообщения");
    }
    return message;
}

void outputEncryptedMessage(const vector<pair<mpz_class, mpz_class>>& encrypted) {
    int outputChoice;
    cout << "\nВыберите способ вывода зашифрованного сообщения:\n";
    cout << "1 - Вывод на экран\n";
    cout << "2 - Сохранение в файл\n";
    cout << "3 - И то, и другое\n";
    cout << "Ваш выбор: ";
    if (!(cin >> outputChoice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        throw runtime_error("Ошибка ввода выбора вывода");
    }
    cin.ignore();

    if (outputChoice == 1 || outputChoice == 3) {
        cout << "\nЗашифрованное сообщение:\n";
        for (const auto& pair : encrypted) {
            cout << "(" << pair.first.get_str() << ", " << pair.second.get_str() << ") ";
        }
        cout << endl;
    }
    if (outputChoice == 2 || outputChoice == 3) {
        string filename;
        cout << "Введите имя файла для сохранения: ";
        getline(cin, filename);
        writeEncryptedToFile(filename, encrypted);
        cout << "Зашифрованные данные сохранены в файл " << filename << endl;
    }
}

