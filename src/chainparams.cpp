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
            (  2725500, uint256S("0xc0e32ffce667f095de8c39cedb920e7e1cc73396e658b58fa15efe8b69608acb"))
            (  2726000, uint256S("0xb6c26d7ba689054251e7b466c88edaf9952f69dc0126f37e036dc9a0e4a7befc"))
            (  2726500, uint256S("0xa49735795d6c2e5a0b5c1c3ec9be52bc4ee41496b83cf84faf22cd5e4af5379e"))
            (  2727000, uint256S("0x412d84c7f2a53e1535e0b629058de2ca69bac4e50be26116f29b21bcafdcaa8c"))
            (  2727500, uint256S("0xb517e7495d44de16c29667f019b8367432e0bd41fe763e696309ef52c4fbaa68"))
            (  2728000, uint256S("0xeae8e7061dc7c3dcb382e18f846b23fba9e3bf5a482c71a528bd601786c484e0"))
            (  2728500, uint256S("0x08685450aeea1295a8912ec3bbc14fe6f0432bb723062ce11d8fd688c60ecad4"))
            (  2729000, uint256S("0x3fd98d10cbaec9daea3d382c04b58c190c65334892c63ca248db920c52f4b1ca"))
            (  2729500, uint256S("0x009f24c0002e26b338983e7b396b28ba78c6f9c7b94f4df58202e55af69803ef"))
            (  2730000, uint256S("0xcd15851188c1eb9426a38e3552af456b9e972151fa0a0198cc98ef24894affbc"))
            (  2730500, uint256S("0xc8b658da2a4e4fd57cd33e94dc6948f589eed2dfde6f3ae8f9f6097e9ba20f21"))
            (  2731000, uint256S("0x93a672f0e7e2b3553c4e44d3ce64066b65f0f70612ef12462fe3311f5049928f"))
            (  2731500, uint256S("0x8be129af8bf9304d131dcc41b130de4963b577ea1698837dcaf7f786ed37636c"))
            (  2732000, uint256S("0x67da9fdaacb0bdca20fba3345cd328440097439e252456d6af101a0fdd91ae95"))
            (  2732500, uint256S("0x25ae06a6616f060c8fefad7c4fec00a4459f34171853106edb1cf7dd1b01cbe2"))
            (  2733000, uint256S("0x233f58b4958a5eb0cfa6c33d98e94e5aecf0d783fd0fc7260265abf171c74ae4"))
            (  2733500, uint256S("0xe422b3224dc5c49b4a45d3df6c04b7868f988b316eab62f506af045badb459c4"))
            (  2734000, uint256S("0x4e36de66c78fa93b9358223f9b4983b934c50c7f141c77cdea4c796fa9e53d12"))
            (  2734500, uint256S("0x4d476e86f4e8d6522e35ca70147bb913216603bc356edabeb69684e2906eaa00"))
            (  2735000, uint256S("0xeb904239f1a20e409278ba68b1d627cb4e0987ce2a4d7c294b45eb61d6edc6b1"))
            (  2735500, uint256S("0xa0bb352d08176f402dcd6e071004a54563d0eba0d971eb183c501b6b5a19d6fc"))
            (  2736000, uint256S("0x13f09a13422fedae336e48124ea673045e70a1898d5bf4d98901b43dd17af4d7"))
            (  2736500, uint256S("0x61e0067b6ea865226d1b76ffd74693b5ac19e1f03f1db4a283bf8584a79cea95"))
            (  2737000, uint256S("0x2551aedaaab7c876b61e9de5da64d51ad3439e44fb8c95d537ebbff676ec2077"))
            (  2737500, uint256S("0xa18eb56002aaf67afb2b5976d88c5c8110df7b867a4d76c156372e98277bb320"))
            (  2738000, uint256S("0x1ddac67ea6be723f9c628b41bc7c207f9a0be3e952149713b25a3cc195fd3a6f"))
            (  2738500, uint256S("0x17036d9c89d18f3adceb0410a66f68006fc19888ab7ec5727f151629dc266abc"))
            (  2739000, uint256S("0x92d4b05719d1d961c002c84502bbeb8dbcc91bd1678f412c3c994f532850b571"))
            (  2739500, uint256S("0x383f49506fb6b3becc002fbeeb120b9d65b34938266a6d422f1e4ffcd61f4a00"))
            (  2740000, uint256S("0xe69bb4623be22f9d30efad5598f0c6062ac90af6b5403e28f4c6faa35a730c3d"))
            (  2740500, uint256S("0x7195fa29d26ead704dcf3b8fc89aba972d1142fc802d222a85831ae12c503214"))
            (  2741000, uint256S("0xf768a6c88c66022cca67413bef587fef9ed8709ba9e119d990372dec95aa3726"))
            (  2741500, uint256S("0xe04dee9879506eb0f9b7debfd1c83741d1a8179022e5a7207c776da29c808c26"))
            (  2742000, uint256S("0x50f2553ccea6c5cf36c4433a607c04fe0740baf5beeb42cdaed4bad33a4f9b0e"))
            (  2742500, uint256S("0x4f55af6128fa4b425f387c575bec035ae670d0e3d83f2bc9461e9c0323d55ba5"))
            (  2743000, uint256S("0x7e88301aa219e32cfb4fd0bff54de854bcd1d6642b914c8259030723016eeacc"))
            (  2743500, uint256S("0x60f01f587df329b7609eaedf1f6fd6307264a446a35a52a849fa4b32c2cd569f"))
            (  2744000, uint256S("0x2492086cfadb66eecb21328b8f9c534bdc7d197eb045fa5fd19e85779572d776"))
            (  2744500, uint256S("0x135c8b934e0c638b36dbebe9bf43a0b186854c2e0402e164d972401b70bcbe0f"))
            (  2745000, uint256S("0x8d41458249329f7b74224243fd22dc28a686f5b45560dda4f0a4d70663b301c9"))
            (  2745500, uint256S("0x2a7b6fb92cd05a02b6fac0ca8e9f6ced1c855d886791315f419a3ee38ba78aa7"))
            (  2746000, uint256S("0xb433c172599231aa3a7883b0660b71c251600ed2e13b58e14d2ff5283cdcd7c1"))
            (  2746500, uint256S("0x660f6d678d86bbc76ca42d68a9608e20c01d0d339bcf51e5e8858b049aa571fb"))
            (  2747000, uint256S("0xf277e371abfe454501d736a553754a2991131390d54cd32dc928cb2fbaa0d686"))
            (  2747500, uint256S("0x0417289cd34cd2d0df803dd38e4af74179e5e6d221312c0a7158a70b51c969cf"))
            (  2748000, uint256S("0xf0ce661f463854aa17b217eeddda1e25aa9ed3f56a1e67ec7e6c40d71a1b6ccd"))
            (  2748500, uint256S("0x8df65f84c0a8d0ed2588ea9e6d592157905bf127d5201778cd2024dfea77a266"))
            (  2749000, uint256S("0x865221b065bd479d6ad7c46b7c1bcf637bf6e993d41a664c525e9af61c7116f4"))
            (  2749500, uint256S("0x1592f65a32105256305da1f2c013c8878cb3512e7a42b625d021b3d68b747045"))
            (  2750000, uint256S("0x7b30f779771638117001519b842a5d1f14e263f1f5bd3bac907ef0650131d368"))
            (  2750500, uint256S("0x040359dc16781ade8edb60a8f25e5312205cfcfd0a0a91090ff0b849a8d12f19"))
            (  2751000, uint256S("0x135dfb3a090a990e44f143815079e87546ec6c59d529fa4f3a428480c20b468a"))
            (  2751500, uint256S("0xb298febd615ad6b46a583efd08b14d9727657fc1a3b03a60f4fa751ed2369186"))
            (  2752000, uint256S("0xebfe078ca6c159efdeb51fec1c9fb8c8b29d020965f34b58d0fdcbe63e8a5961"))
            (  2752500, uint256S("0x4acad56ca7fe247dc42d0dfc44fd8b3a69da986f28a567d132073a0dbe07798e"))
            (  2753000, uint256S("0x770ad446448bec9dd25405b7c9615e19ad85a76e7ca2ea17bad81554ee6ba28b"))
            (  2753500, uint256S("0x827e7c442b4c9a788f3bb7cad4d9dc68ffcff029047e1e69b2e4a34284e06a38"))
            (  2754000, uint256S("0xd46e4a4ce55da0ea8ed3e719cd4f29077c1014538da51d94aa39574489b8ed01"))
            (  2754500, uint256S("0xd107fa2a1cd336298678aea32ae8cdc4003709e0492371ee44301d7e7ebb7036"))
            (  2755000, uint256S("0x42b1f386ed54ac21eba319f8ed035501a9ad82a5173726475d6ccc17cac20fee"))
            (  2755500, uint256S("0xb2cbbc91f52ad1ef3a0e685821eb7cc23a7a548bab1b49ab68bd2e4f610560c3"))
            (  2756000, uint256S("0xc7587339b39a8a9782e5262f5d365bf6df3e1ecaca1a4376f7bd53c19ad0aaf6"))
            (  2756500, uint256S("0x2d6b9534d04fba648d5c545094b4e9b837e3af02c338893535308c7d978e9386"))
            (  2757000, uint256S("0x0a640ecb0f70fd83f19ef55f250a32bafedf3a891586fcd6478c7ef77d4310ee"))
            (  2757500, uint256S("0x71560adfd2c828aac03576b8ce09b79097e39812f6e9c20a5345fb5acf39e7ac"))
            (  2758000, uint256S("0x087e54b9d76a8ff17b7bc54a276289624f40d111bdb408a52ad2fb6b202e25b9"))
            (  2758500, uint256S("0x51de3a8a089371b56ed7a36b3b9a5a718f5d1e40f66373f0ee5926cb326e857e"))
            (  2759000, uint256S("0x2efce3269745068a7271ed8327069590df997a1336f034216c1b185323bbc7a4"))
            (  2759500, uint256S("0x00bc51744aaa6d73a6e8d8c728bbb63d071147f255ba86bec75afba0fca636ab"))
            (  2760000, uint256S("0x10b107eca65c927fe5addc3d2e76f83e7fa1a356f49581cda60cfe28ac761f76"))
            (  2760500, uint256S("0x85823c25073ca71c007084b5dbd7c2f05f0d99563521c4280d0d5f556ef4d88a"))
            (  2761000, uint256S("0xcc72f20d2b9f7c807736ae092bb63ebcb1bb473a9bb459f125f33cca7093bea2"))
            (  2761500, uint256S("0xa16c70fd951b82fe844722d81cdf686dd53bff2b5f4d9d373b3dd4e21b1f2fb1"))
            (  2762000, uint256S("0x041d5acfbf83867b5a0d01f78d585276db2ca2f3b195329b40c0ce6d81afbcd3"))
            (  2762500, uint256S("0x2e3ca1cb74f248ae2e1b5b1683dd6fced26b80319b8649d3e1e8e7565887df1f"))
            (  2763000, uint256S("0xe42b3a558add8990260c564f00455410980a5b52788f9c04d27f278a91bd0490"))
            (  2763500, uint256S("0x016fdeabb679400e61b2660cbb4d9070e25c0ecb1009d3322f766de5296ed103"))
            (  2764000, uint256S("0x3749317f3b391fb5ef8c17d387276b30b1a2ce0b7aa1fed0216dedb534c9d1c4"))
            (  2764500, uint256S("0xdc0cc1e7681ae5b5047d36299ba07a1f2d2e16f5b6f2843da2bacfedc44712f5"))
            (  2765000, uint256S("0x30b13f7f31999d9d2f31299b3eac2d178859e04a91db89218e0e453e8909015f"))
            (  2765500, uint256S("0x74bfeed331c46d80580739a4c47b9ed64463e6457adea9a6650bb1028984ea70"))
            (  2766000, uint256S("0xbe759760981756b0654fa84ea34fce2f0045e0528517d625c9fbe2c01dfee0f8"))
            (  2766500, uint256S("0x2e6f1c5e2c829c70c6c0feadeb975aa85a11d1d84483f6ca9bb85f035b0ead2c"))
            (  2767000, uint256S("0x115ba182f644fea8ebdb97062b94ebeeee4be08a79fba1139edb3eadcde8c050"))
            (  2767500, uint256S("0x20acba2b36836135d59fa37b7ff13ed719bb48c795ba6245704a93719939878c"))
            (  2768000, uint256S("0x7a05cdcbfc5d6783c4dceac0e9f1d2f36d22f10021619f800c1f56fc4f1150ab"))
            (  2768500, uint256S("0x3b9d816663ae9f5c6f971e9c59ed600b2a756fa87a17855bbabb330a4165c549"))
            (  2769000, uint256S("0x728d9bffdb41de4df1d631175d4b2690a792f94f297cf6aebfad546cdc565dc3"))
            (  2769500, uint256S("0x16914db212e7de0ed3ac6d37c2c6824691b2b6a47a5ccc7cb6ac936000218907"))
            (  2770000, uint256S("0xe3219810a440b3d6eb4e7b846dce4cc4ff7ae02e77882726274987541f68dc6f"))
            (  2770500, uint256S("0x6428e6694f0c7caf5c559c13d46e37b4977b824e75b8e19e2ab7337b23b0f9e0"))
            (  2771000, uint256S("0x4e96e2e86c5f1cf96b1eb2aadaac6ba892772b80e36e5769ea33a996608822ec"))
            (  2771500, uint256S("0xf7599ed0e880ee0ad28e99992e12bc53712b9147694f329598021fa2528db499"))
            (  2772000, uint256S("0xf9aea3484c224920e7c89ad754978628ddcf3cc0ee108069325d0978fc7de599"))
            (  2772500, uint256S("0x89190cb09afce2373eb71656e760154e6d19368e8f8103acedf8a833b932e39d"))
            (  2773000, uint256S("0x6e34b0e4ab03a5363559b820fae40275cdeb1d96a45071d0b124323ba401e608"))
            (  2773500, uint256S("0x26c0ecca43aabf15c70e0adb497fa06b3cddd366563f03515c31d3c01479d591"))
            (  2774000, uint256S("0xdd2f94171db8997794f49fdaf63ebd01cdad6ac3fca7199f54ad979406f59556"))
            (  2774500, uint256S("0xac03b9bbdade0b571b2e12dabe324c598b8de7d2c3bc49e2e69448beee6e890c"))
            (  2775000, uint256S("0x4f756bb50378d1f51581e446ce1fc822d884c8cf85bb11f40d006ebe19d56423"))
            (  2775500, uint256S("0xe4a977def3ac61bfd16dc43e440a60a3610e9e14ec9c34a5a4950516d217f52d"))
            (  2776000, uint256S("0x81f3974871339071254f8caf6e810d6935dad41a2ba6428734046f786a48331f"))
            (  2776500, uint256S("0xf197d28791367b5876be41536be78736a55a5bf276599e332b6863baa3179a06"))
            (  2777000, uint256S("0x9fe71725e4b898144b9273a312163d07ad0e72dfe6add6163b74476781ae8034"))
            (  2777500, uint256S("0xb8d9c77448d280762960af62305ef232b4109f222981bcecb94a6970d83479c9"))
            (  2778000, uint256S("0x4569c516c64fc21058d12dd2aa41afaf408ec51ff5ee17e1242765bda17a0fe1"))
            (  2778500, uint256S("0xe294590736b6037d0a5f5174060f9529e684c1e2af38cad96325e900ed613043"))
            (  2779000, uint256S("0xe8d50f1bf0985068f0cd3c8878da919b40232eb747905b868f7d4b30fa057881"))
            (  2779500, uint256S("0xe9c0a2a34605a300f339750d50ef24e76b9ff7aab89ce1ad62e070d6c373c4ac"))
            (  2780000, uint256S("0x7efdea8727c235620d97ad8975bd7eaa5c022835c5df7671241b652eb990219b"))
            (  2780500, uint256S("0xe35b79697e03c1d2899c88bf7cacd6084b6b5335065f66df59d286b67a23217f"))
            (  2781000, uint256S("0xc5a1fd202218ca8553803f8f321c4ceec9ada152e56126c18fcfd61b4a8873f5"))
            (  2781500, uint256S("0x874d342e44d05a3513d9e0e8aad7c591c12658e84eadabc5acce4b5b1b83a4d1"))
            (  2782000, uint256S("0x536da0c57bb062822d62eec9218dc2ab6745a9316c66bb7ca11dbbc44ce0276f"))
            (  2782500, uint256S("0x0838049bdde4b962b497d571e5068a13572372555d3b309510efdaaeab815240"))
            (  2783000, uint256S("0x71a117801b4565855d137f953060681bc6129c4bd2b54633fc489d8765625eef"))
            (  2783500, uint256S("0x9c581dc4aeded71ba874a33319276a23f91a3dc114afe7a2bc446f97e705bb34"))
            (  2784000, uint256S("0xcc907b1d16ea55bc98649f50881b87a58350eedea00c10aa97a24d87f4119dff"))
            (  2784500, uint256S("0x0311a8237ee11a97db0f9b43b9997e75e545cedf430dea6e57bad0e00c0e6325"))
            (  2785000, uint256S("0x1d040a81763084cc1933c67504ed210ed2af06de720093fca9bd7d8a0f2ea851"))
            (  2785500, uint256S("0xfb0884b7629f45f7f35a42850a881a07db135d06dc0463e90007da52757bec5f"))
            (  2786000, uint256S("0xe01b6edbc2635431bf5cbd839d1482f93deb33e6d7770287fc1526bbfeb3458e"))
            (  2786500, uint256S("0x387f563eca6f2863cd2696ee24db0d3bd47726db66ef4983c205f1205cfa0577"))
            (  2787000, uint256S("0xaf0f94e73a16b6d672b87827f16ebae33aab693ecd9359ba3f24d0c91c6d8343"))
            (  2787500, uint256S("0x0cfb2f16b8ee85ea39af8f1c986a1d7f13eba23fd04e4d0cae4b354c52aa173f"))
            (  2788000, uint256S("0xc842848219a268d3d9095fd816fff97c262e9270436613fdc58aebf2559308e9"))
            (  2788500, uint256S("0xd1f8c7d3369949b4458bcc3c028eb888eb7d3513bc500f3bf77747229cda32a7"))
            (  2789000, uint256S("0x4a200a1c4cd78becf674c43d18d9103295c907e1941d84702bfc0844e42413c5"))
            (  2789500, uint256S("0x2b091cfe69405a663c3072658384d9666c64047a473efa97c38b7ba374a0ee1d"))
            (  2790000, uint256S("0x790c92bfd2cfe87a4864437336a5211b91ca1c3d9aba0e998e54478027a654a4"))
            (  2790500, uint256S("0x23d1fceb90488000aa34ef32f73faadd8d74d35fe9ec3c66f1b56e9db97bd11d"))
            (  2791000, uint256S("0x4fdb17d2e9179940fe067b62eea938b4a8eb4985eb3d2d7b8d6efaa646909c6f"))
            (  2791500, uint256S("0x866bfa5e5f65893c8088733ed071a02882ed1fd133e8778a3b47d1c78d284b7d"))
            (  2792000, uint256S("0x22255d1216b8d525e9b3c463da8587ed4ea13841c953e57949b4f415acdc80d5"))
            (  2792500, uint256S("0xdd028c8654673c86d5985b6b4a5e7f0b402dad580e0a50de41c73348067bf692"))
            (  2793000, uint256S("0xaf874de8296994c949bfef19deb0aaa65e89dd523746b6610df642ade9afec25"))
            (  2793500, uint256S("0x86d20699272cd2aea5a9c953a49e52f8e1d58ae3f8aabcbc1d4fc9e26e65736d"))
            (  2794000, uint256S("0xdf3677b757d84bfcdcb4c6169164afe9b088a7af4ed28d62a71162841b7cafee"))
            (  2794500, uint256S("0x81adc8500127cfd0ff54c8f26e6910e81b33411de07cd5c781601b34ac4594f2"))
            (  2795000, uint256S("0x0205863a09663153e7cfff6c3813e3ae0bb182fea2d89c97be02c4827b525e5e"))
            (  2795500, uint256S("0xd3c85cb137e09584424aec38eb9f9178a35359d9b4b0200990b01d16b1bab5e0"))
            (  2796000, uint256S("0x1407351751850a4441efc7d75aa869b4b0446eb1ea48841bd4d60d3f4abc8c25"))
            (  2796500, uint256S("0x457ba31fb456639648c7db1d86db06683f7e8ba6c4683b2adb0acfa2c0b5356b"))
            (  2797000, uint256S("0x52a40ad12e7d78a7eb501fbf96fdad785d2494caa0d7e7ba9429a13851cc2ecb"))
            (  2797500, uint256S("0x142ef5df4b65d93e492d79ba9f7e539923c65ef5bb7ea14c71f3d98fc0a6bb1f"))
            (  2798000, uint256S("0x1f7955f7bf0affb6bff5cb64465e2523f663e3e5c4fb11fcd23e2bc2c6d973e4"))
            (  2798500, uint256S("0x952b9d1ad6a68b5d38e7f2fdad7897c16c8c43f6233bb84a24f1363a716e5d7b"))
            (  2799000, uint256S("0x228cce658705779895aa1b534d877c3294a7d770d71120b1fccbe8c88c9a7d1c"))
            (  2799500, uint256S("0xd496b95f25cb3fe3c8e0971c9317ba56f04d293cce0e60958257ebaf383f0a86"))
            (  2800000, uint256S("0xfff37136f2a57dfd4d3786b40b34f72544a32552e8d3582268e35d04b5d4ca24"))
            (  2800500, uint256S("0x2782ebe09d9d3a1e87d55b1ee98b34521b0cbf1f11da50e643533e0c041cf942"))
            (  2801000, uint256S("0x5b6f7f3b6d11fbb429e2baf9c85bf631a99c0d89d4aba77fd3a282ea36274ed3"))
            (  2801500, uint256S("0xe9d9e01459a33a63c64ba1873eca880e6165614bbed280356cb260588e01395c"))
            (  2802000, uint256S("0xba5ae0ae33e3b16c51f522d17b72642baf0864dc2ecc9eaba1220f75f68c2db7"))
            (  2802500, uint256S("0xcdb4cf867e69c0fd0c54f862d7c54315787c99d1ca9cf47745843d9e7267e208"))
            (  2803000, uint256S("0x95cc2cfe2494c9460d96c3891bcc4b16ee3dffca892be74301e730c88c985c63"))
            (  2803500, uint256S("0x23dd6623285336d3474634437ffbf16fb017a823c36c1afd7a819962fbd4499f"))
            (  2804000, uint256S("0x707d2493a635764baf382c1cbd50adcada20a20622cc599488d3b2422585d390"))
            (  2804500, uint256S("0xfe0498e860039484cca4b58dc54aedb8a552f742058d7f472cb4041b2fe2c7b5"))
            (  2805000, uint256S("0xcd925c8b698c05882b91ca9133e3e28cffbd328e14a88c82891ec03d72a73ba3"))
            (  2805500, uint256S("0x2a2644d3faca705a81f6dc90cba244d50ad76e7a9665d7b7dac83f4f9d5e5594"))
            (  2806000, uint256S("0x459aad57e8fbaba007a0b382c947bcc9172647443482d48450d8259dd647fd1e"))
            (  2806500, uint256S("0x147351c9e53f84ca82a1b480e22c4fb47b4a12a15226d1b09b3f85651ccd5e4d"))
            (  2807000, uint256S("0x0bf670b12ab3aec885c2265e579eb5a7540c488867bc5fee2a03cb2ee008f657"))
            (  2807500, uint256S("0x5c7f4e72b019ad476d56efdf891cccc01abf44dcc64d2dfdd2ff17164137ee9e"))
            (  2808000, uint256S("0x5ebb031a3c416f861129cf9a85746c86daaaade0a224898537a6d18109bfc3b7"))
            (  2808500, uint256S("0xbf2f90a84d1d629aad63e303a0350ab99301cb84c8d5bd883cea0f64f486491d"))
            (  2809000, uint256S("0x92c994172beae9953e490d82e13d79a4871d0cfa91bc2ae9cdc207bb5bbca98f"))
            (  2809500, uint256S("0x852adffe0a79cfb02f9a5dd36c86cc349a745d6f3cd863bfa3a7944d989a2b95"))
            (  2810000, uint256S("0xbc595ebb10a91f867064b4fc58cab91c6391288d05697799171ce21fcb23e7df"))
            (  2810500, uint256S("0x2b559ae4b5239677e0ca3e451747df6175f4325634bcd06502e3e37b861a5bf1"))
            (  2811000, uint256S("0x200a8814140f2da0b2ff8203ef078ee2a985d2f7373cd60a26c3e3e6dbf8d4c0"))
            (  2811500, uint256S("0x695f4ac768213b3549756d2d75e59a5cb905472aa8a5ba49edece3c077ebf0d5"))
            (  2812000, uint256S("0xca224c0a3a502671c125976ae8ff063b0572605ce920121f6c77472ba6fad92f"))
            (  2812500, uint256S("0xcdafce48d2b360eb3141f2baebcdf0f29442ff77477d7cffa4a2fd2a5caea73c"))
            (  2813000, uint256S("0x4911c161d3dabf7340334ac212f9dd2a63253e5d0fad15109b7ad6f0e30478c3"))
            (  2813500, uint256S("0x4804024a35f3fa3217eff23e90db77b61d3a6c02b726b395f521810b2aeb742d"))
            (  2814000, uint256S("0x8f6ad6070594174cf06a2c207b9277ac17fd7c0b6f2284731fac4df5c091bed6"))
            (  2814500, uint256S("0xf5160ce1c5618a948662cc15e0694c21efde22e787fded94dda37be1e4e4646b"))
            (  2815000, uint256S("0x992d8ad2aa54be895a181c51252bcefe06a948881cf9aecf90aa81d83113812f"))
            (  2815500, uint256S("0xbb5cd09cd4154b50b85ea2e17cbe960a0999df907ea89b99b6c0be4674d22909"))
            (  2816000, uint256S("0x476a66d19ed5ce7347d77823877da31588d6fb2b3f1539fd999a3897e6e7f844"))
            (  2816500, uint256S("0x9e0bd1cd9184c34c86d75c582b1c4d3cbcde76730d3e15cd4003d6d035f6fafe"))
            (  2817000, uint256S("0x3c79e5a6da386d5f39478b21763bd970caec313aa3d09f433f39739997dfdd41"))
            (  2817500, uint256S("0xc923c8221efc86e7da11c9e8a1f5f05a759a14c1e8ed798959cf7bc6fb8bbedf"))
            (  2818000, uint256S("0x054af4154a4006061c5b5d7c5cfd4a4b0910f0c072187decd2ad13e9ec4516cf"))
            (  2818500, uint256S("0x83841f91a9c2b6cbc20b4225a1c9ceb8875ad3cfb0057038aaf45c01bda5148c"))
            (  2819000, uint256S("0xc98435a98c81d7ab20776758a14065ce88a8fe7d60f1919aec72af80bf96e8a2"))
            (  2819500, uint256S("0x717dd1134557ec92f018bde76f37d2c93486e05d1dd683b26cd73abac3691808"))
            (  2820000, uint256S("0xcf3cab647d7354dbcd719a450580cf1e6f75027d0040d08f2b9f28ee1ef7cdd8"))
            (  2820500, uint256S("0x4fb7039a8def8f4c5af42127c46b952b4dfca9ba7e0b2cf12de68131abe480ac"))
            (  2821000, uint256S("0xed66c0ffe839e97d78abed4b7614b659bb77a3f448f06480a3321b66d44fd193"))
            (  2821500, uint256S("0x324b3d0e349067f244a337e5039671d507fb54d7d658704b9958b4a10c604dc8"))
            (  2822000, uint256S("0x253241620964f8bcf3d53a765a891235bfd058dba26267b1c83ba8457d29f5ed"))
            (  2822500, uint256S("0x6d8b588538bf9574cd71f6a791154b1de3361ac8ca24d9857b0d6428552f0779"))
            (  2823000, uint256S("0xb5d3c2dc87dcd23b9d082ddb884e8b4a1fa380ad8800028d9e01f49170189741"))
            (  2823500, uint256S("0x2fe77422d5ade9c3410d132f0a7e888eb96b3e6c9d54647aaca26e8b9f0a5c3f"))
            (  2824000, uint256S("0xd7b7ab143a3a703c79eeb1b1cc610b375cf04ded73dd1a8b6b57d65e34a8a371"))
            (  2824500, uint256S("0x0759aa8f0d1f00b3b3ca2460bbf2bf0269c48a5362b6fcade4f806aba14ead4a"))
            (  2825000, uint256S("0x23ba78362249cce006ebcb52a3792f7e569b406cb0310fe8c0a6da3c391fe56d"))
            (  2825500, uint256S("0x717a8f7932db002078a5b1a33cbcbd6ebc8d1634bd49e39bdfd5f1af37705ee9"))
            (  2826000, uint256S("0xb111ce21633423b2e21bc9439925852d99cbe735a8b18736967fe2e8814c8d78"))
            (  2826500, uint256S("0x9b40efa7fe408978dfd8d49a84963a83dbfb1f87af2402569656cb9b31e8c27f"))
            (  2827000, uint256S("0x6849d300b953fd1dbbd767fc680a10809d34e5b65f7d1dcd7e33400126d65c06"))
            (  2827500, uint256S("0xf5791dfb0b98b7d30a54031c18ffa4b3953d7a32a0dd48de0e283ad77dba60fe"))
            (  2828000, uint256S("0xb4ef50432676d00bb5d9550bd66d8719f585720b8f482dd108a69fe23a0f5c74"))
            (  2828500, uint256S("0x21d6bd981749a158d7075ad7da8bdb3a1915ad7a636ea0add7753c9471445267"))
            (  2829000, uint256S("0x2f0e2f7d4b970fa98e2b86760dc3846766c3f6956fbaf3a8f9f00ac26e826c5c"))
            (  2829500, uint256S("0x7d9a728cf0b6d18eab7eeb1e0f24bf039f8b6bc6cdc0a2dc4f5cdfc756f71fb7"))
            (  2830000, uint256S("0x9c837816ad049660825c02bbc19912b446edd1ac2b7e58e41a60eb7a439f849a"))
            (  2830500, uint256S("0x35950eafe91cfd8de27dfb96a2b9065aa3c0ea17f2f085a87507dbf2e48d6341"))
            (  2831000, uint256S("0xa2e3f55e2b30fe1555191b04e0890ffd777082b76a2328daf065a853e525a49b"))
            (  2831500, uint256S("0x657101bcb5e1c567b727544f3ee401d214533b5a9c20541cea56156504d42bff"))
            (  2832000, uint256S("0xd90d16248bf90adb3ec9724f9413637253ba5ccc8c3e65030974b0723a8e4156"))
            (  2832500, uint256S("0xb1c1a48fae2199bd854238b6b49a6354e363998b0cf7784f06d60790212a6a00"))
            (  2833000, uint256S("0x5a7fff723d0cc0de07bdecbe06bb19900eb6ce8d7b59a7e76f51f96c54b151a2"))
            (  2833500, uint256S("0x2e5350de087574dc537ffe71b9044e8b91f3ccd172efdd7900cb97599e4c5bb7"))
            (  2834000, uint256S("0xdaa0603e2f0c8137cb273fc3e848b290a6b8edd46513efc2088606ca0cd6a208"))
            (  2834500, uint256S("0x0aa7a5781463dfcec048128bb1d40678c27570c62d32cfdbc103eb0bd3df0ab6"))
            (  2835000, uint256S("0xa6b432b88c6f43f39b5b8d3d58f261d41eac8966e5ade4a4a2ecf1d2cca27650"))
            (  2835500, uint256S("0x9a61b6533db62fcf7191a22784404a4f05ec1f3a510b3b2a95824819efaf2ebf"))
            (  2836000, uint256S("0xf765dcef096a7b14ef4c2b809c5510dcc49d0511bc7a0f3c58244e2b4c150451"))
            (  2836500, uint256S("0xffe3b1600de9ba1fc75656f8bfbeabf59bb427a0ad5fda4fb9e9c69f6f5190a3"))
            (  2837000, uint256S("0x45c1a7b93b6d0b54cfecd949d7b987da5fe64c1215874199849e5da5f4b1fdfb"))
            (  2837500, uint256S("0x6b167429fe0b9264902fc03b5ea22ced3c6e4399137a51c469c5cbc3fc492417"))
            (  2838000, uint256S("0xc899cf3a8cec847780620a05c66243e097f9dc1152e3fc8f1aa1b25044c8aa07"))
            (  2838500, uint256S("0x7716a6413a80c8f419f3742592c96251b6246c24c82c19cf21078b1967ed70a9"))
            (  2839000, uint256S("0x77538fc402df2cc21a2a2372445884e623b43c572f2a5f61fc129721b2c24ea8"))
            (  2839500, uint256S("0x530490653c82deb5ac480268f6e66322f38f758715c11e6ae7df7c924e7dfeef"))
            (  2840000, uint256S("0xe98ee2e3f1a3bc388f7a6ce23a589726332ec9d4941ee4cedaae303bfb33fdd4"))
            (  2840500, uint256S("0x9e0228e653386d588b59646983537be05e20c3486c0b5f9b7199fbc73e51f0c6"))
            (  2841000, uint256S("0xda358e64bbb620b5688542d91bd828dae8da96276be1606ac33bd21db8a2038f"))
            (  2841500, uint256S("0x2763ddcd037466d63dd6678e0f8f468f31d37cd4160c77824035e99497dbf2a7"))
            (  2842000, uint256S("0x0df013f1c953b5c124eeaf80610876499b728bb099ca0b9a643af59bb5870074"))
            (  2842050, uint256S("0x88a31e4c27f6d9b277e360c051d768f084262d75bda435e9e8ad0da9b7fd13ed"))
            (  2842100, uint256S("0xbce67aa6adf0b163b58632c0f7c2fa5533453cc88cba946e2014baa1454aa074"))
            (  2842150, uint256S("0xba4258264d808c0e9437d4061667ef88997c7a454cc92cebdc4f90948edf97dc"))
            (  2842200, uint256S("0x72e43c2d406fe7144ab84b8fc36d4cd39ef1cd68c4e4d16d7e434c6e8148d1a5"))
            (  2842250, uint256S("0xd388529a1f6c5df841507fb75a1d2c63f8fdd871c534c86337c712cc153f05ef"))
            (  2842300, uint256S("0x6cd293df9e8890c9427ae288075b9947e6f479cba5a0bcb3174b2f71521dd330"))
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
