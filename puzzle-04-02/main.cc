#include <algorithm>
#include <cassert>
#include <iostream>
#include <set>
#include <string>

#include <openssl/evp.h>

using MD5Digest = unsigned char[EVP_MAX_MD_SIZE];

unsigned int md5(MD5Digest digest, std::string const &s) {
    EVP_MD const* md{EVP_md5()};
    unsigned int md_len;

    EVP_MD_CTX* md_ctxt{EVP_MD_CTX_new()};
    assert(md_ctxt != NULL);
    EVP_DigestInit_ex2(md_ctxt, md, NULL);
    EVP_DigestUpdate(md_ctxt, s.data(), s.length());
    EVP_DigestFinal_ex(md_ctxt, digest, &md_len);
    return md_len;
}

bool is_valid(std::string const &s) {
  MD5Digest digest;
  auto len = md5(digest, s);
  assert(len >= 3);
  return digest[0] == 0 && digest[1] == 0 && digest[2] == 0;
}

int main(int argc, char **argv) {
  for (std::string line; std::getline(std::cin, line);) {
    unsigned i = 0;
    while (!is_valid(line + std::to_string(i))) {
      ++i;
    }
    std::cout << "Value = " << i << "\n";
  }

  return 0;
}