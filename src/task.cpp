
/* ▀█▀ ▄▀█ █▀ █▄▀ █░█░█ ▄▀█ █▀█ █▀█ █ █▀█ █▀█ */
/* ░█░ █▀█ ▄█ █░█ ▀▄▀▄▀ █▀█ █▀▄ █▀▄ █ █▄█ █▀▄ */

#include <memory>      // for allocator, shared_ptr, __shared_ptr_access
#include <string>  // for string, basic_string, char_traits, operator+, to_string
#include <thread>   // for sleep_for, thread
#include <utility>  // for move
#include <vector>   // for vector
#include <iostream>
#include <array>
#include <set>

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

#include <nlohmann/json.hpp>
#include "task.hpp"

using json = nlohmann::json;
using namespace ftxui;
using namespace std;


/* ▀█▀ ▄▀█ █▀ █▄▀    █▀▄ ▄▀█ ▀█▀ ▄▀█ */
/* ░█░ █▀█ ▄█ █░█    █▄▀ █▀█ ░█░ █▀█ */


/* @function extract_projects_for_tag
 * @brief Get vector of projects for a given tag. This is because fxtui radiobox requires a vector of strings 
 * but I've stored the projects as a set within a map */
std::vector<std::string> TaskData::extract_projects_for_tag(std::string tag) {
  std::vector<std::string> retval;
  for (auto const& element : this->tags.at(tag)) {
    retval.push_back(element);
  }
  return retval;
}


/* @function extract_tags
 * @brief Get vector of tags. This is because fxtui radiobox requires a vector of strings 
 * but I've stored the tags within a map */
std::vector<std::string> TaskData::extract_tags() {
  std::vector<std::string> retval;
  for (auto const& element : this->tags) {
    retval.push_back(element.first);
  }
  return retval;
}

/* @function add_tag_and_project 
 * @brief Add a tag and one of its projects to internal data
 **/
void TaskData::add_tag_and_project(const std::string tag, const std::string project) {
  if (this->tags.count(tag) == 0) {
    // tag doesn't exist: create new project list; insert into tag map
    // TODO: surely there are better ways to write this
    projects_t projectlist;
    projectlist.insert(project);
    this->tags.insert(std::pair<std::string, projects_t>(tag, projectlist) );
  } else {
    // tag already exists: insert into its existing project set
    this->tags.at(tag).insert(project);
  }
}

/* @function parse_tasks
 * @brief Fetch all tasks from Taskwarrior and build internal database of tags and projects */
void TaskData::parse_tasks () {
  string result;
  std::array<char, 128> buffer;

  const char * command = "task status:pending export";
  FILE *p = popen(command, "r");

  while (fgets(buffer.data(), 123, p) != NULL) {
    result += buffer.data();
  }

  pclose(p);

  this->data = json::parse(result);
  for (const auto it : this->data) {
    this->add_tag_and_project(it["tags"][0], it["project"]);
  }
}


/* @function query
 * @brief Return all tasks that match the tag and project */
std::vector<std::string> TaskData::query(std::string tag, std::string project) {
  // NOTE: is it better to do this or run another shell command? no idea
  std::vector<std::string> ret;
  for (const auto& t : this->data) {
    if (t["tags"][0] == tag && t["project"] == project) {
      ret.push_back(t["description"]);
    }
  }
  return ret;
}

/* ▀█▀ ▄▀█ █▀ █▄▀    █░█ █ */
/* ░█░ █▀█ ▄█ █░█    █▄█ █ */

const std::vector<std::string> entries = {
  "Project 1",
  "Project 2",
  "Project 3",
  "Project 4",
};


void TaskUI::update_tags_ui() {}

void TaskUI::update_projects_ui() {
  std::string t = this->tag_entries[this->tag_index];
  this->project_index = 0;
  this->project_entries.clear();
  this->project_entries = this->td->extract_projects_for_tag(t);
}

void TaskUI::update_tasklist_ui() {
  std::string t = this->tag_entries[this->tag_index];
  std::string p = this->project_entries[this->project_index];
  this->task_index = 0;
  this->task_entries.clear();
  this->task_entries = this->td->query(t, p);
}

/* @function TaskUI 
 * @brief ctor for TaskUI */
TaskUI::TaskUI(TaskData * td) {
  this->td = td;

  // Tags
  this->tag_entries = td->extract_tags();
  this->tags = Radiobox(&this->tag_entries, &this->tag_index);
  this->tags |= CatchEvent([&](Event event) {
    bool ret = false;
    if (event == Event::Return) {
      this->update_projects_ui();
      this->update_tasklist_ui();
    }
    return ret;
  });

  // Projects
  this->project_entries = td->extract_projects_for_tag(this->tag_entries[0]);
  this->projects = Radiobox(&this->project_entries, &this->project_index);
  this->projects |= CatchEvent([&](Event event) {
    bool ret = false;
    if (event == Event::Return) {
      this->update_tasklist_ui();
    }
    return ret;
  });

  // Tasklist
  this->task_entries = td->query(this->tag_entries[0], this->project_entries[0]);
  this->tasklist = Menu(&task_entries, &this->task_index);

  // Tasklist progress bar

  // Assembly (Renderer)
  auto task_component = Container::Horizontal({
    tags,
    projects,
    tasklist,
  });

  this->ui = Renderer(task_component, [=] {
    auto tags_win = window(text(" Tags ") | bold,
                           (this->tags)->Render() | vscroll_indicator | frame);

    auto projects_win = window(text(" Projects ") | bold,
                               (this->projects)->Render() | vscroll_indicator | frame);

    auto tasklist_win = window(text(""), vbox({
      vbox({
        hbox({
          filler(),
          text(this->tag_entries[this->tag_index]) | bold,
          text(" - ") | bold,
          text(this->project_entries[this->project_index]) | bold,
          filler(),
        }) | flex,
        gauge(40),
      }),
      separator(),
      (this->tasklist)->Render() | vscroll_indicator | frame,
    }) | flex);

    return hbox({
      vbox({
        tags_win | flex,
        projects_win | flex,
      }) | size(WIDTH, EQUAL, 25),
      tasklist_win,
    });
  });
}
