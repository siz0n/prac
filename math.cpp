#include <gmp.h>
#include <gmpxx.h>
#include <tuple>
#include <iostream>
#include <vector>

using namespace std;

// --- 1. Евклид (расширенный) ---
tuple<mpz_class, mpz_class, mpz_class> Euclid(const mpz_class& c, const mpz_class& m) {
    mpz_class x0 = c, x1 = 1, x2 = 0;
    mpz_class y0 = m, y1 = 0, y2 = 1;

    while (y0 != 0) {
        mpz_class q = x0 / y0;
        mpz_class t0 = x0 % y0;
        mpz_class t1 = x1 - q * y1;
        mpz_class t2 = x2 - q * y2;

        x0 = y0; x1 = y1; x2 = y2;
        y0 = t0; y1 = t1; y2 = t2;
    }

    return {x0, x1, x2};
}

// --- 2. Проверка на простоту с помощью 
bool is_prime(const mpz_class& n) {
    if (n < 2) return false;
    int reps = 20;
    
    return mpz_probab_prime_p(n.get_mpz_t(), reps) > 0; // miler-rabin
}

mpz_class input_prime(const string& outas) {
    mpz_class p;
    cout << outas;
    while (true) {
        cin >> p;
        if (!is_prime(p)) {
            cerr << "Ошибка: p должно быть простым. Попробуйте снова: ";
            continue;
        }
        return p;
    }
}

// --- 3. НОД ---
mpz_class gcd(const mpz_class& a, const mpz_class& b) {
    mpz_class g;
    mpz_gcd(g.get_mpz_t(), a.get_mpz_t(), b.get_mpz_t()); // указатель на переменную
    return g;
}

// --- 4. Возведение в степень по модулю ---
mpz_class fast_pow(const mpz_class& base, const mpz_class& exp, const mpz_class& mod) {
    mpz_class result;
    mpz_powm(result.get_mpz_t(), base.get_mpz_t(), exp.get_mpz_t(), mod.get_mpz_t());
    return result;
}

// --- 5. Метод Ферма (то же что и fast_pow, но с приведением степени) ---
mpz_class fermat_method(const mpz_class& a, const mpz_class& x, const mpz_class& p) {
    mpz_class new_x = x % (p - 1);
    return fast_pow(a, new_x, p);
}

// --- 6. Бинарное возведение в степень ---
mpz_class binary_exponentiation(const mpz_class& a, const mpz_class& x, const mpz_class& p) {
    return fast_pow(a, x, p); // mpz_powm already uses binary method
}

// --- Дробь в цепную 
vector<mpz_class> continuedFraction(const mpz_class& p, const mpz_class& q) {
    vector<mpz_class> cf;
    mpz_class a = p, b = q;
    while (b != 0) {
        cf.push_back(a / b);
        mpz_class r = a % b;
        a = b;
        b = r;
    }
    return cf;
}

// --- Диофантово уравнение: ax + by = c с помощью цепных дробей ---
tuple<mpz_class, mpz_class, mpz_class> solveDiophantine(const mpz_class& a, const mpz_class& b, const mpz_class& c) {
    vector<mpz_class> cf = continuedFraction(a, b);
    mpz_class p0 = 1, p1 = 0;
    mpz_class q0 = 0, q1 = 1;
    for (size_t i = 0; i < cf.size(); i++) {
        mpz_class a_i = cf[i];
        mpz_class p = a_i * p1 + p0;
        mpz_class q = a_i * q1 + q0;
        p0 = p1;
        q0 = q1;
        p1 = p;
        q1 = q;
    }
    mpz_class my_gcd = p1;
    if (c % my_gcd != 0)
        throw runtime_error("Error");
    mpz_class factor = c / my_gcd;
    mpz_class x0 = (a > b) ? p0 * factor : q0 * factor;
    mpz_class y0 = (a > b) ? -q0 * factor : -p0 * factor;
    return make_tuple(my_gcd, x0, y0);
}


mpz_class mod_mul(const mpz_class& a, const mpz_class& b, const mpz_class& mod) {
    mpz_class result;
    mpz_mul(result.get_mpz_t(), a.get_mpz_t(), b.get_mpz_t()); // *
    mpz_mod(result.get_mpz_t(), result.get_mpz_t(), mod.get_mpz_t()); // | |
    return result;
}

mpz_class input_xkg(const string& prompt, const mpz_class& max_value) {
    mpz_class value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (value < max_value) {
            return value;
        }
        cerr << "Ошибка: значение должно быть меньше " << max_value.get_str() << "!\n";
    }
}
