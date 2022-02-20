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
        mvwprintw(stdscr, i, 4, "[ ]");

        std::string pin = "D" + std::to_string(i);
        mvwprintw(stdscr, i, 9, pin.c_str());
    }

    this->print_status("All digital pins are low");
    this->list_instructions();
}

ControlPanel::~ControlPanel()
{
    this->connection.close_connection();
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

    mvwprintw(stdscr, 18, 2, "1. Press 'j' key to scroll down and 'k' key to scroll up");
    mvwprintw(stdscr, 19, 2, "2. Scroll to desired pin and press ENTER to toggle pin");
    mvwprintw(stdscr, 20, 2, "3. Press 'c' to connect on serial port specified via command line");
    mvwprintw(stdscr, 21, 2, "3. Press 'd' to disconnect from serial port specified via command line");
    mvwprintw(stdscr, 22, 2, "4. Press 'q' key to quit");

    move(23, 0);
    hline('=', terminal_width);
}

void ControlPanel::connect()
{
    if (this->is_connected)
    {
        this->print_status("Already connected on port " + this->serial_port);
        return;
    }

    if (not this->connection.open_connection(serial_port))
    {
        this->print_status("Could not connect on port " + this->serial_port);
        return;
    }

    if (not this->connection.configure_connection())
    {
        this->print_status("Could not configure port " + this->serial_port);
        this->connection.close_connection();
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
    this->connection.close_connection();

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

    this->connection.write_data("D" + std::to_string(this->cursor) + "\n");
    state_matrix[this->cursor] = !state_matrix[this->cursor];

    static std::string status;

    if (state_matrix[this->cursor])
    {
        status = "Pin " + std::to_string(this->cursor) + " was set to high";
    }
    else
    {
        status = "Pin " + std::to_string(this->cursor) + " was set to low";
    }

    this->print_status(status);

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
