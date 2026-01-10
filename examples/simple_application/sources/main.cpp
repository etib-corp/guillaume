#include "simple_application.hpp"

int main(void) {
  guillaume::simple_application::SimpleApplication application;
  application.addWindow("main");

  const auto &window = application.getWindow("main");
  window->setTitle("Guillaume Demo Window");
  window->setPosition({50.0F, 50.0F});
  window->setSize({640, 480});
  window->show();

  guillaume::simple_application::SimpleRenderer renderer;
  renderer.setDrawColor({255, 255, 255, 255});
  renderer.clear();
  renderer.present();

  return 0;
}