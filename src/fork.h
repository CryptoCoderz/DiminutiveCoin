// Copyright (c) 2016-2023 The CryptoCoderz Team / Espers
// Copyright (c) 2023 The Diminutive Coin Team
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef ESPERS_FORK_H
#define ESPERS_FORK_H

#include "bignum.h"

using namespace std;

/** Genesis Block Height */
static const int64_t nGenesisHeight = 0;
/** Velocity toggle block */
static const int64_t VELOCITY_TOGGLE = 2727950; // Implementation of the Velocity system into the chain.
/** Velocity retarget toggle block */
static const int64_t VELOCITY_TDIFF = 2727950; // Use Velocity's retargetting method.
/** Proof-of-Stake Version 3.0 implementation fork */
inline bool IsVRX_V3_6(int64_t nTime) { return (nTime > 1696761120); } // ON (TOGGLED Oct 08, 2023 10:32:00 UTC)
/** VRX difficulty upgrade to v3.6 */
static const int64_t VRX_V3_6 = 1696761120; // ON (TOGGLED Oct 08, 2023 10:32:00 UTC)

#endif // ESPERS_FORK_H
