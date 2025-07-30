#pragma once

#include <string>

namespace edit
{

struct Args
{
    std::string host;
    std::string port;
    std::string docid;
    std::string apikey;

    Args();
    Args(int argc, char **argv);

  private:
    void parse(int argc, char **argv);
    void usage(const std::string &program, std::ostream &out);
};

} // namespace edit
