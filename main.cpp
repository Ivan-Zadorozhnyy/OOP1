#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class Aircraft {
private:
    string model;
    int capacity;

public:
    Aircraft(string mdl, int cap): model(mdl), capacity(cap) {}

    string getModel() const {return model;}
    int getCapacity() const {return capacity;}
};

class FlightTicket {
private:
    string ticketId;
    string passengerName;
    string journeyDate;
    string seatId;
    bool isBooked;
    double ticketPrice;

public:
    FlightTicket(string tid, string pname, string jdate, string sid, double price)
            : ticketId(tid), passengerName(pname), journeyDate(jdate), seatId(sid), ticketPrice(price), isBooked(false) {}

    string getTicketId() const { return ticketId; }
    string getSeatId() const { return seatId; }
    double getTicketPrice() const { return ticketPrice; }
    bool getIsBooked() const { return isBooked; }

    void bookTicket() {
        if (!isBooked) {
            isBooked = true;
            cout << "Ticket booked successfully." << endl;
        } else {
            cout << "Ticket is already booked." << endl;
        }
    }

    void cancelTicket() {
        if (isBooked) {
            isBooked = false;
            cout << "Ticket cancelled successfully." << endl;
        } else {
            cout << "Ticket is not currently booked." << endl;
        }
    }
};

class Flight {
    string Code;
    Aircraft aircraft;
    string origin;
    string destination;
    string departureTime;
    string arrivalTime;
    vector<FlightTicket> tickets;

public:
    Flight(string c, Aircraft a, string o, string d, string deptTime, string arrTime)
            : code(c), aircraft(a), origin(o), destination(d), departureTime(deptTime), arrivalTime(arrTime) {}

    string getCode() const { return code; }
    string getFlightDetails() const {
        stringstream details;
        details << "Flight Code: " << code
                << "\nAircraft Model: " << aircraft.getModel()
                << "\nCapacity: " << aircraft.getCapacity()
                << "\nOrigin: " << origin
                << "\nDestination: " << destination
                << "\nDeparture: " << departureTime
                << "\nArrival: " << arrivalTime;
        return details.str();
    }
    bool checkSeatAvailability(const string& seatId) const {
        for (const auto& ticket : tickets) {
            if (ticket.getSeatId() == seatId && ticket.getIsBooked()) {
                return false;
            }
        }
        return true;
    }

    void addTicket(const FlightTicket& ticket) {
        tickets.push_back(ticket);
    }
};

class Passenger {
private:
    string name;
    string passportNumber;
    string frequentFlyerNumber;

public:
    Passenger(string n, string pNum, string ffNum): name(n), passportNumber(pNum), frequentFlyerNumber(ffNum) {}

    string getName() const { return name; }
    string getPassportNumber() const { return passportNumber; }
    string getFrequentFlyerNumber() const { return frequentFlyerNumber; }
};

class BookingManager {
private:
    map<string, Flight> flights; // Maps flight code to Flight object
    map<string, Passenger> passengers; // Maps passport number to Passenger object
    map<string, FlightTicket> bookedTickets; // Maps ticket ID to FlightTicket for easy access and management

public:
    BookingManager() {}

    void addFlight(const string& code, const Flight& flight) {
        flights[code] = flight;
    }

    void bookTicket(const string& flightNo, const string& seat, const string& passengerName) {
        // Check if the flight exists
        if (flights.find(flightNo) != flights.end()) {
            // Check if the seat is available
            if (flights[flightNo].checkSeatAvailability(seat)) {
                // Create a ticket ID (for simplicity, concatenating flightNo and seat)
                string ticketId = flightNo + "_" + seat;
                // Create a ticket
                FlightTicket ticket(ticketId, passengerName, "", seat, 0); // Assuming journeyDate and price are not required for this example
                ticket.bookTicket();
                flights[flightNo].addTicket(ticket);
                bookedTickets[ticketId] = ticket; // Save the booked ticket
                cout << "Ticket booked successfully with ID: " << ticketId << endl;
            } else {
                cout << "The seat is not available." << endl;
            }
        } else {
            cout << "Flight not found." << endl;
        }
    }

