
/* ▀█▀ ▄▀█ █▀ █▄▀ █░█░█ ▄▀█ █▀█ █▀█ █ █▀█ █▀█ */
/* ░█░ █▀█ ▄█ █░█ ▀▄▀▄▀ █▀█ █▀▄ █▀▄ █ █▄█ █▀▄ */

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

using namespace ftxui;

// TAGS ======================
const std::vector<std::string> tag_entries = {
  "Test",
  "Cozy",
  "Personal",
  "Self-study",
};

int tag_selected = 0;
Component tags = Radiobox(&tag_entries, &tag_selected);

// PROJECTS ======================
const std::vector<std::string> project_entries = {
  "Project 1",
  "Project 2",
  "Project 3",
  "Project 4",
};

int project_selected = 0;
Component projects = Radiobox(&project_entries, &project_selected);

// TASKLIST ======================
std::vector<std::string> task_entries = {
  "Test", "Test", "Test", "Test", "Test", "Test", "Test", "Test", "Test", "Test", 
  "Test", "Test", "Test", "Test", "Test", "Test", "Test", "Test", "Test", "Test", 
  "Test", "Test", "Test", "Test", "Test", "Test", "Test", "Test", "Test", "Test", 
  "Test", "Test", "Test", "Test", "Test", "Test", "Test", "Test", "Test", "Test", 
  "Test", "Test", "Test", "Test", "Test", "Test", "Test", "Test", "Test", "Test", 
  "Test", "Test", "Test", "Test", "Test", "Test", "Test", "Test", "Test", "Test", 
  "Test", "Test", "Test", "Test", "Test", "Test", "Test", "Test", "Test", "Test", 
  "Test", "Test", "Test", "Test", "Test", "Test", "Test", "Test", "Test", "Test", 
};

int task_selected = 0;
Component tasklist = Radiobox(&task_entries, &task_selected);

// ASSEMBLY ======================

// what is this for?
auto task_component = Container::Horizontal({
  tags,
  projects,
  tasklist,
});

// what is this?
// auto render_command = [] {
//   task_entries.push_back("Test");
// };

auto task = Renderer(task_component, [] {
  auto tags_win = window(text(" Tags "),
                        tags->Render() | vscroll_indicator | frame);

  auto projects_win = window(text(" Projects "),
                        projects->Render() | vscroll_indicator | frame);

  auto tasklist_win = window(text(""), vbox({
    text("Project name") | bold,
    tasklist->Render() | vscroll_indicator | frame,
  }) | flex);

  return hbox({
    vbox({
      tags_win | flex,
      projects_win | flex,
    }) | flex,
    tasklist_win | flex,
  }) | flex;
});

auto tmp = Renderer([] {
  auto frequency = vbox({
    text("Frequency [Mhz]") | hcenter,
    hbox({
      vbox({
        text("2400 "),
        filler(),
        text("1200 "),
        filler(),
        text("0 "),
      }),
    }) | flex,
  });

  auto utilization = vbox({
    text("Utilization [%]") | hcenter,
    hbox({
      vbox({
        text("100 "),
        filler(),
        text("50 "),
        filler(),
        text("0 "),
      }),
    }) | flex,
  });

  auto ram = vbox({
    text("Ram [Mo]") | hcenter,
    hbox({
      vbox({
        text("8192"),
        filler(),
        text("4096 "),
        filler(),
        text("0 "),
      }),
    }) | flex,
  });

  return hbox({
    vbox({
      frequency | flex,
      separator(),
      utilization | flex,
    }) | flex,
    separator(),
    ram | flex,
  }) |
  flex;
});
