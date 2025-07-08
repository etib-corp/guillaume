#pragma once

#define GUIGUI_VERSION "2.02"

#define GUIGUI_COMMANDLIST_SIZE (256 * 1024)
#define GUIGUI_ROOTLIST_SIZE 32
#define GUIGUI_CONTAINERSTACK_SIZE 32
#define GUIGUI_CLIPSTACK_SIZE 32
#define GUIGUI_IDSTACK_SIZE 32
#define GUIGUI_LAYOUTSTACK_SIZE 16
#define GUIGUI_CONTAINERPOOL_SIZE 48
#define GUIGUI_TREENODEPOOL_SIZE 48
#define GUIGUI_MAX_WIDTHS 16
#define GUIGUI_REAL float
#define GUIGUI_REAL_FMT "%.3g"
#define GUIGUI_SLIDER_FMT "%.2f"
#define GUIGUI_MAX_FMT 127

#define guigui_stack(T, n) \
    struct {           \
        int idx;       \
        T items[n];    \
    }
#define guigui_min(a, b) ((a) < (b) ? (a) : (b))
#define guigui_max(a, b) ((a) > (b) ? (a) : (b))
#define guigui_clamp(x, a, b) guigui_min(b, guigui_max(a, x))

enum { GUIGUI_CLIP_PART = 1,
    GUIGUI_CLIP_ALL };

enum {
    GUIGUI_COMMAND_JUMP = 1,
    GUIGUI_COMMAND_CLIP,
    GUIGUI_COMMAND_RECT,
    GUIGUI_COMMAND_TEXT,
    GUIGUI_COMMAND_ICON,
    GUIGUI_COMMAND_MAX
};

enum {
    GUIGUI_COLOR_TEXT,
    GUIGUI_COLOR_BORDER,
    GUIGUI_COLOR_WINDOWBG,
    GUIGUI_COLOR_TITLEBG,
    GUIGUI_COLOR_TITLETEXT,
    GUIGUI_COLOR_PANELBG,
    GUIGUI_COLOR_BUTTON,
    GUIGUI_COLOR_BUTTONHOVER,
    GUIGUI_COLOR_BUTTONFOCUS,
    GUIGUI_COLOR_BASE,
    GUIGUI_COLOR_BASEHOVER,
    GUIGUI_COLOR_BASEFOCUS,
    GUIGUI_COLOR_SCROLLBASE,
    GUIGUI_COLOR_SCROLLTHUMB,
    GUIGUI_COLOR_MAX
};

enum {
    GUIGUI_ICON_CLOSE = 1,
    GUIGUI_ICON_CHECK,
    GUIGUI_ICON_COLLAPSED,
    GUIGUI_ICON_EXPANDED,
    GUIGUI_ICON_MAX
};

enum {
    GUIGUI_RES_ACTIVE = (1 << 0),
    GUIGUI_RES_SUBMIT = (1 << 1),
    GUIGUI_RES_CHANGE = (1 << 2)
};

enum {
    GUIGUI_OPT_ALIGNCENTER = (1 << 0),
    GUIGUI_OPT_ALIGNRIGHT = (1 << 1),
    GUIGUI_OPT_NOINTERACT = (1 << 2),
    GUIGUI_OPT_NOFRAME = (1 << 3),
    GUIGUI_OPT_NORESIZE = (1 << 4),
    GUIGUI_OPT_NOSCROLL = (1 << 5),
    GUIGUI_OPT_NOCLOSE = (1 << 6),
    GUIGUI_OPT_NOTITLE = (1 << 7),
    GUIGUI_OPT_HOLDFOCUS = (1 << 8),
    GUIGUI_OPT_AUTOSIZE = (1 << 9),
    GUIGUI_OPT_POPUP = (1 << 10),
    GUIGUI_OPT_CLOSED = (1 << 11),
    GUIGUI_OPT_EXPANDED = (1 << 12)
};

enum {
    GUIGUI_MOUSE_LEFT = (1 << 0),
    GUIGUI_MOUSE_RIGHT = (1 << 1),
    GUIGUI_MOUSE_MIDDLE = (1 << 2)
};

enum {
    GUIGUI_KEY_SHIFT = (1 << 0),
    GUIGUI_KEY_CTRL = (1 << 1),
    GUIGUI_KEY_ALT = (1 << 2),
    GUIGUI_KEY_BACKSPACE = (1 << 3),
    GUIGUI_KEY_RETURN = (1 << 4)
};

typedef struct guigui_Context guigui_Context;
typedef unsigned guigui_Id;
typedef GUIGUI_REAL guigui_Real;
typedef void* guigui_Font;

typedef struct {
    int x, y;
} guigui_Vec2;
typedef struct {
    int x, y, w, h;
} guigui_Rect;
typedef struct {
    unsigned char r, g, b, a;
} guigui_Color;
typedef struct {
    guigui_Id id;
    int last_update;
} guigui_PoolItem;

