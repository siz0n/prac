#include <iostream>
#include <vector>
#include <string>
#include <gmpxx.h>
#include <unistd.h>
#include "math.h"
#include "crypto.h"

//mpz_class p("6864797660130609714981900799081393217269435300143305409394463459185543183397656052122559640661454554977296311391480858037121987999716643812574028291115057151", 10);
//mpz_init(p.get_mpz_t()); // выделяем память
//mpz_set_ui(p.get_mpz_t(), 2); // только целые числа
//mpz_pow_ui(p.get_mpz_t(), p.get_mpz_t(), 2048); // rop = base^exp
//mpz_sub_ui(p.get_mpz_t(), p.get_mpz_t(), 1); // - ui безнаковые
//mpz_prevprime(p.get_mpz_t(), p.get_mpz_t()); поиск

using namespace std;

enum class Option {
    FERMA = 1,
    EXTENDED_EUCLID,
    INVERSE_EUCLID,
    ELGAMAL,
    CONTINUED_FRACTION,
    ATTACK_SIMULATION,
    EXIT = 0
};

int main() {
    int choice;
    do {
        cout << "\n1. a^x mod p (Ферма)\n"
             << "2. Расширенный алгоритм Евклида (u, v)\n"
             << "3. Нахождение обратного по модулю (c^-1 mod m)\n"
             << "4. Эль-Гамаль шифрование\n"
             << "5. Решение через цепную дробь\n"
             << "6. Эмуляция атаки на Эль-Гамаль\n"
             << "0. Выход\n"
             << "Выберите опцию: ";
        cin >> choice;

        try {
            switch(static_cast<Option>(choice)) {
                case Option::FERMA: {
                    mpz_class a, x, p;
                    
                    cout << "Введите с: ";
                    cin >> a;
                    cout << "Введите m: ";
                    cin >> x;
                    p = input_prime("Введите p (простое): ");

                    if (gcd(a, p) != 1) {
                        cerr << "a и p должны быть взаимно просты.\n";
                        break;
                    }

                    mpz_class fermat_res = fermat_method(a, x, p);
                    mpz_class binary_res = binary_exponentiation(a, x, p);

                    cout << "Результат (Ферма): " << fermat_res.get_str() << endl;
                    cout << "Результат (Бинарный): " << binary_res.get_str() << endl;

                    if (fermat_res != binary_res) {
                        cerr << "Результаты не совпадают!\n";
                    } else {
                        cout << "Результаты совпадают!\n";
                    }
                    break;
                }
                case Option::EXTENDED_EUCLID: {
                    mpz_class c, m;
                    cout << "Введите c: "; cin >> c;
                    cout << "Введите m: "; cin >> m;
                    auto [d, u, v] = Euclid(c, m);
                    cout << "НОД: " << d.get_str() << ", u = " << u.get_str() << ", v = " << v.get_str() << endl;
                    
                    
                    break;
                }
                case Option::INVERSE_EUCLID: {
                    mpz_class c, m;
                    cout << "Введите c: "; cin >> c;
                    cout << "Введите m: "; cin >> m;
                    auto [d, u, v] = Euclid(c, m);
                    if (d != 1) {
                        cerr << "Обратного не существует (НОД != 1)\n";
                    } else {
                        mpz_class inv = u % m;
                        if (inv < 0) inv += m;
                        cout << "Обратный элемент: " << inv.get_str() << endl;
                    }
                    break;
                }
                case Option::ELGAMAL: {
                    
                    const int BITS = 128;  
                    gmp_randclass rng(gmp_randinit_default);
                    rng.seed(time(nullptr));
                    mpz_class rand_num = rng.get_z_bits(BITS);
                    mpz_class p;
                    mpz_nextprime(p.get_mpz_t(), rand_num.get_mpz_t());
                    cout << "Ваше случайное значние чилса p: " << p << endl;


                    mpz_class g = input_xkg("Введите g (<p): ", p);
                    mpz_class x = input_xkg("Введите x (<p): ", p);
                    mpz_class k = input_xkg("Введите k (<p): ", p);

                    mpz_class y = fast_pow(g, x, p);
                    cout << "Открытый ключ (y): " << y.get_str() << endl;

                    string message = getMessageFromUser();
                    auto encrypted = encrypt(message, p, k, g, y);

                    outputEncryptedMessage(encrypted);

                    string decrypted = decrypt(encrypted, p, x);
                    cout << "Расшифрованное сообщение: " << decrypted << endl;
                    break;
                }
                case Option::CONTINUED_FRACTION: {
                    mpz_class num, den;
                    cout << "Введите числитель: "; cin >> num;
                    cout << "Введите знаменатель: "; cin >> den;
                    auto cf = continuedFraction(num, den);
                    cout << "Цепная дробь: [";
                    for (size_t i = 0; i < cf.size(); ++i) {
                        cout << cf[i].get_str();
                        if (i + 1 < cf.size()) cout << ", ";
                    }
                    cout << "]\n";
                    try {
                        mpz_class a, b, c;
                        a= 1256;
                        b = 847;
                        c = 119;
                        auto [gcd, x, y] = solveDiophantine(a, b, c);
                        cout << "НОД(a, b) = " << gcd.get_str() << endl;
                        cout << "Решение: x = " << x.get_str() << ", y = " << y.get_str() << endl;
                    } catch (const std::exception& e) {
                        cout << "Ошибка: " << e.what() << endl;
                        }
                        
                    break;
                }
                case Option::ATTACK_SIMULATION: {
                    gmp_randclass rng(gmp_randinit_default);
                    rng.seed(time(nullptr));
                    mpz_class rand_num = rng.get_z_bits(BITS);
                    mpz_class p;
                    mpz_nextprime(p.get_mpz_t(), rand_num.get_mpz_t());
                    cout << "Ваше случайное значние чилса p: " << p << endl;
                    mpz_class g = input_xkg("Введите g (<p): ", p);
                    mpz_class x = input_xkg("Введите x (<p): ", p);
                    mpz_class k = input_xkg("Введите k (<p): ", p);

                    mpz_class y = fast_pow(g, x, p);
                    cout << "Открытый ключ (y): " << y.get_str() << endl;

                    string message = getMessageFromUser();
                    auto encrypted = encrypt(message, p, k, g, y);

                    outputEncryptedMessage(encrypted);

                    string attacked = attack(encrypted, p, g, y);
                    cout << "Результат атаки: " << attacked << endl;
                    break;
                }
                case Option::EXIT:
                    cout << "Выход...\n";
                    break;
                default:
                    cout << "Неверный выбор, попробуйте снова.\n";
            }
        } catch (const exception& e) {
            cout << "Ошибка выполнения: " << e.what() << endl;
        } catch (...) {
            cout << "Неизвестная ошибка выполнения\n";
        }
        sleep(2);
    } while (choice != 0);

    return 0;
}
