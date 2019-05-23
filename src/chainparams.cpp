// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>
#include <consensus/merkle.h>
#include <consensus/consensus.h>

#include <tinyformat.h>
#include <util.h>
#include <utilstrencodings.h>

#include <assert.h>
#include <memory>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "ICanHazKitteh at epoch 1387779684. Meow. Now pet me.";
    const CScript genesisOutputScript = CScript() << ParseHex("040184710fa689ad5023690c80f3a49c8f13f8d45b8c857fbcbc8bc4a8e4d3eb4b10f4d4604fa08dce601aaf0f470216fe1b51850b4acf21b179c45070ac7b03a9") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

const CMessageHeader::MessageStartChars& CChainParams::MessageStart(int nVersion) const
{
    return (nVersion == 0 || nVersion >= NEW_MAGIC_VERSION) ? pchMessageStart : oldPchMessageStart;
}

bool CChainParams::IsValidMessageStart(const CMessageHeader::MessageStartChars pchMessageStartIn) const
{
    return memcmp(oldPchMessageStart, pchMessageStartIn, CMessageHeader::MESSAGE_START_SIZE) == 0 ||
        memcmp(pchMessageStart, pchMessageStartIn, CMessageHeader::MESSAGE_START_SIZE) == 0;
}

bool CChainParams::IsValidMessageStart(const char pchMessageStartIn[CMessageHeader::MESSAGE_START_SIZE]) const
{
    return memcmp(oldPchMessageStart, pchMessageStartIn, CMessageHeader::MESSAGE_START_SIZE) == 0 ||
        memcmp(pchMessageStart, pchMessageStartIn, CMessageHeader::MESSAGE_START_SIZE) == 0;
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 840000;
        consensus.BIP34Height = 710000;
        consensus.BIP34Hash = uint256S("fa09d204a83a768ed5a7c8d441fa62f2043abf420cff1226c7b4329aeb9d51cf");
        consensus.BIP65Height = 918684; // bab3041e8977e0dc3eeff63fe707b92bde1dd449d8efafb248c27c8264cc311a
        consensus.BIP66Height = 811879; // 7aceee012833fa8952f8835d8b1b3ae233cd6ab08fdb27a771d2bd7bdc491894
        consensus.HardForkHeight = 126250;
        consensus.HardFork2Height = 1711675;
        consensus.HardFork3Height = 1838000;

        consensus.HardForkRule1params = {{2630000, 10},
                                         {2730000, 20},
                                         {2780000, 30},
                                         {2800000, 40},
                                         {2820000, 50}};

        consensus.HardForkRule2params = {{2680000, 2},
                                         {2850000, 3},
                                         {3000000, 4}};

        consensus.HardForkRule3params = {{2760000, 1},
                                         {2940000, 2},
                                         {3500000, 3}};

        consensus.HardForkRule2DifficultyPrevBlockCount = 10;
        consensus.HardForkRule2LowerLimitMinBalance = 1000*COIN;
        consensus.HardForkRule2UpperLimitMinBalance = 100000000*COIN;
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 1 * 60 * 60; // KittehCoin: retarget every 1 hours
        consensus.PowTargetSpacingV1 = 30;
        consensus.PowTargetSpacingV2 = 60;
        consensus.PowTargetSpacingV3 = 30;
        consensus.CoinbaseMaturity = COINBASE_MATURITY;
        consensus.CoinbaseMaturity2 = COINBASE_MATURITY2;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 6048; // 75% of 8064
        consensus.nMinerConfirmationWindow = 8064;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1485561600; // January 28, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1517356801; // January 31st, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x000000000000000000000000000000000000000000000000002025852fc35a72h");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x29c8c00e1a5f446a6364a29633d3f1ee16428d87c8d3851a1c570be8170b04c2"); //1259849

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xcf;
        pchMessageStart[2] = 0xb3;
        pchMessageStart[3] = 0xdc;
        oldPchMessageStart[0] = 0xc0;
        oldPchMessageStart[1] = 0xc0;
        oldPchMessageStart[2] = 0xc0;
        oldPchMessageStart[3] = 0xc0;
        nDefaultPort = 22566;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1387779684, 2714385, 0x1e0ffff0, 1, 88 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x984b30fc9bb5e5ff424ad7f4ec1930538a7b14a2d93e58ad7976c23154ea4a76"));
        assert(genesis.hashMerkleRoot == uint256S("0xc2adb964220f170f6c4fe9002f0db19a6f9c9608f6f765ba0629ac3897028de5"));

        // Note that of those with the service bits flag, most only support a subset of possible options
        vSeeds.emplace_back("node01.getlynx.io");
        vSeeds.emplace_back("node02.getlynx.io");
        vSeeds.emplace_back("node03.getlynx.io");
        vSeeds.emplace_back("node04.getlynx.io");
        vSeeds.emplace_back("node05.getlynx.io");
        //vSeeds.emplace_back("node05.getlynx.io", true);
        //vSeeds.emplace_back("node05.getlynx.io", false);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,45);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,22);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,50);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,173);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        bech32_hrp = "ltc";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (     1492, uint256S("0xe1e3c5fd193697408f7dd987f03d114bee961eee1f506717266548002330fa79"))
            (  1000000, uint256S("0x4c4256a1958653092d1cb4b73ba5415abd91312e03eb928fed64a3e835aae77c"))
            (  2000000, uint256S("0x3ef0c9dbf7ef9b18a72b0e1abed2a7fa8f5993e5cb5d42a626a6007916a02a4d"))
            (  2500000, uint256S("0x4c32be723ec9613b5f948ade387f97e5ebe849e199b3f86b0ff9c84df5fe178c"))
            (  2600000, uint256S("0x81106ee5e61a0946b990d280ef171ae626f055edd3ab5d24d4764a26f9d316c2"))
            (  2700000, uint256S("0x25626cfbdf482bfb34f8541fc04d81293df3c9e928e4c771fb14129360d01f66"))
            (  2743566, uint256S("0x4aee30485f74cd96495683cd2c4bbd8a511e55a7ea94265ca4fa6e7245ee51fa"))
        };

        chainTxData = ChainTxData{
            // Data as of block b44bc5ae41d1be67227ba9ad875d7268aa86c965b1d64b47c35be6e8d5c352f4 (height 1155626).
            1387905669, // * UNIX timestamp of last known number of transactions
            1717,       // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            3.0         // * estimated number of transactions per second after that timestamp
        };
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 840000;
        consensus.BIP34Height = 76;
        consensus.BIP34Hash = uint256S("8075c771ed8b495ffd943980a95f702ab34fce3c8c54e379548bda33cc8c0573");
        consensus.BIP65Height = 76; // 8075c771ed8b495ffd943980a95f702ab34fce3c8c54e379548bda33cc8c0573
        consensus.BIP66Height = 76; // 8075c771ed8b495ffd943980a95f702ab34fce3c8c54e379548bda33cc8c0573
        consensus.HardForkHeight = 1;
        consensus.HardFork2Height = 2;
        consensus.HardFork3Height = 3;

        consensus.HardForkRule1params = {{   250, 10},
                                         {   260, 20},
                                         {   270, 30},
                                         {   280, 40},
                                         {   290, 50},
                                         {200000, 60}};

        consensus.HardForkRule2params = {{255, 2},
                                         {310, 3},
                                         {360, 4}};

        consensus.HardForkRule3params = {{265, 1}};

        consensus.HardForkRule2DifficultyPrevBlockCount = 10;
        consensus.HardForkRule2LowerLimitMinBalance = 0.001*COIN;
        consensus.HardForkRule2UpperLimitMinBalance = 100000000*COIN;
        consensus.powLimit = uint256S("000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 1 * 60 * 60; // // KittehCoin: retarget every 1 hours
        consensus.PowTargetSpacingV1 = 30;
        consensus.PowTargetSpacingV2 = 60;
        consensus.PowTargetSpacingV3 = 30;
        consensus.CoinbaseMaturity = TESTNET_COINBASE_MATURITY;
        consensus.CoinbaseMaturity2 = TESTNET_COINBASE_MATURITY;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 60; // nPowTargetTimespan / nPowTargetSpacingV2
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1483228800; // January 1, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1517356801; // January 31st, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00000000000000000000000000000000000000000000000000000000000010000");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x43a16a626ef2ffdbe928f2bc26dcd5475c6a1a04f9542dfc6a0a88e5fcf9bd4c"); //8711

        pchMessageStart[0] = 0xcf;
        pchMessageStart[1] = 0xcf;
        pchMessageStart[2] = 0xcf;
        pchMessageStart[3] = 0xcf;
        oldPchMessageStart[0] = 0xcf;
        oldPchMessageStart[1] = 0xcf;
        oldPchMessageStart[2] = 0xcf;
        oldPchMessageStart[3] = 0xcf;
        nDefaultPort = 44566;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1387779622, 8069, 0x1e0ffff0, 1, 88 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x16a9688c3fc8b4f0fcb0ea7453dbf0de698bc4e45f7bf111cf4cf149505d77ee"));
        assert(genesis.hashMerkleRoot == uint256S("0xc2adb964220f170f6c4fe9002f0db19a6f9c9608f6f765ba0629ac3897028de5"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        //vSeeds.emplace_back("testnet-seed.litecointools.com", true);
        //vSeeds.emplace_back("seed-b.litecoin.loshan.co.uk", true);
        //vSeeds.emplace_back("dnsseed-testnet.thrasher.io", true);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,58);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "tltc";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            {
                {  546, uint256S("0x000000002a936ca763904c3c35fce2f3556c559c0214345d31b1bcebf76acb70")},
                {10000, uint256S("0x162c95555e7313f9720d8437cb56a66e5f2c83122a459b0d4385fb672ff8f551")},
                {20000, uint256S("0x2369794922491928565ca63245ae5c4ddc70be670c30c8dc2910c16899afbc81")},
                {30000, uint256S("0xf3d8b234a540b61dc3c5b0eb46dd0c313e0f1a6b7b3236f2e480398c917ce3de")},
                {40000, uint256S("0x230c2bac6edf2cec84859d6388371e1d15d6c6f9d2e1d81fda09593c0d3242c3")},
                {50000, uint256S("0xd068092e30fe555d3f64e3e8dda2238e1879378e546977257588ca91d3011f8f")},
                {60000, uint256S("0xe85d4cb581b49573bf100ccb37683a8d5ff8a88884bfaf11952b725a70ceb66c")},
                {70000, uint256S("0x6955e2ade029aac81889b68b42249100e6fb4c1082713e3e3be634099325e389")},
                {80000, uint256S("0x58ebe159a221183fe1ca5474d008dd79c16154683ffe0300de5f938dc3b771cd")},
                {90000, uint256S("0x8d9dfa0204b24493141e5b917806a237a7a755b597a138ecc6a7a250bf71ccda")},
                {100000, uint256S("0xe26212281f5b25f2b6d4f384ebfbfdf8e62c0138caf9a7ce66a077470c4af7ca")},
                {110000, uint256S("0xc8e8c2949f3564659dae6e215d47f1c3adaa5020b7516921b13ec1ced51f8800")},
                {120000, uint256S("0x8eff2ee3b6c9342986c832a1813f84f33a879aff8428d027c309b6ed73e87227")},
                {130000, uint256S("0x32c219c5acc4d69873bbe3625defe86a4aea9d01ef1d5914d20d622e9183b551")},
                {140000, uint256S("0x077ae6f6fe488a0239473c2b38a1000f91201aad4cd634119f596f63bf3d6389")},
                {150000, uint256S("0x2dc9a4dcae6394bf7011c5a6a3ed76d12be5bb3a08a491ff42831e2eb3835ada")}
			}
        };

        chainTxData = ChainTxData{
            1369685559,
            37581,
            300
        };

    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.HardForkHeight = -1;
        consensus.HardFork2Height = -1;
        consensus.HardFork3Height = -1;

        consensus.HardForkRule1params = {{35, 2}};
        consensus.HardForkRule2params = {{40, 1}};
        consensus.HardForkRule3params = {{45, 1}};

        consensus.HardForkRule2DifficultyPrevBlockCount = 10;
        consensus.HardForkRule2LowerLimitMinBalance = 1*COIN;
        consensus.HardForkRule2UpperLimitMinBalance = 100000000*COIN;
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 3.5 * 24 * 60 * 60; // two weeks
        consensus.PowTargetSpacingV1 = 60;
        consensus.PowTargetSpacingV2 = 60;
        consensus.PowTargetSpacingV3 = 60;
        consensus.CoinbaseMaturity = REGTEST_COINBASE_MATURITY;
        consensus.CoinbaseMaturity2 = REGTEST_COINBASE_MATURITY;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        oldPchMessageStart[0] = 0xfa;
        oldPchMessageStart[1] = 0xbf;
        oldPchMessageStart[2] = 0xb5;
        oldPchMessageStart[3] = 0xda;
        nDefaultPort = 19444;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1296688608, 0, 0x207fffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x7b1821f586316703643a5ffbf7a6694c8ffa891390888559f854ca5b23e3d05c"));
        assert(genesis.hashMerkleRoot == uint256S("0xe17e4369f534691fade36848437428efdd6c51141b504aca65568ae564f171bf"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = {
            {
                {0, uint256S("530827f38f93b43ed12af0b3ad25a288dc02ed74d6d7857862df51fc56c416f9")},
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,58);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "rltc";
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}
