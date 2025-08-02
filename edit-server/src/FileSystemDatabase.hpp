#include "IDatabase.hpp"

class FileSystemDatabase : public IDatabase
{
    std::string dir_;

  public:
    FileSystemDatabase(const std::string &dir);

    std::optional<edit::common::Buffer> load(const std::string &docid) override;
    void save(const std::string &docid, const edit::common::Buffer &buffer) override;
};
