

#include <ripple/app/ledger/Ledger.h>
#include <test/jtx.h>
#include <ripple/beast/unit_test.h>

namespace ripple {
namespace test {


class SHAMapV2_test : public beast::unit_test::suite
{
    void
    testSHAMapV2()
    {
        jtx::Env env(*this);
        Config config;

        std::set<uint256> amendments;
        std::vector<uint256> amendments_v;
        amendments_v.push_back(from_hex_text<uint256>("12345"));
        amendments.insert(from_hex_text<uint256>("12345"));

        auto ledger =
            std::make_shared<Ledger>(create_genesis, config,
                amendments_v, env.app().family());
        BEAST_EXPECT(! getSHAMapV2 (ledger->info()));
        BEAST_EXPECT(ledger->stateMap().get_version() == SHAMap::version{1});
        BEAST_EXPECT(ledger->txMap().get_version() == SHAMap::version{1});
        BEAST_EXPECT(getEnabledAmendments(*ledger) == amendments);

        ledger =
            std::make_shared<Ledger>(*ledger, NetClock::time_point{});
        BEAST_EXPECT(! getSHAMapV2 (ledger->info()));
        BEAST_EXPECT(ledger->stateMap().get_version() == SHAMap::version{1});
        BEAST_EXPECT(ledger->txMap().get_version() == SHAMap::version{1});
        BEAST_EXPECT(getEnabledAmendments(*ledger) == amendments);

        ledger->make_v2();
        BEAST_EXPECT(getSHAMapV2 (ledger->info()));
        BEAST_EXPECT(ledger->stateMap().get_version() == SHAMap::version{2});
        BEAST_EXPECT(ledger->txMap().get_version() == SHAMap::version{2});
        BEAST_EXPECT(getEnabledAmendments(*ledger) == amendments);

        ledger = std::make_shared<Ledger>(*ledger, NetClock::time_point{});
        BEAST_EXPECT(getSHAMapV2 (ledger->info()));
        BEAST_EXPECT(ledger->stateMap().get_version() == SHAMap::version{2});
        BEAST_EXPECT(ledger->txMap().get_version() == SHAMap::version{2});
        BEAST_EXPECT(getEnabledAmendments(*ledger) == amendments);
    }

    void run() override
    {
        testSHAMapV2();
    }
};

BEAST_DEFINE_TESTSUITE(SHAMapV2,ledger,ripple);

}  
}  






