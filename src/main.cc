#include <AES.h>
#include <cstring>
#include <fstream>
#include <iostream>

#ifndef WIN32
using strcmpi = strcasecmp;
#endif

enum { ERROR = -1, ENCRYPT, DECRYPT };
const char* input_filename;
const char* output_filename;
const char* key_filename;
AES::Mode cipher_mode = AES::ERROR;

int parse_options(int argc, const char* argv[]);
AES::Mode parse_mode(const char* mode);
void print_usage();
void read_key(unsigned char* key);

int main(int argc, const char* argv[]) {
  int mode = parse_options(argc, argv);
  if (mode == ERROR || input_filename == nullptr ||
      output_filename == nullptr || key_filename == nullptr ||
      cipher_mode == AES::ERROR) {
    print_usage();
    return -1;
  }
  unsigned char key[BLOCK_SIZE];
  read_key(key);
  std::ifstream input(input_filename, std::ios::binary);
  std::ofstream output(output_filename, std::ios::binary);
  AES aes(key, cipher_mode);
  switch (mode) {
    case ENCRYPT:
      aes.Encrypt(input, output);
      break;
    case DECRYPT:
      aes.Decrypt(input, output);
      break;
  }

  input.close();
  output.close();
  return 0;
}

void read_key(unsigned char* key) {
  std::ifstream key_input(key_filename, std::ios::binary);
  key_input.read((char*)key, BLOCK_SIZE);
  key_input.close();
}

void print_usage() {
  std::cerr << "Usage: aes [OPTION]...\n"
               "   -e Mode                     Encrypt Mode(ECB, CBC, CFB, "
               "OFB, CTR)\n"
               "   -d Mode                     Decrypt Mode(ECB, CBC, CFB, "
               "OFB, CTR)\n"
               "   -i filename                 Input File\n"
               "   -o filename                 Output File\n"
               "   -k filename                 Key File";
}

int parse_options(int argc, const char* argv[]) {
  int i = 1;
  int result = ERROR;
  while (i < argc) {
    if (argv[i][0] != '-') {
      return ERROR;
    }
    size_t len = strlen(argv[i]);
    if (len != 2) {
      return ERROR;
    }
    switch (argv[i++][1]) {
      case 'e':
        result = ENCRYPT;
        cipher_mode = parse_mode(argv[i++]);
        break;
      case 'd':
        result = DECRYPT;
        cipher_mode = parse_mode(argv[i++]);
        break;
      case 'i':
        input_filename = argv[i++];
        break;
      case 'o':
        output_filename = argv[i++];
        break;
      case 'k':
        key_filename = argv[i++];
        break;
      default:
        return ERROR;
    }
  }
  return result;
}

AES::Mode parse_mode(const char* mode) {
  if (strcmpi(mode, "ecb") == 0) {
    return AES::ECB;
  } else if (strcmpi(mode, "cbc") == 0) {
    return AES::CBC;
  } else if (strcmpi(mode, "cfb") == 0) {
    return AES::CFB;
  } else if (strcmpi(mode, "ofb") == 0) {
    return AES::OFB;
  } else if (strcmpi(mode, "ctr") == 0) {
    return AES::CTR;
  }
  return AES::ERROR;
}