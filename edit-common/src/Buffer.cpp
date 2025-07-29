#include "edit-common/Buffer.hpp"
#include <algorithm>

edit::common::Buffer::Buffer() : data_(), clock_(0)
{
}

edit::common::Buffer::Buffer(const std::vector<Char> &data) : data_(data), clock_(0)
{
    auto it = std::max_element(data_.begin(), data_.end(), [](auto &a, auto &b) { return a.clock < b.clock; });
    if (it < data_.end())
        clock_ = it->clock;
}

edit::common::Buffer::Buffer(std::vector<Char> &&data) : data_(std::move(data)), clock_(0)
{
    auto it = std::max_element(data_.begin(), data_.end(), [](auto &a, auto &b) { return a.clock < b.clock; });
    if (it < data_.end())
        clock_ = it->clock;
}

bool edit::common::Buffer::insert(const Char &ch)
{
    clock_ = std::max(clock_, ch.clock);
    if (edit::common::find(data_, ch) == -1)
    {
        int i = edit::common::find_sorted_position(data_, ch);
        data_.insert(data_.begin() + i, ch);
        return true;
    }
    else
    {
        return false;
    }
}

bool edit::common::Buffer::remove(const Char &ch)
{
    clock_ = std::max(clock_, ch.clock);
    int i = edit::common::find(data_, ch);
    if ((i >= 0) && !data_[i].is_deleted)
    {
        data_[i].is_deleted = true;
        return true;
    }
    else
    {
        return false;
    }
}

int edit::common::Buffer::clock() const
{
    return clock_;
}

std::size_t edit::common::Buffer::size() const
{
    return data_.size();
}

std::vector<edit::common::Char>::iterator edit::common::Buffer::begin()
{
    return data_.begin();
}

std::vector<edit::common::Char>::iterator edit::common::Buffer::end()
{
    return data_.end();
}

std::vector<edit::common::Char>::const_iterator edit::common::Buffer::begin() const
{
    return data_.begin();
}

std::vector<edit::common::Char>::const_iterator edit::common::Buffer::end() const
{
    return data_.end();
}

edit::common::Char &edit::common::Buffer::operator[](std::size_t index)
{
    return data_[index];
}

const edit::common::Char &edit::common::Buffer::operator[](std::size_t index) const
{
    return data_[index];
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
