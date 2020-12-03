// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "assert.h"

#include "chainparams.h"
#include "main.h"
#include "util.h"

#include <boost/assign/list_of.hpp>

using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"
//
// Main network
//

// Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress> &vSeedsOut, const SeedSpec6 *data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int i = 0; i < count; i++)
    {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

class CMainParams : public CChainParams {
public:
    CMainParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0x18;
        pchMessageStart[1] = 0x2d;
        pchMessageStart[2] = 0x43;
        pchMessageStart[3] = 0xf3;
        vAlertPubKey = ParseHex("047fa7e48a9852045a59a39ebc35bf9428ff739409808443e63e2e8341a5833f988b386a523bcb23276852b42f8f99b1da994212127de511a24bdf73b39b3a2861");

        nDefaultPort = 15814;
        nRPCPort = 15815;
        nMaxReorganizationDepth = 100;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 20);

        // Build the genesis block. Note that the output of the genesis coinbase cannot
        // be spent as it did not originally exist in the database.
        // Genesis info:
        //Found Genesis, Nonce: 45707, Hash: 000007d69ba0f79b4823effb06b08663e2e4c51ed03aaeb547e2e0b83fd37b73
        //Gensis Hash Merkle: 73513debc549137a47f2afb73173a2d2b4b0c13f57a57387ae3849a928e1e08d
        const char* pszTimestamp = "12 Feb 2017 Whitecoin switches to POSv3";
        std::vector<CTxIn> vin;
        vin.resize(1);
        vin[0].scriptSig = CScript() << 0 << CBigNum(42) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        std::vector<CTxOut> vout;
        vout.resize(1);
        vout[0].SetEmpty();
        CTransaction txNew(1, 1486939650, vin, vout, 0);
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = 1486939650;
        genesis.nBits    = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce   = 45707;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x000007d69ba0f79b4823effb06b08663e2e4c51ed03aaeb547e2e0b83fd37b73"));
        assert(genesis.hashMerkleRoot == uint256("0x73513debc549137a47f2afb73173a2d2b4b0c13f57a57387ae3849a928e1e08d"));

        vSeeds.push_back(CDNSSeedData("dnsseed-cn", "dnsseeder.whitecoin.in"));
        vSeeds.push_back(CDNSSeedData("dnsseed-en", "dnsseeder1.whitecoin.in"));
        vSeeds.push_back(CDNSSeedData("dnsseed", "dnsseeder.whitecoin.pub"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 73);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 87);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1, 73+128);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0x7F)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0x94)(0xED).convert_to_container<std::vector<unsigned char> >();
        // BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x9B).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        nLastPOWBlock = 10000;
        nPreMineCoins = 313000000;
        nFirstForkTime  = 1527890400;       //  Fr, 01 Jun 2018 22:00:00 GMT (DriftFix and PoS reward to 5 XWC)
        nSecondForkTime = 1584259200;
        nPosIncreaseTime = 1585641600;
        nFirstHalfTime = 1615795200;
        nSecondHalfTime = 1678867200;
        nThirdHalfTime = 1742025600 ;
        nFouthHalfTime = 1805097600;
        nMainNetDriftTime = 15;
        nDestroyedCoins = (65217844.265830+113468742.46487160);
        
        //Registered Message PubKey
        mapBroadcastMessPubKey.insert(pair<std::string,vector<unsigned char> >("Ray",   ParseHex("0464ae3e87304fb08a296cf4e75407be11214dd5e2bde6554ca2aaa743ecbbb98d8dc517b9f02265dfccafdda81b45fd88311b3147812d982dec213cd5af3ae174")));
        mapBroadcastMessPubKey.insert(pair<std::string,vector<unsigned char> >("Lizhi", ParseHex("04cd377cb31be7b1b4484f8b42e9ca3b748fa9fb3ab1f877ecb9907bfd8623cdaba04c15db1ac897bc384a355e3e099bd78696b3ff03e7955ab43bf3c30bb6e7ec")));
    }

    virtual const CBlock& GenesisBlock() const { return genesis; }
    virtual Network NetworkID() const { return CChainParams::MAIN; }

    virtual const vector<CAddress>& FixedSeeds() const {
        return vFixedSeeds;
    }
protected:
    CBlock genesis;
    vector<CAddress> vFixedSeeds;
};
static CMainParams mainParams;


//
// Testnet
//

class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0x31;
        pchMessageStart[1] = 0xcd;
        pchMessageStart[2] = 0xd1;
        pchMessageStart[3] = 0xf7;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 16);
        nMaxReorganizationDepth = 100;
        vAlertPubKey = ParseHex("045ed1db845f7ed72b4c0d21946a6c651b804a26b76fbeba0008c90c5b3396003487b85c4bcc7e56680da2a01344bf7c4d2007edc913db001195ba12614556609a");
        nDefaultPort = 25070;
        nRPCPort = 25071;
        strDataDir = "testnet";


        // Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nBits  = bnProofOfWorkLimit.GetCompact();

        genesis.nNonce = 55278;
        genesis.nTime    = 1606462200;
        hashGenesisBlock = genesis.GetHash();


