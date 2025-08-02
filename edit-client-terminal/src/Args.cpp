#include "Args.hpp"
#include <getopt.h>
#include <iostream>

using namespace edit;

Args::Args() : host("localhost"), port("443"), docid(""), apikey("")
{
}

Args::Args(int argc, char **argv) : Args()
{
    parse(argc, argv);
}

void Args::parse(int argc, char **argv)
{
    static struct option options[] = {
        {"help",   no_argument,       NULL, 'H'},
        {"host",   required_argument, NULL, 'h'},
        {"port",   required_argument, NULL, 'p'},
        {"docid",  required_argument, NULL, 'd'},
        {"apikey", required_argument, NULL, 'a'},
        {NULL,     no_argument,       NULL, 0  },
    };
    while (1)
    {
        int opt = getopt_long(argc, argv, "?h:p:d:a:", options, NULL);
        if (opt < 0)
        {
            break;
        }

        switch (opt)
        {
        case 'H':
            usage(argv[0], std::cout);
            exit(EXIT_SUCCESS);
        case 'h':
            host = optarg;
            break;
        case 'p':
            port = optarg;
            break;
        case 'd':
            docid = optarg;
            break;
        case 'a':
            apikey = optarg;
            break;
        default:
            usage(argv[0], std::cerr);
            exit(EXIT_FAILURE);
        }
    }
}

void Args::usage(const std::string &program, std::ostream &out)
{
    out << "Usage: " << program << " [OPTIONS]" << std::endl;
    out << std::endl;
    out << "Options:" << std::endl;
    out << "  -h,--host    Host of server (default: localhost)" << std::endl;
    out << "  -p,--port    Port of server (default: 443)" << std::endl;
    out << "  -d,--docid   Name of document (default: DefaultDocument)" << std::endl;
    out << "  -a,--apikey  API key" << std::endl;
    out << "  -H,--help    Show this help message and exit" << std::endl;
    out << std::endl;
    out << "Example:" << std::endl;
    out << "  " << program << " -h localhost -p 443 -d UntitledDocument1 -a MyApiKey1234" << std::endl;
}
