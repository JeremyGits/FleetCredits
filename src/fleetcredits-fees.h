// Copyright (c) 2021 The Fleet Credits Core developers
// Copyright (c) 2025-2026 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef FLEETCREDITS_FLEETCREDITS_FEES_H
#define FLEETCREDITS_FLEETCREDITS_FEES_H

#include "amount.h"
#include "chain.h"
#include "chainparams.h"

#ifdef ENABLE_WALLET

enum FeeRatePreset
{
    MINIMUM,
    MORE,
    WOW,
    AMAZE,
    MANY_GENEROUS,
    SUCH_EXPENSIVE
};

/** Estimate fee rate needed to get into the next nBlocks */
CFeeRate GetFleetCreditsFeeRate(int priority);
const std::string GetFleetCreditsPriorityLabel(int priority);

#endif // ENABLE_WALLET

CAmount GetFleetCreditsMinRelayFee(const CTransaction& tx, unsigned int nBytes, bool fAllowFree);
CAmount GetFleetCreditsDustFee(const std::vector<CTxOut> &vout, const CAmount dustLimit);

#endif // FLEETCREDITS_FLEETCREDITS_FEES_H
