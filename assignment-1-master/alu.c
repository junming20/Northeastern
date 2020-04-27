/*
 * alu.c
 *
 * This file implements functions performed by an arithmetic logic
 * unit of a computer on a sequence of bytes that comprise a word.
 *
 * Note: The example documentation shows 16-bit words divided into
 * 4-bit (nibble) chunks for simplicity. Actual words are wordsize
 * bits divided into 8-bit (byte) chunks.
 *
 * @since 2020-01-02
 * @author philip gust
 */
#include <stdlib.h>

#include "alu.h"

/**
 * Returns true if word is less than zero.
 *
 * Examples (big-endian):
 *   testLtWord(0000 0000 0001 0000) -> false
 *   testLtWord(1111 1111 0000 1111) -> true
 *
 * @param op the operand word
 * @return true if less than 0, false otherwise
 */
bool testLtWord(const word op) {
	// your code here
	//
	// NOTES:
	// A word is negative only if its sign bit
	// (wordtopbit) is set.
	//
	return false;  // delete this line
}

/**
 * Returns true if word is greater or equal to zero.
 *
 * Examples (big-endian):
 *   testGeWord(0000 0000 0000 0000) -> true
 *   testGeWord(0000 0000 0001 0000) -> true
 *   testGeWord(1111 1111 0000 1111) -> false
 *
 * @param op the operand word
 * @return true if greater than or equal 0, false otherwise
 */
bool testGeWord(const word op) {
	// your code here
	//
	// NOTES:
	// This is the inverse of the LT condition.
	//
	return false;  // delete this line
}

/**
 * Returns true if word is zero.
 *
 * Examples:
 *   testEqWord(0000 0000 0000 0000) -> true
 *   testEqWord(0000 1111 1111 1111) -> false
 *
 * @param op the operand word
 * @return true if equal 0, false otherwise
 */
bool testEqWord(const word op) {
	// your code here
	//
	// NOTES:
	// Return true if all bits of word are 0
	//
	return false;  // delete this line
}

/**
 * Arithmetic shift word by count. Same as multiplying or
 * dividing by power of 2.  Shifts bits in word left (+) or
 * right (-) by the specified count. Fills in with 0 from
 * the right, and the sign bit from the left.
 *
 * Examples (big-endian):
 * 	ash(1010 1011 1111 1111, +5) -> 1111 1111 1110 0000
 * 	ash(1111 1111 0000 0000, -5) -> 1111 1111 1111 1000
 *
 * @param result the result word
 * @param op the operand word
 * @param count the shift count
 */
void ashWord(word result, const word op, int count) {
	int c = abs(count);
	if (c >= wordsize) {
		c = wordsize-1;
	}

	// local copy in case op and result overlap
	word localop;
	setWord(localop, op);

	bit sign = getBitOfWord(localop, wordtopbit);
	if (count < 0) {
		// move upper bits of word right
		for (int b = wordtopbit-1; b >= c; b--) {
			bit t = getBitOfWord(localop, b);
			setBitOfWord(result, b-c, t);
		}
		// clear upper bits of word to sign bit
		for (int b = c; b >= 1; b--) {
			setBitOfWord(result, wordtopbit-b, sign);
		}
	} else {
		// move lower bits of word left
		for (int b = wordtopbit-1; b >= c; b--) {
			bit t = getBitOfWord(localop, b-c);
			setBitOfWord(result, b, t);
		}
		// clear lower bits of word
		for (int b = c-1; b >= 0; b--) {
			setBitOfWord(result, b, 0);
		}
	}
	// set top bit of word
	setBitOfWord(result, wordtopbit, sign);
}

/**
 * Circular shift word by count. Shifts bits in word
 * left (+) or right (-) by the specified count. Bits
 * shifted off either end of word are rotated in to
 * the other end of word.
 *
 * Examples (big-endian):
 * 	csh(1010 1011 1111 1111, -4) -> 1011 1111 1111 1010
 * 	csh(1010 1011 1111 1111, -4) -> 1111 1010 1011 1111
 *
 * @param result the result word
 * @param op the operand word
 * @param count the shift count
 */
void cshWord(word result, const word op, int count) {
	// your code here
	//
	// NOTES:
	// Set bits in result, from op, shifted left or right
	// based on the sign of the count. Compute the position
	// in result word using the C remainder operator (%) with
	// wordsize to "wrap around" to the position on the other
	// end of result word.
	//
	setWord(result, zeroWord);  // delete this line
}

