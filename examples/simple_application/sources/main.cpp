#include "application.hpp"

int main(int argc, char *argv[]) {
    simple_application::Application application(argc, argv);
    return application.run();
}
