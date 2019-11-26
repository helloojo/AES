# AES
AES 암호화, 복호화 툴

## TODO
- [x] 시작 인수 처리
- [x] Galois Field 구현
- [x] AES 기본 연산 구현
- [x] AES Mode of Operation 구현
  - [x] ECB
  - [x] CBC
  - [x] CFB
  - [x] OFB
  - [x] CTR
- [x] Padding 처리

## 사용법
    aes [OPTION...]
      -e [MODE] ECB, CBC, CFB, OFB, CTR 5개 모드 지원, AES 암호화 옵션
      -d [MODE] ECB, CBC, CFB, OFB, CTR 5개 모드 지원, AES 복호화 옵션
      -i [INPUT_FILE] 암/복호화 입력 파일
      -o [OUTPUT_FILE] 암/복호화 결과 출력 파일
      -k [KEY_FILE] AES KEY 파일(128bit)

## 기능
  ### 1. 5개의 Mode of Operation 지원  
  - #### ECB(Electronic Codebook)
    평문을 블록 크기만큼 차례대로 잘라서 암호화하는 방법  
    평문의 패턴이 노출되어 재생 공격(Replay Attack)에 취약
  - #### CBC(Cipher-Block Chaining)
    암호화 결과를 다음 블록과 XOR을 통해 암호화  
    평문의 패턴이 노출되지 않음  
    전송오류 발생 시 다음 블록으로 오류 전파  
  - #### CFB(Cipher Feedback)
    평문과 IV 암호화 결과를 XOR한 암호문의 S bit를 IV의 마지막 S bit에 전파해 암호화  
    복호화 알고리즘을 사용하지 않음  
    Initial Vector의 전송 오류는 IV크기/(S bit) 만큼의 블록의 오류 발생
  - #### OFB(Output Feedback)
    IV 암호화 결과의 S bit를 IV의 마지막 S bit에 전파해 암호화  
    복호화 알고리즘을 사용하지 않음  
    Initial Vector의 전송 오류는 모든 블록에 오류 발생 시킴
  - #### CTR(Counter)
    IV와 Counter를 이용해 매 블록마다 Counter의 값을 1씩 증가시킴  
    IV와 Counter를 암호화한 결과를 평문과 XOR해 암호화  
    복호화 알고리즘을 사용하지 않음  
    암/복호화 병렬처리 가능  
    부분 암/복호화 가능