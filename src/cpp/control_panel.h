#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H

#include <curses.h>
#include <string>
#include <map>

#define MIN_BOUND 2
#define MAX_BOUND 13

class ControlPanel
{
    private:
        unsigned int cursor;

    public:
        ControlPanel();
        ~ControlPanel();

        void list_instructions();
        void move_cursor_up();
        void move_cursor_down();
        void toggle_pin();
        void input_handler(int &key);
        void loop();
};

#endif
