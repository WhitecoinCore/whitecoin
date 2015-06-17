// Copyright (c) 2015-2016 The BlackCoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BLACKCOIN_POS_H
#define BLACKCOIN_POS_H

class CBlockIndex;
class uint256;

/** Compute the hash modifier for proof-of-stake */
uint256 ComputeStakeModifier(const CBlockIndex* pindexPrev, const uint256& kernel);

#endif // BLACKCOIN_POS_H