typedef struct {
    int type, size;
} guigui_BaseCommand;
typedef struct {
    guigui_BaseCommand base;
    void* dst;
} guigui_JumpCommand;
typedef struct {
    guigui_BaseCommand base;
    guigui_Rect rect;
} guigui_ClipCommand;
typedef struct {
    guigui_BaseCommand base;
    guigui_Rect rect;
    guigui_Color color;
} guigui_RectCommand;
typedef struct {
    guigui_BaseCommand base;
    guigui_Font font;
    guigui_Vec2 pos;
    guigui_Color color;
    char str[1];
} guigui_TextCommand;
typedef struct {
    guigui_BaseCommand base;
    guigui_Rect rect;
    int id;
    guigui_Color color;
} guigui_IconCommand;

typedef union {
    int type;
    guigui_BaseCommand base;
    guigui_JumpCommand jump;
    guigui_ClipCommand clip;
    guigui_RectCommand rect;
    guigui_TextCommand text;
    guigui_IconCommand icon;
} guigui_Command;

typedef struct {
    guigui_Rect body;
    guigui_Rect next;
    guigui_Vec2 position;
    guigui_Vec2 size;
    guigui_Vec2 max;
    int widths[GUIGUI_MAX_WIDTHS];
    int items;
    int item_index;
    int next_row;
    int next_type;
    int indent;
} guigui_Layout;

typedef struct {
    guigui_Command *head, *tail;
    guigui_Rect rect;
    guigui_Rect body;
    guigui_Vec2 content_size;
    guigui_Vec2 scroll;
    int zindex;
    int open;
} guigui_Container;

typedef struct {
    guigui_Font font;
    guigui_Vec2 size;
    int padding;
    int spacing;
    int indent;
    int title_height;
    int scrollbar_size;
    int thumb_size;
    guigui_Color colors[GUIGUI_COLOR_MAX];
} guigui_Style;

struct guigui_Context {
    /* callbacks */
    int (*text_width)(guigui_Font font, const char* str, int len);
    int (*text_height)(guigui_Font font);
    void (*draw_frame)(guigui_Context* ctx, guigui_Rect rect, int colorid);
    /* core state */
    guigui_Style _style;
    guigui_Style* style;
    guigui_Id hover;
    guigui_Id focus;
    guigui_Id last_id;
    guigui_Rect last_rect;
    int last_zindex;
    int updated_focus;
    int frame;
    guigui_Container* hover_root;
    guigui_Container* next_hover_root;
    guigui_Container* scroll_target;
    char number_edit_buf[GUIGUI_MAX_FMT];
    guigui_Id number_edit;
    /* stacks */
    guigui_stack(char, GUIGUI_COMMANDLIST_SIZE) command_list;
    guigui_stack(guigui_Container*, GUIGUI_ROOTLIST_SIZE) root_list;
    guigui_stack(guigui_Container*, GUIGUI_CONTAINERSTACK_SIZE) container_stack;
    guigui_stack(guigui_Rect, GUIGUI_CLIPSTACK_SIZE) clip_stack;
    guigui_stack(guigui_Id, GUIGUI_IDSTACK_SIZE) id_stack;
    guigui_stack(guigui_Layout, GUIGUI_LAYOUTSTACK_SIZE) layout_stack;
    /* retained state pools */
    guigui_PoolItem container_pool[GUIGUI_CONTAINERPOOL_SIZE];
    guigui_Container containers[GUIGUI_CONTAINERPOOL_SIZE];
    guigui_PoolItem treenode_pool[GUIGUI_TREENODEPOOL_SIZE];
    /* input state */
    guigui_Vec2 mouse_pos;
    guigui_Vec2 last_mouse_pos;
    guigui_Vec2 mouse_delta;
    guigui_Vec2 scroll_delta;
    int mouse_down;
    int mouse_pressed;
    int key_down;
    int key_pressed;
    char input_text[32];
};

guigui_Vec2 guigui_vec2(int x, int y);
guigui_Rect guigui_rect(int x, int y, int w, int h);
guigui_Color guigui_color(int r, int g, int b, int a);

void guigui_init(guigui_Context* ctx);
void guigui_begin(guigui_Context* ctx);
void guigui_end(guigui_Context* ctx);
void guigui_set_focus(guigui_Context* ctx, guigui_Id id);
guigui_Id guigui_get_id(guigui_Context* ctx, const void* data, int size);
void guigui_push_id(guigui_Context* ctx, const void* data, int size);
void guigui_pop_id(guigui_Context* ctx);
void guigui_push_clip_rect(guigui_Context* ctx, guigui_Rect rect);
void guigui_pop_clip_rect(guigui_Context* ctx);
guigui_Rect guigui_get_clip_rect(guigui_Context* ctx);
int guigui_check_clip(guigui_Context* ctx, guigui_Rect r);
guigui_Container* guigui_get_current_container(guigui_Context* ctx);
guigui_Container* guigui_get_container(guigui_Context* ctx, const char* name);
void guigui_bring_to_front(guigui_Context* ctx, guigui_Container* cnt);

