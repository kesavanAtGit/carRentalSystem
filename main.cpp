#include <cstdio>
#include <ios>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <fstream>
#include <vector>
#include <limits>
#include <chrono>
#include <ctime>
#include <sstream>

enum Carnames {
    mitsubishi,
    toyato,
    bentley,
    audi,
    hyundai,
    nissan,
    wolkswagon,
    max_cars,
};

struct Car {
    std::string name{};
    int mileage{};
    int fare{};
    int nSeats{};
    std::string carModel{};
    std::string carNumber{};
    std::string carColor{};
};

std::vector<Car> carArray{
        {"Mbishi", 30, 1000, 4, "Outlander", "M242", "black"},
        {"Toyata", 20, 2000, 5, "Camry", "T123", "blue"},
        {"Bentley", 25, 3000, 5, "Nental GT", "B550", "white"},
        {"Audi", 35, 4000, 6, "Audi AG", "A102", "grey"},
        {"Hyundai", 28, 5000, 6, "Tucson", "H210", "red"},
        {"Nissan",15, 6000, 6, "Magnite", "N893", "brown"},
        {"Wwagon", 40, 7000, 5, "Tiguan", "W584", "green"}
};

template <typename T>
void extractMeRight(T& value) {
    while(true) {
        if(std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Enter a valid input: ";
            std::cin >> value;
        }else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    }
}

std::string getDueString(std::time_t& nowTime, int durationInDays) {
    std::time_t base{nowTime + durationInDays * 24 * 60 * 60};
    std::string dueTimeStr{std::ctime(&base)};
    std::stringstream timeString{dueTimeStr};

    std::string str{};
    str = timeString.str();
    return str;
}

void dueTime(int nDays) {
    std::time_t base{time(0)};

    for(int i{0}; i < nDays; ++i) {
        std::time_t requiredTime{base + i * 24 * 60 * 60};
        std::cout << std::ctime(&requiredTime);
    }
}

void carsList() {
    std::cout << "The cars available for rent are: \n";
    int num{1};

    for(const auto car: carArray) {
        std::cout << num << ") " << car.name << '\n';
        ++num;
    }
}

void listCarDetails(int carIndex) {
    //std::cout << "Car Details \n";
    std::cout << "----------------------------------------------------------------------------------------------- \n";
    std::cout << "Name         Mileage(in kms)     Fare(in Rs)     Seats       CarModel    CarNumber     CarColor \n";
    std::cout << "----------------------------------------------------------------------------------------------- \n";
    std::cout << carArray[carIndex].name << "\t\t  " << carArray[carIndex].mileage << "\t\t    " <<carArray[carIndex].fare
              <<  "\t   " << carArray[carIndex].nSeats << "\t     " << carArray[carIndex].carModel << "\t   "
              <<  carArray[carIndex].carNumber << "\t\t" << carArray[carIndex].carColor << '\n';
    std::cout << "----------------------------------------------------------------------------------------------- \n";
}

Car carDetails() {

    carsList();

    std::cout << "Enter the respective car number to know its details. \n";
    int input{};
    std::cin >> input;

    extractMeRight(input);

    while(true) {
        if(input > 0 && input <= Carnames::max_cars) {
        int carIndex{static_cast<Carnames>(input) - 1};
        listCarDetails(carIndex);
        return carArray[carIndex];

        } else{
            extractMeRight(input);
            std::cin >> input;
        }
    }
}

void payLater(std::time_t& rentTime, int& durationInDays, int& rentDue, std::string& m_username) {
    std::cout << "The car has been booked. \n";
    std::time_t dueTime{rentTime + durationInDays * 24 * 60 * 60};
    std::cout << "Due date: " << std::ctime(&dueTime);

    std::fstream file{m_username + "Rent.txt", std::ios::out};
    if(!file) {
        std::cerr << "File cannot be opened for writing \n";
    }

    file << rentDue << '\n';
    file << durationInDays << '\n';
    file.close();
}

void payNow(int& totalFare) {
    std::cout << "You have chosen to pay now!!! \n";
    std::cout << "You can now pay the prompted amount: Rs." << totalFare << '\n';

    int fare{};
    std::cin >> fare;
    extractMeRight(fare);

    while(true) {
        if(fare == totalFare) {
            std::time_t nowTime{time(0)};
            std::cout << "Booking time: " << std::ctime(&nowTime)
                << "The car has been booked. Enjoy the ride! \n";
            return;

        }else if(fare < totalFare || fare > totalFare) {
            std::cout << "Enter the right amount of money!! \n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin >> fare;
        }
    }
}

bool pay() {
    std::cout << "Do you want to pay now(n) or later(l): n/l? ";
    char ch{};
    std::cin >> ch;

    while(true) {
        if(ch == 'n') {
            return true;
        }else if(ch == 'l') {
            return false;
        }
        else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Enter a valid input: \n";
            std::cin >> ch;
        }
    }
}

bool isEmpty(std::fstream& rentFile) {
    return (rentFile.peek() == std::fstream::traits_type::eof());
}

void isLiable(std::string& m_username) {
    std::fstream rentFile{m_username + "Rent.txt", std::ios::in};

    if(isEmpty(rentFile)) {
        return;
    }

    if(!rentFile) {
        std::cout << "No amount is due. \n";
    }else {
        std::cout << "Rent is due for the last booking. \n";
    }

    int rentDue{};
    int durationInDays{};
    rentFile >> rentDue;
    rentFile >> durationInDays;

    if(rentDue) {
        if(pay()) {
            payNow(rentDue);
            std::string fileStr{m_username + "Rent.txt"};
            std::remove(fileStr.c_str());
        }else {
            std::time_t timeNow{time(0)};
            payLater(timeNow, durationInDays, rentDue, m_username);
        }
    }
    rentFile.close();
}

