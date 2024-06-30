#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <windows.h>
using namespace std;

#define COLOR_RED 12
#define COLOR_GREEN 10
#define COLOR_YELLOW 14
#define COLOR_CYAN 13
#define COLOR_WHITE 15
#define COLOR_BLUE	12

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

class admin {
public:
    static bool adlogin(string username, string password) {
        if (username == "azain" && password == "azain") {
            return true;
        }
        else {
            return false;
        }
    }
};

class User {
private:
    string UserName;
    string Password;
    string PhoneNumber;
    static vector<User> Users;

public:
    User(string Username, string Password, string phone) : UserName(Username), Password(Password), PhoneNumber(phone) {
        Users.push_back(*this);
    }

    string GetUserName() const {
        return UserName;
    }
    string GetPassword() const {
        return Password;
    }
    string GetPhone() const {
        return PhoneNumber;
    }
    static bool Login(string username, string password) {
        for (size_t i = 0; i < Users.size(); i++) {
            if (Users[i].GetUserName() == username && Users[i].GetPassword() == password) {
                return true;
            }
        }
        return false;
    }
    static void Signup(string username, string password, string phonenumber) {
        Users.push_back(User(username, password, phonenumber));
        ofstream NewFile;
        NewFile.open("User Data.txt", ios::app);
        if (NewFile.is_open()) {
            NewFile << username << " " << password << " " << phonenumber << endl;
            NewFile.close();
            setColor(COLOR_GREEN);
            cout << "User data saved to file" << endl;
            setColor(COLOR_WHITE);
        }
        else {
            setColor(COLOR_RED);
            cout << "Error: unable to open file for writing" << endl;
            setColor(COLOR_WHITE);
        }
    }
};
vector<User> User::Users;

class Vehicle {
protected:
    string VehicleID;
    string Brand;

public:
    Vehicle(string ID, string brand) : VehicleID(ID), Brand(brand) {}
    virtual void DisplayDetails() const = 0;
    string GetVehicleID() const {
        return VehicleID;
    }
    string GetBrand() const {
        return Brand;
    }
};

class Car : public Vehicle {
private:
    string Make;
    string Model;
    int Year;
    double RentalPricePerDay;
    bool IsRented;
public:
    Car(string brand, string model, string ID, int enginesize, double price) : Vehicle(ID, brand), Model(model),
        RentalPricePerDay(price), IsRented(false) {}

    void DisplayDetails() const {
        cout << "Vehicle ID: " << VehicleID << endl;
        cout << "Brand: " << Brand << endl;
        cout << "Make: " << Make << endl;
        cout << "Model: " << Model << endl;
        cout << "Year: " << Year << endl;
        cout << "Rental Price Per Day: " << RentalPricePerDay << " $" << endl;
        cout << "Status: " << (IsRented ? "Rented" : "Available") << endl;
    }
    void RentOut() {
        if (!IsRented) {
            IsRented = true;
        }
        else {
            throw runtime_error("Car is already rented");
        }
    }
    void ReturnCar() {
        if (IsRented) {
            IsRented = false;
        }
        else {
            throw runtime_error("Car is not rented");
        }
    }
    string GetMake() const {
        return Make;
    }
    string GetModel() const {
        return Model;
    }
    double GetRentalPricePerDay() const {
        return RentalPricePerDay;
    }
};

class RentalManager {
private:
    vector<Vehicle*> vehicles;

public:
    void AddVehicle(string brand, string model, string ID, int enginesize, double price) {
        if (vehicles.size() < 5) {
            vehicles.push_back(new Car( brand, model,ID, enginesize, price));
            setColor(COLOR_GREEN);
            cout << "Vehicle added successfully" << endl;
            setColor(COLOR_WHITE);

            ofstream outFile("Vehicle Data.txt", ios::app);
            if (outFile.is_open()) {
                 outFile << brand << " " << model << " " << ID << " " << enginesize << " " << price << endl;
                outFile.close();
            }
            else {
                setColor(COLOR_RED);
                cout << "Error: Unable to open Vehicle Data file for writing." << endl;
                setColor(COLOR_WHITE);
            }
        }
        else {
            setColor(COLOR_RED);
            cout << "Cannot add more vehicles, maximum limit reached" << endl;
            setColor(COLOR_WHITE);
        }
    }

