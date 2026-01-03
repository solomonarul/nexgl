#include "core/cursor.hpp"

#include <SDL3/SDL_mouse.h>

using namespace NEX::Core;

static struct CursorManagerInstance {
    Cursor default_cursor = CURSOR_NORMAL;
    Cursor current;

    SDL_Cursor* cursors[CURSOR_COUNT];

    CursorManagerInstance();
    ~CursorManagerInstance();
} self;

CursorManagerInstance::CursorManagerInstance()
{
    this->cursors[CURSOR_NORMAL] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);
    this->cursors[CURSOR_POINT] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_POINTER);
}

CursorManagerInstance::~CursorManagerInstance()
{
    for(auto index = 0; index < CURSOR_COUNT; index++)
        SDL_DestroyCursor(this->cursors[index]);
}

void CursorManager::set_default_cursor(Cursor cursor)
{
    self.default_cursor = cursor;
}

void CursorManager::set_cursor(Cursor cursor)
{
    if(cursor == self.default_cursor) return;
    self.current = cursor;
}

void CursorManager::commit(void)
{
    SDL_SetCursor(self.cursors[self.current]);
}

void CursorManager::reset(void)
{
    self.current = self.default_cursor;
}