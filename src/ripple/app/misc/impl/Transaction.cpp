

#include <ripple/app/misc/Transaction.h>
#include <ripple/app/tx/apply.h>
#include <ripple/basics/Log.h>
#include <ripple/core/DatabaseCon.h>
#include <ripple/app/ledger/LedgerMaster.h>
#include <ripple/app/main/Application.h>
#include <ripple/app/misc/HashRouter.h>
#include <ripple/protocol/Feature.h>
#include <ripple/protocol/jss.h>
#include <boost/optional.hpp>

namespace ripple {

Transaction::Transaction (std::shared_ptr<STTx const> const& stx,
    std::string& reason, Application& app)
    noexcept
    : mTransaction (stx)
    , mApp (app)
    , j_ (app.journal ("Ledger"))
{
    try
    {
        mTransactionID  = mTransaction->getTransactionID ();
    }
    catch (std::exception& e)
    {
        reason = e.what();
        return;
    }

    mStatus = NEW;
}


void Transaction::setStatus (TransStatus ts, std::uint32_t lseq)
{
    mStatus     = ts;
    mInLedger   = lseq;
}

TransStatus Transaction::sqlTransactionStatus(
    boost::optional<std::string> const& status)
{
    char const c = (status) ? (*status)[0] : txnSqlUnknown;

    switch (c)
    {
    case txnSqlNew:       return NEW;
    case txnSqlConflict:  return CONFLICTED;
    case txnSqlHeld:      return HELD;
    case txnSqlValidated: return COMMITTED;
    case txnSqlIncluded:  return INCLUDED;
    }

    assert (c == txnSqlUnknown);
    return INVALID;
}

Transaction::pointer Transaction::transactionFromSQL (
    boost::optional<std::uint64_t> const& ledgerSeq,
    boost::optional<std::string> const& status,
    Blob const& rawTxn,
    Application& app)
{
    std::uint32_t const inLedger =
        rangeCheckedCast<std::uint32_t>(ledgerSeq.value_or (0));

    SerialIter it (makeSlice(rawTxn));
    auto txn = std::make_shared<STTx const> (it);
    std::string reason;
    auto tr = std::make_shared<Transaction> (
        txn, reason, app);

    tr->setStatus (sqlTransactionStatus (status));
    tr->setLedger (inLedger);
    return tr;
}

Transaction::pointer Transaction::transactionFromSQLValidated(
    boost::optional<std::uint64_t> const& ledgerSeq,
    boost::optional<std::string> const& status,
    Blob const& rawTxn,
    Application& app)
{
    auto ret = transactionFromSQL(ledgerSeq, status, rawTxn, app);

    if (checkValidity(app.getHashRouter(),
            *ret->getSTransaction(), app.
                getLedgerMaster().getValidatedRules(),
                    app.config()).first !=
                        Validity::Valid)
        return {};

    return ret;
}

Transaction::pointer Transaction::load(uint256 const& id, Application& app)
{
    std::string sql = "SELECT LedgerSeq,Status,RawTxn "
            "FROM Transactions WHERE TransID='";
    sql.append (to_string (id));
    sql.append ("';");

    boost::optional<std::uint64_t> ledgerSeq;
    boost::optional<std::string> status;
    Blob rawTxn;
    {
        auto db = app.getTxnDB ().checkoutDb ();
        soci::blob sociRawTxnBlob (*db);
        soci::indicator rti;

        *db << sql, soci::into (ledgerSeq), soci::into (status),
                soci::into (sociRawTxnBlob, rti);
        if (!db->got_data () || rti != soci::i_ok)
            return {};

        convert(sociRawTxnBlob, rawTxn);
    }

    return Transaction::transactionFromSQLValidated (
        ledgerSeq, status, rawTxn, app);
}

Json::Value Transaction::getJson (JsonOptions options, bool binary) const
{
    Json::Value ret (mTransaction->getJson (JsonOptions::none, binary));

    if (mInLedger)
    {
        ret[jss::inLedger] = mInLedger;        
        ret[jss::ledger_index] = mInLedger;

        if (options == JsonOptions::include_date)
        {
            auto ct = mApp.getLedgerMaster().
                getCloseTimeBySeq (mInLedger);
            if (ct)
                ret[jss::date] = ct->time_since_epoch().count();
        }
    }

    return ret;
}

} 





