// Copyright (c) 2016-2023 The CryptoCoderz Team / Espers
// Copyright (c) 2023 The Diminutive Coin Team
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "blockparams.h"
#include "chainparams.h"
#include "main.h"

using namespace std;
using namespace boost;

//////////////////////////////////////////////////////////////////////////////
//
// Standard Global Values
//

//
// Section defines global values for retarget logic
//

double VLF1 = 0;
double VLF2 = 0;
double VLF3 = 0;
double VLF4 = 0;
double VLF5 = 0;
double VLFtmp = 0;
double VRFsm1 = 1;
double VRFdw1 = 0.75;
double VRFdw2 = 0.5;
double VRFup1 = 1.25;
double VRFup2 = 1.5;
double VRFup3 = 2;
double TerminalAverage = 0;
double TerminalFactor = 10000;
double debugTerminalAverage = 0;
CBigNum newBN = 0;
CBigNum oldBN = 0;
int64_t VLrate1 = 0;
int64_t VLrate2 = 0;
int64_t VLrate3 = 0;
int64_t VLrate4 = 0;
int64_t VLrate5 = 0;
int64_t VLRtemp = 0;
int64_t DSrateNRM = BLOCK_SPACING;
int64_t DSrateMAX = BLOCK_SPACING_MAX;
int64_t FRrateDWN = DSrateNRM - 60;
int64_t FRrateFLR = DSrateNRM - 80;
int64_t FRrateCLNG = DSrateMAX * 3;
int64_t difficultyfactor = 0;
int64_t AverageDivisor = 5;
int64_t scanheight = 6;
int64_t scanblocks = 1;
int64_t scantime_1 = 0;
int64_t scantime_2 = 0;
int64_t prevPoW = 0; // hybrid value
int64_t prevPoS = 0; // hybrid value
uint64_t blkTime = 0;
uint64_t cntTime = 0;
uint64_t prvTime = 0;
uint64_t difTime = 0;
uint64_t hourRounds = 0;
uint64_t minuteRounds = 0;
uint64_t difCurve = 0;
uint64_t debugminuteRounds = 0;
uint64_t debugDifCurve = 0;
bool fDryRun;
bool fCRVreset;
const CBlockIndex* pindexPrev = 0;
const CBlockIndex* BlockVelocityType = 0;
CBigNum bnVelocity = 0;
CBigNum bnOld;
CBigNum bnNew;
std::string difType ("");
unsigned int retarget = DIFF_VRX; // Default with VRX

//////////////////////////////////////////////////////////////////////////////
//
// Debug section
//

//
// Debug log printing
//

void VRXswngdebug(bool fProofOfStake)
{
    // Print for debugging
    LogPrintf("Previously discovered %s block: %u: \n",difType.c_str(),prvTime);
    LogPrintf("Current block-time: %u: \n",difType.c_str(),cntTime);
    LogPrintf("Time since last %s block: %u: \n",difType.c_str(),difTime);
    // Handle updated versions as well as legacy
    if(pindexPrev->GetBlockTime() > VRX_V3_6) {
        debugminuteRounds = minuteRounds;
        debugTerminalAverage = TerminalAverage;
        debugDifCurve = difCurve;
        LogPrintf("VRX_Threadcurve - Previous difficulty found: %d \n", VRX_GetPrevDiff(fProofOfStake));
        while(difTime > (debugminuteRounds * 60)) {
            if(VRX_GetPrevDiff(fProofOfStake) < 1) {
                // Skip
                LogPrintf("VRX_Threadcurve - Difficulty too low for curve, skipping \n");
                return;
            }
            debugTerminalAverage /= debugDifCurve;
            LogPrintf("diffTime%s is greater than %u Minutes: %u \n",difType.c_str(),debugminuteRounds,cntTime);
            LogPrintf("Difficulty will be multiplied by: %d \n",debugTerminalAverage);
            // Break loop after 20 minutes, otherwise time threshold will auto-break loop
            if (debugminuteRounds > (5 + 15)){
                break;
            }
            debugDifCurve ++;
            debugminuteRounds ++;
        }
    } else {
        if(difTime > (hourRounds+0) * 60 * 60) {LogPrintf("diffTime%s is greater than 1 Hours: %u \n",difType.c_str(),cntTime);}
        if(difTime > (hourRounds+1) * 60 * 60) {LogPrintf("diffTime%s is greater than 2 Hours: %u \n",difType.c_str(),cntTime);}
        if(difTime > (hourRounds+2) * 60 * 60) {LogPrintf("diffTime%s is greater than 3 Hours: %u \n",difType.c_str(),cntTime);}
        if(difTime > (hourRounds+3) * 60 * 60) {LogPrintf("diffTime%s is greater than 4 Hours: %u \n",difType.c_str(),cntTime);}
    }
    return;
}

