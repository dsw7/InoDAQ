#include <curses.h>
#include <string>

#define MIN_BOUND 2
#define MAX_BOUND 13

class ControlPanel
{
    private:
        unsigned int cursor;

    public:
        ControlPanel();
        ~ControlPanel();

        void move_cursor_up();
        void move_cursor_down();
        void input_handler(int &key);
        void loop();
};