    void RemoveVehicle(string ID) {
    for (size_t i = 0; i < vehicles.size(); i++) {
        if (vehicles[i]->GetVehicleID() == ID) {
            vehicles.erase(vehicles.begin() + i);

            ofstream outFile("Vehicle Data.txt");
            if (outFile.is_open()) {
                for (size_t j = 0; j < vehicles.size(); j++) {
                    Car* car = dynamic_cast<Car*>(vehicles[j]); 
                    if (car) {
                        outFile << car->GetBrand() << " " << car->GetModel() << " " << car->GetVehicleID() << " " << car->GetRentalPricePerDay() << endl;
                    }
                }
                outFile.close();
                cout << "Vehicle with ID: " << ID << " removed successfully" << endl;
            } else {
                cout << "Error: Unable to open Vehicle Data file for writing." << endl;
            }

            return;
        }
    }
              
    setColor(COLOR_RED);
    cout << "Vehicle with ID: " << ID << " not found" << endl;
    setColor(COLOR_WHITE);
}

    void DisplayAvailableCars() {
        ifstream file("Vehicle Data.txt");
        if (file.is_open()) {
            string brand, model, vehicleID;
            int engineSize;
            double price;
            setColor(COLOR_BLUE);
            cout << "Available Cars:" << endl;
            setColor(COLOR_WHITE);
            while (file >> brand >> model >> vehicleID >> engineSize >> price) {
                setColor(COLOR_GREEN);
                cout << "Brand: " << brand << ", Model: " << model << ", Vehicle ID: " << vehicleID << ", Engine Size: " << engineSize << " CC"
                    << ", Rent per day: PKR " << price << endl;
            }
            setColor(COLOR_WHITE);
            file.close();
        }
        else {
            setColor(COLOR_RED);
            cout << "Error: Unable to open Vehicle Data file." << endl;
            setColor(COLOR_WHITE);
        }
    }

