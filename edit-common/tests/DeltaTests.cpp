#include "edit-common/Delta.hpp"
#include <gtest/gtest.h>

TEST(DeltaTests, SerializeToJson)
{
    // Arrange.
    edit::common::Delta d{
        .is_delete = true,
        .ch = {
               .parent_clock = 1,
               .parent_site = 2,
               .clock = 3,
               .site = 4,
               .ch = '\0',
               .is_deleted = true,
               }
    };

    // Act.
    nlohmann::json jd = d;

    // Assert.
    EXPECT_EQ(jd.dump(), "{\"c\":{\"c\":3,\"ch\":0,\"d\":true,\"pc\":1,\"ps\":2,\"s\":4},\"d\":true}");
}
