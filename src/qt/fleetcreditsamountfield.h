// Copyright (c) 2011-2015 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef FLEETCREDITS_QT_FLEETCREDITSAMOUNTFIELD_H
#define FLEETCREDITS_QT_FLEETCREDITSAMOUNTFIELD_H

#include "amount.h"

#include <QWidget>
#include <QAbstractSpinBox>

QT_BEGIN_NAMESPACE
class QValueComboBox;
QT_END_NAMESPACE

/** QSpinBox that uses fixed-point numbers internally and uses our own
 * formatting/parsing functions.
 */
class AmountSpinBox: public QAbstractSpinBox
{
    Q_OBJECT

public:
    explicit AmountSpinBox(QWidget *parent);

    QValidator::State validate(QString &text, int &pos) const override;
    void fixup(QString &input) const override;

    CAmount value(bool *valid_out=0) const;
    void setValue(const CAmount& value);

    void stepBy(int steps) override;
    void setDisplayUnit(int unit);
    void setSingleStep(const CAmount& step);

    QSize minimumSizeHint() const override;

protected:
    bool event(QEvent *event) override;
    StepEnabled stepEnabled() const override;

Q_SIGNALS:
    void valueChanged();

private:
    int currentUnit;
    CAmount singleStep;
    mutable QSize cachedMinimumSizeHint;

    /**
     * Parse a string into a number of base monetary units and
     * return validity.
     * @note Must return 0 if !valid.
     */
    CAmount parse(const QString &text, bool *valid_out=0) const;
};

/** Widget for entering fleetcredits amounts.
  */
class FleetCreditsAmountField: public QWidget
{
    Q_OBJECT

    // ugly hack: for some unknown reason CAmount (instead of qint64) does not work here as expected
    // discussion: https://github.com/fleetcredits/fleetcredits/pull/5117
    Q_PROPERTY(qint64 value READ value WRITE setValue NOTIFY valueChanged USER true)

public:
    explicit FleetCreditsAmountField(QWidget *parent = 0);

    CAmount value(bool *value=0) const;
    void setValue(const CAmount& value);

    /** Set single step in satoshis **/
    void setSingleStep(const CAmount& step);

    /** Make read-only **/
    void setReadOnly(bool fReadOnly);

    /** Mark current value as invalid in UI. */
    void setValid(bool valid);
    /** Perform input validation, mark field as invalid if entered value is not valid. */
    bool validate();

    /** Change unit used to display amount. */
    void setDisplayUnit(int unit);

    /** Make field empty and ready for new input. */
    void clear();

    /** Enable/Disable. */
    void setEnabled(bool fEnabled);

    /** Qt messes up the tab chain by default in some cases (issue https://bugreports.qt-project.org/browse/QTBUG-10907),
        in these cases we have to set it up manually.
    */
    QWidget *setupTabChain(QWidget *prev);

Q_SIGNALS:
    void valueChanged();

protected:
    /** Intercept focus-in event and ',' key presses */
    bool eventFilter(QObject *object, QEvent *event);

private:
    AmountSpinBox *amount;
    QValueComboBox *unit;

private Q_SLOTS:
    void unitChanged(int idx);

};

// Legacy typedef removed - use FleetCreditsAmountField directly

#endif // FLEETCREDITS_QT_FLEETCREDITSAMOUNTFIELD_H
