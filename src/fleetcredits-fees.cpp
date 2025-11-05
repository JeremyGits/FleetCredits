// Fleet Credits: Fee calculation implementation
// Copyright (c) 2021 The Fleet Credits Core developers
// Copyright (c) 2025-2026 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/random/uniform_int.hpp>

#include "policy/policy.h"
#include "arith_uint256.h"
#include "fleetcredits.h"
#include "txmempool.h"
#include "util.h"
#include "validation.h"
#include "fleetcredits-fees.h"
#include "amount.h"
#ifdef ENABLE_WALLET
#include "wallet/wallet.h"
#endif

#ifdef ENABLE_WALLET

CFeeRate GetFleetCreditsFeeRate(int priority)
{
    switch(priority)
    {
    case SUCH_EXPENSIVE:
        return CFeeRate(COIN / 100 * 521); // 5.21 FC, but very carefully avoiding floating point maths
    case MANY_GENEROUS:
        return CFeeRate(CWallet::minTxFee.GetFeePerK() * 100);
    case AMAZE:
        return CFeeRate(CWallet::minTxFee.GetFeePerK() * 10);
    case WOW:
        return CFeeRate(CWallet::minTxFee.GetFeePerK() * 5);
    case MORE:
        return CFeeRate(CWallet::minTxFee.GetFeePerK() * 2);
    case MINIMUM:
    default:
        break;
    }
    return CWallet::minTxFee;
}

const std::string GetFleetCreditsPriorityLabel(int priority)
{
    switch(priority)
    {
    case SUCH_EXPENSIVE:
        return _("Such expensive");
    case MANY_GENEROUS:
        return _("Many generous");
    case AMAZE:
        return _("Amaze");
    case WOW:
        return _("Wow");
    case MORE:
        return _("More");
    case MINIMUM:
        return _("Minimum");
    default:
        break;
    }
    return _("Default");
}

#endif

CAmount GetFleetCreditsMinRelayFee(const CTransaction& tx, unsigned int nBytes, bool fAllowFree)
{
    // Calculate total output value for micro-transaction check
    CAmount nValueOut = 0;
    for (const auto& txout : tx.vout) {
        nValueOut += txout.nValue;
    }
    
    // Zero fees for transactions < 1000 FC (micro-transactions)
    if (nValueOut < MICRO_TX_THRESHOLD) {
        return 0;
    }
    
    {
        LOCK(mempool.cs);
        uint256 hash = tx.GetHash();
        double dPriorityDelta = 0;
        CAmount nFeeDelta = 0;
        mempool.ApplyDeltas(hash, dPriorityDelta, nFeeDelta);
        if (dPriorityDelta > 0 || nFeeDelta > 0)
            return 0;
    }

    CAmount nMinFee = ::minRelayTxFeeRate.GetFee(nBytes);
    nMinFee += GetFleetCreditsDustFee(tx.vout, nDustLimit);

    if (fAllowFree)
    {
        // There is a free transaction area in blocks created by most miners,
        // * If we are relaying we allow transactions up to DEFAULT_BLOCK_PRIORITY_SIZE - 1000
        //   to be considered to fall into this category. We don't want to encourage sending
        //   multiple transactions instead of one big transaction to avoid fees.
        if (nBytes < (DEFAULT_BLOCK_PRIORITY_SIZE - 1000))
            nMinFee = 0;
    }

    if (!MoneyRange(nMinFee))
        nMinFee = MAX_MONEY;
    return nMinFee;
}

CAmount GetFleetCreditsDustFee(const std::vector<CTxOut> &vout, const CAmount dustLimit) {
    CAmount nFee = 0;

    // To limit dust spam, add the dust limit for each output
    // under the dust limit, as the fee needed to spend that output
    BOOST_FOREACH(const CTxOut &txout, vout)
        if (txout.nValue < dustLimit)
            nFee += dustLimit;

    return nFee;
}
