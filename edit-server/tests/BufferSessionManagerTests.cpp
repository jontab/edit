#include "BufferSessionManager.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;

class MockDatabase : public IDatabase
{
  public:
    MOCK_METHOD(std::optional<edit::common::Buffer>, load, (const std::string &), (override));
    MOCK_METHOD(void, save, (const std::string &, const edit::common::Buffer &), (override));
};

class MockWebSocketServer : public IWebSocketServer
{
  public:
    MOCK_METHOD(void, send, (void *, const std::string &), (override));
    MOCK_METHOD(void, broadcast, (const std::string &, const std::string &), (override));
    MOCK_METHOD(void, subscribe, (void *, const std::string &), (override));
};

TEST(BufferSessionManagerTests, SingleClient_JoinsAndLeaves)
{
    // Arrange.
    auto db = std::make_unique<MockDatabase>();
    auto wss = std::make_unique<MockWebSocketServer>();

    EXPECT_CALL(*db, load("room")).Times(1);
    EXPECT_CALL(*wss, subscribe((void *)0x1, "room")).Times(1);
    // TODO: EXPECT_CALL(*wss, send((void *)0x1, _)).Times(1);
    EXPECT_CALL(*db, save("room", _)).Times(1);

    BufferSessionManager manager{std::move(db), std::move(wss)};

    // Act.
    manager.on_client_connected((void *)0x1, "room");
    manager.on_client_disconnected((void *)0x1);
}
