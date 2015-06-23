// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CONSENSUS_PARAMS_H
#define BITCOIN_CONSENSUS_PARAMS_H

#include "uint256.h"

namespace Consensus {
/**
 * Parameters that influence chain consensus.
 */
struct Params {
    uint256 hashGenesisBlock;
    int nMaxReorganizationDepth;
    /** Used to check majorities for block version upgrade */
    int nMajorityEnforceBlockUpgrade;
    int nMajorityRejectBlockOutdated;
    int nMajorityWindow;
    /** Proof of work parameters */
    uint256 powLimit;
    uint256 posLimit;
    uint256 posLimitV2;
    bool fPowAllowMinDifficultyBlocks;
    int64_t nTargetSpacingV1;
    int64_t nTargetSpacing;
    int64_t nTargetTimespan;
    int64_t DifficultyAdjustmentInterval() const { return nTargetTimespan / nTargetSpacing; }
    int64_t nProtocolV1RetargetingFixedTime;
    int64_t nProtocolV2Time;
    bool IsProtocolV2(int64_t nTime) const { return nTime > nProtocolV2Time && nTime != 1407053678; }
    int64_t nProtocolV3Time;
    int nLastPOWBlock;
    int nStakeTimestampMask;
};
} // namespace Consensus

#endif // BITCOIN_CONSENSUS_PARAMS_H
