// Copyright (c) 2010-2016 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "ui_interface.h"
#include "util.h"

#include <boost/signals2.hpp>

CClientUIInterface uiInterface;

// CRITICAL: Connect dummy slots at global scope IMMEDIATELY after uiInterface construction
// This must happen before any code can call InitError() or emit signals
// We use a function-scope static to ensure it runs exactly once, as early as possible
namespace {
    void EnsureEarlySignalConnections() {
        static bool connected = false;
        if (!connected) {
            connected = true;
            // Connect dummy slots to all boost signals that might be emitted early
            uiInterface.InitMessage.connect([](const std::string&){});
            uiInterface.ShowProgress.connect([](const std::string&, int){});
            uiInterface.ThreadSafeMessageBox.connect([](const std::string&, const std::string&, unsigned int) { return false; });
            uiInterface.ThreadSafeQuestion.connect([](const std::string&, const std::string&, const std::string&, unsigned int) { return false; });
#ifdef ENABLE_WALLET
            uiInterface.LoadWallet.connect([](CWallet*){});
#endif
        }
    }
    // Force initialization at global scope - this runs before main()
    static struct {
        bool dummy;
        operator bool() { EnsureEarlySignalConnections(); return true; }
    } forceEarlyConnection = {true};
}

void CClientUIInterface::SafeShowProgress(const std::string &title, int nProgress)
{
    try {
        ShowProgress(title, nProgress);
    } catch (const boost::signals2::no_slots_error&) {
        // Signal emitted before slots connected - ignore during early initialization
    }
}

void CClientUIInterface::SafeInitMessage(const std::string &message)
{
    try {
        InitMessage(message);
    } catch (const boost::signals2::no_slots_error&) {
        // Signal emitted before slots connected - ignore during early initialization
    }
}

void CClientUIInterface::SafeLoadWallet(CWallet* wallet)
{
    try {
        LoadWallet(wallet);
    } catch (const boost::signals2::no_slots_error&) {
        // Signal emitted before slots connected - ignore during early initialization
    }
}

bool CClientUIInterface::SafeThreadSafeMessageBox(const std::string& message, const std::string& caption, unsigned int style)
{
    try {
        return ThreadSafeMessageBox(message, caption, style);
    } catch (const boost::signals2::no_slots_error&) {
        // Signal emitted before slots connected - log to console instead
        LogPrintf("ERROR: %s%s\n", caption.empty() ? "" : (caption + ": "), message);
        return false;
    }
}

bool InitError(const std::string& str)
{
    // Ensure dummy slots are connected before we try to emit any signals
    static bool ensured = []() {
        static bool connected = false;
        if (!connected) {
            connected = true;
            uiInterface.ThreadSafeMessageBox.connect([](const std::string&, const std::string&, unsigned int) { return false; });
            uiInterface.ThreadSafeQuestion.connect([](const std::string&, const std::string&, const std::string&, unsigned int) { return false; });
        }
        return true;
    }();
    (void)ensured;
    
    return uiInterface.SafeThreadSafeMessageBox(str, "", CClientUIInterface::MSG_ERROR);
}

void InitWarning(const std::string& str)
{
    uiInterface.SafeThreadSafeMessageBox(str, "", CClientUIInterface::MSG_WARNING);
}

std::string AmountHighWarn(const std::string& optname)
{
    return strprintf(_("%s is set very high!"), optname);
}

std::string AmountErrMsg(const char* const optname, const std::string& strValue)
{
    return strprintf(_("Invalid amount for -%s=<amount>: '%s'"), optname, strValue);
}