/**
 * Logical shift word by count. Shifts bits in word
 * left (+) or right (-) by the specified count. Shifts
 * bits off end and fills in with 0 in either direction.
 *
 * Examples (big-endian):
 * 	lsh(1111 1111 1111 1111, +5) -> 1111 1111 1110 0000
 * 	lsh(1111 1111 1111 1111, -5) -> 0000 0111 1111 1111
 *
 * @param result the result word
 * @param op the operand word
 * @param count the shift count
 */
void lshWord(word result, const word op, int count) {
	// your code here
	//
	// NOTES:
	// Set bits in result from op, shifted left or
	// right based on the sign o the count. Then fill
	// in result bits from left or right with 0 bits.
	//
	setWord(result, zeroWord);  // delete this line
}

/**
 * Mask out all but lower (+) or upper (-) count bits
 * of word operand.
 *
 * Examples (big-endian):
 * 	mask(1010 1011 1111 1111, +5) -> 0000 0000 0001 1111
 * 	mask(1010 1011 1111 1111, -5) -> 1010 1000 0000 0000
 *
 * @param result the result
 * @param op the operand word
 * @param count the mask count
 */
void maskWord(word result, const word op, int count) {
	int c = abs(count);
	if (c > wordsize) {
		c = wordsize;
	}

	if (count < 0) {
		int wtbc = wordtopbit - c;

		// copy upper bits of word
		for (int b = wordtopbit; b > wtbc; b--) {
			bit t = getBitOfWord(op, b);
			setBitOfWord(result, b, t);
		}

		// clear lower bits of word
		for (int b = wtbc; b >= 0; b--) {
			setBitOfWord(result, b, 0);
		}
	} else {
		// copy lower bits of word
		for (int b = 0; b < c; b++) {
			bit t = getBitOfWord(op, b);
			setBitOfWord(result, b, t);
		}

		// clear upper bits of word
		for (int b = c; b <= wordtopbit; b++) {
			setBitOfWord(result, b, 0);
		}
	}
}

/**
 * Logical AND of two word operands.
 *
 * Examples:
 *   and(1111 0000 0110 1001, 1111 1111 0000 0000) -> 1111 0000 0000 0000
 *
 * @param result the result
 * @param op1 the first operand word
 * @param op2 the second operand word
 */
void andWord(word result, const word op1, const word op2) {
	// your code here
	//
	// NOTES:
	// Use bit-wise AND operator (&) on each bit of op1
	// and op2 to set each bit of the result.
	//
	setWord(result, zeroWord);  // delete this line
}

/**
 * Logical OR of two word operands.
 *
 * Examples:
 *   or(1111 0000 0110 1001, 0000 1111 0000 0000) -> 1111 1111 0110 1001
 *
 * @param result the result
 * @param op1 the first operand word
 * @param op2 the second operand word
 */
void orWord(word result, const word op1, const word op2) {
	// your code here
	//
	// NOTES:
	// Use bit-wise OR operator (|) on each bit of op1
	// and op2 to set each bit of the result.
	//
	setWord(result, zeroWord);  // delete this line
}

/**
 * Logical XOR of two word operands.
 *
 * Examples:
 *   xor(1111 0000 0110 1001, 1111 1111 1111 1111) -> 0000 1111 1001 0110
 *
 * @param result the result
 * @param op1 the first operand word
 * @param op2 the second operand word
 */
void xorWord(word result, const word op1, const word op2) {
	// your code here
	//
	// NOTES:
	// Use bit-wise XOR operator (^) on each bit of op1
	// and op2 to set each bit of the result.
	//
	setWord(result, zeroWord);  // delete this line
}

/**
 * Logical NOT of word operand.
 *
 * Examples:
 * 	not(0011 0000 0000 0000) -> 1100 1111 1111 1111
 *
 * @param result the result word
 * @param op the operand word
 */
void notWord(word result, const word op) {
	for (int b = 0; b <= wordtopbit; b++) {
		setBitOfWord(result, b, notBit(getBitOfWord(op,b)));
	}
}

/**
 * Negative of word operand.
 *
 * Examples: (big-endian):
 * 	negative(0000 0000 0000 0011) -> 1111 1111 1111 1101
 * 	negative(1111 1111 1111 1111) -> 0000 0000 0000 0001
 *
 * @param result the result word
 * @param op the operand word
 */
void negativeWord(word result, const word op) {
	// your code here
	//
	// NOTES:
	// Equivalent to (0 - op).
	//
	setWord(result, zeroWord);  // delete this line
}

/**
 * Sum of two word operands.
 *
 * @param result the result word
 * @param op1 the first operand word
 * @param op2 the second operand word
 */
