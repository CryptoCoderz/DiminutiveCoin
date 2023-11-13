// Copyright (c) 2016-2023 The CryptoCoderz Team / Espers
// Copyright (c) 2023 The Diminutive Coin Team
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef DIMINUTIVECOIN_BLOCKPARAMS_H
#define DIMINUTIVECOIN_BLOCKPARAMS_H

#include "net.h"
#include "core.h"
#include "bignum.h"

// Define difficulty retarget algorithms
enum DiffMode {
    DIFF_DEFAULT = 0, // Default to invalid 0
    DIFF_DGW     = 1, // Retarget using DarkGravityWave v3.1
    DIFF_VRX     = 2, // Retarget using Terminal-Velocity-RateX
};

void VRXswngdebug(bool fProofOfStake);
void VRXdebug();
void GNTdebug();
void VRX_BaseEngine(const CBlockIndex* pindexLast, bool fProofOfStake);
void VRX_ThreadCurve(const CBlockIndex* pindexLast, bool fProofOfStake);
void VRX_Dry_Run(const CBlockIndex* pindexLast);
void VRX_Simulate_Retarget();
double VRX_GetPrevDiff(bool fPoS);
unsigned int DarkGravityWave(const CBlockIndex* pindexLast, bool fProofOfStake);
unsigned int VRX_Retarget(const CBlockIndex* pindexLast, bool fProofOfStake);
unsigned int GetNextTargetRequired(const CBlockIndex* pindexLast, bool fProofOfStake);
int64_t GetProofOfWorkReward(int64_t nHeight, int64_t nFees);
int64_t GetProofOfStakeReward(int64_t nCoinAge, int64_t nFees);

#endif // DIMINUTIVECOIN_BLOCKPARAMS_H
