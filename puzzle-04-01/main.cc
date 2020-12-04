#include <algorithm>
#include <cassert>
#include <iostream>
#include <set>
#include <string>

#include <openssl/md5.h>

using MD5Digest = unsigned char[MD5_DIGEST_LENGTH];

void md5(MD5Digest digest, std::string const &s) {
  MD5_CTX ctxt;
  MD5_Init(&ctxt);
  MD5_Update(&ctxt, s.data(), s.length());
  MD5_Final(digest, &ctxt);
}

bool is_valid(std::string const &s) {
  MD5Digest digest;
  md5(digest, s);
  return digest[0] == 0 && digest[1] == 0 && (digest[2] & 0xf0) == 0;
}

int main(int argc, char **argv) {
  for (std::string line; std::getline(std::cin, line);) {
    unsigned i = 0;
    MD5Digest digest;
    while (!is_valid(line + std::to_string(i))) {
      ++i;
    }
    std::cout << "Value = " << i << "\n";
  }

  return 0;
}