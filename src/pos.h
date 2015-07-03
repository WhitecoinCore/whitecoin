// Copyright (c) 2015-2016 The BlackCoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BLACKCOIN_POS_H
#define BLACKCOIN_POS_H

#include <stdint.h>

class CBlockIndex;
class CCoins;
class COutPoint;
class uint256;

/** Compute the hash modifier for proof-of-stake */
uint256 ComputeStakeModifier(const CBlockIndex* pindexPrev, const uint256& kernel);

// Check whether the coinstake timestamp meets protocol
bool CheckCoinStakeTimestamp(int64_t nTimeBlock, int64_t nTimeTx);
bool CheckStakeBlockTimestamp(int64_t nTimeBlock);

bool CheckStakeKernelHash(const CBlockIndex* pindexPrev, unsigned int nBits, const CCoins* txPrev, const COutPoint& prevout, unsigned int nTimeTx);

#endif // BLACKCOIN_POS_H
