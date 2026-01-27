#include "simple_application.hpp"

int main(int argc, char *argv[]) {
    guillaume::simple_application::SimpleApplication application(argc, argv);
    return application.run();
}
