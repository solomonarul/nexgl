#pragma once

namespace NEX::Core
{
    enum Cursor
    {
        CURSOR_NORMAL = 0,
        CURSOR_POINT,
        CURSOR_COUNT
    };

    struct CursorManager
    {
        CursorManager() = delete;

        static void set_default_cursor(Cursor);
        static void set_cursor(Cursor);
        static void commit(void);
        static void reset(void);
    };
}; // namespace NEX::Core
