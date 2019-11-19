#include <AES.h>
#include <random>

AES::AES(unsigned char key[16], Mode mode) : mode(mode) {
  CreateRoundConstant();
  CreateSBox();
  KeyExpansion(key);
}

void AES::Encrypt(std::ifstream& in, std::ofstream& out) {
  in.seekg(0, in.end);
  size_t length = in.tellg();
  size_t padding_len = 0;
  if (length % BLOCK_SIZE != 0) {
    padding_len = (length / BLOCK_SIZE + 1) * BLOCK_SIZE - length;
    length += padding_len;
  }
  in.seekg(0, in.beg);

  unsigned char* input = new unsigned char[length];
  in.read((char*)input, length);
  unsigned char* output = new unsigned char[length];
  std::ofstream IV_output;
  unsigned char IV[BLOCK_SIZE];

  switch (mode) {
    case ECB:
      for (size_t i = 0; i < length; i += BLOCK_SIZE) {
        Cipher(input + i, output + i);
      }
      out.write((char*)output, length);
      break;
    case CBC:
      IV_output.open("init_vec", std::ios::binary);
      CreateInitialVector(IV);
      IV_output.write((char*)IV, BLOCK_SIZE);
      IV_output.close();
      for (size_t i = 0; i < length; i += BLOCK_SIZE) {
        for (size_t j = 0; j < BLOCK_SIZE; j++) {
          if (i + j == length - 1) {
            input[i + j] = padding_len;
          }
          input[i + j] ^= IV[j];
        }
        Cipher(input + i, output + i);
        for (size_t j = 0; j < BLOCK_SIZE; j++) {
          IV[j] = output[i + j];
        }
      }
      out.write((char*)output, length);
      break;
    case CFB:
      IV_output.open("init_vec", std::ios::binary);
      CreateInitialVector(IV);
      IV_output.write((char*)IV, BLOCK_SIZE);
      IV_output.close();
      for (size_t i = 0; i < length; i += 4) {
        Cipher(IV, output + i);
        for (int j = 0; j < 4; j++) {
          if (i + j == length - 1) {
            input[i + j] = padding_len;
          }
          output[i + j] ^= input[i + j];
        }
        for (int j = 0; j < 12; j++) {
          IV[j] = IV[j + 4];
        }
        for (int j = 12; j < 16; j++) {
          IV[j] = output[i + 15 - j];
        }
      }
      out.write((char*)output, length);
      break;
    case OFB:
      IV_output.open("init_vec", std::ios::binary);
      CreateInitialVector(IV);
      IV_output.write((char*)IV, BLOCK_SIZE);
      IV_output.close();
      for (size_t i = 0; i < length; i += 4) {
        Cipher(IV, output + i);
        for (int j = 0; j < 12; j++) {
          IV[j] = IV[j + 4];
        }
        for (int j = 12; j < 16; j++) {
          IV[j] = output[i + 15 - j];
        }
        for (int j = 0; j < 4; j++) {
          if (i + j == length - 1) {
            input[i + j] = padding_len;
          }
          output[i + j] ^= input[i + j];
        }
      }
      out.write((char*)output, length);
      break;
    case CTR:
      break;
    default:
      break;
  }
}

void AES::Decrypt(std::ifstream& in, std::ofstream& out) {
  in.seekg(0, in.end);
  size_t length = in.tellg();
  in.seekg(0, in.beg);

  unsigned char* input = new unsigned char[length];
  in.read((char*)input, length);
  unsigned char* output = new unsigned char[length];
  std::ifstream IV_input;
  unsigned char IV[BLOCK_SIZE];

  switch (mode) {
    case ECB:
      for (size_t i = 0; i < length; i += BLOCK_SIZE) {
        InvCipher(input + i, output + i);
      }
      out.write((char*)output, length);
      break;
    case CBC:
      IV_input.open("init_vec", std::ios::binary);
      IV_input.read((char*)IV, BLOCK_SIZE);
      IV_input.close();
      for (size_t i = 0; i < length; i += BLOCK_SIZE) {
        InvCipher(input + i, output + i);
        for (size_t j = 0; j < BLOCK_SIZE; j++) {
          output[i + j] ^= IV[j];
          if (i + j == length - 1) {
            length -= output[i + j];
          }
          IV[j] = input[i + j];
        }
      }
      out.write((char*)output, length);
      break;
    case CFB:
      IV_input.open("init_vec", std::ios::binary);
      IV_input.read((char*)IV, BLOCK_SIZE);
      IV_input.close();
      for (size_t i = 0; i < length; i += 4) {
        Cipher(IV, output + i);
        for (int j = 0; j < 4; j++) {
          output[i + j] ^= input[i + j];
          if (i + j == length - 1) {
            length -= output[i + j];
          }
        }
        for (int j = 0; j < 12; j++) {
          IV[j] = IV[j + 4];
        }
        for (int j = 12; j < 16; j++) {
          IV[j] = input[i + 15 - j];
        }
      }
      out.write((char*)output, length);
      break;
    case OFB:
      IV_input.open("init_vec", std::ios::binary);
      IV_input.read((char*)IV, BLOCK_SIZE);
      IV_input.close();
      for (size_t i = 0; i < length; i += 4) {
        Cipher(IV, output + i);
        for (int j = 0; j < 12; j++) {
          IV[j] = IV[j + 4];
        }
        for (int j = 12; j < 16; j++) {
          IV[j] = output[i + 15 - j];
        }
        for (int j = 0; j < 4; j++) {
          output[i + j] ^= input[i + j];
          if (i + j == length - 1) {
            length -= output[i + j];
          }
        }
      }
      out.write((char*)output, length);
      break;
    case CTR:
      break;
    default:
      break;
  }
}