/*
        hashGenesisBlock = uint256("0x01");
        if (true && genesis.GetHash() != hashGenesisBlock)
        {
                uint256 hashTarget = CBigNum().SetCompact(genesis.nBits).getuint256();
                    LogPrintf("recalculating params for mainnet.\n");
                    LogPrintf("old TestNet genesis nonce: %d\n", genesis.nNonce);
                    LogPrintf("old TestNet genesis hash:  %s\n", hashGenesisBlock.ToString().c_str());
                    // deliberately empty for loop finds nonce value.

                    for(genesis.nNonce = 0; genesis.GetHash() > hashTarget; genesis.nNonce++){ }

                    LogPrintf("new TestNet genesis merkle root: %s\n", genesis.hashMerkleRoot.ToString().c_str());
                    LogPrintf("new TestNet genesis nonce: %d\n", genesis.nNonce);
                    LogPrintf("new TestNet genesis hash: %s\n", genesis.GetHash().ToString().c_str());
        }
*/


        assert(hashGenesisBlock == uint256("0x0000d6164124157a3274cf1efd49ec5fb355afb598758ba545b3892c67b24c44"));
        assert(genesis.hashMerkleRoot == uint256("0x73513debc549137a47f2afb73173a2d2b4b0c13f57a57387ae3849a928e1e08d"));

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("dnsseed", "dnsseedtest.xwccore.com"));
        vSeeds.push_back(CDNSSeedData("seed1", "112.126.86.246"));
        vSeeds.push_back(CDNSSeedData("seed2", "101.201.37.171"));
        vSeeds.push_back(CDNSSeedData("seed3", "112.126.86.246"));
        vSeeds.push_back(CDNSSeedData("seed3", "39.107.66.180"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 196);

        base58Prefixes[SECRET_KEY]     = std::vector<unsigned char>(1, 239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        nPreMineCoins = 2000000000;
        nLastPOWBlock = 10000;//
        nFirstForkTime  = 1527890400;      //  Fr, 01 Jun 2018 22:00:00 GMT (DriftFix and PoS reward to 5 XWC)
        nSecondForkTime = 1583557200;   //2020-03-7 13:00:00
        nFirstHalfTime = 1607593002;        //2020-03-1 10:00:00
        nSecondHalfTime = nFirstHalfTime+10000;   //2020-03-2 10:00:00
        nThirdHalfTime = nSecondHalfTime+10000 ;     //2020-03-3 10:00:00
        nFouthHalfTime = nThirdHalfTime+10000;     //2020-03-04 10:00:00
        nPosIncreaseTime = nFouthHalfTime+ 10000 ;//2020-02-29 20:00:00
        nDestroyedCoins = 0;

        nMainNetDriftTime = 120;
    }
    virtual Network NetworkID() const { return CChainParams::TESTNET; }
};
static CTestNetParams testNetParams;


//
// Regression test
//
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        pchMessageStart[0] = 0x7b;
        pchMessageStart[1] = 0x92;
        pchMessageStart[2] = 0xb3;
        pchMessageStart[3] = 0xee;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 1);
        nMaxReorganizationDepth = 100;
        genesis.nTime = 1411111111;
        genesis.nBits  = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 2;
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 18444;
        strDataDir = "regtest";
        //assert(hashGenesisBlock == uint256("0x523dda6d336047722cbaf1c5dce622298af791bac21b33bf6e2d5048b2a13e3d"));

        vSeeds.clear();  // Regtest mode doesn't have any DNS seeds.
    }

    virtual bool RequireRPCPassword() const { return false; }
    virtual Network NetworkID() const { return CChainParams::REGTEST; }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = &mainParams;

const CChainParams &Params() {
    return *pCurrentParams;
}

void SelectParams(CChainParams::Network network) {
    switch (network) {
        case CChainParams::MAIN:
            pCurrentParams = &mainParams;
            break;
        case CChainParams::TESTNET:
            pCurrentParams = &testNetParams;
            break;
        case CChainParams::REGTEST:
            pCurrentParams = &regTestParams;
            break;
        default:
            assert(false && "Unimplemented network");
            return;
    }
}

bool SelectParamsFromCommandLine() {
    bool fRegTest = GetBoolArg("-regtest", false);
    bool fTestNet = GetBoolArg("-testnet", true);

    if (fTestNet && fRegTest) {
        return false;
    }

    if (fRegTest) {
        SelectParams(CChainParams::REGTEST);
    } else if (fTestNet) {
        SelectParams(CChainParams::TESTNET);
    } else {
        SelectParams(CChainParams::MAIN);
    }
    return true;
}


