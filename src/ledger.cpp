
/* █░░ █▀▀ █▀▄ █▀▀ █▀▀ █▀█ */
/* █▄▄ ██▄ █▄▀ █▄█ ██▄ █▀▄ */

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

#include "ledger.hpp"

using namespace ftxui;

/* @function Ledger
 * @brief ctor */
Ledger::Ledger() {

  // tbh i don't know what this is for
  auto ledger_component = Container::Horizontal({});

  // Define renderer
  this->ui = Renderer(ledger_component, [=] {

    auto top_total = window(text(" Total ") | bold, text("$123.00")) | flex;
    auto top_checking = window(text(" Checking ") | bold, text("$123.00")) | flex;
    auto top_savings = window(text(" Savings ") | bold, text("$123.00")) | flex;
    auto top_cash = window(text(" Cash ") | bold, text("$123.00")) | flex;
    auto top_income = window(text(" Income ") | bold, text("$123.00")) | flex;
    auto top_expenses = window(text(" Expenses ") | bold, text("$123.00")) | flex;

    auto topbar = hflow({
      top_total,
      top_checking,
      top_savings,
      top_cash,
      top_income,
      top_expenses,
    });

    auto budget = window(text(" Budget ") | bold, vbox({
    })) | flex;
    
    auto transactions = window(text(" Transactions ") | bold, vbox({
    })) | flex;
    
    auto debts = window(text(" Reimbursements & Liabilities") | bold, vbox({
    })) | flex;
    
    auto trend = window(text(" Yearly Balance ") | bold, vbox({
    })) | flex;

    return vbox({
      topbar,
      hbox({
        budget,
        transactions,
        vbox({
          debts,
          trend,
        }) | size(WIDTH, EQUAL, 30),
      }) | flex,
    });
  });
}
