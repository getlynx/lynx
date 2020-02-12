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

        consensus.WhiteAddress = "KQoKm4bzQvDAwiiFsPz3AE4UJHkHBvX6Bz";
        consensus.BlackAdresses = {{2820000, "KJ2MGS3jq4DPkVmE1ephMCbT7ojDcDSJRG", 1000000000 * COIN, 1000000 * COIN, COIN / 10},
                                   {2820000, "KSho9zUYrFdTPPxfF6ye9sLurgKygeUEzL", 1000000000 * COIN, 1000000 * COIN, COIN / 10}};

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
            (  100000, uint256S("0x85e878aa560cfc91a0cd15770377231d9a6617c718658ec64fdfaf50055ca72d"))
            (  200000, uint256S("0x42e8f8d9875223a2b35dfb52e1a608a558cc69d8b7c13d9c6b5ee6ff2f69f19b"))
            (  300000, uint256S("0xf91e23230390e9b876358b8fb079cfff6fb0a5401df421e6b73db99d03fcf765"))
            (  400000, uint256S("0x96df6ecb127acdb80115bd23cc979aeb524a3a9579af7a9c8541344ceace684c"))
            (  500000, uint256S("0x5b28ee0fb36e012ec2c1c9dad371479f1629e4cd7aff325829377938fcc4ef8a"))
            (  600000, uint256S("0x6fa92a212f5d42a774e42954a5ab0da8777441476a0625eb4e8f0a48c636f4f6"))
            (  700000, uint256S("0x354eef8293ad4ab5d0ae6069b0c755ca903b12830247fa0e49f2c595e53fa27d"))
            (  800000, uint256S("0xa940b11b5addc5dbd2eec0aa597c98920aa5e43e28936b32d6758655745c302e"))
            (  900000, uint256S("0xe15703726c220a58d29c1f3b28612b6b2c7b97a53a2dfaa89aef757a1669bb09"))
            (  1000000, uint256S("0x4c4256a1958653092d1cb4b73ba5415abd91312e03eb928fed64a3e835aae77c"))
            (  1050000, uint256S("0xdfedf36f30a21e9f8fc6cfd5581c25116b610530af2b59ffb8a17531290a3c65"))
            (  1100000, uint256S("0xf502adf14b5c66d1a28e73ba34e02320ba405fbb00040d7ca287ec79d4678ce5"))
            (  1150000, uint256S("0x532b05e1d56c4293999d46ba90fb257c31ad4c76beaa6a096a56f9c288eee206"))
            (  1200000, uint256S("0x8bfc473dd201ed0421c0a275477aa20979e416bdb8571a9c5c7a8b910a354498"))
            (  1250000, uint256S("0xc776aafde099b60ce8818da3ff35076cd3898907bb5416cad296f9644e43da3f"))
            (  1300000, uint256S("0xc13472382e8fc2516219d1ebfd6d32498bebde6aba2218cb1c37bb1e2597fc0e"))
            (  1350000, uint256S("0x6524b59dff3c870f154890314fc8c8786a142387dd3a66d6ac75d06b85ddd230"))
            (  1400000, uint256S("0xebf7fef90f1527e126f2d435059c4246f5da59bba9e7432baf67ead9ef2d8753"))
            (  1450000, uint256S("0xec2066d34c2b39fecce2e48cc349a2924a456ec262d5ff2be1ee4f42105e88df"))
            (  1500000, uint256S("0xf56d06f7024fad702ef37a462a8124c4176022b173c3fadb83e8ba5a52e5ee71"))
            (  1550000, uint256S("0x7d7a9d7713840a7268aa03191c9ff25d72e411d85ea371a89dccda959d3a6a35"))
            (  1600000, uint256S("0x92e08000d86cd2363ea738fe85edd01178b23c7f1c7f0777566ed291865c24fb"))
            (  1650000, uint256S("0x61877bd8134f10b24f41115fde0867e4dc0c5455b454f777c89cf43dcde1357e"))
            (  1700000, uint256S("0x34b423a67b364c5ca8a1ae89bab3519615c6eb00c0100a1e5f61731ac9c8e742"))
            (  1750000, uint256S("0x1161c21acedb02a0c1acfb8a8cadf1fb56ed0e4cc89ebbe8d75846e5902419a4"))
            (  1800000, uint256S("0x3bc5f6c051d62bf089255ebbcbff145cff420c4517e3bf19a84ade9e94bbb078"))
            (  1850000, uint256S("0x4092e2e333bffbb11c52fcb51e348b9762f18c40166431f1b2fe9047ee05edd3"))
            (  1900000, uint256S("0xb4b596be3597626eeb34b2af5397a7b27fe847338d9033e5543837dcdfbc2c70"))
            (  1950000, uint256S("0xcfa104bbda324ea1d81c5f752bb18aa0e1fe04216ec9d6e3bbafc29d784ad7a1"))
            (  2000000, uint256S("0x3ef0c9dbf7ef9b18a72b0e1abed2a7fa8f5993e5cb5d42a626a6007916a02a4d"))
            (  2025000, uint256S("0xfaff3dcc9eab0cfcfeae1eef4fb7634ffe0245746a4380d07d6dc86e2d3f79ea"))
            (  2050000, uint256S("0xf8a0a4eef648811b7d554aec67a12b7e3a9e0a036fea301f986166bafeb173b9"))
            (  2075000, uint256S("0xc43c3ccdede4d6d505bd867d629685166f64f57a9f9b89fbe1939632371c8f2a"))
            (  2100000, uint256S("0x0c44cee6464380768d7b5f256832b09119a00fa9183cfcef782093159b5be699"))
            (  2125000, uint256S("0x8990357a9b6a7b6f9349e105bd2945117a7a610743d8a59fb00d09b9de5c0204"))
            (  2150000, uint256S("0xd872ff24da589c00b2ba9a743456bc5b78b70a1f2b871ece75e9b68ad8a13f90"))
            (  2175000, uint256S("0x0ad6111af1d65fa9c078e0baa1a9a72e77c3dbb0bdb250a9854379d20b47b71b"))
            (  2200000, uint256S("0x84d1b4a482d04f674eeb0b3f211cb014a8f9c5059ea022a3d9740206ea5958f7"))
            (  2225000, uint256S("0x0a581b2a143a990e451a977613aa8bd2b86fba064bc04331a19b31c43f2d8b0f"))
            (  2250000, uint256S("0x497d176388eb21751af8b45680b470a7a87c3cedb6239cdb2a8f9e46d3e23507"))
            (  2275000, uint256S("0x0ee97d2dcabeff567c7c9d92703d1d41d6b73d21ef4664dd2275523ce72ab66e"))
            (  2300000, uint256S("0xb494abe7ac5a45f85a919174f80af372d1d7a3ec03152a648b8065028093fcf8"))
            (  2325000, uint256S("0x6a6bda84ded49dfca2648c0fb47c48692f9835e61d1aea745a3beb359c56955e"))
            (  2350000, uint256S("0x8e4d5093d41f72d772fb5b7e504601bc5561d535e618cee76122cc405510b411"))
            (  2375000, uint256S("0x21967b9371985c70f35a7e52e3d51d630bcce1ff6e5e260116feb53bd7228cc7"))
            (  2400000, uint256S("0x1c2c5a44612f0d9926f7282f13f554dfbca6314bd4decf60a440ffd4df0b96da"))
            (  2425000, uint256S("0xdfde1b47bae2f8cb0bddf138da5ac022ab7b5e2ab22590c4239e8e24b521fe90"))
            (  2450000, uint256S("0xc2a574cc0ce948c939adfd31732781212ad0a2e6763ee52d0d849728ddcbbb73"))
            (  2475000, uint256S("0x31a24fd6bcbc0a5d070139e2b82ca38e065761b30f92d0052db583df10885bbf"))
            (  2500000, uint256S("0x4c32be723ec9613b5f948ade387f97e5ebe849e199b3f86b0ff9c84df5fe178c"))
            (  2512500, uint256S("0x01fe591dba0021ede37de5db62e7bb7d29c14e0a8001c30b4f11fb79c25b4174"))
            (  2525000, uint256S("0x77f4522e0d94731e4413c0b45b2aadbec4912dd8512034e95f2637f6b0c23b8d"))
            (  2537500, uint256S("0xb6b9f3068f610eb681e18eca2058951507794e0ae8bc857d2c2ac3fc0e6adc40"))
            (  2550000, uint256S("0x07dbba1012a6707d99c19d253c92b807413a1bc30b71c631f463d379949ae55c"))
            (  2562500, uint256S("0x5614087ba0bfc76440315a6a651727fc15b96418039113bdae2b3002d37b2246"))
            (  2575000, uint256S("0xf6584effb1c79f547e59778b4fe4b584c4b7476e3973333a64ef6a0336ac9a68"))
            (  2587500, uint256S("0xebd302766dc770ffbb37fd176a50235608b7839b7164f795f2de24e8f0d1174f"))
            (  2600000, uint256S("0x81106ee5e61a0946b990d280ef171ae626f055edd3ab5d24d4764a26f9d316c2"))
            (  2612500, uint256S("0x415626e44b3210d4bdbda73e85dfa609a82a78e2c2dbf9ef5ed4c2574932fd26"))
            (  2625000, uint256S("0xf21fb18808d6f69d19f6ea4dad4ea66d96f79843c5605904b221c4f44268e98f"))
            (  2637500, uint256S("0xd5c58920ca04cf68ad018775b269c6425ea8c678f6c5d4d9d1d14f524191802d"))
            (  2650000, uint256S("0x9b2ada037f176981c69b1e03bbf407b71904dc97d9e31c1f91a3183541e87acc"))
            (  2662500, uint256S("0xaf4dac65a2be36cb62b943da32f78cc103d894b2faab776d8a04100cbef35cb7"))
            (  2675000, uint256S("0xe686f2126ec138f379d7ca7acedb86e0cf49953de89c8b534bca095a16a8c44b"))
            (  2687500, uint256S("0xd7209eb4f14fb8db9ea9dd1a880eef956701603c2e9261aead3a8dbd4e042384"))
            (  2700000, uint256S("0x25626cfbdf482bfb34f8541fc04d81293df3c9e928e4c771fb14129360d01f66"))
            (  2712500, uint256S("0xa99465d4019c2e5abbe808785c04b3b88dc66b7f1d2bf06ed77f565ab481ea6f"))
            (  2725000, uint256S("0x66a7fbb4e9232d00f8b65667af96519a1f4ef5505c4a78e1297e5f15ab977c5d"))
            (  2728000, uint256S("0xeae8e7061dc7c3dcb382e18f846b23fba9e3bf5a482c71a528bd601786c484e0"))
            (  2731000, uint256S("0x93a672f0e7e2b3553c4e44d3ce64066b65f0f70612ef12462fe3311f5049928f"))
            (  2734000, uint256S("0x4e36de66c78fa93b9358223f9b4983b934c50c7f141c77cdea4c796fa9e53d12"))
            (  2737000, uint256S("0x2551aedaaab7c876b61e9de5da64d51ad3439e44fb8c95d537ebbff676ec2077"))
            (  2740000, uint256S("0xe69bb4623be22f9d30efad5598f0c6062ac90af6b5403e28f4c6faa35a730c3d"))
            (  2743000, uint256S("0x7e88301aa219e32cfb4fd0bff54de854bcd1d6642b914c8259030723016eeacc"))
            (  2746000, uint256S("0xb433c172599231aa3a7883b0660b71c251600ed2e13b58e14d2ff5283cdcd7c1"))
            (  2749000, uint256S("0x865221b065bd479d6ad7c46b7c1bcf637bf6e993d41a664c525e9af61c7116f4"))
            (  2752000, uint256S("0xebfe078ca6c159efdeb51fec1c9fb8c8b29d020965f34b58d0fdcbe63e8a5961"))
            (  2755000, uint256S("0x42b1f386ed54ac21eba319f8ed035501a9ad82a5173726475d6ccc17cac20fee"))
            (  2758000, uint256S("0x087e54b9d76a8ff17b7bc54a276289624f40d111bdb408a52ad2fb6b202e25b9"))
            (  2761000, uint256S("0xcc72f20d2b9f7c807736ae092bb63ebcb1bb473a9bb459f125f33cca7093bea2"))
            (  2764000, uint256S("0x3749317f3b391fb5ef8c17d387276b30b1a2ce0b7aa1fed0216dedb534c9d1c4"))
            (  2767000, uint256S("0x115ba182f644fea8ebdb97062b94ebeeee4be08a79fba1139edb3eadcde8c050"))
            (  2770000, uint256S("0xe3219810a440b3d6eb4e7b846dce4cc4ff7ae02e77882726274987541f68dc6f"))
            (  2773000, uint256S("0x6e34b0e4ab03a5363559b820fae40275cdeb1d96a45071d0b124323ba401e608"))
            (  2776000, uint256S("0x81f3974871339071254f8caf6e810d6935dad41a2ba6428734046f786a48331f"))
            (  2779000, uint256S("0xe8d50f1bf0985068f0cd3c8878da919b40232eb747905b868f7d4b30fa057881"))
            (  2782000, uint256S("0x536da0c57bb062822d62eec9218dc2ab6745a9316c66bb7ca11dbbc44ce0276f"))
            (  2785000, uint256S("0x1d040a81763084cc1933c67504ed210ed2af06de720093fca9bd7d8a0f2ea851"))
            (  2788000, uint256S("0xc842848219a268d3d9095fd816fff97c262e9270436613fdc58aebf2559308e9"))
            (  2791000, uint256S("0x4fdb17d2e9179940fe067b62eea938b4a8eb4985eb3d2d7b8d6efaa646909c6f"))
            (  2794000, uint256S("0xdf3677b757d84bfcdcb4c6169164afe9b088a7af4ed28d62a71162841b7cafee"))
            (  2797000, uint256S("0x52a40ad12e7d78a7eb501fbf96fdad785d2494caa0d7e7ba9429a13851cc2ecb"))
            (  2800000, uint256S("0xfff37136f2a57dfd4d3786b40b34f72544a32552e8d3582268e35d04b5d4ca24"))
            (  2803000, uint256S("0x95cc2cfe2494c9460d96c3891bcc4b16ee3dffca892be74301e730c88c985c63"))
            (  2806000, uint256S("0x459aad57e8fbaba007a0b382c947bcc9172647443482d48450d8259dd647fd1e"))
            (  2809000, uint256S("0x92c994172beae9953e490d82e13d79a4871d0cfa91bc2ae9cdc207bb5bbca98f"))
            (  2812000, uint256S("0xca224c0a3a502671c125976ae8ff063b0572605ce920121f6c77472ba6fad92f"))
            (  2815000, uint256S("0x992d8ad2aa54be895a181c51252bcefe06a948881cf9aecf90aa81d83113812f"))
            (  2818000, uint256S("0x054af4154a4006061c5b5d7c5cfd4a4b0910f0c072187decd2ad13e9ec4516cf"))
            (  2821000, uint256S("0xed66c0ffe839e97d78abed4b7614b659bb77a3f448f06480a3321b66d44fd193"))
            (  2824000, uint256S("0xd7b7ab143a3a703c79eeb1b1cc610b375cf04ded73dd1a8b6b57d65e34a8a371"))
            (  2827000, uint256S("0x6849d300b953fd1dbbd767fc680a10809d34e5b65f7d1dcd7e33400126d65c06"))
            (  2830000, uint256S("0x9c837816ad049660825c02bbc19912b446edd1ac2b7e58e41a60eb7a439f849a"))
            (  2833000, uint256S("0x5a7fff723d0cc0de07bdecbe06bb19900eb6ce8d7b59a7e76f51f96c54b151a2"))
            (  2836000, uint256S("0xf765dcef096a7b14ef4c2b809c5510dcc49d0511bc7a0f3c58244e2b4c150451"))
            (  2839000, uint256S("0x77538fc402df2cc21a2a2372445884e623b43c572f2a5f61fc129721b2c24ea8"))
            (  2842000, uint256S("0x0df013f1c953b5c124eeaf80610876499b728bb099ca0b9a643af59bb5870074"))
            (  2845000, uint256S("0xe4959169e2122b2fff442ffc38388761e20e5892daf161ac691a58265b4fa673"))
            (  2848000, uint256S("0xd23c5247894aac050d1ae5f1606886b6eb598b4850e0a5aacdc118424dcc3be9"))
            (  2851000, uint256S("0xd037be587de58dba7fe1c5585763c143decca75834b31cce5ff0f278f1cc8427"))
            (  2854000, uint256S("0x14df5bede615958eb1632191b711b8a4cd9a4af44c1d4bd2f4d2894d986331b5"))
            (  2857000, uint256S("0x10a809f4d7ee35370b67a8ae7c572bdcc47c5af9fa2ba1bcbfd5c6a8ab98f6ec"))
            (  2860000, uint256S("0xc19c671605c6ea8794ad41b04b96d2b0da95e6b9a9eec6cb8361c56af6b58a75"))
            (  2863000, uint256S("0xe4235fb3c98713ccdeadfa50571c5f39e35fb90976f91f66a1b2680667e8d6be"))
            (  2866000, uint256S("0x9b82f031d26c9d52bd5d29420301ba0759cf6a86d82e435b1ccfb4cf6715e951"))
            (  2869000, uint256S("0x78992b305933a358723bb01ab2eb91cee550dc32954185614340a4d84a15b971"))
            (  2882000, uint256S("0x3263a095e01e64ede81dabed3506948fbcb7d300a1d3eb0614b38d0b9266027b"))
            (  2882100, uint256S("0xad70d6c0387c932290fbbb7fd7d00fb5ba7f7af8575e437755bcf8e1332fbb52"))
            (  2882200, uint256S("0xc87b0cdebc8bf1d065e219215ef9b8f1d465ca860f8aec9e9a7454ea232aff05"))
            (  2882300, uint256S("0x0451e328f773fb38a875d170e5658d1e4df012495906be8f477d261eeab13060"))
            (  2882400, uint256S("0x8f00ea3f7a0868b37d7829b3db2fbee03ab583f741a026458c8a7dbe969177e0"))
            (  2882500, uint256S("0x608a8ac136c2148f256c0ba67e35f2575427ea9cc1b3f4b8b2715a92c73a112d"))
            (  2882600, uint256S("0xc0fb8e20d5675cbce76611e062fb9e13e33b726197d3670ce2c1a0a2877dba1f"))
            (  2882700, uint256S("0x5224f5597b33bc2e8e6d62b95ddb4d19b60fa1fa42b52c472a5c213fee6cc268"))
            (  2882800, uint256S("0x7ea66ddfbef7e6dde715bfdb30632e8b7d2cca60edcd95bd83f38251573a54c9"))
            (  2882900, uint256S("0x47d3b7479a198d4baddfa3dfa991f7d9155f46b8afb457dc2179c721fb4dd302"))
            (  2883000, uint256S("0xefe3b67ed68bab8bc56a914777ede57abb9189ebc2623ff865ce4f9964eb1e48"))
            (  2883100, uint256S("0xc0296049c067a770d2a01dd37c5b5d7956fe72e888374b099863d130cc0717df"))
            (  2883200, uint256S("0xcfae074aff55c5eb1a8d37e76565db0e8fe96ac63fee60ed46b03ceda6aac82d"))
            (  2883300, uint256S("0xa3dab2a4cb9bad2bb69a86d186c2e4c98888629316c5b39d039f2fe6139d7142"))
            (  2883400, uint256S("0x58c753525db9c7a6453768d1bbc42de55631e56da5b30e77efc5f52daed94278"))
            (  2883500, uint256S("0x08314cffe80028bb910326ce7d89e172d8ae8b92f39eb05b9e4f30db0e256b4d"))
            (  2883600, uint256S("0xabfe7de3ef5837db2b74cff66f2a0813567061078f98de57413a7301153e9dda"))
            (  2883700, uint256S("0x70ebae1775f3859421628cfe3dc3a5b7ff06c3afa787cd1b95089b08ba38cbe3"))
            (  2883800, uint256S("0xf1e2bd70df4f69f7fff7d14656a73a1113696b348456911ee5b2a0b9359111d9"))
            (  2883900, uint256S("0xdf830c8cce0c4f7c302a85e198e24cc874814abeaf51f2a1ece143adcddcbdef"))
            (  2884000, uint256S("0x27039bc723e0e83bf089fbf40405af3ac102970e859109fabc03802939c3e6d9"))
            (  2884100, uint256S("0x3115888c11fcfc16472d4829503a644e768899d2e3131a02aef7d3b704708997"))
            (  2884200, uint256S("0x3cd84728217f24a83630f774d22621f25c7ab6700980cd4102a30e7da388407f"))
            (  2884300, uint256S("0x5efe09190bda7cfa2db9eb4b887234df0ae4a81851fc6eaff91a01d9efbb6042"))
            (  2884400, uint256S("0xe23a3be19febf16abf8fd31a87c0d9aa57e95c801289a39ecd1242a2ff6d22ac"))
            (  2884500, uint256S("0x7d0626d1bca7ee64ae3a33076b959c9789fe7a8bf4425205695ac35e2b0de20a"))
            (  2884600, uint256S("0xa3c561d917acd05c253dce6992139c7c1ef8f75db81ca48b74742907a285997f"))
            (  2884700, uint256S("0xafa5cb49caae064cf58038f340e52a5fe9bc228cbdcc9ec202158f9eee63a825"))
            (  2884800, uint256S("0x15c2ef7bfe6ccfc21d08600bcb9b796825cfe4ce1f6d091758201bdae44f45bf"))
            (  2884900, uint256S("0x129ae9a018bdac64d137c01b16500947d0e42f40ac6e4fb5462736c722c9f461"))
            (  2885000, uint256S("0x1acb903b5619e6f131ac0a3f62b725e8fdfd62abc74096984517b1e9a70ac993"))
            (  2885100, uint256S("0xb1d21c0d5b810e485b56ab07223a22a693320e2407a87146f9a17d36ee24141d"))
            (  2885200, uint256S("0x3cc3f3239fe2665c7ee11d867fb5d78e596a40e89b7ae0aeaca692e34fe26b37"))
            (  2885300, uint256S("0xafdbdc5cdf332652d369adf489ce5389a56ceec132723b972493872d4c65f012"))
            (  2885400, uint256S("0x75c87b54e6c97cb3199fc3a72154264c39ae8debfb3ff796dfa3c2a787099493"))
            (  2885500, uint256S("0x4785681ef0f41f4c65c1852a7d880b2045719711cbf006984505f816565a011f"))
            (  2885600, uint256S("0xdb492c2f7409dfffa4fcbeef72cefd6113e0c107c6a39ff8ceb56ca60a1ae2a3"))
            (  2885700, uint256S("0x4462b02582402589355fac344dbeec1d50d91d7ca7fccf3148e43acbfcb829d7"))
            (  2885800, uint256S("0x4fbf17e25dda699f2cdf402006d71f37f20aa4568c733ebd9801d2f7197a7996"))
            (  2885900, uint256S("0xd59035f5b74be5d37a94808b7e59c7d5998554315c59aaaa7b64ad982b95d4b3"))
            (  2886000, uint256S("0xe228fc2135fe412972d17b9936f33c43fc8c2d7527b3f7622d925aaf59c18768"))
            (  2886100, uint256S("0xf5049c3ede390a5c0ecc2ef46f96529ba750d75bea990bf341fba4c35237d66b"))
            (  2886200, uint256S("0x3daa7b76c999939d2ffb34ae634ce41cf68cec73d3d4e68a5eb22ff289bd59c4"))
            (  2886300, uint256S("0x5d19dac51419800180de6af251fa52175c1ab84f0d40e896cc5ec1b40ece5a1d"))
            (  2886400, uint256S("0xfd88c3e02357f7d86ed289e983a57cfb25dff9c6d324047913031ca001e23a98"))
            (  2886500, uint256S("0x4f90591194fc9652ad121f7f7b6d2d4bd0fe65b496dcbcb42c33270cf3b70098"))
            (  2886600, uint256S("0x129bc0602dd8f68f2952782ffdaaa31694c8c7e11993b6cd6f2e744ce48a5b20"))
            (  2886700, uint256S("0x1f8e27a54df7eef21126a52556fa22cf2aed97f4b7647f57cb6c6226c457918d"))
            (  2886800, uint256S("0x3841179721e1bbdb97fda5ce1b5d18bb0adf4c4db30a384e7a58e4453efe0276"))
            (  2886900, uint256S("0x60bdf587da41f0dfa2de17c3947665d4a36af24d1448e2382ab806a1005ad156"))
            (  2887000, uint256S("0x715041d183b022554b4052e741c3b9d466ac7b281f8f873d880dc94c990bf39f"))
            (  2887100, uint256S("0xa5d54ddc64185a7f7011fb0040049d51f9ce1628b8bce621ecce254c391c7233"))
            (  2887200, uint256S("0xd64a79db075482672e83fcc8ef97eb94f3eda3ae892b9f7893552c171b12f5d4"))
            (  2887300, uint256S("0x50f738ca23768fb9ee8cbf3a10fcb34ed304c319147052f6f07fe7df8c0dc488"))
            (  2887400, uint256S("0xb6a86693dd98d6ab87d8db553e0540a9d891a0b9089566592da25e6a53159eab"))
            (  2887500, uint256S("0x867ddbee783bbc109b0f4968928c3f4b123f5e87b7df4b0d19b9b811f0e2d6c8"))
            (  2887600, uint256S("0xcb2b065733233c86adfa085953222c50fd172644879e899fd35f1f26206e39aa"))
            (  2887700, uint256S("0x6c356c75ab86cef1558ee8875613215de18d0a62cab5db05e9af63991a32621f"))
            (  2887800, uint256S("0xf92eab5158e8756239f019850fb9e04f2505754c6e81d2a0931754cfe92b03ef"))
            (  2887900, uint256S("0xed3387bf56a58c9d10708a2da37a377ceacc7bd8ed80584c4c9a99e6df6d5d6f"))
            (  2888000, uint256S("0xf06a60ecc3a8f73e5ad9a8d79bdefff81ff0cb91cc941868be068a80ec5e1c3c"))
            (  2888100, uint256S("0xcd4f8904ef1c473b831c0d3d2674386615245e554ee4ec54e777b5e659f9d80b"))
            (  2888200, uint256S("0xa552598db59c3203c94d506293d7ca26c69916b7b832f45b66533d105f5b8829"))
            (  2888300, uint256S("0x9aa7e1f00178212f3e2555299d25a855e8582a01d762598a32e1300d39131f6b"))
            (  2888400, uint256S("0xae98a539d4b102b316e7ccdd8a79cd93d0ce718e5fa3f6f975eeadb4cb0a8aa6"))
            (  2888500, uint256S("0xcdaf2fb0a960163f568b2e5dcc4f0097301a686ac6810425cc7e1f0864b0a795"))
            (  2888600, uint256S("0xa2f40ac637eb81e5e2d2238189636a16a86b727ca6a732514ec9dd5ba6699800"))
            (  2888700, uint256S("0x90e50ba4c721296c247a75bb45f3119d6219aa047ce5e4b57515eb3bd0bdade8"))
            (  2888800, uint256S("0xa71c7dfcef19d1d50acf95bc060929624e96b5201b29a7ca7adc4be0425f979b"))
            (  2888900, uint256S("0x468a898995d8abb2b15e0b7ad440dea3e41ad7e4977c10302cadc5c5f676a917"))
            (  2889000, uint256S("0x4e0dabf9c43c9422af120f545a0a907acc88c4c74e60e1ebdbcde24ba02d7a2d"))
            (  2889100, uint256S("0x466e60f4b3ab304b0e93118f525677d7edbdf354fdbaf304082d6158641ed51f"))
            (  2889200, uint256S("0x38c9a1eff6828145ec0f89a4f598f6703142e3a4b24fa4c144cac9413f327e73"))
            (  2889300, uint256S("0xf51eab042e9abc5b6ececead72c8969be6d8edf5100c6310e9c536f345bf8880"))
            (  2889400, uint256S("0xf3803eef6d7b67b9b7964ec481f4c6b5135af439bb68a7de95d1aca3c563f465"))
            (  2889500, uint256S("0xcca806a5508b8675991da9f7cf772263751bf398d8b0b5812c6d080445381bad"))
            (  2889600, uint256S("0x18f52fe71f8c627eaf2c06d61a34895e8ee0716383652e52e92c82bad6f7a7c3"))
            (  2889700, uint256S("0x80ba10b7a066689587f2360ae1976951bc10ffe5f3d95f739c145ad6db07568d"))
            (  2889800, uint256S("0x4bc5bfaf9701e8b6490ed948715ca6fd1f19745fa31c2117e95f38911c28c6e6"))
            (  2889900, uint256S("0x8ac4f24d140dcf3ca12007cb19371c06e9828c46564ba945cd8bf4b109c589d3"))
            (  2890000, uint256S("0x8911aa13caba75fcd73e6f6a9a34265a3283c9aa45937226e0311f00c7761d82"))
            (  2890100, uint256S("0x9b0c1f1a348a0d6dc8612fad1bbdf774c3ca9be0b7ad866796c6810128addb22"))
            (  2890200, uint256S("0x58ea17346a99722202abcbdcfe1074755ab603bab1d6ec920460bb17ee4bddde"))
            (  2890300, uint256S("0x466281fb1c2566d53b21fb4c11afc9c042e3f88b5ce1f07d8cca98d53239e96f"))
            (  2890400, uint256S("0x89612bb29d3f6dff7913efdac64379359457ff57514a06a0123188b940435cef"))
            (  2890500, uint256S("0xa3434d7daa0167bc9f712cfc1713b9444edc39605ae3e4406d03c09f7ff5d5d3"))
            (  2890600, uint256S("0xdd4af0e33e4562f9633d9219ea043d13d053eec259253478c6da279ab78b57c2"))
            (  2890700, uint256S("0x21f63feb1fcdd8ff5bdee9724f66b0a78998342832acb5c130ce6e93bc93d3cd"))
            (  2890800, uint256S("0x6eb4acaa8ebf8644b02be40c66433b6ca054571f09c98a3b9243ad14905b8413"))
            (  2890900, uint256S("0x8965da03c6b5a5e31da33b5cc7edab439deeea8f63d4f4bb48464667f86fb86f"))
            (  2891000, uint256S("0x97f586c3a77307fbbfc3255049d80566e769289dbcc7f05adf702bfe1009990a"))
            (  2891100, uint256S("0xdf000bed805ea82492e6635140a3734c39594ee1ab73282641a54624f9307e86"))
            (  2891200, uint256S("0xeb9769e8f77d9ba530c773c51a64fe0a467ef8ffd0e5d666ecfd1494403adc3f"))
            (  2891300, uint256S("0x5622e972c54d0b9845c32254e1e7923133f2661f3c328e7b7611f40cbd2bfa9b"))
            (  2891400, uint256S("0x3581dd27e7181229538de5b0e0e1085f64ec84073169d01964481c227e17a45a"))
            (  2891500, uint256S("0x14af258b55878eb5b3409214bd517c515ae228d8b4ad3f81944ccbf5e8cd6701"))
            (  2891600, uint256S("0x6f64d5cead706a92d9e87de54730af971aa9fabd4f0b636e89ee0fd5dac4a5f0"))
            (  2891700, uint256S("0xa55d815dde904a6acc5a6b1a3d27c0a5c8636d557f8bb620c43ac7d8c0220bdd"))
            (  2891800, uint256S("0x4070429fd68601d78705d3557a5e71719e05d91338c2d1a71cc835cb57de9193"))
            (  2891900, uint256S("0xc2c0a5bbd4558c293a6537919e329d62b402969ed8d9b4570fd3a2da2aa18b02"))
            (  2892000, uint256S("0x8d66b35bb66ea853c89692e4c82b60a6416243fee258c10ab428f5e565a38a64"))
            (  2892100, uint256S("0x79dc3de604c552d41be926aa20adc329c8fbbb03a4fea06398cd3ac111f51c1c"))
            (  2892200, uint256S("0x3c6c6c659f6e72b1787cb52e3199fd124f134422a58c6eaae3b7aafcece9cc7a"))
            (  2892300, uint256S("0xc27d6de0b59652e446068252d6daadfe6c3c9eef0fdafe56ac4b85a4dfbbaa43"))
            (  2892400, uint256S("0xbacad5d49bc28f71a1b9f631e9a213a30b465bb6a6106256468fc9cad4ce0769"))
            (  2892500, uint256S("0x255b26db1f18df2f700303d4c05ba0c88188190606a43656a27fdcf353e69b9f"))
            (  2892600, uint256S("0x359f5bc9fc7d05cfa596acf0feb854c129e21ba5aaf85375ac1822eb61713271"))
            (  2892700, uint256S("0x49470b32305fabf7914e9a7121fce1a176efa01262df3c43dc3be80a7d9c0659"))
            (  2892800, uint256S("0x44c1f8b08c9033dabe2f11ecc37ce23a802393402e862cfb66e6f6e18403ddbb"))
            (  2892900, uint256S("0xe5609e68d44a6b7b6ee7d68089afa74407c7cfd95978bc7325bbae0b5059ec75"))
            (  2893000, uint256S("0x18218ac9370a400ac369a51dac9d72ef7820be7559798d840359c8f77fb8743d"))
            (  2893100, uint256S("0xfbf937b46f0086ab0dec37b326fd357a7a1c2a6a2cda4a34b21d3c355ce6ae7b"))
            (  2893200, uint256S("0x0fd642277a4bc30bac3e34a22c530705f0df9b346cb25c6b4153b20f8c55d8c5"))
            (  2893300, uint256S("0x1012d32166c123d48348e3eb15f876abf7e54194167d611d6da9ff3daae249ef"))
            (  2893400, uint256S("0xaa59764f08289dcfa06c047af245cd713dfa55abed0efb9f8508335afc8c0565"))
            (  2893500, uint256S("0x7ace3a6c50ae23eef71c7dcc9fe855eea3a32af33e5f88fb1d9caab3f4a30053"))
            (  2893600, uint256S("0xe09b58496435e1db9a7515a6560ed7dce475906c9e1feca52e3673d674fdd2d5"))
            (  2893700, uint256S("0x0b1c24c1d27569111038af147447f285f0b326f1149c87625f605ecb443623fd"))
            (  2893800, uint256S("0x4f666396508493f64ba8f4742e319c11cf783d8799bcae7a522f4fc314f3aefd"))
            (  2893900, uint256S("0xe49c128fa150a9c8088a8fb79d33f67dfec35c8cb319284db96aa106c0de61dc"))
            (  2894000, uint256S("0x0625089e22cdeab31d3ccee0c46726d28bcd9c7216f93feb1e31f6ccb3f796ae"))
            (  2894100, uint256S("0xf8814c970f89620eb859a57f3a7f6812990c9ce0c516dc84cadedd449b26df55"))
            (  2894200, uint256S("0x73842300ef5fe5ee3c4a2d856e4377134d73535db904c130bfbe9447b0c10d04"))
            (  2894300, uint256S("0x96106a8ef384be674d6a1f6b9d62a6ace22c92b65b4eadec34548deb2796f283"))
            (  2894400, uint256S("0x5a42b05d3890b17a02a78ffb99f74029ec5b00b1f4d99d4740d30f3274a0a482"))
            (  2894500, uint256S("0xbfbd08f6ab430400f01ac4bcab141e3b0633d87bc7142c54bb3044a36ebfd461"))
            (  2894600, uint256S("0x82696980267dd0ba0a51a96e5637dfbc3763d0b279ec36d8c551653a90e65969"))
            (  2894700, uint256S("0xa6d2fb6463f5b5a4f9b4d01c21d1c752772cb8aad1ced738945d3adf57aa74eb"))
            (  2894800, uint256S("0x67c921a959f8585d97e7a4301039535c7babe112e9aa75c9dfe2e341079c6afe"))
            (  2894900, uint256S("0x5abd5993c5172c818a0b4fd486eedef3bea4e9229fc1d39436b44207f588bafe"))
            (  2895000, uint256S("0x78be1f02be5a2240b1e44e5990e03e017d95bf9e35122eb5637e793736ee476a"))
            (  2895100, uint256S("0xac4a0331c1c3ad766859bc441cb0d5a5c2b129d782b4f3f35d60444b4354f29a"))
            (  2895200, uint256S("0x6c240cca0b90b8224542c4a78b2215d9f269116c7fa236d7dcacc9fff224c3f0"))
            (  2895300, uint256S("0xef7413ebe34b1cde156120a324e0547b3807ffdeff5e2cdbae1aab595e34a685"))
            (  2895400, uint256S("0x261e57c1b3339e9f2a6450883f88e66fabfa9f0cb3d908dc297f68167973f07f"))
            (  2895500, uint256S("0xe13754f5fe36c3c549223c2ec468f33834844e656cb4e57820f78b799d341388"))
            (  2895600, uint256S("0x6eee27f5f8fcf0edd0e08746e9880f57d74aec93bc83112f11b15703f99f69f0"))
            (  2895700, uint256S("0x05129a59a4c845bfca6709207dc3443d4b6c21329bf5d36a6acbbbdef2f64bb3"))
            (  2895800, uint256S("0x7ffb0dc0b34fb10dff294c570e12512257ac3a7ee7e0bb9b1c1d20b1d55de1a0"))
            (  2895900, uint256S("0x46e2502e9bb1a7536a0e8dc92a87d214b2a688acd24caa2466946b867319c492"))
            (  2896000, uint256S("0x7f1fa198860f327927f945dbf9d7f5df7df39a87e088bc9d402ff26721effbb4"))
            (  2896100, uint256S("0xb1dd7f71e884383acd7d603ad23e27d766c0aee6d0a6907266f8745452e37a6b"))
            (  2896200, uint256S("0x3bae836b4df6c3f77991fc5ab31315b03c94ebb7bc3cb1dcc4d86c13c78acedf"))
            (  2896300, uint256S("0x4efdea3bcd1eb56164e7d9fee8e530ea11a11f6f506f471afb889f82a8e450ef"))
            (  2896400, uint256S("0xb8e9c9e0b09f90414aea7fcc9a4f8a63aff8f3ca4103c7518d3afc68980e2fef"))
            (  2896500, uint256S("0x20eb074dbd173d5eebe401e4c0e41ec7f80b50d4785092702bd1ffcae710491b"))
            (  2896600, uint256S("0xf75839f06a88de8fde040a4c0d12fe65cdea4600eda4611e54e48835fc827737"))
            (  2896700, uint256S("0x95b2f65c0a7e6d5858cc5554b947ec23696062908e1e474b5c1a33351f4d6ad2"))
            (  2896800, uint256S("0x6e87d4b951efdd0e77f098dc0ccbfee51cc393dc06331cad984978bd010b0c1f"))
            (  2896900, uint256S("0x62be8bfdac5d67848f46a762b2458fa09b7dea66e002c940669b2d9b25cab2c0"))
            (  2897000, uint256S("0x19ffe2dac6f4d351cb594dd65b5fe4eb6c9b6104635d250e7989508057d36d20"))
            (  2897100, uint256S("0xe060daa0bb82bc5234202da7dbd380fe36c9a66ba03434478bb62583e6e3d175"))
            (  2897200, uint256S("0xa5588ac8f278b42f1ae53562660bd7dd688ac66ba4c129e1ae6d0ddd34b3569b"))
            (  2897300, uint256S("0x6f06d128b13727d77782c9ba30287d592df77b449ca2f6bdb86ae404e27cd310"))
            (  2897400, uint256S("0x3b6ff78acc024b003509f583f9844cd72a4cb900a4aecac9b667577ea09ac285"))
            (  2897500, uint256S("0x3967f779d9f402cc86feb1f69cf42738c29f322fda15863ad8069e8e51ef1c36"))
            (  2897600, uint256S("0xdff303eaae72f338b4da7bd904fcc9539455cc101db71ce606065d81a823ccb8"))
            (  2897700, uint256S("0x8631b5815994897d4317124e1bbf727e2b495cb62b797b4369d7289bc50aba30"))
            (  2897800, uint256S("0x6d51b2e4371a2012c70ecc2eb5a0c5d83f921facaaee015dd44f46d99377de2f"))
            (  2897900, uint256S("0xfeadb53f41235c4206f3559a87ac33c2585dc20b3646fbbeac8fe18fdca01c9f"))
            (  2898000, uint256S("0xe6e7b8ab2baec283193aea1d6c6ef043ab445c89b3ec1507f39a127deac7f9f3"))
            (  2898100, uint256S("0xee6e5c6d80b763f9abdf7b281da6f6bf789c4f4d593b436853d7516d1c38029d"))
            (  2898200, uint256S("0xd0312cf3b59e19e435ad974cba4d5fbc84031276cb45888315f57646f27df98c"))
            (  2898300, uint256S("0x86fa1921c1116a3a17f195ef8439c99cbe6391a4d9660aeae03523b4b7c404c9"))
            (  2898400, uint256S("0xbe937b41aa701134f8324bdc9f90aef29da06a8825d38af4b8536f0421b1d6df"))
            (  2898500, uint256S("0xcd05e98af172be4bd14da992ebf9e0275128074953b930323fac04006a6e4233"))
            (  2898600, uint256S("0x703f2a06fe2a70a91b89413f4b7e5d6171f1764c61240a3b0d2c7bc1b1272247"))
            (  2898700, uint256S("0x053f310b26440742aef9b16e27d82829d479e7ce8040a38a8871a5795b2f5f45"))
            (  2898800, uint256S("0x0baba6d632e84ba0c090e53ba961f01276c7342de4f1f6651c2b96595746087c"))
            (  2898900, uint256S("0x40f481c6cb9b1ef843c6868894c95d1cd90851844177e9ab213a3bcd3ee5907a"))
            (  2899000, uint256S("0xe9e89d4dd891cacb2f1f5d4427ad48cb9ddc8a94f417ad659ca1f9395c95b86b"))
            (  2899100, uint256S("0xf6eb2cabbb059412de5714786bc388a565f0a1884c0d2abab3108f56a93cfda2"))
            (  2899200, uint256S("0xa51981f2339dd7eabf3223d18357b57133e85d8611e0ce746a6ef4b1edda2952"))
            (  2899300, uint256S("0x8aa083dd289a0b0354ed7e6ca5e200a4652b483ab08c16b87759b7bbdd643093"))
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

        consensus.WhiteAddress = "mtzbBN6s3VN1AZoyXuaACzR4mWG1qwWdgq";
        consensus.BlackAdresses = {{60, "mgk3Z3R2S7RhrTU7P1z4J7vJwVwcQSpmzi", 5 * COIN,  1 * COIN, COIN / 10},
                                   {60, "mnzkVQKfQ6TjvLev7y9QfJAhrzM4pLDfiK", 5 * COIN,  1 * COIN, COIN / 10}};


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
                {  1000, uint256S("0xe720d4d08f03d9aef8950b14b4917b6ea3daa6bd6448d1b96da76bee67edca9b")},
                { 10000, uint256S("0x162c95555e7313f9720d8437cb56a66e5f2c83122a459b0d4385fb672ff8f551")},
                {100000, uint256S("0xe26212281f5b25f2b6d4f384ebfbfdf8e62c0138caf9a7ce66a077470c4af7ca")},
                {150000, uint256S("0x2dc9a4dcae6394bf7011c5a6a3ed76d12be5bb3a08a491ff42831e2eb3835ada")},
                {200000, uint256S("0x383d8bea67f9c16e875e5d49cb918facf850094a6d2653746a989d4c25a240ac")},
                {250000, uint256S("0x2687ec7509fcbc5064d3abf0b7ecb651fc6c7a8bec08b685a159d59a80589156")},
                {255000, uint256S("0x4ed47783a897387a418e03cb5f85c8accdcbe3431ddbac316be48769064c7151")}
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
