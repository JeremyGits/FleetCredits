// Copyright (c) 2009-2016 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// 
// note from the developer:
// thanks bitcoin and dogecoin, I love you still and forever, thank you Satoshi (and Shibetoshi) for everything
// what you have done for the world is amazing, you are a true legend.
// you are the reason why I am here today, I will always be grateful to you for your contributions to the world.
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "checkpoints.h"

#include "chain.h"
#include "chainparams.h"
#include "validation.h"
#include "uint256.h"

#include <stdint.h>

#include <boost/foreach.hpp>

namespace Checkpoints {

    CBlockIndex* GetLastCheckpoint(const CCheckpointData& data)
    {
        const MapCheckpoints& checkpoints = data.mapCheckpoints;

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            BlockMap::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }

} // namespace Checkpoints