void addWord(word result, const word op1, const word op2) {
	// your code here
	//
	// NOTES:
	// The algorithm requires adding the bits of
	// op1 and op2 starting from the least significant
	// bit. If the sum is greater than 1, then use the
	// lower bit of the sum as the bit for that position
	// and carry 1 to the next bits if the sum is 2 or 3.
	// The initial value of the carry bit is 0.
	//
	//     0 1 1 0  (6)
	//   + 1 1 1 1  (-1)
	//     -------
	//           1  sum
	//         0    carry
	//     -------
	//         0 1  sum
	//       1      carry
	//     -------
	//       1 0 1  sum
	//     1        carry
	//     -------
	//     0 1 0 1  sum
	//   1          carry
	//
	// Final sum is 0101 (5)
	setWord(result, zeroWord);  // delete this line
}

/**
 * Difference of two word operands.
 *
 * @param result the result word
 * @param op1 the first operand word
 * @param op2 the second operand word
 */
void subWord(word result, const word op1, const word op2) {
	bit carry = 1;
	for (uint8_t b = 0; b <= wordtopbit; b++) {
		// can be 0..3
		uint8_t r = getBitOfWord(op1,b) + notBit(getBitOfWord(op2,b)) + carry;
		carry = r >> 1;
		setBitOfWord(result, b, toBit(r));
	}
}

/**
 * Product of two word operands.
 *
 * @param result the result word
 * @param op1 the first operand word
 * @param op2 the second operand word
 */
void mulWord(word result, const word op1, const word op2) {
	// your code here
	//
	// NOTES:
	// The algorithm require making op2 positive.
	// Can be done by making taking the negative of
	// both operands if op2 is negative, or by making
	// both operands positive and applying the sign at
	// the end.
	//
	// Here is how to multiply two binary numbers. For
	// each bit in op2 from the least to most significant
	// bit, left shift op1 left by 1, and if bit of op2
	// is set, add shifted op1 into results. Here is an
	// example for a 4-bit number.
	//
	//          0 0 1 1
	//       x  1 1 0 1
	//       ----------
	//          0 0 1 1
	//        0 0 0 0
	//      0 0 1 1
	//  + 0 0 1 1
	//    -------------
	//    0 1 0 0 1 1 1
	//
	setWord(result, zeroWord);  // delete this line
}

/**
 * Quotient of two word operands also returning remainder.
 * The sign of the quotient is positive if the signs of
 * the operands match, and negative if they do not.
 * The sign of the remainder matches the sign of the dividend.
 *
 * @param result the result word
 * @param remainder the remainder word
 * @param op1 the first operand word
 * @param op2 the second operand word
 */
void div2Word(word result, word remainder, const word op1, const word op2) {
	setWord(result, zeroWord);
	setWord(remainder, zeroWord);

	if (testEqWord(op2)) {
		// handle divide by 0 by returning largest
		// positive or negative number
		setWord(result, (testGeWord(op1) ? maxWord : minWord));
	} else {
		word w1, w2;
		bool resultNegative = false;
		// operands must be positive
		if (testLtWord(op1)) {
			negativeWord(w1, op1);
			resultNegative = !resultNegative;
		} else {
			setWord(w1, op1);
		}
		if (testLtWord(op2)) {
			negativeWord(w2, op2);
			resultNegative = !resultNegative;
		} else {
			setWord(w2, op2);
		}

		//
		for (int b = wordtopbit; b >= 0; b--) {
			lshWord(remainder, remainder, 1);	// position remainder
			bit t = getBitOfWord(w1, b);	// bring down next bit
			setBitOfWord(remainder, 0, t);

			word test;
			subWord(test, remainder, w2);  // do trial subtract
			if (testGeWord(test)) {	// division successful if still positive
				setBitOfWord(result, b, 1);	// shift bit into result
				setWord(remainder, test);   // update remainder
			}
		}

		if (resultNegative) {	// set correct sign of result
			negativeWord(result, result);
		}

		if (testLtWord(op1)) { // remainder negative if op1 is negative
			negativeWord(remainder, remainder);
		}
	}
}

/**
 * Quotient of two word operands.
 * The sign of the quotient is positive if the signs of
 * the operands match, and negative if they do not.
 *
 * @param result the result word
 * @param op1 the first operand word
 * @param op2 the second operand word
 */
void divWord(word result, const word op1, const word op2) {
	word remainder;
	div2Word(result, remainder, op1, op2);
}

/**
 * Remainder of two word operands.
 * The sign of the remainder matches the sign of the dividend.
 *
 * @param result the result word
 * @param op1 the first operand word
 * @param op2 the second operand word
 */
void remainderWord(word result, const word op1, const word op2) {
	word quotient;
	div2Word(quotient, result, op1, op2);
}
