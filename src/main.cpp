
/* █░█ █▀█ █▀▄▀█ █▀▀ █▀ █▀▀ █▀█ █▀▀ █▀▀ █▄░█ */
/* █▀█ █▄█ █░▀░█ ██▄ ▄█ █▄▄ █▀▄ ██▄ ██▄ █░▀█ */

#include <stddef.h>    // for size_t
#include <array>       // for array
#include <atomic>      // for atomic
#include <chrono>      // for operator""s, chrono_literals
#include <cmath>       // for sin
#include <functional>  // for ref, reference_wrapper, function
#include <memory>      // for allocator, shared_ptr, __shared_ptr_access
#include <string>  // for string, basic_string, char_traits, operator+, to_string
#include <thread>   // for sleep_for, thread
#include <utility>  // for move
#include <vector>   // for vector

#include "ftxui/component/component.hpp"  // for Checkbox, Renderer, Horizontal, Vertical, Input, Menu, Radiobox, ResizableSplitLeft, Tab
#include "ftxui/component/component_base.hpp"  // for ComponentBase, Component
#include "ftxui/component/component_options.hpp"  // for MenuOption, InputOption
#include "ftxui/component/event.hpp"              // for Event, Event::Custom
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for text, color, operator|, bgcolor, filler, Element, vbox, size, hbox, separator, flex, window, graph, EQUAL, paragraph, WIDTH, hcenter, Elements, bold, vscroll_indicator, HEIGHT, flexbox, hflow, border, frame, flex_grow, gauge, paragraphAlignCenter, paragraphAlignJustify, paragraphAlignLeft, paragraphAlignRight, dim, spinner, LESS_THAN, center, yframe, GREATER_THAN
#include "ftxui/dom/flexbox_config.hpp"  // for FlexboxConfig
#include "ftxui/screen/color.hpp"  // for Color, Color::BlueLight, Color::RedLight, Color::Black, Color::Blue, Color::Cyan, Color::CyanLight, Color::GrayDark, Color::GrayLight, Color::Green, Color::GreenLight, Color::Magenta, Color::MagentaLight, Color::Red, Color::White, Color::Yellow, Color::YellowLight, Color::Default, Color::Palette256, ftxui
#include "ftxui/screen/color_info.hpp"  // for ColorInfo
#include "ftxui/screen/terminal.hpp"    // for Size, Dimensions

#include "task/ui.cpp"

using namespace ftxui;

int shift = 0;

int main(void) {
  auto screen = ScreenInteractive::Fullscreen();

  int tab_index = 0;
  std::vector<std::string> tab_entries = {
    "task", "time", "cal", "ledger", 
  };
  auto tab_selection =
    Menu(&tab_entries, &tab_index, MenuOption::HorizontalAnimated());
  auto tab_content = Container::Tab(
    {
      task,
      task,
      task,
      task,
    },
    &tab_index);

  auto main_container = Container::Vertical({
    tab_selection,
    tab_content,
  });

  auto main_renderer = Renderer(main_container, [&] {
    return vbox({
      text("Homescreen") | bold | hcenter,
      tab_selection->Render(),
      tab_content->Render() | flex,
    });
  });

  std::atomic<bool> refresh_ui_continue = true;
  std::thread refresh_ui([&] {
    while (refresh_ui_continue) {
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(0.05s);
      // The |shift| variable belong to the main thread. `screen.Post(task)`
      // will execute the update on the thread where |screen| lives (e.g. the
      // main thread). Using `screen.Post(task)` is threadsafe.
      screen.Post([&] { shift++; });
      // After updating the state, request a new frame to be drawn. This is done
      // by simulating a new "custom" event to be handled.
      screen.Post(Event::Custom);
    }
  });

  screen.Loop(main_renderer);
  refresh_ui_continue = false;
  refresh_ui.join();
}
