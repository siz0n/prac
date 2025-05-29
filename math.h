#include <gmpxx.h>
#include <tuple>
#include <vector>
using namespace std;

tuple<mpz_class, mpz_class, mpz_class> Euclid(const mpz_class& c, const mpz_class& m);
bool is_prime(const mpz_class& n);
mpz_class input_prime(const string& prompt);
mpz_class gcd(const mpz_class& a, const mpz_class& b);
mpz_class fast_pow(const mpz_class& base, const mpz_class& exp, const mpz_class& mod);
mpz_class fermat_method(const mpz_class& a, const mpz_class& x, const mpz_class& p);
mpz_class binary_exponentiation(const mpz_class& a, const mpz_class& x, const mpz_class& p);
vector<mpz_class> continuedFraction(const mpz_class& a, const mpz_class& b);
tuple<mpz_class, mpz_class, mpz_class> solveDiophantine(const mpz_class& a, const mpz_class& b, const mpz_class& c);
mpz_class mod_mul(const mpz_class& a, const mpz_class& b, const mpz_class& mod);
mpz_class input_xkg(const string& prompt, const mpz_class& max_value);