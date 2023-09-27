#ifndef __TASK_H__
#define __TASK_H__

#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <nlohmann/json.hpp>
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

using json = nlohmann::json;
using namespace ftxui;

typedef std::set<std::string> projects_t;

/* ▀█▀ ▄▀█ █▀ █▄▀    █▀▄ ▄▀█ ▀█▀ ▄▀█ */
/* ░█░ █▀█ ▄█ █░█    █▄▀ █▀█ ░█░ █▀█ */

class TaskData {
friend class TaskUI;
private:
  // Interacting with Taskwarrior directly
  void parse_tasks();

  // Data... stuff
  void add_tag_and_project(std::string tag, std::string project);
  std::vector<std::string> extract_tags();
  std::vector<std::string> extract_projects_for_tag(std::string tag);

  std::vector<std::string> query(std::string tag, std::string project);

public:
  json data;
  std::map<std::string, projects_t> tags;

  TaskData() {
    self:parse_tasks();
  }
};


/* ▀█▀ ▄▀█ █▀ █▄▀    █░█ █ */
/* ░█░ █▀█ ▄█ █░█    █▄█ █ */

class TaskUI {
private:
  TaskData * td;

public:
  // Backend-y stuff
  int tag_index = 0;
  int project_index = 0;
  int task_index = 0;
  int tasklist_progress = 50;
  std::vector<std::string> tag_entries;
  std::vector<std::string> project_entries;
  std::vector<std::string> task_entries;

  // Update component UI
  void update_tags_ui();
  void update_projects_ui();
  void update_tasklist_ui();

  // UI components
  Component tags;
  Component projects;
  Component tasklist;
  Component progressbar;
  Component ui;

  TaskUI(TaskData * td);
};

#endif
