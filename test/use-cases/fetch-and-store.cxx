#include <boost/asio/io_service.hpp>
#include <boost/format.hpp>
#include <boost/thread.hpp>
#include <functional>
#include <riak/client.hxx>
#include <riak/transports/single-serial-socket.hxx>
#include <test/tools/use-case-control.hxx>

using namespace boost;
using namespace riak::test;

::riak::sibling no_sibling_resolution (const ::riak::siblings&);

void run(boost::asio::io_service& ios)
{
    ios.run();
}

int main (int argc, const char* argv[])
{
    boost::asio::io_service ios;
    std::unique_ptr<boost::asio::io_service::work> work(new boost::asio::io_service::work(ios));
    boost::thread worker(std::bind(&run, std::ref(ios)));
    
    announce_with_pause("Connecting!");
    auto connection = riak::make_single_socket_transport("localhost", 8082, ios);
    auto my_store = riak::make_client(connection, no_sibling_resolution, ios);
    
    announce_with_pause("Ready to fetch item test/doc");
    auto cached_object = my_store->bucket("test")["doc"];
    auto result = cached_object->fetch();
    
    announce("Waiting for operation to respond...");
    result.wait();
    if (result.has_value()) {
        announce("Fetch appears successful.");
        std::string stored_value = (argc > 1)? argv[1] : "oogaboogah";
        RpbContent c;
        c.set_value(stored_value);
        c.set_content_type("text/plain");

        announce_with_pause(str(format("Ready to store '%1%' to item test/doc") % stored_value));
        auto result = cached_object->put(c);
        
        announce("Waiting for operation to respond...");
        result.wait();
        if (result.has_value()) {
            announce("Store appears successful.");
        } else {
            assert(result.has_exception());
            try {
                result.get();
            } catch (const std::exception& e) {
                announce(str(format("Store reported exception %1%: %2%.") % typeid(e).name() % e.what()));
            } catch (...) {
                announce("Fetch produced a nonstandard exception.");
            }
        }
    } else {
        assert(result.has_exception());
        try {
            result.get();
        } catch (const std::exception& e) {
            announce(str(format("Fetch reported exception %1%: %2%.") % typeid(e).name() % e.what()));
        } catch (...) {
            announce("Fetch produced a nonstandard exception.");
        }
    }
    
    announce_with_pause("Scenario completed.");

    work.reset();
    worker.join();
    return 0;
}

::riak::sibling no_sibling_resolution (const ::riak::siblings&)
{
    announce("Siblings being resolved!");
    ::riak::sibling garbage;
    garbage.set_value("<result of sibling resolution>");
    return garbage;
}