    void RentVehicle() {
        DisplayAvailableCars();

        string vehicleID;
        setColor(COLOR_BLUE);
        cout << "\nEnter vehicle id to rent a car: ";
        setColor(COLOR_WHITE);
        cin >> vehicleID;

        bool found = false;
        ifstream file("Vehicle Data.txt");
        if (file.is_open()) {
            string brand, model, id;
            int engineCC, price;

            while (file >> brand >> model >> id >> engineCC >> price) {
                if (id == vehicleID) {
                    found = true;
                    ofstream rentFile("Rented Vehicle Data.txt", ios::app);
                    if (rentFile.is_open()) {
                        rentFile << brand << " " << model << " " << id << " " << engineCC << " " << price << endl;
                        rentFile.close();
                    }
                    else {
                        setColor(COLOR_RED);
                        cout << "Error: unable to open rented vehicle data file for writing" << endl;
                        setColor(COLOR_WHITE);
                    }

                    setColor(COLOR_GREEN);
                    cout << "\nVehicle found:" << endl;
                    setColor(COLOR_CYAN);
                    cout << setw(15) << left << "Brand: " << setw(15) << left << brand << endl;
                    cout << setw(15) << left << "Model: " << setw(15) << left << model << endl;
                    cout << setw(15) << left << "ID: " << setw(15) << left << id << endl;
                    cout << setw(15) << left << "Engine: " << setw(15) << left << engineCC << "CC" << endl;
                    cout << setw(15) << left << "Rent per day: " << setw(15) << left << price << "PKR" << endl;

                    int days;
                    setColor(COLOR_YELLOW);
                    cout << "\nEnter number of days to rent: ";
                    setColor(COLOR_WHITE);
                    cin >> days;

                    double totalPrice = days * price;
                    int RandNumber = rand() % 1000 + 100;
                    char RandLetter = 'A' + rand() % 26;
                    stringstream InvoiceNum;
                    InvoiceNum << RandLetter << RandNumber;
                    Sleep(1000);
                    system("CLS");

                    setColor(COLOR_GREEN);
                    cout << "\n                       Car Rental - Customer Invoice                  " << endl;
                    cout << "    ///////////////////////////////////////////////////////////" << endl;
                    cout << "    | Invoice No. :" << "------------------|" << setw(10) << InvoiceNum.str() << " |" << endl;
                    cout << "    | Vehicle ID   :" << "-----------------|" << setw(10) << id << " |" << endl;
                    cout << "    | Brand :" << "------------------------|" << setw(10) << brand << " |" << endl;
                    cout << "    | Model :" << "------------------------|" << setw(10) << model << " |" << endl;
                    cout << "    | Engine CC :" << "--------------------|" << setw(10) << engineCC << " |" << endl;
                    cout << "    | Rental Days :" << "------------------|" << setw(10) << days << " |" << endl;
                    cout << "    | Rental Price per day :" << "---------|" << setw(7) << "PKR" << price << "|" << endl;
                    cout << "    | Total Price:" << "-------------------|" << setw(6) << "PKR" << totalPrice << "|" << endl;
                    cout << "     ________________________________________________________" << endl;
                    setColor(COLOR_WHITE);
                    ofstream InvoiceFile("Invoice.txt", ios::app);
                    if(InvoiceFile.is_open()){
                        InvoiceFile << InvoiceNum.str() << " " << brand << " " << model << " " << id << " " << engineCC << " " << price << " " << 
                        days << " " << totalPrice << endl;
                        InvoiceFile.close(); 
                    }
                    else{
                        setColor(COLOR_RED);
                        cout << "Error: unable to open Invoice file for writing" << endl;
                        setColor(COLOR_WHITE);
                    }

                    vector<string> tempData;

                    ifstream file("Vehicle Data.txt");
                    if (file.is_open()) {
                        string brand, model, vehicleID;
                        int engineSize, price;

                        while (file >> brand >> model >> vehicleID >> engineSize >> price) {
                            if (vehicleID != id) {
                                stringstream ss;
                                ss << brand << " " << model << " " << vehicleID << " " << engineSize << " " << price;
                                tempData.push_back(ss.str());
                            }
                        }

                        file.close();

                        ofstream outFile("Vehicle Data.txt");
                        if (outFile.is_open()) {
                            for (size_t i = 0; i < tempData.size(); i++) {
                                outFile << tempData[i] << endl;
                            }

                            outFile.close();

                        }
                        else {
                            setColor(COLOR_RED);
                            cout << "Error: Unable to open Vehicle Data file for writing." << endl;
                            setColor(COLOR_WHITE);
                        }
                        setColor(COLOR_GREEN);
                        cout << "Vehicle with ID: " << id << " rented successfully" << endl;
                        setColor(COLOR_WHITE);
                    }
                    else {
                        setColor(COLOR_RED);
                        cout << "Error: Unable to open Vehicle Data file." << endl;
                        setColor(COLOR_WHITE);
                    }

                    break;
                }
            }
            file.close();
        }
        else {
            setColor(COLOR_RED);
            cout << "Error: Unable to open Vehicle Data file." << endl;
            setColor(COLOR_WHITE);
        }

        if (!found) {
            setColor(COLOR_RED);
            cout << "Vehicle with ID: " << vehicleID << " not found" << endl;
            setColor(COLOR_WHITE);
        }
    }


    void ReturnVehicle(string ID) {
        ifstream rentedFile("Rented Vehicle Data.txt");
        if (rentedFile.is_open()) {
            string brand, model, vehicleID;
            int engineSize;
            double price;

            ofstream vehicleFile("Vehicle Data.txt", ios::app);
            if (vehicleFile.is_open()) {
                while (rentedFile >> brand >> model >> vehicleID >> engineSize >> price) {
                    if (vehicleID == ID) {
                        vehicleFile << brand << " " << model << " " << vehicleID << " " << engineSize << " " << price << endl;
                    }
                }
                vehicleFile.close();

                rentedFile.close();

                ofstream clearRentedFile("Rented Vehicle Data.txt");
                clearRentedFile.close();

                setColor(COLOR_GREEN);
                cout << "Vehicle with ID: " << ID << " added successfully"<< endl;
                setColor(COLOR_BLUE);
                DisplayAvailableCars();
            }
            else {
                setColor(COLOR_RED);
                cout << "Error: Unable to open Vehicle Data file for appending." << endl;
                setColor(COLOR_WHITE);
            }
        }
        else {
            setColor(COLOR_RED);
            cout << "Error: Unable to open Rented Vehicle Data file." << endl;
            setColor(COLOR_WHITE);
        }
    }

};

