
#include"game/game.h"

map			world;
mainmenu 	menum;
config		cfg;
std::string NAME=		"forespend"				;
std::string VERSION=	"0.03g"					;
std::string PATH=		"../"					;

scene* current=&world;
//scene* current=&menum;
int main(int argc, char* argv[]) {
	init();
	while (!WindowShouldClose()) {
	render();
	}
	close();
	return 0;
}

#ifdef mobile
#include <android_native_app_glue.h>
#include <pthread.h>
#include <unistd.h>

// wrapper thread that just calls your existing main()
static void* call_main_thread(void* /*unused*/) {
    char* argv[] = { (char*)"a13", nullptr };
    main(1, argv);
    return nullptr;
}

extern "C" void android_main(struct android_app* app) {
    app_dummy(); // ensure native_app_glue linked

    int events;
    struct android_poll_source* source;

    // wait until a window/surface is available
    while (1) {
        while (ALooper_pollAll(-1, nullptr, &events, (void**)&source) >= 0) {
            if (source) source->process(app, source);
            if (app->destroyRequested) return;
        }
        if (app->window) break; // ready to create EGL / window-backed rendering
    }

    // start main in a background thread (UI thread not blocked)
    pthread_t thr;
    if (pthread_create(&thr, nullptr, call_main_thread, nullptr) != 0) {
        return;
    }

    // keep processing events until the activity is destroyed
    while (!app->destroyRequested) {
        while (ALooper_pollAll(0, nullptr, &events, (void**)&source) >= 0) {
            if (source) source->process(app, source);
        }
        usleep(10000);
    }

    // wait for main thread to finish
    pthread_join(thr, nullptr);
}

#endif