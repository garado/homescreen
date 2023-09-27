#ifndef __CALENDAR_H__
#define __CALENDAR_H__

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

class Schedule {
private:

public:
  Component ui;

  Schedule();
};

#endif