void VRXdebug()
{
    // Print for debugging
    LogPrintf("Terminal-Velocity 1st spacing: %u: \n",VLrate1);
    LogPrintf("Terminal-Velocity 2nd spacing: %u: \n",VLrate2);
    LogPrintf("Terminal-Velocity 3rd spacing: %u: \n",VLrate3);
    LogPrintf("Terminal-Velocity 4th spacing: %u: \n",VLrate4);
    LogPrintf("Terminal-Velocity 5th spacing: %u: \n",VLrate5);
    LogPrintf("Desired normal spacing: %u: \n",DSrateNRM);
    LogPrintf("Desired maximum spacing: %u: \n",DSrateMAX);
    LogPrintf("Terminal-Velocity 1st multiplier set to: %f: \n",VLF1);
    LogPrintf("Terminal-Velocity 2nd multiplier set to: %f: \n",VLF2);
    LogPrintf("Terminal-Velocity 3rd multiplier set to: %f: \n",VLF3);
    LogPrintf("Terminal-Velocity 4th multiplier set to: %f: \n",VLF4);
    LogPrintf("Terminal-Velocity 5th multiplier set to: %f: \n",VLF5);
    LogPrintf("Terminal-Velocity averaged a final multiplier of: %f: \n",TerminalAverage);
    LogPrintf("Prior Terminal-Velocity: %u\n", oldBN);
    LogPrintf("New Terminal-Velocity:  %u\n", newBN);
    return;
}

void GNTdebug()
{
    // Print for debugging
    // Retarget using DGW-v3
    if (retarget == DIFF_DGW)
    {
        // debug info for testing
        LogPrintf("DarkGravityWave-v3 retarget selected \n");
        LogPrintf("Espers retargetted using: DGW-v3 difficulty algo \n");
        return;
    }
    // Retarget using Terminal-Velocity
    // debug info for testing
    LogPrintf("Terminal-Velocity retarget selected \n");
    LogPrintf("Espers retargetted using: Terminal-Velocity difficulty curve \n");
    return;
}

double VRX_GetPrevDiff(bool fPoS)
{
    const CBlockIndex* blockindex;

    // Floating point number that is a multiple of the minimum difficulty,
    // minimum difficulty = 1.0.
    if (pindexBest == NULL) {
        return 1.0;
    } else {
        blockindex = GetLastBlockIndex(pindexBest, fPoS);
    }

    int nShift = (blockindex->nBits >> 24) & 0xff;

    double dDiff =
        (double)0x0000ffff / (double)(blockindex->nBits & 0x00ffffff);

    while (nShift < 29)
    {
        dDiff *= 256.0;
        nShift++;
    }
    while (nShift > 29)
    {
        dDiff /= 256.0;
        nShift--;
    }

    return dDiff;
}

