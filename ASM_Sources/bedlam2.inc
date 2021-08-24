
extrn fopen:proc
extrn fclose:proc
extrn fread:proc
extrn fwrite:proc
extrn fseek:proc
extrn ftell:proc
extrn fflush:proc

extrn printf:proc
extrn puts:proc
extrn putchar:proc
extrn getcwd:proc
extrn mkdir:proc

extrn malloc:proc
extrn free:proc
extrn exit:proc
extrn dos_sleep:proc

extrn set_palette:proc
extrn swap_palette_with_animation:proc
extrn wait_palette_animation:proc

extrn get_screen_buffer_ptr:proc
extrn clear_screen:proc
extrn copy_buffer_to_screen_and_unlock:proc
extrn redraw:proc
extrn unlock_surface_and_wait:proc
extrn redraw_copy_buff_to_screen_and_unlock:proc
extrn draw_map_and_game_screen:proc
extrn copy_screen_to_buffer:proc
extrn correct_screen_pos:proc

extrn show_cursor:proc
extrn hide_cursor:proc

extrn load_raw:proc
extrn load_music:proc
extrn play_sound:proc
extrn play_music:proc
extrn stop_music:proc
extrn set_volume:proc

extrn play_wav_music:proc
extrn stop_wav_music:proc

extrn free_win:proc

extrn SDL_events:proc

extrn SCREEN_BUFFER_PTR:dword
extrn SCREEN_SURFACE_WIDTH:dword
extrn SCREEN_SURFACE_HEIGHT:dword
extrn SIDEBAR_BUFFER_PTR:dword
extrn MAP_BUFFER_PTR:dword
extrn GAME_SCREEN_PTR:dword
extrn GAME_SCREEN_WIDTH:dword
extrn LIMIT_GAME_SCREEN_WIDTH:dword
extrn LIMIT_GAME_SCREEN_HEIGHT:dword
extrn GAME_SCREEN_SIZE:dword

extrn VETRICAL_TILE_LEVEL_OFFSET:dword
extrn NUM_TILES_X:dword
extrn NUM_TILES_Y:dword
extrn NOT_VISIBLE_TILES_X:dword
extrn NOT_VISIBLE_TILES_Y:dword
extrn TILES_START_POS_X:dword
extrn TILES_START_POS_Y:dword
extrn Z_BUFFER_PTR:dword

extrn CURSOR_ICON:dword
extrn CURSOR_POS_X:dword
extrn CURSOR_POS_Y:dword
extrn CURSOR_POS_LCLICK_X:dword
extrn CURSOR_POS_LCLICK_Y:dword
extrn CURSOR_POS_RCLICK_X:dword
extrn CURSOR_POS_RCLICK_Y:dword
extrn MOUSE_CLICK:dword
extrn MOUSE_BUTTONS_STATE1:dword
extrn UPDATE_CURSOR_BY_TIMER:dword
extrn WAITING_TIMER:dword
extrn GAME_UPDATE_TIMER:dword
extrn SIDEBAR_START_POS_X:dword

extrn PRESSED_KEY_ARR:byte
extrn KEY_ESC:dword
extrn KEY_F1:dword
extrn KEY_F2:dword
extrn KEY_F3:dword
extrn KEY_1:dword
extrn KEY_2:dword
extrn KEY_3:dword
extrn KEY_4:dword
extrn KEY_5:dword
extrn KEY_6:dword
extrn KEY_7:dword
extrn KEY_P:dword
extrn KEY_M:dword

;==========================================================

public screen_x_pos
public screen_y_pos
public map_active
public game_is_playing
public dead_screen_scale