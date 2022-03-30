#include "control_panel.h"

ControlPanel::ControlPanel(std::string &serial_port): serial_port(serial_port)
{

    // I follow RAII (resource acquisition is initialization) and do all setup in the constructor

    initscr();
    cbreak();
    noecho();

    curs_set(0);

    keypad(stdscr, true);

    this->cursor = MIN_BOUND;

    // Cursor should start at 0, 0 by default
    hline('=', getmaxx(stdscr));

    mvwprintw(stdscr, this->cursor, 2, ">");

    for (unsigned int i = MIN_BOUND; i < MAX_BOUND + 1; i++)
    {
        std::string pin = "D" + std::to_string(i);
        mvwprintw(stdscr, i, 9, pin.c_str());
    }

    this->list_instructions();
    this->list_pid();

    this->print_status("All digital pins are low");
    this->reset_state_matrix();
    this->reset_state_panel();
}

ControlPanel::~ControlPanel()
{
    this->disconnect();
    endwin();
}

void ControlPanel::print_status(const std::string &status)
{
    static bool prefix_enabled = false;

    if (not prefix_enabled)
    {
        mvwprintw(stdscr, 15, 4, "Status:");
        prefix_enabled = true;
    }

    move(15, 12);
    clrtoeol();
    mvwprintw(stdscr, 15, 12, status.c_str());
}

void ControlPanel::list_instructions()
{
    static unsigned int terminal_width = getmaxx(stdscr);

    move(17, 0);
    hline('=', terminal_width);

    wattron(stdscr, A_BOLD | A_UNDERLINE);
    mvwprintw(stdscr, 19, 2, "Key");
    mvwprintw(stdscr, 19, 18, "Description");
    wattroff(stdscr, A_BOLD | A_UNDERLINE);

    mvwprintw(stdscr, 21, 2, "j, KEY_DOWN");
    mvwprintw(stdscr, 22, 2, "k, KEY_UP");
    mvwprintw(stdscr, 23, 2, "c");
    mvwprintw(stdscr, 24, 2, "d");
    mvwprintw(stdscr, 25, 2, "ENTER");
    mvwprintw(stdscr, 26, 2, "q");

    mvwprintw(stdscr, 21, 18, "Scroll cursor down");
    mvwprintw(stdscr, 22, 18, "Scroll cursor up");
    mvwprintw(stdscr, 23, 18, "Connect to serial port specified via command line");
    mvwprintw(stdscr, 24, 18, "Disconnect from serial port specified via command line");
    mvwprintw(stdscr, 25, 18, "Toggle pin at current cursor position");
    mvwprintw(stdscr, 26, 18, "Quit the program");

    move(28, 0);
    hline('=', terminal_width);
}

void ControlPanel::list_pid()
{
    std::string pid = "PID: " + std::to_string(getpid());
    mvwprintw(stdscr, 30, 2, pid.c_str());
}

void ControlPanel::reset_state_matrix()
{
    this->state_matrix = {
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
}

void ControlPanel::reset_state_panel()
{
    for (unsigned int i = MIN_BOUND; i < MAX_BOUND + 1; i++)
    {
        if (this->state_matrix[i])
        {
            mvwprintw(stdscr, i, 4, "[x]");
        }
        else
        {
            mvwprintw(stdscr, i, 4, "[ ]");
        }
    }
}

void ControlPanel::connect()
{
    if (this->is_connected)
    {
        this->print_status("Already connected on port " + this->serial_port);
        return;
    }

    if (not this->connection.connect())
    {
        this->print_status("Could not connect on port " + this->serial_port);
        return;
    }

    this->print_status("Connected on port " + this->serial_port);
    this->is_connected = true;
}

void ControlPanel::disconnect()
{
    if (not this->is_connected)
    {
        this->print_status("Not disconnecting. Was not connected to port " + this->serial_port);
        return;
    }

    this->print_status("Disconnecting from port " + this->serial_port);

    if (not this->connection.disconnect())
    {
        this->print_status("Failed to cleanly disconnect from device!");
        return;
    }

    this->reset_state_matrix();
    this->reset_state_panel();

    this->is_connected = false;
}

void ControlPanel::move_cursor_up()
{
    mvwprintw(stdscr, this->cursor, 2, " ");
    this->cursor--;

    if (this->cursor < MIN_BOUND)
    {
        this->cursor = MAX_BOUND;
    }
    mvwprintw(stdscr, this->cursor, 2, ">");
}

void ControlPanel::move_cursor_down()
{
    mvwprintw(stdscr, this->cursor, 2, " ");
    this->cursor++;

    if (this->cursor > MAX_BOUND)
    {
        this->cursor = MIN_BOUND;
    }
    mvwprintw(stdscr, this->cursor, 2, ">");
}

void ControlPanel::toggle_pin()
{
    if (not this->is_connected)
    {
        this->print_status("Not toggling pin. A connection to port " + this->serial_port + " does not exist!");
        return;
    }

    this->connection.write_data("DIG" + std::to_string(this->cursor) + "\n");
    this->connection.flush_buffer();

    this->state_matrix[this->cursor] = !this->state_matrix[this->cursor];

    static std::string status;
    if (this->state_matrix[this->cursor])
    {
        status = "Pin " + std::to_string(this->cursor) + " was set to high";
    }
    else
    {
        status = "Pin " + std::to_string(this->cursor) + " was set to low";
    }

    this->print_status(status);
    this->reset_state_panel();
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
        case KEY_DOWN:
            this->move_cursor_down();
            break;
        case KEY_UP:
            this->move_cursor_up();
            break;
        case 'c':
            this->connect();
            break;
        case 'd':
            this->disconnect();
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
