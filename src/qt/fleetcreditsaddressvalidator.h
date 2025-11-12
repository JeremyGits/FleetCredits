// Copyright (c) 2011-2014 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef FLEETCREDITS_QT_FLEETCREDITSADDRESSVALIDATOR_H
#define FLEETCREDITS_QT_FLEETCREDITSADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class FleetCreditsAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit FleetCreditsAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Fleet Credits address widget validator, checks for a valid fleetcredits address.
 */
class FleetCreditsAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit FleetCreditsAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

// Legacy typedefs removed - use FleetCreditsAddress* classes directly

#endif // FLEETCREDITS_QT_FLEETCREDITSADDRESSVALIDATOR_H