class CarRentalApplication {
public:
    RentalManager RentManager;
    bool IsAuthenticated;

    CarRentalApplication() : IsAuthenticated(false) {}

    void AuthenticateUser() {
        setColor(COLOR_CYAN);
        cout << "\n\t1. Login" << endl;
        cout << "\t2. Signup" << endl;
        setColor(COLOR_WHITE);
        int choice;
        cout << "\nEnter your choice: ";
        cin >> choice;
        switch (choice) {
        case 1:
            Login();
            break;
        case 2:
            Signup();
            break;
        default:
            setColor(COLOR_RED);
            cout << "Invalid choice" << endl;
            setColor(COLOR_WHITE);
        }
    }

    void Login() {
        string username, password;
        setColor(COLOR_YELLOW);
        cout << "Enter username: ";
        setColor(COLOR_WHITE);
        cin >> username;
        setColor(COLOR_YELLOW);
        cout << "Enter password: ";
        setColor(COLOR_WHITE);
        cin >> password;
        Sleep(1000);
        system("CLS");

        IsAuthenticated = User::Login(username, password);

        if (!IsAuthenticated) {
            setColor(COLOR_RED);
            cout << "Authentication failed" << endl;
            setColor(COLOR_WHITE);
        }
    }

    void Signup() {
        string username, password, phone;
        setColor(COLOR_YELLOW);
        cout << "Enter username: ";
        setColor(COLOR_WHITE);
        cin >> username;
        setColor(COLOR_YELLOW);
        cout << "Enter password: ";
        setColor(COLOR_WHITE);
        cin >> password;
        setColor(COLOR_YELLOW);
        cout << "Enter phone number: ";
        setColor(COLOR_WHITE);
        cin >> phone;
        Sleep(1000);
        system("CLS");

        User::Signup(username, password, phone);
    }

    void adminlogin() {
        string username, password;
        setColor(COLOR_YELLOW);
        cout << "Enter username: ";
        setColor(COLOR_WHITE);
        cin >> username;
        setColor(COLOR_YELLOW);
        cout << "Enter password: ";
        setColor(COLOR_WHITE);
        cin >> password;
        Sleep(1000);
        system("CLS");
        IsAuthenticated = admin::adlogin(username, password);
        if (!IsAuthenticated) {
            setColor(COLOR_RED);
            cout << "Authentication failed" << endl;
            setColor(COLOR_WHITE);
        }
    }

    void DisplayMenu() {
        setColor(COLOR_RED);
        cout << "\n1. Rent a Car" << endl;
        cout << "2. Return a Car" << endl;
        cout << "3. Exit the application" << endl;
        setColor(COLOR_WHITE);
    }

    void DisplayAdminMenu() {
        setColor(COLOR_RED);
        cout << "\n1. Add a Car" << endl;
        cout << "2. Remove a Car" << endl;
        cout << "3. Display Available Cars" << endl;
        cout << "4. Exit the application" << endl;
        setColor(COLOR_WHITE);
    }

    void HandleUserInput() {
	    int choice;
	    string vehicleID; 
	    cout << "Enter your choice: ";
	    cin >> choice;
	    Sleep(1000);
	    system("CLS");
	
	    switch (choice) {
	        case 1:
	            RentManager.RentVehicle();
	            break;
	        case 2: {
	            setColor(COLOR_YELLOW);
	            cout << "Enter vehicle id to return a car: ";
	            setColor(COLOR_WHITE);
	            cin >> vehicleID; 
	            RentManager.ReturnVehicle(vehicleID);
	            break;
	        }
	        case 3:
	            cout << "Exiting..." << endl;
	            IsAuthenticated = false;
	            break;
	        default:
	            setColor(COLOR_RED);
	            cout << "Invalid choice" << endl;
	            setColor(COLOR_WHITE);
	    }
	}