void rentCar(std::string& m_username) {
    Car selectedCar{carDetails()};

    std::cout << "Would you like to rent the selected car: y/n? ";
    char ch{};
    std::cin >> ch;
    extractMeRight(ch);

    if(ch == 'y') {
        std::cout << "The fare for the selected car for a day is " << selectedCar.fare << ".\n";
        std::cout << "How long would you like to rent the car in days? \n";

        int durationInDays{};
        std::cin >> durationInDays;
        extractMeRight(durationInDays);

        int totalFare{durationInDays * selectedCar.fare};
        std::cout << "The total fare amount for " << durationInDays << " days for the car is Rs." << totalFare << ".\n";

        std::time_t timeNow(time(0));
        if(pay()) {
            payNow(totalFare);
        }else {
            //std::string dueStr{getDueString(timeNow, durationInDays)};
            payLater(timeNow, durationInDays, totalFare, m_username);
        }
    }
    else {
        std::cout << "Thanks for taking a look! Come again later. \n";
        return;
    }
}

class T {
    private:
        using Clock = std::chrono::high_resolution_clock;
        using duration = std::chrono::duration<double, std::ratio<1>>;
        std::chrono::time_point<Clock> someTime{Clock::now()};

    public:
        void reset() {
            someTime = Clock::now();
        }

        double gimmeTime() {
            return std::chrono::duration_cast<duration>(Clock::now() - someTime).count();
        }
};

class Register {
    protected:
        std::string m_username{};
        std::string m_password{};
        std::string m_copyUsername{};
        std::fstream file{};
        int m_isSignedUp{};

    public:
        bool isSignedUp() {
            if(m_username == m_copyUsername) {
                std::cout << "Username already exists! Try a different username :( \n";
                return true;
            }
            return false;
        }

        void signUp() {

            std::cout << "Enter your username: ";
            std::cin >> m_username;

            file.open(m_username + ".txt", std::ios::out);
            if(!file) {
                std::cerr << "Cannot open it for writing the credentials. \n";
            } else {
                // return early if the same username is used again to register
                if(isSignedUp()) {
                    file.close();
                    signUp();
                }
                else {
                    m_copyUsername = m_username;
                    std::cout << "Enter your password: ";
                    std::cin >> m_password;

                    file << m_username << '\n' << m_password;
                    std::cout << "Sign up successful! \n";
                    std::cout << "Username: " << m_username << '\n' << "Password: " << m_password << '\n';

                    ++m_isSignedUp;
                    std::cout << '\n';
                }
            }
            file.close();
        }

        std::string* getUsername() {return &m_username;}
        int& getSignUps() {return m_isSignedUp;}
};

class Login : public Register {
    private:
        std::string m_readUsername{};
        std::string m_readPassword{};
        std::string m_username{};
        std::string m_password{};
        std::string* m_ptr{getUsername()};
        int m_preLogin{};

    public:

        bool canLogin() {

            file.open(m_username + ".txt", std::ios::in);
            std::getline(file, m_readUsername);
            std::getline(file, m_readPassword);
            file.close();

            if(m_username == m_readUsername) {
                return true;
            }else {
                ++m_preLogin;
                return false;
            }
        }

        void login() {
            std::cout << "Username: ";
            std::cin >> m_username;

            // avoid the canLogin() function ever after preLogin becomes 1
            if(m_preLogin == 1 || getSignUps() > 0) {
                *m_ptr = m_username;
            }

            std::cout << "Password: ";
            std::cin >> m_password;

            if(!canLogin()) {
                std::cout << "You don't have an account named " << m_username << ". Sign up first! \n";
                signUp();
            }else {
                file.open(m_username + ".txt", std::ios::in);
                if(!file) {
                    std::cerr << "Can't open the goddamn file! \n";
                } else {
                    std::getline(file, m_readUsername);
                    std::getline(file, m_readPassword);

                    if(m_username == m_readUsername && m_password == m_readPassword) {
                        std::cout << "You logged in successfully! \n"; std::cout << '\n';

                        // check for any unpaid amount before booking any other car
                        isLiable(m_username);
                        rentCar(m_username);
                    }

                    else if(m_username == m_readUsername && m_password != m_readPassword) {
                        std::cerr << "Invalid password{:?} Check your password! \n";
                        std::cout << '\n';
                    }

                    else if(m_username != m_readUsername && m_password == m_readPassword) {
                        std::cerr << "Invalid username{:?} Check your username! \n";
                        std::cout << '\n';
                    }

                    else {
                        std::cerr << "Check your username and password and try again! \n";
                        std::cout << '\n';
                    }
                }
                file.close();
            }
        }
};

int main() {

    T time{};

    std::cout << "-------------------------------------------------Welcome to our car rental system!----- \n";
    Register signUp{};
    Login login{};

    while(true) {
        std::cout << "Choose one option: \n";
        std::cout << "1. SignIn/Login (press 1 to select this) \n";
        std::cout << "2. SignUp/Register (press 2 to select this) \n";
        std::cout << "Press any key and enter to exit \n";

        int input{};
        std::cin >> input;

        extractMeRight(input);

        if(input == 2)
            signUp.signUp();
        else if(input == 1)
            login.login();
        else {
            std::cout << "Sayonara :)! \n";
            break;
        }
    }
    std::cout << "process finished in " << time.gimmeTime() << "seconds. \n";

    return 0;
}
