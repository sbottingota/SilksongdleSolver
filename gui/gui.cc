#include "gui.h"

#include "application.h"

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

#include <SDL3/SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL3/SDL_opengles2.h>
#else
#include <SDL3/SDL_opengl.h>
#endif

#include <cstdio>
#include <cstdlib>
#include <iostream>

static SDL_Window *window = nullptr;
static SDL_GLContext gl_context = nullptr;

static bool running = false;

extern "C" void gui_init(void) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        std::exit(EXIT_FAILURE);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK,
        SDL_GL_CONTEXT_PROFILE_CORE
    );

    window = SDL_CreateWindow(
        "Silksongdle Solver",
        1280,
        720,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    if (!window) {
        std::fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        std::exit(EXIT_FAILURE);
    }

    gl_context = SDL_GL_CreateContext(window);

    if (!gl_context) {
        std::fprintf(
            stderr,
            "SDL_GL_CreateContext failed: %s\n",
            SDL_GetError()
        );
        std::exit(EXIT_FAILURE);
    }

    if (!SDL_GL_MakeCurrent(window, gl_context)) {
        std::fprintf(
            stderr,
            "SDL_GL_MakeCurrent failed: %s\n",
            SDL_GetError()
        );
        std::exit(EXIT_FAILURE);
    }

    if (!SDL_GL_SetSwapInterval(1)) {
        std::fprintf(
            stderr,
            "SDL_GL_SetSwapInterval failed: %s\n",
            SDL_GetError()
        );
        std::exit(EXIT_FAILURE);
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 330");

    running = true;
}

extern "C" bool gui_is_running(void) {
    return running;
}

extern "C" void gui_frame(void) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL3_ProcessEvent(&event);

        if (event.type == SDL_EVENT_QUIT)
            running = false;
    }

    // imgui frame
    draw_app_frame();

    // render imgui
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(window);
}

extern "C" void gui_shutdown(void) {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();

    ImGui::DestroyContext();

    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    running = false;
    window = nullptr;
    gl_context = nullptr;
}