    void cancelTicket(const string& ticketId) {
        // Check if the ticket exists
        if (bookedTickets.find(ticketId) != bookedTickets.end()) {
            bookedTickets[ticketId].cancelTicket();
            // Remove the ticket from the booked tickets map
            bookedTickets.erase(ticketId);
            cout << "Ticket cancelled successfully." << endl;
        } else {
            cout << "Ticket not found." << endl;
        }
    }

    void checkSeatAvailability(const string& flightNo) {
        if (flights.find(flightNo) != flights.end()) {
            cout << "Checking seat availability for flight " << flightNo << ":" << endl;
            // Logic to display available seats
            // For simplicity, let's assume we have a method in Flight that can list available seats
            // flights[flightNo].listAvailableSeats();
        } else {
            cout << "Flight not found." << endl;
        }
    }

    void viewBookingByTicketId(const string& ticketId) {
        if (bookedTickets.find(ticketId) != bookedTickets.end()) {
            cout << "Ticket ID: " << ticketId << endl;
            cout << "Passenger Name: " << bookedTickets[ticketId].getPassengerName() << endl;
            cout << "Seat: " << bookedTickets[ticketId].getSeatId() << endl;
            // And other details as required
        } else {
            cout << "Ticket not found." << endl;
        }
    }

    void viewBookingsByUsername(const string& username) {
        cout << "Bookings for " << username << ":" << endl;
        for (const auto& pair : bookedTickets) {
            if (pair.second.getPassengerName() == username) {
                cout << "Ticket ID: " << pair.first << ", Seat: " << pair.second.getSeatId() << endl;
                // And other details as required
            }
        }
    }
};

class FileReader {
public:
    vector<Flight> readFlightsFromFile(const string& filename) {
        vector<Flight> flights;
        ifstream file(filename);

        if (!file) {
            cerr << "Unable to open file: " << filename << endl;
            return flights;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string code, origin, destination, departureTime, arrivalTime, model;
            int capacity;
            if (ss >> code >> origin >> destination >> departureTime >> arrivalTime >> model >> capacity) {
                Aircraft aircraft(model, capacity);
                flights.push_back(Flight(code, aircraft, origin, destination, departureTime, arrivalTime));
            }
        }

        file.close();
        return flights;
    }
};

int main() {
    FileReader fileReader;
    vector<Flight> flights = fileReader.readFlightsFromFile("flights.txt");

    BookingManager bookingManager;

    // Populate the BookingManager with flights
    for (const auto& flight : flights) {
        bookingManager.addFlight(flight.getCode(), flight);
    }

    // User command processing loop
    string command;
    while (true) {
        cout << "Enter command (type 'exit' to quit): ";
        getline(cin, command);

        stringstream ss(command);
        string cmd;
        ss >> cmd;

        if (cmd == "exit") {
            cout << "Exiting the booking system." << endl;
            break;
        } else if (cmd == "check") {
            string date, flightNo;
            ss >> date >> flightNo;
            bookingManager.checkSeatAvailability(flightNo);
        } else if (cmd == "book") {
            string date, flightNo, seat, username;
            ss >> date >> flightNo >> seat >> username;
            bookingManager.bookTicket(flightNo, seat, username);
        } else if (cmd == "return") {
            string ticketId;
            ss >> ticketId;
            bookingManager.cancelTicket(ticketId);
        } else if (cmd == "view") {
            string identifier;
            ss >> identifier;
            if (isdigit(identifier[0])) {
                bookingManager.viewBookingByTicketId(identifier);
            } else {
                bookingManager.viewBookingsByUsername(identifier);
            }
        } else {
            cout << "Unknown command. Please try again." << endl;
        }
    }

    return 0;
}