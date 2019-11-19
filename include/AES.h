#ifndef AES_H
#define AES_H
#include <GaloisField.h>

class AES {
 public:
  enum Mode { ECB, CBC, CFB, OFB, CTR };
  AES(unsigned char* key);
  void Encrypt(unsigned char* in, unsigned char* out);
  void Decrypt(unsigned char* in, unsigned char* out);

 private:
  const GaloisField Matrix[4][4] = {
		0x02, 0x03, 0x01, 0x01, 
		0x01, 0x02, 0x03, 0x01,
		0x01, 0x01, 0x02, 0x03,
		0x03, 0x01, 0x01, 0x02
	};  // mixcolumn matrix
  const GaloisField InvMatrix[4][4] = {
		0x0e, 0x0b, 0x0d, 0x09,
		0x09, 0x0e, 0x0b, 0x0d, 
		0x0d, 0x09, 0x0e, 0x0b,
    0x0b, 0x0d, 0x09, 0x0e
	};	// invmixcolumn matrix
  GaloisField Rcon[11];
  unsigned char SBox[16][16];
  unsigned char InvSBox[16][16];
  const int Nb = 4;
  const int Nk = 4;
  const int Nr = 10;
  unsigned int w[44];
  void Cipher(unsigned char* in, unsigned char* out);
  void InvCipher(unsigned char* in, unsigned char* out);
  void CreateRoundConstant();
  void CreateSBox();
  void CopyStateToArr(unsigned char* dest, unsigned char src[][4]);
  void CopyArrToState(unsigned char dest[][4], unsigned char* src);
  void KeyExpansion(unsigned char* key);
  void AddRoundKey(unsigned char state[][4], int round);
  void InvMixColumns(unsigned char state[][4]);
  void InvShiftRows(unsigned char state[][4]);
  void InvSubBytes(unsigned char state[][4]);
  void MixColumns(unsigned char state[][4]);
  unsigned int RotWord(unsigned int word);
  void ShiftRows(unsigned char state[][4]);
  void SubBytes(unsigned char state[][4]);
  unsigned int SubWord(unsigned int word);
  unsigned int Word(unsigned int k1, unsigned int k2, unsigned int k3,
                    unsigned int k4);
};

#endif