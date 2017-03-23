/*********************************************************************
 * This file is part of the cpplibs suite.
 *
 * Copyright (C) 2001 Topi M‰enp‰‰
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

#include "BitOperation.h"

	int BitOperation::onecount(unsigned int c, int bits)
	{
		int count = 0, base = 1;
		for (int i=0;i<bits;i++)
			{
				if (c & base) count++;
				base <<= 1;
			}
		return count;
	}

	int BitOperation::transitions(unsigned int c, int bits)
	{
		int base = 1;
		int current = c & base, current2, changes = 0;
		for (int i=1;i<bits;i++)
			{
				base <<= 1;
				current2 = (c & base) >> i;
				if (current ^ current2) changes++;
				current = current2;
			}
		return changes; //(changes <= 2)? 1 : 0;
	}

	unsigned int BitOperation::rotmin(unsigned int n, int bits)
	{
		unsigned int tmp = n << sizeof(int)*4;
		//INTMAX
		unsigned int lowmask = std::numeric_limits<int>::max()  >> (sizeof(int)*4-1);
		unsigned int min = tmp;
		int minIndex=0;
		for (int i=1;i<bits;i++)
			{
				tmp >>= 1;
				tmp |= (tmp & lowmask) << bits;
				tmp &= ~lowmask;
				if (tmp<min)
					{
						min = tmp;
						minIndex = i;
					}
			}
		tmp = n << sizeof(int)*4 - minIndex;
		tmp |= (tmp & lowmask) << bits;
		tmp &= ~lowmask;
		return tmp >> sizeof(int)*4;
	}

	int BitOperation::hammingDistance(unsigned int a, unsigned int b, int bits)
	{
		int c = a^b;
		int dist = 0, base = 1;
		for (int i=0;i<bits;i++)
			{
				if (base & c) dist++;
				base <<= 1;
			}
		return dist;
	}

	//Return a <bits>-bit binary representation of the given number.
	string BitOperation::binary(unsigned int a, int bits)
	{
		string result(bits,'0');

		int base = 1;
		for (int i=bits-1;i>=0;i--)
			{
				result[i] = (a & base)? '1' : '0';
				base <<= 1;
			}
		return result;
	}

