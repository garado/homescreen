#ifndef __TASK_H__
#define __TASK_H__

#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <nlohmann/json.hpp>
#include "ftxui/component/component.hpp"

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

  // Data manip
  void add_tag_and_project(std::string tag, std::string project);
  std::vector<std::string> extract_tags();
  std::vector<std::string> extract_projects_for_tag(std::string tag);

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
  Component tags;
  Component projects;
  Component tasklist;

public:
  // Backend-y stuff
  int tag_index = 0;
  int project_index = 0;
  int task_index = 0;
  std::vector<std::string> tag_entries;
  std::vector<std::string> project_entries;

  // UI components
  Component ui;

  TaskUI(TaskData * td);
};

#endif
