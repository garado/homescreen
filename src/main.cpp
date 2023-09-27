
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

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/flexbox_config.hpp"
#include "ftxui/screen/color.hpp"
#include "ftxui/screen/color_info.hpp"
#include "ftxui/screen/terminal.hpp"

#include "task.hpp"
#include "ledger.hpp"
#include "schedule.hpp"

using namespace ftxui;

int shift = 0;

int main(void) {
  auto screen = ScreenInteractive::Fullscreen();
  // screen.TrackMouse(false); // why is this not defined

  // init objects
  TaskData * task_d = new TaskData;
  TaskUI * task_ui = new TaskUI(task_d);
  Ledger * ledger = new Ledger;
  Schedule * schedule = new Schedule;

  int tab_index = 0;
  std::vector<std::string> tab_entries = {
    "Schedule", "Taskwarrior", "Ledger",
  };
  auto tab_selection =
    Menu(&tab_entries, &tab_index, MenuOption::HorizontalAnimated());
  auto tab_content = Container::Tab(
    {
      schedule->ui,
      task_ui->ui,
      ledger->ui,
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
