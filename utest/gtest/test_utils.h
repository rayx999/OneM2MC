/*
 * test_utils.h
 *
 *  Created on: 2015年7月15日
 *      Author: weimi_000
 */

#ifndef GTEST_UTEST_TEST_UTILS_H_
#define GTEST_UTEST_TEST_UTILS_H_

#include <iostream>
#include "CommonTypes.h"

namespace MicroWireless {

namespace OneM2M {

namespace UTest {

#define ASSERT_TSEQ(A, B)	\
	ASSERT_EQ(A.tv_sec, B.tv_sec);	\
	ASSERT_EQ(A.tv_usec, B.tv_usec)

#define EXPECT_TSEQ(A, B)	\
	ASSERT_EQ(A.tv_sec, B.tv_sec);	\
	EXPECT_GE(A.tv_usec, B.tv_usec)

template<typename T>
void ASSERT_ARREQ(T A[], T B[], int size) {
	for (int i = 0; i < size; i++) {
		ASSERT_EQ(A[i], B[i]);
	}
}

template<typename T>
void PrintArray(const T * array, int size) {
	for (int i = 0; i < size; i++) {
		std::cout << array[i] << ", ";
	}
	std::cout << std::endl;
}

bool CopyFile(const char * srcFn, const char * destFn);

}	// UTest

}	// OneM2M

}	// MicroWireless



#endif /* GTEST_UTEST_TEST_UTILS_H_ */
