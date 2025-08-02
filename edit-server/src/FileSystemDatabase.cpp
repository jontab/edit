#include "FileSystemDatabase.hpp"
#include <cerrno>
#include <cstring>
#include <fstream>
#include <iostream>

FileSystemDatabase::FileSystemDatabase(const std::string &dir) : dir_(dir)
{
}

std::optional<edit::common::Buffer> FileSystemDatabase::load(const std::string &docid)
{
    return std::nullopt;
}

void FileSystemDatabase::save(const std::string &docid, const edit::common::Buffer &buffer)
{
}
