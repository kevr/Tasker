#ifndef SRC_TUI_OBJECT_HPP
#define SRC_TUI_OBJECT_HPP

namespace tasker::tui
{

/**
 * @brief Base TUI object interface
 *
 * This class should be used as a base for any types derivative
 * of an ncurses WINDOW.
 **/
class object
{
public:
    virtual ~object() noexcept = default;

    //! init() interface which must be overridden in derivatives
    virtual int init() noexcept = 0;

    //! void() interface which must be overridden in derivatives
    virtual int draw() noexcept = 0;

    //! refresh() interface which must be overridden in derivatives
    virtual int refresh() noexcept = 0;

    //! refresh() this window, including all children
    virtual int refresh_all() noexcept = 0;

    //! end() interface which must be overridden in derivatives
    virtual int end() noexcept = 0;
};

}; // namespace tasker::tui

#endif /* SRC_TUI_OBJECT_HPP */
