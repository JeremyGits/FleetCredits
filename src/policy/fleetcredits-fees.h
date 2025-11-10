// Fleet Credits: Fee calculation helper function
// Returns minimum fee required for a transaction, implementing zero-fee for micro-transactions
#ifndef FLEETCREDITS_POLICY_FLEETCREDITS_FEES_H
#define FLEETCREDITS_POLICY_FLEETCREDITS_FEES_H

#include "amount.h"
#include "primitives/transaction.h"
#include "policy/policy.h"

class CTransaction;

// Fleet Credits: Zero-fee threshold for micro-transactions
// Transactions with total output value < MICRO_TX_THRESHOLD (0.01 FC)
// are exempt from fees to encourage micro-transactions and reduce friction.
CAmount GetFleetCreditsMinFee(const CTransaction& tx, unsigned int nBytes, bool fAllowFree);

#endif // FLEETCREDITS_POLICY_FLEETCREDITS_FEES_H

