#include <iostream>
#include <cmath>

#include "admin.h"
#include "io.h"
#include "login.h"
#include "operations.h"

void Admin::handle() {
  bool select = true;
  while(select) {
    switch(IO::prompt(IO::OPTIONS_ADMIN)) {
    case IO::OPTIONS_ADMIN_LOGOUT:
      select = false;
      break;
    case IO::OPTIONS_ADMIN_SUB:
      Admin::subInSafetyBox();
      break;
    case IO::OPTIONS_ADMIN_ADD:
      Admin::addInSafetyBox();
      break;
    case IO::OPTIONS_ADMIN_BALANCE:
      Operations::printBalance();
      break;
    case IO::OPTIONS_ADMIN_OPERATIONS:
      Admin::handleOperations();
      break;
    default:
      std::cout << "Invalid option selected." << std::endl;
      break;
    }
    if(select) std::cout << std::endl;
  }
}

void Admin::addInSafetyBox() {
  double initial = Login::user()->getBalance();

  std::cout << "Input deposit amount: ";
  std::string input;

  if(!IO::inputNumber(input, true)) {
    std::cout << "Amount must be a positive number." << std::endl;
    return;
  }

  double amount = std::ceil(stod(input) * 100.0) / 100.0;
  Login::user()->setBalance(initial + amount);

  Operations::printBalance();
}

void Admin::subInSafetyBox() {
  double initial = Login::user()->getBalance();

  std::cout << "Input withdrawal amount: ";
  std::string input;

  if(!IO::inputNumber(input, true)) {
    std::cout << "Amount must be a positive number." << std::endl;
    return;
  }
  double amount = std::ceil(stod(input) * 100.0) / 100.0;
  if(initial - amount < 0) {
    std::cout << "Insufficient credit in ATM's safety box. Operation aborted." << std::endl;
    return;
  } else {
    Login::user()->setBalance(initial - amount);
  }
  Operations::printBalance();
}


void Admin::handleOperations() {
  std::string number;

  std::cout << "Input user card number: ";
  if(!IO::inputNumber(number, true, true, 16)) {
    std::cout << "Invalid card number." << std::endl;
    return;
  }

  std::string adminNumber = Login::user()->getCardNumber();
  std::string adminPin = Login::user()->getPin();

  Login::logout();

  std::vector<int> match = IO::credentials->getCol(1)->has(&number, 1);
  int found = match[0];
  if(found != -1) {
    int id = IO::credentials->getCell(found, 0)->iget();
    // User with ID = 0 is admin
    Login::login(id);
    std::cout << std::endl;
    if(!Login::user()->isAdmin()) Operations::handle();
    else std::cout << "You are already logged in as admin." << std::endl;

    Login::login(IO::ADMIN_USER_ID);
    std::cout << "Logging back as admin..." << std::endl;
  }
}
