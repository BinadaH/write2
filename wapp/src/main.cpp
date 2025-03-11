#include <iostream>

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include "canvas.h"

int main() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    Uint32 window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
    SDL_Window* window = SDL_CreateWindow("Dear ImGui SDL3+OpenGL3 example", 1280, 720, window_flags);

    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);

    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync
    SDL_ShowWindow(window);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    
    ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;



    ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);


    //load fonts


    bool show_demo_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop flag
    bool quit = false;
    SDL_Event e;


    Canvas canvas;

    // Main loop
    while (!quit) {
        // Handle events
        while (SDL_PollEvent(&e) != 0) {
            //ImGui_ImplSDL3_ProcessEvent(&e);
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_EVENT_MOUSE_MOTION) {
                /*io.MousePos.x = e.motion.x;
                io.MousePos.y = e.motion.y;*/
                io.AddMouseSourceEvent(ImGuiMouseSource_Mouse);
                io.AddMousePosEvent(e.motion.x, e.motion.y);
            }
            else if (e.type == SDL_EVENT_PEN_MOTION) {
                io.AddMouseSourceEvent(ImGuiMouseSource_Pen);
                io.AddMousePosEvent(e.motion.x, e.motion.y);
            }
            else if (e.type == SDL_EVENT_PEN_DOWN) {
                io.AddMouseSourceEvent(ImGuiMouseSource_Pen);
                io.AddMouseButtonEvent(0, 1);
            }
            else if (e.type == SDL_EVENT_PEN_UP) {
                io.AddMouseSourceEvent(ImGuiMouseSource_Pen);
                io.AddMouseButtonEvent(0, 0);
            } 
            else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN || e.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                int mouse_button = -1;
                if (e.button.button == SDL_BUTTON_LEFT) { mouse_button = 0; }
                if (e.button.button == SDL_BUTTON_RIGHT) { mouse_button = 1; }
                if (e.button.button == SDL_BUTTON_MIDDLE) { mouse_button = 2; }
                if (e.button.button == SDL_BUTTON_X1) { mouse_button = 3; }
                if (e.button.button == SDL_BUTTON_X2) { mouse_button = 4; }
                if (mouse_button == -1)
                    break;
                io.AddMouseSourceEvent(ImGuiMouseSource_Mouse);
                io.AddMouseButtonEvent(mouse_button, (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN));
                canvas.SetCurrPress(1);

            }
            else if (e.type == SDL_EVENT_PEN_AXIS && e.paxis.axis == SDL_PEN_AXIS_PRESSURE) {
                canvas.SetCurrPress(e.paxis.value);
            }
            else
                ImGui_ImplSDL3_ProcessEvent(&e);
        }

        if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED)
        {
            SDL_Delay(10);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();




        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);
        canvas.Process(io);

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    }
    return 0;
}