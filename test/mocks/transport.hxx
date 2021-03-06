#pragma once
#include <riak/transport.hxx>
#include <gmock/gmock.h>

//=============================================================================
namespace riak {
    namespace mock {
//=============================================================================

class transport
{
public:
    class option_to_terminate_request;
    MOCK_METHOD2( deliver,
            ::riak::transport::option_to_terminate_request(
                    const std::string&,
                    ::riak::transport::response_handler) );
};

class transport::option_to_terminate_request
{
public:
    MOCK_METHOD0( exercise, void() );
};

//=============================================================================
    }   // namespace mock
}   // namespace riak
//=============================================================================
