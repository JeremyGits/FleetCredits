// Copyright (c) 2011-2016 The Fleet Credits Core developers
// Copyright (c) 2021-2022 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// 
// note from the developer:
// thanks bitcoin and dogecoin, I love you still and forever, thank you Satoshi (and Shibetoshi) for everything
// what you have done for the world is amazing, you are a true legend.
// you are the reason why I am here today, I will always be grateful to you for your contributions to the world.
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef FLEETCREDITS_WALLET_COINCONTROL_H
#define FLEETCREDITS_WALLET_COINCONTROL_H

#include "primitives/transaction.h"
#include "fleetcredits-fees.h"

/** Coin Control Features. */
class CCoinControl
{
public:
    CTxDestination destChange;
    //! If false, allows unselected inputs, but requires all selected inputs be used
    bool fAllowOtherInputs;
    //! Includes watch only addresses which match the ISMINE_WATCH_SOLVABLE criteria
    bool fAllowWatchOnly;
    //! Minimum absolute fee (not per kilobyte)
    CAmount nMinimumTotalFee;
    //! Override estimated feerate
    bool fOverrideFeeRate;
    //! Feerate to use if overrideFeeRate is true
    CFeeRate nFeeRate;
    //! Override the default transaction speed, 0 = use default
    FeeRatePreset nPriority;

    CCoinControl()
    {
        SetNull();
    }

    void SetNull()
    {
        destChange = CNoDestination();
        fAllowOtherInputs = false;
        fAllowWatchOnly = false;
        setSelected.clear();
        nMinimumTotalFee = 0;
        nFeeRate = CFeeRate(0);
        fOverrideFeeRate = false;
        nPriority = MINIMUM;
    }

    bool HasSelected() const
    {
        return (setSelected.size() > 0);
    }

    bool IsSelected(const COutPoint& output) const
    {
        return (setSelected.count(output) > 0);
    }

    void Select(const COutPoint& output)
    {
        setSelected.insert(output);
    }

    void UnSelect(const COutPoint& output)
    {
        setSelected.erase(output);
    }

    void UnSelectAll()
    {
        setSelected.clear();
    }

    void ListSelected(std::vector<COutPoint>& vOutpoints) const
    {
        vOutpoints.assign(setSelected.begin(), setSelected.end());
    }

private:
    std::set<COutPoint> setSelected;
};

#endif // FLEETCREDITS_WALLET_COINCONTROL_H
