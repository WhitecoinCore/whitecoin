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
    typedef std::map<int, uint160> MapSupperCheckpoints;
    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        (  0,                uint256("0x000007d69ba0f79b4823effb06b08663e2e4c51ed03aaeb547e2e0b83fd37b73") ) // genesis
        (  2,                uint256("0x750129f3369574f0c397e28273cc5e795f5a78d3c43eba4b58583bac96e62e18") ) // Premine for swap
        (  2500,          uint256("0x4a7f2569e40a991bda8fc8510b31e0b79344719c03fe96a5c3d2069c15aee89a") )
        (  5000,          uint256("0xc207e945e7b09b79b6601be24d891a96a1a632933887fc8f07ee4f7a1af2e612") )
        (  10000,        uint256("0xb191f69a673fe281d091003c41729204aa1b9b86b839c03f49f8665bffd87e53") )
        (  25000,        uint256("0x2fea86d43efd05fe0b4c9a78d71a5a3238e2133476aeddc56d7128c510405bd9") )
        (  50000,        uint256("0xc0e4332839da0d1f8f41702da22efa2c438c1a108ce6a8ec7a567798ff7e7869") )
        (  100000,      uint256("0xfa254c1010b93ed6944d9d3489792a034ae1ab5d795e0cfb538fccaac672f7c5") )
        (  200000,      uint256("0x381945f7b9b8ab95b6b9de199a2027fd9ae1f57ad4b766369e8a1a9b27448b49") )
        (  300000,      uint256("0x60aca7679ad42be9b1359a18c9823544b471ed113077efee5a7097d80905103e") )
        (  400000,      uint256("0xbd3ff208033a486ae4dc1d94020bad478433a7933c2109cd8cf4675ee0477fe2") )
        (  609332,      uint256("0xc2683173c64c3cd223ac2cc2a8d8f841b7e2c15142e89d1bed3b6dcf2580f22c") ) // Hardfork PoS increase
        (  650000,      uint256("0x2443cb1d00a0a2b1679fb9b203946cbf9fdd662b8b36322505c96c9f20157451") )
        (  700000,      uint256("0xc3b608dfaf97dfef9e74a4095a69981692186c1f86f9e94de4a63b6ad2a6f733") )
        (  750000,      uint256("0xc2ac6306b609f6912aa7a8e617050dc0330d7a8a10c3943d27d14b4b9b6f88cd") )
        (  800000,      uint256("0x0710b931960777f43ef5ed27beaa615a86b6992cfcb8174d16221cacc6860f15") )
        (  850000,      uint256("0x95ce0d9ac188f2463360f856a71141c5e393a8625941d50bda0f5a2c09cf416b") )
        (  900000,      uint256("0x5885eb82e0afb10f2bf5f07d6362920ab45257b42f8867e16f04586b9b2c93c8") )
        (  1000000,    uint256("0x901d89f6362cae0dd9238a4a7037f4b6af736ffb97e846ff5e22f76fcaf7e449") )
        (  1050000,    uint256("0xe79321fa932f54df966ee3a61048c00cd514415bd8ef8d169d8f33ba6456840f") )
        (  1100000,    uint256("0xae15b87899cfed5bebf35f3fd68b7f2e373232c4322ebf12e4eec0e4321fa822") )
        (  1150000,    uint256("0x86abc81c6a2924850b9442605ab6e591b132ef04adf0e9c1d1bfd727d7e64a96") )
        (  1200000,    uint256("0x2584870fdc78b827528827fabe06ec278b70e4637f19bc899bfa32afa350787e") )
        (  1250000,    uint256("0xa16dcbaee629899304969c6842fa5584c5ec4248563d6b12877f8fcd59bbcba3") )
        (  1300000,    uint256("0x27e5892afdf24e5ab25121593b592592fd13c8192b4f19091a1a43c5be572637") )
        (  1350000,    uint256("0x5b8046b2a7b25cc4c2f7cd3abe857e75b97bf9df5712d38834943fe5692b64ff") )
        (  1400000,    uint256("0xd851b41cb3e54f87accc0dd029c7cc88ee64ba3a699e647af4614bc5cf4dd525") )
        (  1450000,    uint256("0xd92c3e04b0e48634a0f87fcc6bcb34540d71d378784894757468bc60f94fb620") )
        (  1455858,    uint256("0x5c46f2af9121181750a0abff58978a6550203588a8cd7a7200bfbcda3dd0dee2") )



    ;
    static MapSupperCheckpoints mapSupperCheckpoints =
        boost::assign::map_list_of
            (  500,         				 uint160("0xbc1f86c6517764ca04da424d394df69cebaa6b24") )
            (  1000,                uint160("0xc3c8c42a79e4b016831c1cccb07b73daebc6b21c") )
            (  1500,                uint160("0xd0870ec8f2f9ff0ce53173727a24bbc8ae9563b0") )
            (  2000,                uint160("0x95f1d495c4407144695194e2c8396f87c489c068") )
            (  2500,                uint160("0xf9d376c3604e0d0ac938acd949e49eb7c7502622") )
            (  3000,                uint160("0xe78e7865665c5e5c3b7c7c467443f7f3f5604127") )
            (  3500,                uint160("0x15a46c7cbf807ec98076cf0d8e6facc7256a4b42") )
            (  4000,                uint160("0x1a4f02be92d79e07270654af0e51ce73ea6d1e7e") )
            (  4500,                uint160("0x4fd993522952b98d5f2b418411bcc6bad05f8a6d") )
            (  5000,                uint160("0xb8fc949f06f6a19c86f73bb87d4879d674b3da2f") )
            (  5500,                uint160("0xe1156272702811220777014c5d1bd4889c599b13") )
            (  6000,                uint160("0xa06c8a2ec79ad51324f30d90af6012c1a0a18eb0") )
            (  6500,                uint160("0x45d4d63014a00d64185cd6a8eca99d063fda5b8f") )
            (  7000,                uint160("0xbe341897623c8298743ffb971087bfe8a26c3465") )
            (  7500,                uint160("0x3cc31dfcc681344a83f11b480b7c80001cf483f9") )
            (  8000,                uint160("0x230758d96652745bc38dde5a9a5ac1d0a4bd92b8") )
            (  8500,                uint160("0x8f23009c18f9824140d6c1e8e14405b7f13f0c65") )
            (  9000,                uint160("0x3e707556553dc49325b01b302ad78e2da93c95c7") )
            (  9500,                uint160("0xd51eede1cb7679db50927135ec14380a9592bb48") )
            (  10000,               uint160("0x38ca18e508917089229497c7adc6a9afa489bffb") )
            (  10500,               uint160("0x524550e92874c57cf01f93b7e41a7a7e5a614ea4") )
            (  11000,               uint160("0x10440c5f22ff0d537ababfb00720eecebdf173ae") )
            (  11500,               uint160("0x6764352eb6f4991c1e4105d4fbf9058bc1c1239f") )
            (  12000,               uint160("0xe8b4922fb88b665273e37187979805d0ebdbae28") )
            (  12500,               uint160("0x7252cae5d7bd441be11550f5703f348c16c5a4b3") )
            (  13000,               uint160("0x359f4b2a282e6532f8194f11c0f8905eb333a736") )
            (  13500,               uint160("0xd41ad3da7aaae211a5b0da94c1e768910594a441") )
            (  14000,               uint160("0xf9f88dcd060725db0e4e0f24981a17029b8eaf81") )
            (  14500,               uint160("0xeaaf1e30cd12281ddfcff2f68e48d00c060595d3") )
            (  15000,               uint160("0xe0d0cc070a8bbc3754f11bbc2c96b0beb27ca189") )
            (  15500,               uint160("0xc4e5848e254943303d81a63e29f551895005cb96") )
            (  16000,               uint160("0x3e45c0607dadb8b7606af5d441fba30b1a84a43e") )
            (  16500,               uint160("0x38fe60fb7bf7e6af96890d4585880731fc65f52d") )
            (  17000,               uint160("0xa6b5d0d38ea1fc25625ba0aafd751c6d90bbf83b") )
            (  17500,               uint160("0xf3e326a504bfc65df201dff20d454aadd4e5dd4e") )
            (  18000,               uint160("0xf3e686d342f8f56b44e65eef198f396d6fdd4abf") )
            (  18500,               uint160("0x08a4ab948e2ce01d91cc4c13a7fa55ee71cc3e75") )
            (  19000,               uint160("0x0286abd5a209bedd89a556665a4ee9b5f4868f5c") )
            (  19500,               uint160("0x2bd726fd2f92ea8992e3c4d0a3c348405dfc52e0") )
            (  20000,               uint160("0x623dcc8f043f930ec34133dc5ef749eca5fec604") )
            (  20500,               uint160("0xeed6417f585ec78ce2324b1d75d0b6e5ca98d274") )
            (  21000,               uint160("0xf0786a2cb056bbceadb5333399c794c82a4c8e9c") )
            (  21500,               uint160("0xfdf24a7f40809771fdfd96e05c0a552ff002c992") )
            (  22000,               uint160("0x8de8ecdbca67d9e304c82e18ee8a58cca3b5bb59") )
            (  22500,               uint160("0xa964631a43ff5fb5be98c2cc8ff98552562ef6ce") )
            (  23000,               uint160("0xb6c7cf0ebdfbb0ecc47eb0f61896d9455e83ad31") )
            (  23500,               uint160("0x0cb4c315b499f36d90933d1865e41a8a57725dc1") )
            (  24000,               uint160("0xa3f53d1de100a2e4486bbd381ed2645bd121d5a2") )
            (  24500,               uint160("0xd241b0ab73c6142e572a1e5550ff1fa36248365d") )
            (  25000,               uint160("0x5fdd130185b41703239c844f822e492bae6b3702") )
            (  25500,               uint160("0xc529fd803fc3721d3b3a28932ae664327c8842c8") )
            (  26000,               uint160("0xeb71b025fef2c8e3cede98acec82b90b261dbddb") )
            (  26500,               uint160("0x4e314cb7d19a024b4921d84f4f65a18ac73df1dc") )
            (  27000,               uint160("0x1d2449a2b9808b383b84dccd2c9891f4cf2e09ec") )
            (  27500,               uint160("0x263a121453fffba9897e8efa3b28d026cbccbb51") )
            (  28000,               uint160("0x3df2af71928ae22226f1f906e7d841b61acd6832") )
            (  28500,               uint160("0xfd987ccd56483efb14cf994aa917429952aa9f46") )
            (  29000,               uint160("0xc65a222c8ef7c54a88bebd1d9b803d743cebf91b") )
            (  29500,               uint160("0xb1647356aa26b06ab80ee406ebdb81fde4639604") )
            (  30000,               uint160("0x5bc61bc7697019632d207c390db8edfafa748f33") )
            (  30500,               uint160("0xc0bb39890a6cb742dac1252774e6accca56aa9c4") )
            (  31000,               uint160("0x5dc72daa4180d21725d1520282cd8c2a387c9094") )
            (  31500,               uint160("0xc49d77f6168de58de02968da5615387c5fcb00c4") )
            (  32000,               uint160("0xa2f2bc847a2b8ef0dcfe430805ee8dc6b5d5ddc3") )
            (  32500,               uint160("0x4a62ad0d430fcb1868279f8b38d5334833c20776") )
            (  33000,               uint160("0xa1545793a1adecffab0bdb49c23e12b18bd79596") )
            (  33500,               uint160("0x5538236b3fa562267160802c9aa046321b6dcaca") )
            (  34000,               uint160("0x60e487419b50374bd8d118684bf6cabefa30adcb") )
            (  34500,               uint160("0x2862712d7ec437d06c052d4ce77fd10620620777") )
            (  35000,               uint160("0x2509197d83b41b722846e7fdea33890bd852226f") )
            (  35500,               uint160("0xb48b11d101d9963a02180d0d890419c44c579211") )
            (  36000,               uint160("0x8ae3616ee3ce4244191ba812b94734dc7493418a") )
            (  36500,               uint160("0xfcce1d0cccd898bf6a8a24c3e0761631ba36e094") )
            (  37000,               uint160("0xc658e77037893640f2906cb2758c8dbc2e0852be") )
            (  37500,               uint160("0xcf0aab56584b12ac1207825031e2e44dc894b52e") )
            (  38000,               uint160("0xa3f042b2606850d1c85c8be14b77a9858eacb14b") )
            (  38500,               uint160("0x42f1c0d2c7ad2f2a190e3de812cdafec0023bd38") )
            (  39000,               uint160("0x23f759d250b99e620ff904f13b9a01ccdfcabbc8") )
            (  39500,               uint160("0xf405524fe9073e76f62a1d71b77b0acc2c73038c") )
            (  40000,               uint160("0x4f2821e00dfdee9779162959716cf9b5f13a3d8b") )
            (  40500,               uint160("0x7e727227b6a5973398aff7bdd2d4c881c636f895") )
            (  41000,               uint160("0x3c934dbde06ced8ce1b55fe05e19364c26f077f3") )
            (  41500,               uint160("0xdaae509db580fa1bdcf3cd43b18bd30732da3675") )
            (  42000,               uint160("0x1db7f9d57b4dd1e2a1e8b7c8dd26eccc4aae6305") )
            (  42500,               uint160("0x975b33c38af0dd696450a8ee71b47582a3b7ea41") )
            (  43000,               uint160("0x95c64c5e74203a209bdca0d4ae5d31f9ed950111") )
            (  43500,               uint160("0x343b4fbfa89e5b90b122f5d7ea7a4907931dd27e") )
            (  44000,               uint160("0xa2795621d41a1ed1584dd1367f53c2ed09c4fab7") )
            (  44500,               uint160("0x6e757a4474d90cb86a9198466f354b616d2f1f16") )
            (  45000,               uint160("0x992c1d263fac5aacaeeee20eb4b53ce30eee5aba") )
            (  45500,               uint160("0x8df37eca999887a2572c27fea0e6a33f4f4364da") )
            (  46000,               uint160("0x1be48934b7d5949ba412168c8f310d81f3d53743") )
            (  46500,               uint160("0x75f1798d8883a5126874753b90ade76b7e072006") )
            (  47000,               uint160("0x1d3a82761ba56e56fee87cfdc3ab946fde22b91f") )
            (  47500,               uint160("0x7fba96613ebc9dabe8af40ec21ee2701e3012324") )
            (  48000,               uint160("0x34fc6df0f99056dfce65f39ef1ef316f757d5a2a") )
            (  48500,               uint160("0x24ce95358a5e5dd1538c2be19f798dd82162a9be") )
            (  49000,               uint160("0xbf6deb451fc85abe2965b0cde85b65bc22942c3f") )
            (  49500,               uint160("0x1c5f4527109237da1e49ed012bedf56cfd666a1b") )
            (  50000,               uint160("0x9e6cfb53f10a69612c86dee2d81eea7692acb80d") )
            (  50500,               uint160("0xa042b4de86389635dbc2b5090bcf06fa5ad4ac24") )
            (  51000,               uint160("0xbe38db535fde45a0b6b40cca75d8e79f4b4ec583") )
            (  51500,               uint160("0xc70c84f79df409a79e887b2f0de9b8bc676a455d") )
            (  52000,               uint160("0x05d009eadc52c5891d0a306c6e5d229c44e035ff") )
            (  52500,               uint160("0x6a11a83a1df6b356f8e0625c47b5147163f3163e") )
            (  53000,               uint160("0xf3736f9d80c727544ac2be8f7a44a066284df7dd") )
            (  53500,               uint160("0x45f035e58a9f90eb0d139f3d4165a0dc707beb3d") )
            (  54000,               uint160("0x9bd8fa393c83d996a87ae38b23aa4432b6174ba7") )
            (  54500,               uint160("0x75851bdac20411c766809419e0611a17458c1fec") )
            (  55000,               uint160("0xab4c3c6f9f31609b251026e411a929725ac2db4a") )
            (  55500,               uint160("0x943ae9e6fab62525acdd55c703e97e03c8936fb3") )
            (  56000,               uint160("0x59aeb03d21909729df3a6e6b33dc13ad3937507f") )
            (  56500,               uint160("0x794b6b2ad8b19ea238f2525e6933345c9f672d6b") )
            (  57000,               uint160("0x30ed9e0c407c84d58723578ce1a793d54499381e") )
            (  57500,               uint160("0x7f0271a873e6a7ee02bebd70cda9388594018e2e") )
            (  58000,               uint160("0x34f29c135edf1f49b785a1be0572fa00ce2d37db") )
            (  58500,               uint160("0xc1e5669a110d20331e0ebba0177476686548bbd5") )
            (  59000,               uint160("0xee70957b964be65b8e145284b642a0c39a1f52ad") )
            (  59500,               uint160("0x055a2db2988118c352c232c19fd39eaee452785a") )
            (  60000,               uint160("0xc9f471bdfa32c74b9845bf43aa0f516b3647c8ca") )
            (  60500,               uint160("0x1eaeaac16ef04578d2e2d8d94f1769ff6d880c3b") )
            (  61000,               uint160("0xd8f11433445e189b8f63698522b22a1a00ffa85b") )
            (  61500,               uint160("0xc92aacc4062b2c96cd670aae3d7341d2e68464f3") )
            (  62000,               uint160("0xbaa866662e25e155d49176ba598907b0026a711f") )
            (  62500,               uint160("0xaa3ef3bc38a6919adf0e1b93b7d7d6d1b4df998a") )
            (  63000,               uint160("0xef826766b40b3433be3b0affdaf68eb3d0bbb27a") )
            (  63500,               uint160("0xd6a3bb1a24fa67e1e113c7d539ad1ef61e02b617") )
            (  64000,               uint160("0xc30affe04dea3cb208e105d2686117a2eaf902ff") )
            (  64500,               uint160("0xc70d38e8f3964b53b51c37a86af66855796257e0") )
            (  65000,               uint160("0x2b23cd841df011c3315c2a9cd1471a12c1e66daa") )
            (  65500,               uint160("0xc930f29ab7da9b24f19dfbaa252fed47b25d1938") )
            (  66000,               uint160("0x8029386631cbd813a539ae0af99957eabe22d35d") )
            (  66500,               uint160("0x31d3a4eb9222c2a94dfcb3325a17f415b857ee95") )
            (  67000,               uint160("0x8ac5df50c2da18450578e0deda46dce9da8b41da") )
            (  67500,               uint160("0x443a85e2937c11c1bb9f1a83c33aade90e32368d") )
            (  68000,               uint160("0x7f978b2c906a89faf5b7bd4de51db848eb8d3c6d") )
            (  68500,               uint160("0xfe3f6893b50e86eb77b11e767e9777064fd2db87") )
            (  69000,               uint160("0x2fd94711d9f49bb570a5722ae3d34cc646b6ea0e") )
            (  69500,               uint160("0x02b1e1b18f56bd416fdce84959c67b32fe7e4b03") )
            (  70000,               uint160("0xecb66439e419db382df17536506c6cf77535bf7d") )
            (  70500,               uint160("0xffcb7287e470ed9acd73188e4868376e87c503fe") )
            (  71000,               uint160("0xf83970019f3cd4172d856571a461cbfe4666f31b") )
            (  71500,               uint160("0xc0b39b0d488f8311daa65e3709010d1ab0650f3f") )
            (  72000,               uint160("0x5ed9dc943fda625c8ed0e8a8b53fca865b1f59c4") )
            (  72500,               uint160("0x53a0c7f2e31a717a8bdf99336ca4ce7ff9bc44ac") )
            (  73000,               uint160("0x7d118fdba1c5d7ded952a794c3db2d00a0495ac8") )
            (  73500,               uint160("0xbe23de05dfa44b6da93e54e729fb614292cc7689") )
            (  74000,               uint160("0x2fb450fd79eadcff24b7310ee72ceac9dc9d3d02") )
            (  74500,               uint160("0x47d43dbf876939ae272ad7d2a793437358baead7") )
            (  75000,               uint160("0xe4c23a2512117dac5b0b6579f82918b527bf5dae") )
            (  75500,               uint160("0x6ea9a4162a086e493e48c9f998b52bb2f10fccbd") )
            (  76000,               uint160("0x8c07991f6e22b3d28293e46bc8f85c1181fb652c") )
            (  76500,               uint160("0xb09d7684b2a56abbb81671f3484acf385909ecbd") )
            (  77000,               uint160("0x715674570600dfa6b3901b3eb4ced272acc7015f") )
            (  77500,               uint160("0x0eae7c84cd8cfd992efcdda1ea80a2e46e6c8620") )
            (  78000,               uint160("0xc39a64be30212a2fa27423c59af77d490c843b87") )
            (  78500,               uint160("0x17f49a757245d846eafd0db53e9ad2148afb1c9a") )
            (  79000,               uint160("0x544953712ae795951708c294433e8ef7bc279be1") )
            (  79500,               uint160("0xab6da5b1669bbeb69765448d919067ea1513125b") )
            (  80000,               uint160("0xec3d4c0b1ccf99f4a0fdb7e752126dd910fbd430") )
            (  80500,               uint160("0x67ac30b850e5de0184bc623a7932d99bcf9b0a92") )
            (  81000,               uint160("0x025b10cb70b81d154305adc899492866525901f2") )
            (  81500,               uint160("0x6480c4920019909b3cfd37d9d61c96abcfd15666") )
            (  82000,               uint160("0xa67a52c9cb2f71a5a938e5217f90ac12f9b91346") )
            (  82500,               uint160("0x2970d9ac6aae4a9520a94e31fb6f66c6d2bee17d") )
            (  83000,               uint160("0x7788d323ae5b15c52485d9ca6858ae35b5953b47") )
            (  83500,               uint160("0x05b2a02cc576aab22c24167544795e129d9b7cc4") )
            (  84000,               uint160("0xa65dbb9e6a431ba4e67e4c0fc921c2bac02dc823") )
            (  84500,               uint160("0x5882215eb89c6dfbc3b9394d580222fd3ae2f754") )
            (  85000,               uint160("0x5e80664c40edfa9ca9a51c736d78420b6961714e") )
            (  85500,               uint160("0x44ed7a5b082a9ebbfea06eeab1b8b391ef9ccc64") )
            (  86000,               uint160("0xa5909015f07b73eda3d7089dad275d6d25b83b15") )
            (  86500,               uint160("0x0be9fa9948c012829d28a5af86b404b45dc41907") )
            (  87000,               uint160("0xbda2c31d5b1afc2d7f7b007ae40cf70f23ca86fb") )
            (  87500,               uint160("0x32321e0e1a373ae981d4d63d11e3987385a8ae4c") )
            (  88000,               uint160("0xe32a4e59baf4f4288118663a2216445a995aa233") )
            (  88500,               uint160("0xbaada13ef0777b3d53fff19ae0dc4ddd42ae2864") )
            (  89000,               uint160("0xe9ddf6cf44c5cf19054bd2029e6e7426afe5a264") )
            (  89500,               uint160("0x148ab6a4dc70801abd690911ca5e6e00a8f67c58") )
            (  90000,               uint160("0x174b238019fb0024764d88d11ca52846b31a8049") )
            (  90500,               uint160("0xc372c780e76cd7da5b4f6223b5cc23128a0130ef") )
            (  91000,               uint160("0xef3cbb90e8130046c14f1e7d9af95b3ebb668f64") )
            (  91500,               uint160("0xb8452704475ef368c8851eab2169ecc54fedff72") )
            (  92000,               uint160("0x0a006562e03ee09749a385695f494a2ca8f49076") )
            (  92500,               uint160("0x427d31a7356bb07d6a8d96d7997c0f828a8deca3") )
            (  93000,               uint160("0x1589cf0c4d2b43bc5e1f4d3bee027640292de457") )
            (  93500,               uint160("0xbdffc0f71446b61b9f1bc51655fe97f3b248994c") )
            (  94000,               uint160("0x9809dffd6ffee8f9a6ee12818eb794646f99de10") )
            (  94500,               uint160("0x678ed95f05c9a896600f3677c97e7c8ad787a42a") )
            (  95000,               uint160("0x1d6ae50fae0fea862cc65beed0f50a7496e1692f") )
            (  95500,               uint160("0x3c08207136a635024226afdd2e2c6237161a7917") )
            (  96000,               uint160("0x0e7395f5c97ac1d04a4cfac9f8d92fc102e9bdba") )
            (  96500,               uint160("0x2af034297124b5508a22f799728922f5e01248fb") )
            (  97000,               uint160("0xb862616fc5a089554dd87d14fdaa26d9e32b15f6") )
            (  97500,               uint160("0xeb97b2cde8e0855607ed764f8a7681a4966f1356") )
            (  98000,               uint160("0xc176c20d40a3ebde5415c4890e91b9b3801185c0") )
            (  98500,               uint160("0x15fa39e4d7439b30698c062f1e89eb64b36d170d") )
            (  99000,               uint160("0xcc9168db059f4c7fe7cf92bc348cb510f504bc8f") )
            (  99500,               uint160("0x9d43361b307341b9bdf1ba4a77f4055e674b642f") )
            (  100000,              uint160("0x67e2556f80ca3dcd7e93d1e6c4a38eaf111ded3c") )
            (  100500,              uint160("0xf1379c802f3e4a0e8778ad98e1b23b301cf3062c") )
            (  101000,              uint160("0x658efc2b4fb20b88099207c6e47130219f0ace62") )
            (  101500,              uint160("0xb4a3d6aa3f07445fc36a495bd80fc729a48210e4") )
            (  102000,              uint160("0x4fe651a0ffe90bb5106aa51c7d29ef48769b67e0") )
            (  102500,              uint160("0xa66873736d12564b6d96afdb0bc25038f90fa052") )
            (  103000,              uint160("0xa69caf30515068db44be3e87a2a75d8f5d9d3f55") )
            (  103500,              uint160("0xed5816b04d7129aedd812cc7cd96e609ed38283b") )
            (  104000,              uint160("0x84171272e567f27ea7dd73f7c3b60044d66ab0f5") )
            (  104500,              uint160("0x3f102f8fd267f119cd303a49d690b4760ad7bb0e") )
            (  105000,              uint160("0x0b193bb987c7534976b278dd0fd5c7dce08b8cce") )
            (  105500,              uint160("0x97aa454a9b5215cbe9fd4b46161dd23b5000b57f") )
            (  106000,              uint160("0xedcef14484b8874498dbbf23539e4fab63a97024") )
            (  106500,              uint160("0xe681b5b774a67b07b5b77e454fa57f4937dfd891") )
            (  107000,              uint160("0xe5f9a5ca069b737b095e1df4ede104b3e01069a9") )
            (  107500,              uint160("0x3de362f9adeb368c79d1e7b32db9607cf9417575") )
            (  108000,              uint160("0x44264f8a5f0ff6e2e048568bf0391a0bab14c2cf") )
            (  108500,              uint160("0xfec61d9298bc86ae2d3839cabd363a8caf43beaa") )
            (  109000,              uint160("0xb365d09d3357255e8fa0f4bd8ef7c0e67bccc7b6") )
            (  109500,              uint160("0x4a4fb441621050eb42cc44ad77b8cf899d1efce3") )
            (  110000,              uint160("0x2c4f54ab992ef2d43e05e5494cb3e1fc79f9f7eb") )
            (  110500,              uint160("0xef704a4ad49b4839e75f09adaf49f9f913ff717e") )
            (  111000,              uint160("0x78e840cf4f34e68014dd07ad0aaf706e0fa29dea") )
            (  111500,              uint160("0xdcfb90f44889df5607c75920abc1cf13fcd258c7") )
            (  112000,              uint160("0x7e38c5ee6cc6eede85d57069b9012ce418f905eb") )
            (  112500,              uint160("0x08a39bac217c5926e461df79efe19b2202aceeb6") )
            (  113000,              uint160("0xdcaebd75fc332a6848e99d65bf2a719df5961256") )
            (  113500,              uint160("0x5cc189f2bf9ed21e71c5042a6cb04a735dc8c7cf") )
            (  114000,              uint160("0xa32d4208db5c79d41359e872daceaabebe86ff6a") )
            (  114500,              uint160("0x8d13eaddb2f5db3029cb18be1053a75f00b5ea3e") )
            (  115000,              uint160("0xe7119f2b2ceb8f9905bff1c31bb46d563a3e1a45") )
            (  115500,              uint160("0xf48cddf0e72dd1b8f142336652686002abbc5806") )
            (  116000,              uint160("0xbca3a7288db1e70a6181db6e71d8edd92b825b65") )
            (  116500,              uint160("0xf74bdec0bdf3a6a69e26cebee9f752469a5b1ba1") )
            (  117000,              uint160("0xf69e895de76db369d8a45360b7e0be71e7759686") )
            (  117500,              uint160("0x20f5981067be01ef89066b66b86d2d1ed52ed73e") )
            (  118000,              uint160("0x69326657f59bd98097d2188c5cbf6ca5181f2219") )
            (  118500,              uint160("0xc2ba7f93a7b41d3377bbca76db9868183d03fbc1") )
            (  119000,              uint160("0xe0020402428bb438eff98010a613d1b6fda14355") )
            (  119500,              uint160("0x6f84cfbeeaefc1fdd416ec9c829f70adfa7b71a7") )
            (  120000,              uint160("0x814e8824f82e9518e0e11335c3a1a36b0d5c98e8") )
            (  120500,              uint160("0x2e1b5ef743ffeac611467c9f4a5aaebb7c3f9003") )
            (  121000,              uint160("0x2f9c1ed2f3d4f9cdd9d427071aea1ea8ada61477") )
            (  121500,              uint160("0x9e2b988a1b0bd68a783c87c9c4120cd94a1432de") )
            (  122000,              uint160("0xadeb6dff7d36f7eb17cfbd564731ef15a6e7ba7f") )
            (  122500,              uint160("0x9c284ec11e4696d008239b17ca63124cf71fae7a") )
            (  123000,              uint160("0xbefa7f75e27f2ba5f01763ca22680fa3e8d1044d") )
            (  123500,              uint160("0xd23187d2021fea6432fa93da562ecb6f2851e23b") )
            (  124000,              uint160("0x7da34c2c21027ee58f02f98cc10c7a7dd1b9a008") )
            (  124500,              uint160("0xe74f2cae36089ae39f86f83d746811fc5818a43f") )
            (  125000,              uint160("0x03c4af3b86bc6f909a19ba22be7d05add97ff458") )
            (  125500,              uint160("0x59b652b6b94bb5fe2e700d2fed46b76c4be2a740") )
            (  126000,              uint160("0xca03094501abf9adc46ff429a30ea60e1ead62e5") )
            (  126500,              uint160("0x5bbaf79515213b2ee22f181906497c7adfd97c4e") )
            (  127000,              uint160("0xd5401a7a5015f8295f6ca7c7f40c8caca3f2e944") )
            (  127500,              uint160("0xf51f39952d83b4cefe27142da9ad06baac9eb020") )
            (  128000,              uint160("0xd8a2ad9cd1debe3112ef6893bf61587bc61add1a") )
            (  128500,              uint160("0xe045f692f47bc776acd4c5ae6ced6fd42677ccda") )
            (  129000,              uint160("0x72835720f0229aefe5287b585bd2d57ea725e636") )
            (  129500,              uint160("0x03c42355cd08d9db93a70f002a0e11a517f723e0") )
            (  130000,              uint160("0x0e56e1de3d7d82d15de2e43b967d0408ced9c902") )
            (  130500,              uint160("0xa7e3abd0370cc75fc7dfa7658cce3c6ca9117347") )
            (  131000,              uint160("0x67ecdc378db3cb7cda7ac932dd972cfe040c3d44") )
            (  131500,              uint160("0xe165b0283fdd951c851e753e300677a62e456ae3") )
            (  132000,              uint160("0x43a51bf37133d162dd82bb5305491f8bb7fd6335") )
            (  132500,              uint160("0x0eafa83f7b25ac0d411d8a41c7ab6864057b548e") )
            (  133000,              uint160("0x8de455f59ef1f96691780009bfcf240c8721fe8b") )
            (  133500,              uint160("0x0bb480b9065ad8edd535d9205609993c441a06bb") )
            (  134000,              uint160("0x135b30a06e47c4832cc3fbc1141c1daf41e5f4bc") )
            (  134500,              uint160("0x2d41320053f6bce3fa72ceea963e14533ad6e868") )
            (  135000,              uint160("0x1425933167cea3faba28f10acaea72b6198aedef") )
            (  135500,              uint160("0xbcde8cf0da4721aed57b34ff813a394615b6d38e") )
            (  136000,              uint160("0xeb03434125c65ff7eea15416e59d3b7fb49f24b0") )
            (  136500,              uint160("0xa415852e849b4a51cd3a4b63ecb8cde41565979e") )
            (  137000,              uint160("0xe1f49e757418d138d1ea4a5aee515af91badba9d") )
            (  137500,              uint160("0xc335bdee7bd27af281ff0dd9aea7cf50b4c273f8") )
            (  138000,              uint160("0xe81c0687a2550801a1232b9b81590b194ca7b545") )
            (  138500,              uint160("0x045c66c4c4eb52050e81a65a4e477757dce1f8c0") )
            (  139000,              uint160("0xe8ba713171fe89ed87f896c8fb224d80bbc90612") )
            (  139500,              uint160("0xdee7ca8bb7e2d5688820c220ab8171dbe9a03796") )
            (  140000,              uint160("0x26d3ef76a474e12ee579e1dad6889a922aa2dd7a") )
            (  140500,              uint160("0xcc3fc59a0b0c7b24ba8f66fa2e454000bc914370") )
            (  141000,              uint160("0x9babef5db77ca2fcda0aa00165d92df2948fc742") )
            (  141500,              uint160("0x748d70ec45fbb2aa30c8c47c75018712182e19c6") )
            (  142000,              uint160("0x3b6bc9d563a6aae07b4478b8ab4174bfbcda96b8") )
            (  142500,              uint160("0xe6d296289495843546a28cb8ceeecd7ec62fdafb") )
            (  143000,              uint160("0x536b62c6de4190b072a2d079c07b8a0bb4bec93a") )
            (  143500,              uint160("0xeb192aed49d2030fee3be21dd12a9878b61f5691") )
            (  144000,              uint160("0xbb7f6e9dbe07347a7dd12a045e0133d2187eaed1") )
            (  144500,              uint160("0x51dd312131eef5b2bedf1d611b5f971f102e23be") )
            (  145000,              uint160("0x91e595df1ba38fb24e873e715fd693a7a01ed971") )
            (  145500,              uint160("0x304758e4e855aa06ca1d9bd0f2b14d4fd7288f57") )
            (  146000,              uint160("0x77e20483efe23856933826636a1069385ed4d9fb") )
            (  146500,              uint160("0x94303761a8696f87647e9580698d1b8da254e2e8") )
            (  147000,              uint160("0xe51e5675b576eac2b656113331c9d5e2a6ee8d70") )
            (  147500,              uint160("0x82e7be86cc2ddacf4d8280c36dd4adbcfc626e4c") )
            (  148000,              uint160("0x6e178129e7d1b4ae098b13988c21c051fb4f0bee") )
            (  148500,              uint160("0x814a19cafabdfaf47ace359ea8b12ee8f2a0fcef") )
            (  149000,              uint160("0x33864ef5040aec70763222d5985d49bedfd0dabf") )
            (  149500,              uint160("0xe3fa13be92ce440837624173dd8a339f78062ae7") )
            (  150000,              uint160("0xc1c3103a1b0184c1a240a1135c411b680be9e537") )
            (  150500,              uint160("0x48c5af43ca05ebaf5f535f59798db324cee7eaee") )
            (  151000,              uint160("0x7e7cef7b032a0051f5f9162535864adb7f77289a") )
            (  151500,              uint160("0x74050483b9a80af0a199a853c926ade6caa47f34") )
            (  152000,              uint160("0xd107584f92b8f5ca018fd6d593df9c31e5251bcb") )
            (  152500,              uint160("0x2f11fd9949c07ff1f1d90fe7af5454071c75bef9") )
            (  153000,              uint160("0x2a315264a261fb70c3a716b479c3cd636bab5a77") )
            (  153500,              uint160("0x2de9155fff81546b02a2475baceb90ecd37f6f9f") )
            (  154000,              uint160("0x3eabb4a2a733215516a394a93c54219b789a56cb") )
            (  154500,              uint160("0x657b30ec76690bd6be55591f08aa15b5b45bb2e7") )
            (  155000,              uint160("0xb7fdb8925a35fa2e28f4bdf7cde7355b49e9811e") )
            (  155500,              uint160("0x439fc2fb6e0439ff51bc5380232f7562b24390e3") )
            (  156000,              uint160("0x067579c35e84d2f6ce36c6173d5deb20927cb496") )
            (  156500,              uint160("0x9a7fc91fa051bccdf48834d9756efc6259d19d7c") )
            (  157000,              uint160("0x3d9462564294c35a1377493cb02dbc931ab0f856") )
            (  157500,              uint160("0xb34a5da88a8b8c177c6b731d629be19686cf295a") )
            (  158000,              uint160("0x14a063ec71a3c3e9a5a294997346a2191bf32752") )
            (  158500,              uint160("0xff411aabbcbb51419b0e73e17892deaa9177f702") )
            (  159000,              uint160("0xe255d79d7884e086dcd7b1d8ae8d92e4cfd7a31a") )
            (  159500,              uint160("0x234a6d09401b2d7aac4873c291a1720f7e14cbd2") )
            (  160000,              uint160("0x9cb29bb3496b5d42ba002565f8bbc6db729b7956") )
            (  160500,              uint160("0xec76bbf4cfd09f4129d6705d75d21a4c34571c9e") )
            (  161000,              uint160("0x99613b0a4ff28ad0421f473477fbaa695257b9c6") )
            (  161500,              uint160("0x7fe44bd711e2899a935f1a3ad0040254617bd5f7") )
            (  162000,              uint160("0xd45fa82de0329e3a1779ad29ab5d52c9921a435b") )
            (  162500,              uint160("0xfadd541e2ea0cdc131314fcdafc11545918508e3") )
            (  163000,              uint160("0xd4deec44304715c3edcf0c2b38720b377d02f45c") )
            (  163500,              uint160("0x567cd751ec86d58b30e9f4b38889edb39fe8c518") )
            (  164000,              uint160("0xa8749515dfdde48a87dbcf65b9bcbfb9399276ef") )
            (  164500,              uint160("0xf93648d02866d50d3468713f9a168966453d2624") )
            (  165000,              uint160("0x253502ef89fe1cf02d87886196eac4f3ce2dd5fe") )
            (  165500,              uint160("0x6207e265ba182b1286101b689d3aa94c8ca6e3b8") )
            (  166000,              uint160("0x5ded819db46a47329a536337bd09433d5fe1b887") )
            (  166500,              uint160("0xdb0282d3fe5918ffc34c48ed57a832a1b04b3fae") )
            (  167000,              uint160("0xac8d9c13b2309120ec7a060ab2194b87da2be7c6") )
            (  167500,              uint160("0x8b29c7311a1c8343254d3b79a1f01036a5629617") )
            (  168000,              uint160("0x9df6e7c133662ba1c3984d0da85b67e8c415cb5a") )
            (  168500,              uint160("0x84c26cab0c9394e04c858fd89a79e39a1e91c4f9") )
            (  169000,              uint160("0xf7e83421362146af37d0ed058ea766f14d903145") )
            (  169500,              uint160("0x7324042e009e3cd2623df5f7ca59e1030fbe1dc1") )
            (  170000,              uint160("0xecd9e1994d7b7954a902daa26b7a83da56e77418") )
            (  170500,              uint160("0xc70b84b86b8ca8cc37a2951a1c0afe70ff31aa8e") )
            (  171000,              uint160("0xb17fb5d382d981b5a576e808f4f69e342356ce07") )
            (  171500,              uint160("0xa78a68e780cd8532c9e7c3637a6556a61e9f632c") )
            (  172000,              uint160("0x24e31668dbe1e683aeabfb4dced561104452fc3a") )
            (  172500,              uint160("0xa47712f9af2566e66fe6e58b1d307ffcbdab40fa") )
            (  173000,              uint160("0x89667e9e264db9bc1c771f18689f0bf2134ffbf2") )
            (  173500,              uint160("0xfff20e0c0656eb5f64b3ea2090643668ed144fd3") )
            (  174000,              uint160("0x0245ec92e5ccf46ca0a057e5a8909d526e4eb5ae") )
            (  174500,              uint160("0xe3b5c8e427d9236282752adf276f894eb0d68b79") )
            (  175000,              uint160("0xa23b75cef46fd2ac6e2bb371317a00ce399a77dd") )
            (  175500,              uint160("0x0311aa6ad87d5d67d42a44b3f02a0cabe2ce1b22") )
            (  176000,              uint160("0xb8b798ed04f4dbadc8f3e698dca1ae8566b5ef82") )
            (  176500,              uint160("0x4077645bd7650b0917b9700ecc1058ce4addb3c3") )
            (  177000,              uint160("0x9c1e510ca88b03aae71c6dafe4cd614a7d7662ac") )
            (  177500,              uint160("0x301c92ee2a3f411625ec7abb86868524dd12fd22") )
            (  178000,              uint160("0x12afd7bdbb34382c7dfbbca23b13c5b99e3ca346") )
            (  178500,              uint160("0x0e235ccad7cf3c247aec90830d0e5544150616fd") )
            (  179000,              uint160("0xb602a38d45beab7b7471764837419718133dd436") )
            (  179500,              uint160("0x097302065f4013ae090ce7eac9024dc10b820b07") )
            (  180000,              uint160("0xf303eaa66aefb1471c84c7b96d879d574f96c5b3") )
            (  180500,              uint160("0xce77598d586adc91171e4b6af2a0105534063670") )
            (  181000,              uint160("0x3b411b973e68de6d7279bd278182694be5f5a54b") )
            (  181500,              uint160("0x7b528cc59054400d40b15df31949ffe5fdde278e") )
            (  182000,              uint160("0x731d58f05f7e4bc8702422f58a9a43d609625551") )
            (  182500,              uint160("0x1611936f362e1872fe84048914ca0b009f47c320") )
            (  183000,              uint160("0xb05708dbd01a5de7ebc5e5ccf677b6a74a6d3717") )
            (  183500,              uint160("0xfedaedda0e77e37c8336ce97a305eae33ca5fe85") )
            (  184000,              uint160("0xeae3e59192ec766b388a785a1f45da39a2e43b5e") )
            (  184500,              uint160("0xedee1ba2ce606b987b75755d7f751d473b7b4048") )
            (  185000,              uint160("0xa51ade2a47a716ea633d2073e97568def83a1d1f") )
            (  185500,              uint160("0x598a29a7162e3b8c4ed831540386c2d5e16a0bd1") )
            (  186000,              uint160("0x89e7ecd23cb09b9d967f3bfa93ebbbf6f70c8d09") )
            (  186500,              uint160("0xaee34fffbfcfca21c797981299a04372868db783") )
            (  187000,              uint160("0x8f674cce892a1aa0a80b0786eb6b845388ca9082") )
            (  187500,              uint160("0x816b59afac005d927b6e5ad0e117214a03802f32") )
            (  188000,              uint160("0x1778856ff6ae9248c70729e8871083b0ec40caf2") )
            (  188500,              uint160("0x3bb26682c6f9c3d6345ff175c4285d02e094d9ee") )
            (  189000,              uint160("0x25078174da06b08b79a92e40d8b77909d64e6612") )
            (  189500,              uint160("0x978f3b2b304f038547f5803a63f8c07a196d646b") )
            (  190000,              uint160("0x3b747a01087c1607cdd7ca0855d049b8281be1fd") )
            (  190500,              uint160("0x953bfbc176e5d95e0242a067c169dffd887f9d7b") )
            (  191000,              uint160("0x8913303f59ae6372463246051ca8e8b7385eced2") )
            (  191500,              uint160("0x8d1d6a9e07721075ac41fdbcedbed6e9c95e9711") )
            (  192000,              uint160("0x61c9a18aa6b67442ad07b09a73cb42f0a11291b1") )
            (  192500,              uint160("0xb70ba05ebdde45655067d23562ecf7633dfc7fc4") )
            (  193000,              uint160("0x174c4a079d16f601feb9a55be9ec976e611bfb05") )
            (  193500,              uint160("0x2144d8223293f69a4e773883f34a202d04d23567") )
            (  194000,              uint160("0x5ae7bec7fa2ee1ffde5c269de2391504a36c7dcf") )
            (  194500,              uint160("0x2902c3fcdd4fcaa399056fd0db7e32c5e06ec38a") )
            (  195000,              uint160("0x2a7fe7cba2afacc143924b44da8162853055c74b") )
            (  195500,              uint160("0x5c082c2d4fb06e0469d31d7acf707efdef266511") )
            (  196000,              uint160("0x21a82638570b4864c4c5ba8b0af1efeb687ddbb1") )
            (  196500,              uint160("0xaba0256c92c664b8b058617e2bb644758fc06a06") )
            (  197000,              uint160("0x96af17f787a9a5e38ee6663e2dd2ecce6c8175e9") )
            (  197500,              uint160("0xb34329e2ebf84b43d417b58e9e09fab5a2135079") )
            (  198000,              uint160("0xf98f05b5246c0fadd506d2c946aa4eb5d9291cc3") )
            (  198500,              uint160("0xc0301d03778d4c65ce614bf1913f2565a993949e") )
            (  199000,              uint160("0x08ceb9ebd75cef1b23194ccece70e92fbc183036") )
            (  199500,              uint160("0xe63c4a2cc28102e8bb0a7454baa4e83a8abcc028") )
            (  200000,              uint160("0x4bc216ad328a671555f2040c0b4c77a78db7665b") )
            (  200500,              uint160("0x74141ca390192a922b2c467cf33027926cde1908") )
            (  201000,              uint160("0xaeee50cc35c57818ed0cda2eedde8de7faa8631f") )
        ;

    // TestNet has no checkpoints
    static MapCheckpoints mapCheckpointsTestnet =
            boost::assign::map_list_of
            ( 0,           uint256("0x000098b61e0e462e7c95e6e999d5b2f4f14cd89d6712511187ee86781bc87451") ) // genesis

         ;
    static MapSupperCheckpoints mapSupperCheckpointsTestnet =
            boost::assign::map_list_of
            (  500,                 uint160("0xdee9ceaa56f945516f5a037741e8f44358b0d192") )
            (  1000,               uint160("0x19632bc7c7aff3ce9d98b6969694715db48af7d7") )


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

    uint160 GetSupperBlocksEstimate()
    {
        MapSupperCheckpoints& checkpoints = (TestNet() ? mapSupperCheckpointsTestnet : mapSupperCheckpoints);

        if (checkpoints.empty())
            return 0;
        return checkpoints.rbegin()->second;
    }
    SUPPER_CHECK_POINT_TYPE GetSupperCheckpoint(const uint160& Checkhash)
    {
        MapSupperCheckpoints& checkpoints =   (TestNet() ? mapSupperCheckpointsTestnet : mapSupperCheckpoints); ;

        BOOST_REVERSE_FOREACH(const MapSupperCheckpoints::value_type& i, checkpoints)
        {

            const uint160& hash = i.second;
            if (Checkhash == hash)
            {
                if(i.first == checkpoints.begin()->first )
                    return SUPPER_CHECK_LEVEL1;
                else if( i.first == checkpoints.rbegin()->first )
                    return SUPPER_CHECK_LEVEL3;
                else
                    return SUPPER_CHECK_LEVEL2;
            }
        }
        return SUPPER_CHECK_LEVEL3;
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
