
#define UNITY_INCLUDE_EXEC_TIME
#if defined(ARDUINO)
#define UNITY_CLOCK_MS millis
#endif
#include <unity.h>

#include "system/types.h"

void test_matrix_1x1_assignment(void) {
    Matrix<1, 1, uint16_t> matrix;
    matrix[0][0] = 10;
    TEST_ASSERT_EQUAL_UINT16(10, matrix.array[0][0]);
}

void test_matrix_10x10_assignment(void) {
    Matrix<10, 10, uint16_t> matrix;
    matrix[5][6] = 10;
    matrix[1][9] = 10;
    TEST_ASSERT_EQUAL_UINT16(10, matrix.array[5][6]);
    TEST_ASSERT_EQUAL_UINT16(10, matrix.array[1][9]);
}

void test_matrix_constant_mult_assignment(void) {
    Matrix<10, 10, uint16_t> matrix;
    matrix[5][6] = 10;
    matrix[1][9] = 10;
    matrix *= 2;
    TEST_ASSERT_EQUAL_UINT16(20, matrix.array[5][6]);
    TEST_ASSERT_EQUAL_UINT16(20, matrix.array[1][9]);
    TEST_ASSERT_EQUAL_UINT16(0, matrix.array[0][0]);
    TEST_ASSERT_EQUAL_UINT16(0, matrix.array[0][9]);
    TEST_ASSERT_EQUAL_UINT16(0, matrix.array[9][0]);
    TEST_ASSERT_EQUAL_UINT16(0, matrix.array[9][9]);
}


void test_matrix_two_matrix_sum_assignment(void) {
    Matrix<10, 10, uint16_t> matrix1;
    Matrix<10, 10, uint16_t> matrix2;
    matrix1[5][6] = 10;
    matrix1[1][9] = 10;
    matrix2[1][4] = 400;
    matrix2[2][8] = 400;
    matrix2[5][6] = 400;
    matrix1 += matrix2;
    TEST_ASSERT_EQUAL_UINT16(410, matrix1.array[5][6]);
    TEST_ASSERT_EQUAL_UINT16(10, matrix1.array[1][9]);
    TEST_ASSERT_EQUAL_UINT16(400, matrix1.array[1][4]);
    TEST_ASSERT_EQUAL_UINT16(400, matrix1.array[2][8]);
    TEST_ASSERT_EQUAL_UINT16(0, matrix1.array[0][0]);
    TEST_ASSERT_EQUAL_UINT16(0, matrix1.array[0][9]);
    TEST_ASSERT_EQUAL_UINT16(0, matrix1.array[9][0]);
    TEST_ASSERT_EQUAL_UINT16(0, matrix1.array[9][9]);
}

void runTests () {
    UNITY_BEGIN();
    RUN_TEST(test_matrix_1x1_assignment);
    RUN_TEST(test_matrix_10x10_assignment);
    RUN_TEST(test_matrix_constant_mult_assignment);
    RUN_TEST(test_matrix_two_matrix_sum_assignment);
    UNITY_END();
}

#if !defined(ARDUINO)

int main(int argc, char **argv) {
    runTests();
    return 0;
}

#else

#include <Arduino.h>

void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);
    runTests();
}

void loop() {
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(500);
}

#endif // ARDUINO
