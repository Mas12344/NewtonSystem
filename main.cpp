#include <gtkmm.h>
#include <gtkmm/application.h>

#include "Window.h"

int main(int argc, char** argv) {
    auto app = Gtk::Application::create(argc, argv, "pl.poznan.put.student.grzybowski.emil");
    MyWindow window;
	return app->run(window);

}