unsigned int GetNextTargetRequired(const CBlockIndex* pindexLast, bool fProofOfStake)
{
    // DarkGravityWave v3.1, written by Evan Duffield - evan@dashpay.io
    // Modified & revised by bitbandi for PoW support [implementation (fork) cleanup done by CryptoCoderz]
    const CBigNum nProofOfWorkLimit = fProofOfStake ? Params().ProofOfStakeLimit() : Params().ProofOfWorkLimit();
    const CBlockIndex *BlockLastSolved = GetLastBlockIndex(pindexLast, fProofOfStake);
    const CBlockIndex *BlockReading = BlockLastSolved;
    int64_t nActualTimespan = 0;
    int64_t LastBlockTime = 0;
    int64_t PastBlocksMin = 7;
    int64_t PastBlocksMax = 24;
    int64_t CountBlocks = 0;
    int64_t nTargetSpacing = BLOCK_SPACING;
    CBigNum PastDifficultyAverage;
    CBigNum PastDifficultyAveragePrev;

            if (BlockLastSolved == NULL || BlockLastSolved->nHeight == 0 || BlockLastSolved->nHeight < PastBlocksMax) {
                return nProofOfWorkLimit.GetCompact();
            }

            for (unsigned int i = 1; BlockReading && BlockReading->nHeight > 0; i++) {
                if (PastBlocksMax > 0 && i > PastBlocksMax) { break; }
                CountBlocks++;

                if(CountBlocks <= PastBlocksMin) {
                    if (CountBlocks == 1) { PastDifficultyAverage.SetCompact(BlockReading->nBits); }
                    else { PastDifficultyAverage = ((PastDifficultyAveragePrev * CountBlocks) + (CBigNum().SetCompact(BlockReading->nBits))) / (CountBlocks + 1); }
                    PastDifficultyAveragePrev = PastDifficultyAverage;
                }

                if(LastBlockTime > 0){
                    int64_t Diff = (LastBlockTime - BlockReading->GetBlockTime());
                    nActualTimespan += Diff;
                }
                LastBlockTime = BlockReading->GetBlockTime();
                BlockReading = GetLastBlockIndex(BlockReading->pprev, fProofOfStake);
            }

            CBigNum bnNew(PastDifficultyAverage);

            int64_t _nTargetTimespan = CountBlocks * nTargetSpacing;

            if (nActualTimespan < _nTargetTimespan/3)
                nActualTimespan = _nTargetTimespan/3;
            if (nActualTimespan > _nTargetTimespan*3)
                nActualTimespan = _nTargetTimespan*3;

            // Retarget
            bnNew *= nActualTimespan;
            bnNew /= _nTargetTimespan;

            if (bnNew > nProofOfWorkLimit){
                bnNew = nProofOfWorkLimit;
            }

            return bnNew.GetCompact();
}

//
//    Diminutive Wave POW / POS  Protocol
//        Peak POW     Next Peak POW
//         #            #
//        # #          # #
//       #   #        #   #
//      #     #      #     #
//     #       #    #       #
//    #         #  #         # Graph Above  POW   #
//  --------------Midpoint breach----            # #
//   #           ##           # Graph Below POS   #
//    #         #  #         #
//     #       #    #       #
//      #     #      #     #
//        # #          #  #
//         #            #
//	 Peak POS      Next Peak POS
//   Block POW  1 - 2 reward Loop + Block POS 1 - 2 reward Loop with a halving sub

// miner's coin base reward
int64_t GetProofOfWorkReward(int64_t nFees, int nHeight)
{
    int64_t nSubsidy = 0 * COIN; //DiminutiveCoin Protocol rewards 0.01 DIMI curve up and down to 0.04 DIMI

    if(nHeight == 3)
    {
        nSubsidy = 11000 * COIN;
    }
        else if((nHeight > 12) && (nHeight % 2))
    {
        nSubsidy = 0.04 * COIN;
    }
        else if(nHeight > 12)
    {
        nSubsidy = 0.01 * COIN;
    }

    nSubsidy >>= (nHeight / 500000);
    LogPrint("creation", "GetProofOfWorkReward() : create=%s nSubsidy=%d nHeight=%d\n", FormatMoney(nSubsidy), nSubsidy, nHeight);

    // hardCap v2.2
    if(pindexBest->nMoneySupply > MAX_MONEY)
    {
        nSubsidy = 0;
        LogPrint("MINEOUT", "GetProofOfWorkReward(): create=%s nFees=%d\n", FormatMoney(nFees), nFees);
        return nSubsidy + nFees;
    }
    return nSubsidy + nFees;
}




// miner's coin stake reward
int64_t GetProofOfStakeReward(int64_t nCoinAge, int64_t nFees, int nHeight)
{
    int64_t nSubsidy= 0 * COIN;

    if(IsWavePOS(nHeight)) //Even blocks rewards 0.01 DIMI curve up and down 0.00001 DIMI

{
        if(nHeight % 2)
        {
            nSubsidy = 0.00001 * COIN;
        }
        else
        {
            nSubsidy = 0.01 * COIN;
        }

    }

    else
    {
        nSubsidy = 0.01 * COIN;
    }

    nSubsidy >>= (nHeight / 1000000);
    LogPrint("creation", "GetProofOfStakeReward(): create=%s nCoinAge=%d nHeight=%d\n", FormatMoney(nSubsidy), nCoinAge, nHeight);

    // hardCap v2.2
    if(pindexBest->nMoneySupply > MAX_MONEY)
    {
        nSubsidy = 0.0;
        LogPrint("MINEOUT", "GetProofOfStakeReward(): create=%s nFees=%d\n", FormatMoney(nFees), nFees);
        return nSubsidy + nFees;
    }

    return nSubsidy + nFees;
}
