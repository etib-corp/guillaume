#include "simple_application.hpp"

int main(int argc, const char *const argv[], const char *const envp[]) {
    guillaume::simple_application::SimpleApplication application(argc, argv,
                                                                 envp);
    return application.run();
}
