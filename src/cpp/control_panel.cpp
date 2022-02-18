#include "control_panel.h"

ControlPanel::ControlPanel()
{

    // I follow RAII (resource acquisition is initialization) and do all setup in the constructor

    initscr();
    cbreak();
    noecho();

    curs_set(0); // disable blinking cursor

    keypad(stdscr, true);

    this->cursor = MIN_BOUND;

    mvwprintw(stdscr, this->cursor, 2, ">");

    for (unsigned int i = MIN_BOUND; i < MAX_BOUND + 1; i++)
    {
        mvwprintw(stdscr, i, 4, "[ ]");

        std::string pin = "D" + std::to_string(i);
        mvwprintw(stdscr, i, 9, pin.c_str());
    }

    attron(A_REVERSE);
    mvwprintw(stdscr, LINES - 1, 0, " Press 'j' key to scroll down | Press 'k' key to scroll up ");
    attroff(A_REVERSE);
}

ControlPanel::~ControlPanel()
{
    endwin();
}

void ControlPanel::move_cursor_up()
{
    mvwprintw(stdscr, this->cursor, 2, " ");
    this->cursor--;

    if (this->cursor < MIN_BOUND)
    {
        this->cursor = MIN_BOUND;
    }
    mvwprintw(stdscr, this->cursor, 2, ">");
}

void ControlPanel::move_cursor_down()
{
    mvwprintw(stdscr, this->cursor, 2, " ");
    this->cursor++;

    if (this->cursor > MAX_BOUND)
    {
        this->cursor = MAX_BOUND;
    }
    mvwprintw(stdscr, this->cursor, 2, ">");
}

void ControlPanel::toggle_pin()
{
    static std::map<int, bool> state_matrix {
        {2,  false},
        {3,  false},
        {4,  false},
        {5,  false},
        {6,  false},
        {7,  false},
        {8,  false},
        {9,  false},
        {10, false},
        {11, false},
        {12, false},
        {13, false}
    };

    state_matrix[this->cursor] = !state_matrix[this->cursor];

    for (unsigned int i = MIN_BOUND; i < MAX_BOUND + 1; i++)
    {
        if (state_matrix[i])
        {
            mvwprintw(stdscr, i, 4, "[x]");
        }
        else
        {
            mvwprintw(stdscr, i, 4, "[ ]");
        }
    }
}

void ControlPanel::input_handler(int &key)
{
    switch(key)
    {
        case 'j':
            this->move_cursor_down();
            break;
        case 'k':
            this->move_cursor_up();
            break;
        case 10:
            this->toggle_pin();
            break;
    }
}

void ControlPanel::loop()
{
    int key;

    while (true)
    {
        key = getch();

        if (key == 'q')
        {
            break;
        }
        this->input_handler(key);
    }
}
