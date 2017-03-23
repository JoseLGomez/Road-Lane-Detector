/*********************************************************************
 * This file is part of the cpplibs suite.
 *
 * Copyright (C) 2001 Topi Mäenpää
 * All rights reserved.
 *
 * This program is free software. You can redistribute and/or modify
 * it under the terms of the free software licence found in the
 * accompanying file "COPYING". The licence terms must always be
 * redistributed with this source file. The above copyright notice
 * must be reproduced in all modified and unmodified copies of this
 * source file.
 *
 * $Revision: 1.1 $
 *********************************************************************/

#ifndef _BITOPERATION_H
#define _BITOPERATION_H

#if _MSC_VER > 1000
	#pragma once
#endif

#include <limits>
#include <string>
using std::string;

	/**
	 * A wrapper class for conventional naming of static functions for
	 * performing bit operations.
	 **/
	class BitOperation
	{
	public:
		/**
		 * Rotate a binary code right by n positions.
		 *
		 * @param c the number to be rotated
		 * @param n the number of positions to rotate
		 * @param bits the number of bits to consider
		 **/
		static unsigned int ror(unsigned int c, int n, int bits = 8)
			{
				return (c >> n) | ((c & ((1 << n) - 1)) << (bits - n));
			}
		
		/**
		 * Rotate a binary code left by n positions.
		 *
		 * @param c the number to be rotated
		 * @param n the number of positions to rotate
		 * @param bits the number of bits to consider
		 **/
//		static unsigned int rol(unsigned int c, int n, int bits = 8)
//			{
//				return (c << n) | ((c & (((1 << n) - 1) << (bits - n))) >> (bits - n)) & ((1 << bits) - 1);
//			}
		
		/**
		 * Get the number of ones in a binary number.
		 * @param c the number
		 * @param bits the number of bits to consider
		 **/
		static int onecount(unsigned int c, int bits = 8);
		/**
		 * Get the number of 0-to-1 or 1-to-0 transitions in a binary number.
		 * @param c the number
		 * @param bits the number of bits to consider
		 **/
		static int transitions(unsigned int c, int bits = 8);
		/**
		 * Rotate a binary number to its minimum value.
		 * @param c the number
		 * @param bits the number of bits to consider
		 **/
		static unsigned int rotmin(unsigned int n, int bits = 8);
		/**
		 * Calculate the Hamming distance between two binary numbers.
		 * @param a a number
		 * @param b another number
		 * @param bits the number of bits to consider
		 **/
		static int hammingDistance(unsigned int a, unsigned int b, int bits = 8);
		/**
		 * Get a textual representation for a binary number.
		 * @param a the number
		 * @param bits the number of bits to consider
		 * @return a binary representation of <i>a</i>
		 **/
		static std::string binary(unsigned int a, int bits = 8);
	};


#endif
