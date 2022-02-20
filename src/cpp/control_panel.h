#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H

#include <curses.h>
#include <string>
#include <map>

#include "serial.h"

#define MIN_BOUND 2
#define MAX_BOUND 13

class ControlPanel
{
    private:
        unsigned int cursor;
        std::string serial_port;
        bool is_connected = false;

        Serial connection{false};

    public:
        ControlPanel(std::string &serial_port);
        ~ControlPanel();

        void print_status(const std::string &status);
        void list_instructions();
        void connect();
        void disconnect();
        void move_cursor_up();
        void move_cursor_down();
        void toggle_pin();
        void input_handler(int &key);
        void loop();
};

#endif
