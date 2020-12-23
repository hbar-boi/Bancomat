#include <iostream>
#include <iomanip>
#include <fstream>

#include "csv/csv_file.h"
#include "login.h"
#include "operations.h"
#include "admin.h"

//Intervenire in caso di guasti: (Controllo accesso admin)
// - Cancellare righe
// - Aggiungere righe
//Extra: creare account

//Ritirare denaro ed assegni versati:

int main(int argc, char* argv[]) {
  std::ifstream splash("splash.txt");

  bool exit = false;
  std::cout << std::setprecision(2) << std::fixed;

  std::cout << splash.rdbuf();

  int count = 0;
  while(!exit) {
    std::string number, pin;

    std::cout << "Please input your card number: ";
    if(!IO::inputNumber(number, true, true, 16)) {
      std::cout << "Invalid card number." << std::endl;
      continue;
    }

    std::cout << "Please input your PIN: ";
    if(!IO::inputPin(pin)) {
      std::cout << std::endl << "Invalid pin." << std::endl;
      continue;
    }

    if(Login::login(number, pin)) {
      std::cout << std::endl;
      if(Login::user()->isAdmin()) Admin::handle();
      else Operations::handle();
      std::cout << "Logging out..." << std::endl << std::endl;
      Login::logout();
      count = 0;
    } else if(count == 3) {
      std::cout << "Maximum number of wrong attempts reached." << std::endl;
      return 0;
    }
    count++;
  }
  return 0;
}
