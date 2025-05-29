#include <gmpxx.h>
#include <vector>
#include <string>
using namespace std;

vector<pair<mpz_class, mpz_class>> encrypt(const string& message, const mpz_class& p, const mpz_class& k, const mpz_class& g, const mpz_class& y);
string decrypt(const vector<pair<mpz_class, mpz_class>>& encrypted, const mpz_class& p, const mpz_class& x);
mpz_class mod_inverse(const mpz_class& a, const mpz_class& m);
mpz_class find_k(const mpz_class& g, const mpz_class& a, const mpz_class& p);
string attack(const vector<pair<mpz_class, mpz_class>>& encrypted, const mpz_class& p, const mpz_class& g, const mpz_class& y);
string readMessageFromFile(const string& filename);
void writeEncryptedToFile(const string& filename, const vector<pair<mpz_class, mpz_class>>& encrypted);
string getMessageFromUser();
void outputEncryptedMessage(const vector<pair<mpz_class, mpz_class>>& encrypted);