int guigui_pool_init(guigui_Context* ctx, guigui_PoolItem* items, int len, guigui_Id id);
int guigui_pool_get(guigui_Context* ctx, guigui_PoolItem* items, int len, guigui_Id id);
void guigui_pool_update(guigui_Context* ctx, guigui_PoolItem* items, int idx);

void guigui_input_mousemove(guigui_Context* ctx, int x, int y);
void guigui_input_mousedown(guigui_Context* ctx, int x, int y, int btn);
void guigui_input_mouseup(guigui_Context* ctx, int x, int y, int btn);
void guigui_input_scroll(guigui_Context* ctx, int x, int y);
void guigui_input_keydown(guigui_Context* ctx, int key);
void guigui_input_keyup(guigui_Context* ctx, int key);
void guigui_input_text(guigui_Context* ctx, const char* text);

guigui_Command* guigui_push_command(guigui_Context* ctx, int type, int size);
int guigui_next_command(guigui_Context* ctx, guigui_Command** cmd);
void guigui_set_clip(guigui_Context* ctx, guigui_Rect rect);
void guigui_draw_rect(guigui_Context* ctx, guigui_Rect rect, guigui_Color color);
void guigui_draw_box(guigui_Context* ctx, guigui_Rect rect, guigui_Color color);
void guigui_draw_text(guigui_Context* ctx, guigui_Font font, const char* str, int len,
    guigui_Vec2 pos, guigui_Color color);
void guigui_draw_icon(guigui_Context* ctx, int id, guigui_Rect rect, guigui_Color color);

void guigui_layout_row(guigui_Context* ctx, int items, const int* widths, int height);
void guigui_layout_width(guigui_Context* ctx, int width);
void guigui_layout_height(guigui_Context* ctx, int height);
void guigui_layout_begin_column(guigui_Context* ctx);
void guigui_layout_end_column(guigui_Context* ctx);
void guigui_layout_set_next(guigui_Context* ctx, guigui_Rect r, int relative);
guigui_Rect guigui_layout_next(guigui_Context* ctx);

void guigui_draw_control_frame(guigui_Context* ctx, guigui_Id id, guigui_Rect rect, int colorid,
    int opt);
void guigui_draw_control_text(guigui_Context* ctx, const char* str, guigui_Rect rect,
    int colorid, int opt);
int guigui_mouse_over(guigui_Context* ctx, guigui_Rect rect);
void guigui_update_control(guigui_Context* ctx, guigui_Id id, guigui_Rect rect, int opt);

#define guigui_button(ctx, label) guigui_button_ex(ctx, label, 0, GUIGUI_OPT_ALIGNCENTER)
#define guigui_textbox(ctx, buf, bufsz) guigui_textbox_ex(ctx, buf, bufsz, 0)
#define guigui_slider(ctx, value, lo, hi) \
    guigui_slider_ex(ctx, value, lo, hi, 0, GUIGUI_SLIDER_FMT, GUIGUI_OPT_ALIGNCENTER)
#define guigui_number(ctx, value, step) \
    guigui_number_ex(ctx, value, step, GUIGUI_SLIDER_FMT, GUIGUI_OPT_ALIGNCENTER)
#define guigui_header(ctx, label) guigui_header_ex(ctx, label, 0)
#define guigui_begin_treenode(ctx, label) guigui_begin_treenode_ex(ctx, label, 0)
#define guigui_begin_window(ctx, title, rect) \
    guigui_begin_window_ex(ctx, title, rect, 0)
#define guigui_begin_panel(ctx, name) guigui_begin_panel_ex(ctx, name, 0)

void guigui_text(guigui_Context* ctx, const char* text);
void guigui_label(guigui_Context* ctx, const char* text);
int guigui_button_ex(guigui_Context* ctx, const char* label, int icon, int opt);
int guigui_checkbox(guigui_Context* ctx, const char* label, int* state);
int guigui_textbox_raw(guigui_Context* ctx, char* buf, int bufsz, guigui_Id id, guigui_Rect r,
    int opt);
int guigui_textbox_ex(guigui_Context* ctx, char* buf, int bufsz, int opt);
int guigui_slider_ex(guigui_Context* ctx, guigui_Real* value, guigui_Real low, guigui_Real high,
    guigui_Real step, const char* fmt, int opt);
int guigui_number_ex(guigui_Context* ctx, guigui_Real* value, guigui_Real step, const char* fmt,
    int opt);
int guigui_header_ex(guigui_Context* ctx, const char* label, int opt);
int guigui_begin_treenode_ex(guigui_Context* ctx, const char* label, int opt);
void guigui_end_treenode(guigui_Context* ctx);
int guigui_begin_window_ex(guigui_Context* ctx, const char* title, guigui_Rect rect,
    int opt);
void guigui_end_window(guigui_Context* ctx);
void guigui_open_popup(guigui_Context* ctx, const char* name);
int guigui_begin_popup(guigui_Context* ctx, const char* name);
void guigui_end_popup(guigui_Context* ctx);
void guigui_begin_panel_ex(guigui_Context* ctx, const char* name, int opt);
void guigui_end_panel(guigui_Context* ctx);
