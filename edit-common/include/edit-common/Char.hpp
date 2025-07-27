#pragma once

namespace edit::common
{

struct Char
{
    int parent_clock;
    int parent_site;
    int clock;
    int site;
    char ch;
    bool is_deleted;

    bool operator==(const Char &other) const;
    bool operator!=(const Char &other) const;
};

} // namespace edit::common
