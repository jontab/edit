#include "App.h"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    uWS::App()
        .get("/hello/:name",
             [](auto *res, auto *req) {
                 res->writeStatus("200 OK");
                 res->writeHeader("Content-Type", "text/html; charset=utf-8");
                 res->write("<h1>Hello, ");
                 res->write(req->getParameter("name"));
                 res->end("!</h1>");
             })
        .listen(3000,
                [](auto *listener) {
                    if (listener)
                    {
                        std::cout << "Listening on port " << 3000 << std::endl;
                    }
                })
        .run();
    return 0;
}
