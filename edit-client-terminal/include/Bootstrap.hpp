#pragma once

namespace edit
{

/**
 * @brief To be called by `main`.
 *
 * There are some things we want to initialize before the editor actually opens. E.g., if the user wants to connect to
 * a WebSocket server, then we should do that before we clear and set up the terminal. This is the natural extension of
 * that: provide a space for services to spawn before the `Editor` begins.
 */
class Bootstrap
{
  public:
    int run(int argc, char **argv);
};

} // namespace edit
