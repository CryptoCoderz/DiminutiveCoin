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
         ( 0,         uint256("0x2ce9a9e271caa8fc164d29ea497f60f0a0f6a05998db2430bc5003da148b6699") )
         ( 6500,      uint256("0x0000000008aaf279a144c9fe085f76046eae800fe49e7743ef477cc1dea55946") )    // Version 1.0.0.2
         ( 20000,     uint256("0xd75b1c9bbae68bc5f1a556b82b851651a2bbd3701c0516bb6f4a34ae37425cc8") )    // Version 1.0.0.3
         ( 50000,     uint256("0x0000000007ea31edeeea9b5038047cac91639bfcac3f4876230deeb4e569449e") )    // Version 1.0.0.4
         ( 500000,    uint256("0x71ab8d761718638410f151b8b34290085fed410ec7a40d248b955a27e24acb6c") )  
         ( 938888,    uint256("0x000000019d99432d683aea41a13f6f0ee9d5e18866d955cadc135dba12d04132") )    // Version 1.0.1.0
         ( 1000000,   uint256("0x2d16597353d1229a6bfb0fcf3c217803c458ff667c50199886c88d59b387a12c") )
         ( 1200000,   uint256("0x6edb68e014b58d395e1b4ab73c8fd5fa93ece8ef2fa4d9ad8b083675ecb8eefd") )
         ( 1500000,   uint256("0xf6baaa66a04811a6b3cff52ce1009db0a3877682d86464f3d629e5e6269e2e23") )
         ( 2000000,   uint256("0x00000000503ee5a2905d7e15a36128bd6371625dff4df14ac8fdf8ccb8ee479d") )
         ( 2200000,   uint256("0x00000000a35b72dafcf3b62624380a8662f8fd12c10594831a3db4b19ea6b217") )
         ( 2500000,   uint256("0xbc1d838524de7fd49e3fffab146ad45062ca5082042fdadc12d4cf4a66c20787") )
         ( 2700000,   uint256("0x000000048111f876530734b82bcaa2fc032aef07d5952a11a4aa7dc605ec9526") )
         ( 2727900,   uint256("0x8da5c57a6299b38287073a506443e89034d8994f624a9d39e314452faa88a66c") )    // Checkpoint befor Hardfork to Version 1.0.1.2
         ;

    // TestNet has no checkpoints
    static MapCheckpoints mapCheckpointsTestnet;

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
        // Taking into account our 120 block depth + reorganize depth
        while (pindex->pprev && pindex->nHeight + (BLOCK_TEMP_CHECKPOINT_DEPTH + 500) > pindexBest->nHeight)
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
