#pragma once

#include "NetworkConnectResult.hpp"

namespace edit::net
{

class INetworkConnector
{
  public:
    virtual NetworkConnectResult connect(const std::string &host, const std::string &port, const std::string &path,
                                         const std::string &apikey) = 0;
};

} // namespace edit::net
