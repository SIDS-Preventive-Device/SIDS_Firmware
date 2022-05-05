
#define UNITY_INCLUDE_EXEC_TIME
#if defined(ARDUINO)
#define UNITY_CLOCK_MS millis
#endif
#include <unity.h>

#include "system/utils/matrix.h"
#include <math.h>

void test_matrix_1x1_assignment(void) {
    Matrix<1, 1, uint16_t> matrix;
    matrix[0][0] = 10;
    TEST_ASSERT_EQUAL_UINT16(10, matrix.array[0][0]);
}

void test_matrix_from_array_assignment(void) {
    uint16_t array[10][10] = {
        {0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
        {0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
        {0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
        {0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
        {0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
        {0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
        {0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
        {0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
        {0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
        {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}
    };
    Matrix<10, 10, uint16_t> matrix (array);
    TEST_ASSERT_EQUAL_UINT16(6, matrix.array[5][6]);
    TEST_ASSERT_EQUAL_UINT16(9, matrix.array[1][9]);
}

void test_matrix_10x10_assignment(void) {
    Matrix<10, 10, uint16_t> matrix;
    matrix[5][6] = 10;
    matrix[1][9] = 10;
    TEST_ASSERT_EQUAL_UINT16(10, matrix.array[5][6]);
    TEST_ASSERT_EQUAL_UINT16(10, matrix.array[1][9]);
}

void test_matrix_constant_mult(void) {
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


void test_matrix_two_matrix_sum(void) {
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

void test_matrix_two_matrix_mult(void) {
    Matrix<3, 3, uint16_t> matrix1;
    Matrix<3, 3, uint16_t> matrix2;
    Matrix<3, 3, uint16_t> result;
    matrix1[0][0] = 10; matrix1[1][0] = 10; matrix1[2][0] = 10;
    matrix1[0][1] = 10; matrix1[1][1] = 10; matrix1[2][1] = 10;
    matrix1[0][2] = 10; matrix1[1][2] = 10; matrix1[2][2] = 10;
    matrix2[0][0] = 10; matrix2[1][0] = 10; matrix2[2][0] = 10;
    matrix2[0][1] = 10; matrix2[1][1] = 10; matrix2[2][1] = 10;
    matrix2[0][2] = 10; matrix2[1][2] = 10; matrix2[2][2] = 10;
    result = (matrix1 * matrix2);

    Matrix<3, 3, uint16_t> expected;
    expected[0][0] = 300; expected[1][0] = 300; expected[2][0] = 300;
    expected[0][1] = 300; expected[1][1] = 300; expected[2][1] = 300;
    expected[0][2] = 300; expected[1][2] = 300; expected[2][2] = 300;
    TEST_ASSERT_EQUAL_UINT16_ARRAY(expected.array, result.array, expected.elements());
}

void test_matrix_matrix_vector_mult(void) {
    Matrix<3, 3, uint16_t> matrix1;
    Matrix<3, 1, uint16_t> matrix2;
    Matrix<3, 1, uint16_t> result;
    matrix1[0][0] = 10; matrix1[1][0] = 10; matrix1[2][0] = 10;
    matrix1[0][1] = 10; matrix1[1][1] = 10; matrix1[2][1] = 10;
    matrix1[0][2] = 10; matrix1[1][2] = 10; matrix1[2][2] = 10;

    matrix2[0][0] = 10; matrix2[1][0] = 10; matrix2[2][0] = 10;
    result = (matrix1 * matrix2);

    Matrix<3, 1, uint16_t> expected;
    expected[0][0] = 300; expected[1][0] = 300; expected[2][0] = 300;
    TEST_ASSERT_EQUAL_UINT16_ARRAY(expected.array, result.array, expected.elements());
}

void test_matrix_matrix_float_vector_int_mult(void) {
    Matrix<3, 3, float> matrix1;
    Matrix<3, 1, int16_t> matrix2;
    Matrix<3, 1, float> result;
    matrix1[0][0] = 2.5f; matrix1[1][0] = 2.5f; matrix1[2][0] = 2.5f;
    matrix1[0][1] = 2.5f; matrix1[1][1] = 2.5f; matrix1[2][1] = 2.5f;
    matrix1[0][2] = 2.5f; matrix1[1][2] = 2.5f; matrix1[2][2] = 2.5f;

    matrix2[0][0] = 10; matrix2[1][0] = 10; matrix2[2][0] = 10;
    result = (matrix1 * matrix2);

    Matrix<3, 1, float> expected;
    expected[0][0] = 75.0f; expected[1][0] = 75.0f; expected[2][0] = 75.0f;
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected.array, result.array, expected.elements());
}

void test_matrix_matrix_get_rows(void) {
    Matrix<3, 1, uint16_t> matrix;
    TEST_ASSERT_EQUAL_UINT16(3, matrix.rows());
}

void test_matrix_matrix_get_cols(void) {
    Matrix<3, 1, uint16_t> matrix;
    TEST_ASSERT_EQUAL_UINT16(1, matrix.cols());
}

void test_matrix_matrix_get_elements(void) {
    Matrix<2, 5, uint16_t> matrix;
    TEST_ASSERT_EQUAL_UINT16(10, matrix.elements());
}

void test_matrix_matrix_transpose_zeros(void) {
    Matrix<2, 2, uint16_t> matrix;
    Matrix<2, 2, uint16_t> result;
    Matrix<2, 2, uint16_t> expected;
    matrix[0][0] = 0;
    matrix[0][1] = 0;
    matrix[1][0] = 0;
    matrix[1][1] = 0;
    result = ~matrix;

    expected[0][0] = 0;
    expected[0][1] = 0;
    expected[1][0] = 0;
    expected[1][1] = 0;
    TEST_ASSERT_EQUAL_UINT16_ARRAY(expected.array, result.array, expected.elements());
}

void test_matrix_matrix_transpose_values(void) {
    Matrix<2, 2, uint16_t> matrix;
    Matrix<2, 2, uint16_t> result;
    Matrix<2, 2, uint16_t> expected;
    matrix[0][0] = 3;
    matrix[0][1] = 4;
    matrix[1][0] = 2;
    matrix[1][1] = 1;
    result = ~matrix;

    expected[0][0] = 3;
    expected[0][1] = 2;
    expected[1][0] = 4;
    expected[1][1] = 1;
    TEST_ASSERT_EQUAL_UINT16_ARRAY(expected.array, result.array, expected.elements());
}

void test_matrix_matrix_transpose_reset(void) {
    Matrix<2, 2, uint16_t> matrix;
    Matrix<2, 2, uint16_t> result;
    Matrix<2, 2, uint16_t> expected;
    matrix[0][0] = 3;
    matrix[0][1] = 4;
    matrix[1][0] = 2;
    matrix[1][1] = 1;
    result = ~(~matrix);

    expected[0][0] = 3;
    expected[0][1] = 4;
    expected[1][0] = 2;
    expected[1][1] = 1;
    TEST_ASSERT_EQUAL_UINT16_ARRAY(expected.array, result.array, expected.elements());
}

void runTests () {
    UNITY_BEGIN();
    RUN_TEST(test_matrix_1x1_assignment);
    RUN_TEST(test_matrix_10x10_assignment);
    RUN_TEST(test_matrix_from_array_assignment);
    RUN_TEST(test_matrix_constant_mult);
    RUN_TEST(test_matrix_two_matrix_sum);
    RUN_TEST(test_matrix_two_matrix_mult);
    RUN_TEST(test_matrix_matrix_vector_mult);
    RUN_TEST(test_matrix_matrix_float_vector_int_mult);
    RUN_TEST(test_matrix_matrix_get_cols);
    RUN_TEST(test_matrix_matrix_get_rows);
    RUN_TEST(test_matrix_matrix_get_elements);
    RUN_TEST(test_matrix_matrix_transpose_zeros);
    RUN_TEST(test_matrix_matrix_transpose_values);
    RUN_TEST(test_matrix_matrix_transpose_reset);
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