void AES::Cipher(unsigned char* in, unsigned char* out) {
  unsigned char state[4][4];
  CopyArrToState(state, in);
  AddRoundKey(state, 0);
  for (int round = 1; round < Nr; round++) {
    SubBytes(state);
    ShiftRows(state);
    MixColumns(state);
    AddRoundKey(state, round);
  }
  SubBytes(state);
  ShiftRows(state);
  AddRoundKey(state, Nr);
  CopyStateToArr(out, state);
}

void AES::InvCipher(unsigned char* in, unsigned char* out) {
  unsigned char state[4][4];
  CopyArrToState(state, in);
  AddRoundKey(state, Nr);
  for (int round = Nr - 1; round > 0; round--) {
    InvShiftRows(state);
    InvSubBytes(state);
    AddRoundKey(state, round);
    InvMixColumns(state);
  }
  InvShiftRows(state);
  InvSubBytes(state);
  AddRoundKey(state, 0);
  CopyStateToArr(out, state);
}

void AES::CreateRoundConstant() {
  Rcon[0] = 0x01;
  for (int i = 1; i < Nr; i++) {
    Rcon[i] = Rcon[i - 1] * 2;
  }
}

void AES::CreateSBox() {
  const unsigned char matrix[8] = {0x8F, 0xC7, 0xE3, 0xF1,
                                   0xF8, 0x7C, 0x3E, 0x1F};
  const unsigned char XOR = 0x63;
  for (int i = 0; i < 256; i++) {
    unsigned char t = GaloisField::inverse(i);
    unsigned char ret = 0;
    for (int j = 0; j < 8; j++) {
      unsigned char temp = 0;
      for (int k = 0; k < 8; k++) {
        if (((matrix[j] >> (7 - k)) & 0x01) & ((t >> k) & 0x01)) {  //행렬 곱셈
          temp ^= 1;  //행렬곱 연산 1이면 1 덧셈
        }
      }
      if (temp) {
        ret |= (1u << j);
      }
    }
    ret ^= XOR;
    SBox[(i >> 4)][(i & 0x0F)] = ret;
    InvSBox[(ret >> 4)][(ret & 0x0F)] = i;
  }
}

void AES::CreateInitialVector(unsigned char IV[BLOCK_SIZE]) {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<> dist(0, 255);
  for (int i = 0; i < BLOCK_SIZE; i++) {
    IV[i] = dist(rng);
  }
}

void AES::CopyStateToArr(unsigned char* dest, unsigned char src[][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      dest[i + (j << 2)] = src[i][j];
    }
  }
}
void AES::CopyArrToState(unsigned char dest[][4], unsigned char* src) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      dest[i][j] = src[i + (j << 2)];
    }
  }
}

void AES::KeyExpansion(unsigned char key[16]) {
  unsigned int temp;
  int i = 0;
  while (i < Nk) {
    RoundKey[i] = Word(key[(i << 2)], key[(i << 2) + 1], key[(i << 2) + 2],
                       key[(i << 2) + 3]);
    i++;
  }
  while (i < Nb * (Nr + 1)) {
    temp = RoundKey[i - 1];
    if ((i & 0x03) == 0) {
      temp = SubWord(RotWord(temp)) ^ Rcon[(i >> 2)].number();
    }
    RoundKey[i] = RoundKey[i - Nk] ^ temp;
    i++;
  }
}

void AES::AddRoundKey(unsigned char state[][4], int round) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      state[i][j] ^= ((RoundKey[(round << 2) + j] >> (24 - (i << 3))) & 0xFF);
    }
  }
}

void AES::InvMixColumns(unsigned char state[][4]) {
  GaloisField result[4][4] = {0};
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      for (int k = 0; k < 4; k++) {
        result[i][j] += (InvMatrix[i][k] * state[k][j]);
      }
    }
  }
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      state[i][j] = result[i][j].number();
    }
  }
}

void AES::InvShiftRows(unsigned char state[][4]) {
  for (int i = 0; i < 4; i++) {
    unsigned char temp = state[i][3];
    for (int j = 3; j > 0; j--) {
      state[i][j] = state[i][j - 1];
    }
    state[i][0] = temp;
  }
}

void AES::InvSubBytes(unsigned char state[][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      state[i][j] = InvSBox[state[i][j] >> 4][state[i][j] & 0x0F];
    }
  }
}

void AES::MixColumns(unsigned char state[][4]) {
  GaloisField result[4][4] = {0};
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      for (int k = 0; k < 4; k++) {
        result[i][j] += (Matrix[i][k] * state[k][j]);
      }
    }
  }
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      state[i][j] = result[i][j].number();
    }
  }
}

unsigned int AES::RotWord(unsigned int word) {
  return ((word << 8) | (word >> 24));
}

void AES::ShiftRows(unsigned char state[][4]) {
  for (int i = 0; i < 4; i++) {
    unsigned char temp = state[i][0];
    for (int j = 0; j < 3; j++) {
      state[i][j] = state[i][j + 1];
    }
    state[i][3] = temp;
  }
}

void AES::SubBytes(unsigned char state[][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      state[i][j] = SBox[state[i][j] >> 4][state[i][j] & 0x0F];
    }
  }
}

unsigned int AES::SubWord(unsigned int word) {
  unsigned char temp[4];
  for (int i = 0; i < 4; i++) {
    temp[i] = ((word >> (24 - (i << 3))) & 0xFF);
    temp[i] = SBox[temp[i] >> 4][temp[i] & 0x0F];
  }
  return Word(temp[0], temp[1], temp[2], temp[3]);
}

unsigned int AES::Word(unsigned int k1, unsigned int k2, unsigned int k3,
                       unsigned int k4) {
  return (k1 << 24) | (k2 << 16) | (k3 << 8) | k4;
}