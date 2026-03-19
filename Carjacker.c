#include <furi.h>
#include <furi_hal_random.h>
#include <gui/gui.h>
#include <gui/elements.h>
#include <notification/notification_messages.h>
#include <input/input.h>

#define MAX_CARS 100

// The Legendary Cyber-Squatch Icon (10x10)
static const uint8_t sasquatch_icon_bits[] = {
    0x1C, 0x00, 0x3E, 0x00, 0x7F, 0x00, 0x6B, 0x00, 0x7F, 0x00, 
    0x3E, 0x00, 0x3E, 0x00, 0x22, 0x00, 0x63, 0x00, 0x00, 0x00
};

const char* vehicles[] = {
    "Bugatti Chiron", "Lamborghini Huracan", "Ferrari SF90", "Porsche 918",
    "Tesla Roadster", "Rusty Tractor", "Shopping Cart", "Clown Car"
};

typedef struct {
    FuriMutex* mutex;
    bool running, scanning;
    uint8_t car_count;
    const char* captured_cars[MAX_CARS];
    uint32_t last_detection;
} CarJackerApp;

static void render_callback(Canvas* canvas, void* context) {
    CarJackerApp* app = context;
    if(furi_mutex_acquire(app->mutex, 25) != FuriStatusOk) return;

    canvas_clear(canvas);
    canvas_draw_frame(canvas, 0, 0, 128, 64);
    
    // Draw the Cyber-Squatch
    canvas_draw_xbm(canvas, 110, 5, 10, 10, sasquatch_icon_bits);

    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 12, AlignCenter, AlignBottom, "CYBER SCANNER");

    canvas_set_font(canvas, FontSecondary);
    for(uint8_t i = 0; i < 5; i++) {
        int8_t idx = app->car_count - 1 - i;
        if(idx >= 0) {
            canvas_draw_str(canvas, 10, 25 + (i * 8), app->captured_cars[idx]);
        }
    }

    if(app->scanning) {
        canvas_draw_str_aligned(canvas, 64, 60, AlignCenter, AlignBottom, "--- SCANNING ---");
    } else {
        canvas_draw_str_aligned(canvas, 64, 60, AlignCenter, AlignBottom, "OK TO START");
    }

    furi_mutex_release(app->mutex);
}

static void input_callback(InputEvent* event, void* context) {
    CarJackerApp* app = context;
    if(event->type == InputTypeShort) {
        if(event->key == InputKeyBack) app->running = false;
        if(event->key == InputKeyOk) app->scanning = !app->scanning;
    }
}

int32_t carjacker_app(void* p) {
    CarJackerApp* app = malloc(sizeof(CarJackerApp));
    app->mutex = furi_mutex_alloc(FuriMutexTypeNormal);
    app->running = true;
    app->scanning = false;
    app->car_count = 0;

    ViewPort* view_port = view_port_alloc();
    view_port_draw_callback_set(view_port, render_callback, app);
    view_port_input_callback_set(view_port, input_callback, app);

    Gui* gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);

    while(app->running) {
        uint32_t ticks = furi_get_tick();
        if(app->scanning && (ticks - app->last_detection > 2000) && (app->car_count < MAX_CARS)) {
            furi_mutex_acquire(app->mutex, FuriWaitForever);
            app->captured_cars[app->car_count++] = vehicles[furi_hal_random_get() % 8];
            furi_mutex_release(app->mutex);
            app->last_detection = ticks;
        }
        view_port_update(view_port);
        furi_delay_ms(50);
    }

    gui_remove_view_port(gui, view_port);
    view_port_free(view_port);
    furi_mutex_free(app->mutex);
    free(app);
    furi_record_close(RECORD_GUI);
    return 0;
}
