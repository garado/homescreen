
/* █▀▀ ▄▀█ █░░ █▀▀ █▄░█ █▀▄ ▄▀█ █▀█ */
/* █▄▄ █▀█ █▄▄ ██▄ █░▀█ █▄▀ █▀█ █▀▄ */

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

#include "schedule.hpp"

using namespace ftxui;

/* @function schedule
 * @brief ctor */
Schedule::Schedule() {

  auto calendar = window(text(" Calendar ") | bold, vbox({
    filler(),
    hbox({
      filler(),
      text("placeholder"),
      filler(),
    }),
    filler(),
  }) | flex);

  auto placeholder = window(text(" ??? ") | bold, vbox({}));

  auto list = window(text(" Schedule ") | bold, vbox({

  }));

  // tbh i don't know what this is for
  auto schedule_component = Container::Horizontal({});

  // Define renderer
  this->ui = Renderer(schedule_component, [=] {
    return hbox({
      vbox({
        calendar | flex,
        placeholder | flex,
      }) | size(WIDTH, EQUAL, 40),
      list | flex,
    });
  });
}
