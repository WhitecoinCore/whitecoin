// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "txdb.h"
#include "main.h"
#include "uint256.h"


static const int nCheckpointSpan = 500;

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        (  0,         uint256("0x000007d69ba0f79b4823effb06b08663e2e4c51ed03aaeb547e2e0b83fd37b73") ) // genesis
        (  2,         uint256("0x750129f3369574f0c397e28273cc5e795f5a78d3c43eba4b58583bac96e62e18") ) // Premine for swap
        (  2500,      uint256("0x4a7f2569e40a991bda8fc8510b31e0b79344719c03fe96a5c3d2069c15aee89a") ) 
        (  5000,      uint256("0xc207e945e7b09b79b6601be24d891a96a1a632933887fc8f07ee4f7a1af2e612") ) 
        (  10000,     uint256("0xb191f69a673fe281d091003c41729204aa1b9b86b839c03f49f8665bffd87e53") ) 
		(  25000,     uint256("0x2fea86d43efd05fe0b4c9a78d71a5a3238e2133476aeddc56d7128c510405bd9") ) 
		(  50000,     uint256("0xc0e4332839da0d1f8f41702da22efa2c438c1a108ce6a8ec7a567798ff7e7869") ) 
		(  100000,    uint256("0xfa254c1010b93ed6944d9d3489792a034ae1ab5d795e0cfb538fccaac672f7c5") ) 
		(  200000,    uint256("0x381945f7b9b8ab95b6b9de199a2027fd9ae1f57ad4b766369e8a1a9b27448b49") ) 
		(  300000,    uint256("0x60aca7679ad42be9b1359a18c9823544b471ed113077efee5a7097d80905103e") ) 
		(  400000,    uint256("0xbd3ff208033a486ae4dc1d94020bad478433a7933c2109cd8cf4675ee0477fe2") ) 
		(  609332,    uint256("0xc2683173c64c3cd223ac2cc2a8d8f841b7e2c15142e89d1bed3b6dcf2580f22c") ) // Hardfork PoS increase
		(  650000,    uint256("0x2443cb1d00a0a2b1679fb9b203946cbf9fdd662b8b36322505c96c9f20157451") ) 
		(  700000,    uint256("0xc3b608dfaf97dfef9e74a4095a69981692186c1f86f9e94de4a63b6ad2a6f733") ) 
		(  750000,    uint256("0xc2ac6306b609f6912aa7a8e617050dc0330d7a8a10c3943d27d14b4b9b6f88cd") ) 
		(  800000,    uint256("0x0710b931960777f43ef5ed27beaa615a86b6992cfcb8174d16221cacc6860f15") ) 
		(  850000,    uint256("0x95ce0d9ac188f2463360f856a71141c5e393a8625941d50bda0f5a2c09cf416b") ) 
		(  900000,    uint256("0x5885eb82e0afb10f2bf5f07d6362920ab45257b42f8867e16f04586b9b2c93c8") ) 
    ;

    // TestNet has no checkpoints
    static MapCheckpoints mapCheckpointsTestnet =
            boost::assign::map_list_of
            ( 0,   uint256("0000a17e5bfeea11754901c7acf929cf157856b386b851c5a3f7920b9e5dab49") ) // genesis
        ;

    bool CheckHardened(int nHeight, const uint256& hash)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    int GetTotalBlocksEstimate()
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        if (checkpoints.empty())
            return 0;
        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }

    // Automatically select a suitable sync-checkpoint 
    const CBlockIndex* AutoSelectSyncCheckpoint()
    {
        const CBlockIndex *pindex = pindexBest;
        // Search backward for a block within max span and maturity window
        while (pindex->pprev && pindex->nHeight + nCheckpointSpan > pindexBest->nHeight)
            pindex = pindex->pprev;
        return pindex;
    }

    // Check against synchronized checkpoint
    bool CheckSync(int nHeight)
    {
        const CBlockIndex* pindexSync = AutoSelectSyncCheckpoint();

        if (nHeight <= pindexSync->nHeight)
            return false;
        return true;
    }
}
