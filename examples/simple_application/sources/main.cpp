#include "application.hpp"

int main(int argc, char *argv[])
{
	simple_application::Application application(argc, argv);
	application.setActiveScene<simple_application::scenes::Main>();
	return application.run();
}
