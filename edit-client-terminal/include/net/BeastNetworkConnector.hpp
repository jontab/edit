#pragma once

#include "INetworkConnector.hpp"

namespace edit::net
{

class BeastNetworkConnector : public INetworkConnector
{
  public:
    BeastNetworkConnector();

    NetworkConnectResult connect(const std::string &host, const std::string &port, const std::string &path,
                                 const std::string &apikey) override;
};

} // namespace edit::net
