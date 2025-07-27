#include "edit-common/Buffer.hpp"
#include <algorithm>

edit::common::Buffer::Buffer() : data()
{
}

edit::common::Buffer::Buffer(const std::vector<Char> &data) : data(data)
{
}

edit::common::Buffer::Buffer(std::vector<Char> &&data) : data(std::move(data))
{
}

bool edit::common::Buffer::insert(const Char &ch)
{
    if (edit::common::find(data, ch) == -1)
    {
        int i = edit::common::find_sorted_position(data, ch);
        data.insert(data.begin() + i, ch);
        return true;
    }
    else
    {
        return false;
    }
}

bool edit::common::Buffer::remove(const Char &ch)
{
    int i = edit::common::find(data, ch);
    if ((i >= 0) && !data[i].is_deleted)
    {
        data[i].is_deleted = true;
        return true;
    }
    else
    {
        return false;
    }
}

std::vector<edit::common::Char>::iterator edit::common::Buffer::begin()
{
    return data.begin();
}

std::vector<edit::common::Char>::iterator edit::common::Buffer::end()
{
    return data.end();
}

std::vector<edit::common::Char>::const_iterator edit::common::Buffer::begin() const
{
    return data.begin();
}

std::vector<edit::common::Char>::const_iterator edit::common::Buffer::end() const
{
    return data.end();
}

int edit::common::find(const std::vector<Char> &chars, const edit::common::Char &ch)
{
    auto result = std::find(chars.begin(), chars.end(), ch);
    if (result < chars.end())
    {
        return std::distance(chars.begin(), result);
    }
    else
    {
        return -1;
    }
}

int edit::common::find_sorted_position(const std::vector<Char> &chars, const edit::common::Char &ch)
{
    int parent = edit::common::find(chars, {.clock = ch.parent_clock, .site = ch.parent_site});
    int i;

    // See: https://github.com/josephg/reference-crdts/blob/fed747255df9d457e11f36575de555b39f07e909/crdts.ts#L401-L459.
    // This algorithm enforces the following rules:
    //   (1). Siblings (characters with the same parent) are sorted in order of increasing site.
    //   (2). A character will either come before its sibling and the sibling's children or after, based on rule (1).
    for (i = parent + 1; i < (int)chars.size(); i++)
    {
        const auto &i_ch = chars.at(i);
        if (i_ch.clock < ch.clock)
        {
            break;
        }

        int i_parent = edit::common::find(chars, {.clock = i_ch.parent_clock, .site = i_ch.parent_site});
        if (i_parent < parent)
        {
            // This enacts rule (2).
            break;
        }

        if ((i_parent == parent) && (i_ch.clock == ch.clock) && (i_ch.site > ch.site))
        {
            // This enacts rule (1).
            break;
        }
    }

    return i;
}
