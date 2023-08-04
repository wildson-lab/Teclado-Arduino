#include <Arduino.h>
#include <Keypad.h>

// #define CODIGO_TECLA_11     0x1E    // Numero 1
// #define CODIGO_TECLA_12     0x1F    // Numero 2
// #define CODIGO_TECLA_13     0x20    // Numero 3
// #define CODIGO_TECLA_14     0x04    // Letra A
// #define CODIGO_TECLA_21     0x21    // Numero 4
// #define CODIGO_TECLA_22     0x22    // Numero 5
// #define CODIGO_TECLA_23     0x23    // Numero 6
// #define CODIGO_TECLA_24     0x05    // Letra B
// #define CODIGO_TECLA_31     0x24    // Numero 7
// #define CODIGO_TECLA_32     0x25    // Numero 8
// #define CODIGO_TECLA_33     0x26    // Numero 9
// #define CODIGO_TECLA_34     0x06    // Letra C
// #define CODIGO_TECLA_41     0x21    // Caracter *
// #define CODIGO_TECLA_42     0x27    // Numero 0
// #define CODIGO_TECLA_43     0x01
// #define CODIGO_TECLA_44     0x07    // Letra D

#define CODIGO_TECLA_11     '1'
#define CODIGO_TECLA_12     '2'
#define CODIGO_TECLA_13     '3'
#define CODIGO_TECLA_14     'A'   
#define CODIGO_TECLA_21     '4'
#define CODIGO_TECLA_22     '5'
#define CODIGO_TECLA_23     '6'
#define CODIGO_TECLA_24     'B'
#define CODIGO_TECLA_31     '7'
#define CODIGO_TECLA_32     '8'
#define CODIGO_TECLA_33     '9'
#define CODIGO_TECLA_34     'C'
#define CODIGO_TECLA_41     '*'
#define CODIGO_TECLA_42     '0'
#define CODIGO_TECLA_43     '#'
#define CODIGO_TECLA_44     'D'

#define MODIFICADOR_SHIFT    2

// Constantes
const byte LINHAS = 4;                              // Número de linhas do teclado
const byte COLUNAS = 4;                             // Número de colunas do teclado
const byte PINOS_LINHAS[LINHAS] = {9, 8, 7, 6};     // Pinos de conexao com as linhas do teclado
const byte PINOS_COLUNAS[COLUNAS] = {5, 4, 3, 2};   // Pinos de conexao com as colunas do teclado

const char TECLAS_MATRIZ[LINHAS][COLUNAS] = {       // Matriz de caracteres (mapeamento do teclado)
  {CODIGO_TECLA_11, CODIGO_TECLA_12, CODIGO_TECLA_13, CODIGO_TECLA_14},
  {CODIGO_TECLA_21, CODIGO_TECLA_22, CODIGO_TECLA_23, CODIGO_TECLA_24},
  {CODIGO_TECLA_31, CODIGO_TECLA_32, CODIGO_TECLA_33, CODIGO_TECLA_34},
  {CODIGO_TECLA_41, CODIGO_TECLA_42, CODIGO_TECLA_43, CODIGO_TECLA_44}
};

// Protótipos de funções
void EnviaComando(uint8_t comando, uint8_t modificador = 0x00);

// Instâncias de classes
Keypad teclado = Keypad(makeKeymap(TECLAS_MATRIZ), PINOS_LINHAS, PINOS_COLUNAS, LINHAS, COLUNAS);

void setup() {
  Serial.begin(9600);
}

void loop() {
  char tecla = teclado.getKey();    // Realiza a leitura do teclado e atribui à variável

  if (tecla) {
    uint8_t codigo = 0;
    uint8_t modificador = 0;

    if (tecla >= '1' && tecla <= '9'){
      codigo = 30 + (int(tecla) - 49);
    }
    else if (tecla == '0') {
      codigo = 39;
    }
    else if (tecla >= 'A' && tecla <= 'D'){
      codigo = 4 + (int(tecla) - 65);
    }
    else if (tecla == '*'){
      codigo = 37;
      modificador = MODIFICADOR_SHIFT;
    }
    else if (tecla == '#'){
      codigo = 32;
      modificador = MODIFICADOR_SHIFT;
    }

    EnviaComando(codigo, modificador);
  }
}

void EnviaComando(uint8_t comando, uint8_t modificador){
  uint8_t buf[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  // Cria um novo buffer vazio
  buf[0] = modificador;             // Adiciona o comando na posição 2 do buffer, padrão dos dispositivos HID
  buf[2] = comando;             // Adiciona o comando na posição 2 do buffer, padrão dos dispositivos HID
  Serial.write(buf, 8);         // Envia o buffer pela Serial
  delay(50);                    // Simula o tempo em que a tecla ficaria pressionada
  buf[0] = 0x00;                // Limpa o modificador do buffer, 
  buf[2] = 0x00;                // Limpa o comando do buffer, 
  Serial.write(buf, 8);         // Envia o buffer vazio, para sinalizar que a tecla foi solta
}


