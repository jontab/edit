#define TB_IMPL

#include "ViewContext.hpp"
#include "termbox2.h"
#include <stdexcept>

edit::ViewContext::ViewContext()
{
    if (tb_init() == TB_ERR)
        throw std::runtime_error("tb_init failed");
}

edit::ViewContext::~ViewContext() noexcept(false)
{
    if (tb_shutdown() == TB_ERR)
        throw std::runtime_error("tb_shutdown failed");
}
