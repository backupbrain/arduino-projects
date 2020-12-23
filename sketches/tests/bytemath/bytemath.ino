int delayTime = 1000;

byte byte1 = 5;
byte byte2 = 4;
unsigned int int1 = 5;
unsigned int int2 = 4;


const int MULTIPLY = 1;
const int DIVIDE = 2;
const int ADD = 3;
const int SUBTRACT = 4;

int counter = 1;

void setup() {
  
  Serial.begin(9600);
  delay(delayTime);
  
  unsigned int multiplyResult = int1*int2;
  unsigned int divideResult = int1/int2;
  unsigned int divideResultInverse = int2/int1;
  unsigned int addResult = int1+int2;
  unsigned int subtractResult = int1-int2;
  unsigned int subtractResultInverse = int2-int1;
  
  
  
  assertByteByte("multiply two bytes", multiplyResult, byte1, byte2, MULTIPLY, counter++);
  assertByteByte("divide byte1 by byte2", divideResult, byte1, byte2, DIVIDE, counter++);
  assertByteByte("divide byte2 by byte1", divideResultInverse, byte2, byte1, DIVIDE, counter++);
  assertByteByte("add two bytes", addResult, byte1, byte2, ADD, counter++);
  assertByteByte("subtract byte2 from byte1", subtractResult, byte1, byte2, SUBTRACT, counter++);
  assertByteByte("subtract byte1 from byte2", subtractResultInverse, byte2, byte1, SUBTRACT, counter++);
  
  
  assertIntByte("multiply a byte and an int", multiplyResult, int1, byte2, MULTIPLY, counter++);
  assertIntByte("divide int1 by byte2", divideResult, int1, byte2, DIVIDE, counter++);
  assertByteInt("divide byte2 by int1", divideResultInverse, byte2, int1, DIVIDE, counter++);
  assertIntByte("add a byte and an int", addResult, int1, byte2, ADD, counter++);
  assertIntByte("subtract byte2 from int1", subtractResult, int1, byte2, SUBTRACT, counter++);
  assertByteInt("subtract int1 from byte2", subtractResultInverse, byte2, int1, SUBTRACT, counter++);
  
  
}

void loop() {
}


void assert(char* description, int expectedResult, int int1, int int2, const int op, int counter) {
  char char_op;
  int actualResult;
  Serial.println("============================");
  Serial.print("test #");
  Serial.println(counter);
  Serial.println("----------------------------");
  Serial.print(description);
  Serial.print(", expecting: ");
  Serial.println(expectedResult);
  Serial.print(int1);
  switch (op) {
    case MULTIPLY:
      char_op = 'x';
      actualResult = int1 * int2;
      break;
    case DIVIDE:
      char_op = '/';
      actualResult = (float)int1 / (float)int2;
      break;
    case ADD:
      char_op = '+';
      actualResult = int1 + int2;
      break;
    case SUBTRACT:
      char_op = '-';
      actualResult = int1 - int2;
      break;
  }
  Serial.print(" ");
  Serial.print(char_op);
  Serial.print(" ");
  Serial.print(int2);
  Serial.print(" = ");
  Serial.println(actualResult);
  
  if (actualResult == expectedResult) {
    Serial.print(actualResult);
    Serial.print(" = ");
    Serial.println(expectedResult);
    Serial.println("== SUCCESS ==");
    Serial.println("--------- SUCCESS ----------");
  } else {
    Serial.print(actualResult);
    Serial.print(" != ");
    Serial.println(expectedResult);
    Serial.println("!!!!!!!!!!! FAIL !!!!!!!!!!!");
  }
  
}

void assertByteInt(char* description, int expectedResult, byte number1, int number2, const int op, int counter) {
  assert(description, expectedResult, (int)number1, number2, op, counter);
}

void assertIntByte(char* description, int expectedResult, int number1, byte number2, const int op, int counter) {
  assert(description, expectedResult, number1, number2, op, counter);
}

void assertByteByte(char* description, int expectedResult, byte number1, byte number2, const int op, int counter) {
  assert(description, expectedResult, (int)number1, (int)number2, op, counter);
}
