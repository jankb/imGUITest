#include <iostream>
#include <map>
#include <chrono>
#include <thread>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl2.h"
#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>

float translateX(float x)
{
  float scale = ImGui::GetIO().DisplaySize.x / 640;
  return x * scale;
}

float translateY(float y)
{
  float scale = ImGui::GetIO().DisplaySize.y / 480;
  return y * scale;
}

int main()
{
  std::cout << "Starting ..." << std::endl;
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER ) != 0)
  {
    printf("Error: %s\n", SDL_GetError());
    return -1;
  }

  // Setup window
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI );
  SDL_Window* window = SDL_CreateWindow("My ImGUI Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, window_flags);
  if (SDL_SetWindowOpacity(window, 0.1f) != 0)
    printf("Opacity not supported...");
  SDL_GLContext gl_context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, gl_context);
  SDL_GL_SetSwapInterval(1); // Enable vsync
  

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;


  ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
  ImGui_ImplOpenGL2_Init();

  float scaleFactor = io.DisplaySize.x / 640;
  if (scaleFactor <= 1) scaleFactor = 1;
  io.Fonts->AddFontDefault();
  std::map<int, ImFont*>fontmap;

  for (int fontSize = 1; fontSize < 40; fontSize++)
  {
      ImFont *font = io.Fonts->AddFontFromFileTTF("./fonts/Vera.ttf", fontSize);
      std::cout << "font " << fontSize << std::endl;
      fontmap.insert(std::pair<int, ImFont*>(fontSize, font));
  }

  /*
  int fontSize  = 18;
  ImFont *font = io.Fonts->AddFontFromFileTTF("./fonts/Vera.ttf", fontSize);
  std::cout << "font " << fontSize << " is " << (font->IsLoaded() ? "" : "not") << " loaded" << std::endl;
  fontmap.insert(std::pair<int, ImFont*>(fontSize, font));

  fontSize = 24;
  font = io.Fonts->AddFontFromFileTTF("./fonts/Vera.ttf", fontSize);
  std::cout << "font " << fontSize << " is " << (font->IsLoaded() ? "" : "not") << " loaded" << std::endl;
  fontmap.insert(std::pair<int, ImFont*>(fontSize, font));
*/
  ImFont* currentFont = fontmap[18];

  ImVec4 clear_color = ImVec4(0.0f, 0.8f, 0.0f, 0.5f);
  bool done = false;

  while (!done)
  {


    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.



    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
    {
        done = true;
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::GetStyle().WindowRounding = 0.0f;
    ImGui::GetStyle().WindowBorderSize = 0.0f;
    ImGui::GetStyle().WindowPadding.x = 0.0f;
    ImGui::GetStyle().WindowPadding.y = 0.0f;
    ImGui::GetStyle().FrameBorderSize = 0.0f;
    ImGui::GetStyle().FramePadding.x = 0.0f;
    ImGui::GetStyle().FramePadding.y = 0.0f;
    ImGui::GetStyle().FrameRounding = 0.0f;

    ImGui::NewFrame();

    ImGui::PushFont(currentFont);

    {  // Quit button
      char buttonText[128] = "Quit";
      ImVec2 windowSize(ImGui::CalcTextSize(buttonText));
      ImGui::SetNextWindowSize(windowSize);
      ImGui::SetNextWindowBgAlpha(0.0f);

      ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x-windowSize.x-5, io.DisplaySize.y-windowSize.y-5));
      ImGui::Begin("QuitWindow", NULL, ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration);
      if(ImGui::Button(buttonText))
      {
        done = true;
      }

      ImGui::End();
    }

    { // Crosshair
      ImVec2 reticleSize(200, 200);
      ImVec2 reticleTopLeftPos((io.DisplaySize.x / 2) - (reticleSize.x / 2),
                               (io.DisplaySize.y / 2) - (reticleSize.y / 2));
      ImGui::SetNextWindowBgAlpha(0.00001f);
      ImGui::SetNextWindowPos(reticleTopLeftPos);
      ImGui::SetNextWindowSize(reticleSize);
      ImGui::Begin("CrossHairWindow", NULL, ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration);
      static ImVec4 colf = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
      const ImU32 col = ImColor(colf);
      ImDrawList *draw_list = ImGui::GetWindowDrawList();
      ImVec2 p = ImGui::GetCursorScreenPos();
      draw_list->AddLine(ImVec2(p.x+100, p.y), ImVec2(p.x+100.0, p.y+200.0), col);
      draw_list->AddLine(ImVec2(p.x, p.y+100), ImVec2(p.x+200.0, p.y+100.0), col);
      ImGui::End();
    }

    { // Blue  box
      ImVec2 zeroingSize(100, 100);
      ImVec2 zeroingTopLeftPos((io.DisplaySize.x / 2) - (zeroingSize.x / 2),
                               (io.DisplaySize.y / 2) - (zeroingSize.y / 2));
      ImGui::SetNextWindowBgAlpha(0.2f);
      ImGui::SetNextWindowPos(zeroingTopLeftPos);
      ImGui::SetNextWindowSize(zeroingSize);
      ImGui::Begin("BlueBoxWindow", NULL, ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration);
      static ImVec4 colf = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
      const ImU32 col = ImColor(colf);
      ImDrawList *draw_list = ImGui::GetWindowDrawList();
      ImVec2 p = ImGui::GetCursorScreenPos();
      draw_list->AddLine(ImVec2(p.x, p.y), ImVec2(p.x+100.0, p.y), col);
      draw_list->AddLine(ImVec2(p.x, p.y+50), ImVec2(p.x+200.0, p.y+200.0), col);
      ImGui::End();
    }

    { //Menu
      char menuItem[20] = ("Settings");
      ImGui::SetNextWindowBgAlpha(0.0f);      
      ImVec2 textSize = ImGui::CalcTextSize(menuItem);
      ImGui::SetNextWindowSize(textSize);
      ImGui::SetNextWindowPos(ImVec2(1,1));
      ImGui::Begin("MenuWindow", NULL, ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration);
      ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f),"%s",menuItem);

      ImGui::End();
    }

    { //Messages to user message
      char bitMessage[128] = ("Message something something, game over.");
      ImVec2 fontSize = ImGui::CalcTextSize(bitMessage);
      ImGui::SetNextWindowBgAlpha(0.0f);
      ImGui::SetNextWindowPos(ImVec2(1.0, translateY(400.0)));
      ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, 0.0));

      ImGui::Begin("UserMessage", NULL, ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration);

      {
        const ImU32 bit_col = ImColor(ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
        ImDrawList *draw_list = ImGui::GetWindowDrawList();
        ImVec2 p = ImGui::GetCursorScreenPos();

        ImVec2 pMax(p.x+io.DisplaySize.x, p.y + fontSize.y);
        draw_list->AddRectFilled(ImVec2(p.x, p.y), pMax, bit_col);
      }
      ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s", bitMessage);

      ImGui::End();
    }

    /*  {
         ImGui::SetNextWindowBgAlpha(0.35f);
      static float f = 0.0f;
      static int counter = 0;
      ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

      ImGui::Text("Windowsize is %f %f", io.DisplaySize.x, io.DisplaySize.y);               // Display some text (you can use a format strings too)

      ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
      ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

      if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        counter++;
      ImGui::SameLine();
      ImGui::Text("counter = %d", counter);

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      ImGui::End();

    }*/
    ImGui::PopFont();
    // Rendering
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    //glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);


    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      ImGui_ImplSDL2_ProcessEvent(&event);
      if (event.type == SDL_QUIT)
        done = true;

      if (event.type == SDL_WINDOWEVENT)
      {
          switch(event.window.event)
          {
          case SDL_WINDOWEVENT_SIZE_CHANGED:
          case SDL_WINDOWEVENT_RESIZED:
          {
              std::cout << "event.window.data1 " << event.window.data1 << std::endl;

              float newX = static_cast<float>(event.window.data1);
              scaleFactor =  newX / 640.0f;
              std::cout << "scaleFactor " << scaleFactor << std::endl;
              if (scaleFactor <= 0.1) scaleFactor = 0.1;
              if (scaleFactor > 2) scaleFactor = 2;
              int fontSize = 18*scaleFactor;
              std::cout << "Loading new scalefactor " << scaleFactor << " Scalefactor as int " << (int)scaleFactor << " Fontsize  " << fontSize << std::endl;
              currentFont = fontmap[fontSize];
             std::cout << "font " << fontSize << " is " << (currentFont->IsLoaded() ? "" : "not") << " loaded" << std::endl;




              break;
          }
          }
      }


    }

  }

  return 0;
}