    void HandleAdminInput() {
	    int choice;
	    cout << "Enter your choice: ";
	    cin >> choice;
	    Sleep(1000);
	    system("CLS");
	    switch (choice) {
	    case 1:
	    {
	        string model, ID, brand;
	        int enginesize;
	        double price;
	        bool isrented;
	        setColor(COLOR_YELLOW);
	        cout << "Enter Vehicle Brand: ";
	        setColor(COLOR_WHITE);
	        cin >> brand;
	        setColor(COLOR_YELLOW);
	        cout << "Enter model: ";
	        setColor(COLOR_WHITE);
	        cin >> model;
	        setColor(COLOR_YELLOW);
	        cout << "Enter Vehicle ID: ";
	        setColor(COLOR_WHITE);
	        cin >> ID;
	        setColor(COLOR_YELLOW);
	        cout << "Enter Engine Size: ";
	        setColor(COLOR_WHITE);
	        cin >> enginesize;
	        setColor(COLOR_YELLOW);
	        cout << "Enter Price: ";
	        setColor(COLOR_WHITE);
	        cin >> price;
	        RentManager.AddVehicle(brand,model,ID,enginesize , price);
	        break;
	    }
	    case 2:
	    {
	        string vehicleID;
	        setColor(COLOR_YELLOW);
	        cout << "Enter vehicle id to remove car: ";
	        setColor(COLOR_WHITE);
	        cin >> vehicleID;
	        RentManager.RemoveVehicle(vehicleID);
	        break;
	    }
	    case 3:
	    {
	        RentManager.DisplayAvailableCars();
	        break;
	    }
	    case 4: {
	        setColor(COLOR_YELLOW);
	        cout << "Exiting..." << endl;
	        IsAuthenticated = false;
	        setColor(COLOR_WHITE);
	        break;
	    }
	    default:
	        setColor(COLOR_RED);
	        cout << "Invalid choice" << endl;
	        setColor(COLOR_WHITE);
	    }
	}

    void Start() {
        vector<User> users;
        vector<Vehicle*> vehicles;
        LoadUserData(users, "User Data.txt");
        LoadVehicleData(vehicles, "Vehicle Data.txt");
        setColor(COLOR_BLUE);
        cout << "\t-------------------Welcome to Fast Car Rental System-------------------\n" << endl;
        cout << "\t---------------------------------------------------------------------\n\n" << endl;
        setColor(COLOR_WHITE);
        int rolechoice;
do {
    setColor(COLOR_YELLOW);
    cout << "1. User" << endl;
    cout << "2. Admin" << endl;
    setColor(COLOR_WHITE);
    cout << "Enter Your Role: ";
    cin >> rolechoice;
    if (rolechoice == 1) {
        AuthenticateUser();
        while (IsAuthenticated) {
            DisplayMenu();
            HandleUserInput();
        }
    }
    else if (rolechoice == 2) {
        adminlogin();
        while (IsAuthenticated) {
            DisplayAdminMenu();
            HandleAdminInput();
        }
    }
    else {
        setColor(COLOR_RED);
        cout << "Invalid Role" << endl;
        setColor(COLOR_WHITE);
    }
} while (rolechoice != 1 && rolechoice != 2);

    }

    void LoadUserData(vector<User>& users, const string& filename) {
        ifstream file(filename.c_str());
        if (file.is_open()) {
            string username, password, phone;
            while (file >> username >> password >> phone) {
                users.push_back(User(username, password, phone));
            }
            file.close();
        }
        else {
            setColor(COLOR_RED);
            cout << "Error: Unable to open User Data file." << endl;
            setColor(COLOR_WHITE);
        }
    }

    void LoadVehicleData(vector<Vehicle*>& vehicles, const string& filename) {
        ifstream file(filename.c_str());
        if (file.is_open()) {
            string id, brand, make, model;
            int year;
            double price;
            while (file >> id >> brand >> make >> model >> year >> price) {
                vehicles.push_back(new Car(brand, model, id, year, price));

            }
            file.close();
        }
        else {
            setColor(COLOR_RED);
            cout << "Error: Unable to open Vehicle Data file." << endl;
            setColor(COLOR_WHITE);
        }
    }
};

int main() {
    CarRentalApplication app;
    app.Start();
    return 0;
}
