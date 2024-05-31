/*
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <list>
#include <algorithm>
#include <sstream>

using namespace std;

class User
{
public:
    std::string id;
    std::string name;
    std::string email;
    std::string password;
    std::string dateOfBirth;
    std::list<std::string> friends;
    std::stack<std::string> lifeEvents;

    User()
    {
        loadFromFile(); //echo
    }
    User(std::string _id, std::string _name, std::string _email, std::string _password, std::string _dob)
        : id(_id), name(_name), email(_email), password(_password), dateOfBirth(_dob)
    {
        loadFromFile();
    }

    void addFriend(const std::string& friendID)
    {
        friends.push_back(friendID);
    }

    void addLifeEvent(const std::string& event)
    {
        lifeEvents.push(event);
    }

    void displayProfile() const
    {
        std::cout << "ID: " << id << std::endl;
        std::cout << "Name: " << name << std::endl;
        std::cout << "Email: " << email << std::endl;
        std::cout << "Date of Birth: " << dateOfBirth << std::endl;
        std::cout << "Friends: ";
        for (const auto& friendID : friends)
        {
            std::cout << friendID << " ";
        }
        std::cout << std::endl;

        std::cout << "Life Events: " << std::endl;
        std::stack<std::string> tempEvents = lifeEvents;
        while (!tempEvents.empty())
        {
            std::cout << tempEvents.top() << std::endl;
            tempEvents.pop();
        }
    }
    void loadFromFile()
    {
        std::ifstream inFile("users.txt");
        if (!inFile)
        {
            std::cerr << "Error opening file for reading. Starting with an empty database.\n";
            return;
        }

    };

    class SocialNetwork
    {
        std::unordered_map<std::string, User> users;
        const std::string filename = "users.txt";

        void loadUsers()
        {
            ifstream inFile(filename);
            if (!inFile.is_open())
            {
                return;
            }

            std::string line;
            while (getline(inFile, line))
            {
                std::istringstream iss(line);
                std::string id, name, email, password, dob;
                if (!(iss >> id >> name >> email >> password >> dob))
                {
                    break;
                }

                User user(id, name, email, password, dob);

                std::string friendID;
                while (iss >> friendID)
                {
                    if (friendID == "LIFE_EVENTS") break;
                    user.addFriend(friendID);
                }

                std::string event;
                while (getline(iss, event))
                {
                    user.addLifeEvent(event);
                }

                users[id] = user;
            }
            inFile.close();
        }

        void saveUsers()
        {
            ofstream outFile(filename);
            for (const auto& pair : users)
            {
                const User& user = pair.second;
                outFile << user.id << " " << user.name << " " << user.email << " " << user.password << " " << user.dateOfBirth;

                for (const auto& friendID : user.friends)
                {
                    outFile << " " << friendID;
                }

                outFile << " LIFE_EVENTS";
                std::stack<std::string> tempEvents = user.lifeEvents;
                std::vector<std::string> events;
                while (!tempEvents.empty())
                {
                    events.push_back(tempEvents.top());
                    tempEvents.pop();
                }
                std::reverse(events.begin(), events.end());
                for (const auto& event : events)
                {
                    outFile << " " << event;
                }
                outFile << std::endl;
            }
            outFile.close();
        }

    public:
        SocialNetwork()
        {
            loadUsers();
        }

        ~SocialNetwork()
        {
            saveUsers();
        }

        void registerUser()
        {
            std::string id, name, email, password, dob;
            std::cout << "Enter ID: ";
            std::cin >> id;
            std::cout << "Enter Name: ";
            std::cin.ignore();
            std::getline(std::cin, name);
            std::cout << "Enter Email: ";
            std::cin >> email;
            std::cout << "Enter Password: ";
            std::cin >> password;
            std::cout << "Enter Date of Birth (DD-MM-YYYY): ";
            std::cin >> dob;

            User newUser(id, name, email, password, dob);
            users[id] = newUser;
            saveUsers();

            std::cout << "Registration successful!" << std::endl;
            manageProfile(id);
        }

        void loginUser()
        {
            std::string id, password;
            std::cout << "Enter ID: ";
            std::cin >> id;
            std::cout << "Enter Password: ";
            std::cin >> password;

            if (users.find(id) != users.end() && users[id].password == password)
            {
                std::cout << "Login successful!" << std::endl;
                manageProfile(id);
            }
            else
            {
                std::cout << "Invalid ID or Password." << std::endl;
            }
        }

        void manageProfile(const std::string& userId)
        {
            int choice;
            do
            {
                std::cout << "1. View Profile\n2. Find Friend\n3. View Existing Friends\n4. Add Life Event\n5. Logout\n";
                std::cout << "Enter your choice: ";
                std::cin >> choice;

                switch (choice)
                {
                case 1:
                    users[userId].displayProfile();
                    break;
                case 2:
                    findFriend(userId);
                    break;
                case 3:
                    viewExistingFriends(userId);
                    break;
                case 4:
                    addLifeEvent(userId);
                    break;
                case 5:
                    std::cout << "Logging out..." << std::endl;
                    break;
                default:
                    std::cout << "Invalid choice." << std::endl;
                }
            }
            while (choice != 5);
        }

        void findFriend(const std::string& userId)
        {
            // BFS implementation to find 2nd and 3rd degree connections
            std::string friendId;
            std::cout << "Enter the ID of the person you want to add: ";
            std::cin >> friendId;

            if (users.find(friendId) != users.end())
            {
                std::cout << "Friend found! Their information: " << std::endl;
                users[friendId].displayProfile();

                char addChoice;
                std::cout << "Do you want to add this person as a friend? (y/n): ";
                std::cin >> addChoice;
                if (addChoice == 'y')
                {
                    users[userId].addFriend(friendId);
                    std::cout << "Friend added successfully!" << std::endl;
                }
            }
            else
            {
                std::cout << "Friend not found." << std::endl;
            }
        }

        void viewExistingFriends(const std::string& userId)
        {
            std::priority_queue<std::string, std::vector<std::string>, std::greater<std::string>> pq;

            for (const auto& friendID : users[userId].friends)
            {
                pq.push(friendID);
            }

            std::cout << "Existing Friends: " << std::endl;
            while (!pq.empty())
            {
                std::cout << pq.top() << std::endl;
                pq.pop();
            }
        }

        void addLifeEvent(const std::string& userId)
        {
            std::string event;
            std::cout << "Enter life event: ";
            std::cin.ignore();
            std::getline(std::cin, event);

            users[userId].addLifeEvent(event);
            std::cout << "Life event added!" << std::endl;
        }
    };

    int main()
    {
        SocialNetwork sn;
        int choice;

        do
        {
            std::cout << "1. Register\n2. Login\n3. Exit\n";
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            switch (choice)
            {
            case 1:
                sn.registerUser();
                break;
            case 2:
                sn.loginUser();
                break;
            case 3:
                std::cout << "Exiting..." << std::endl;
                break;
            default:
                std::cout << "Invalid choice." << std::endl;
            }
        }
        while (choice != 3);

        return 0;
    }
*/

#include <iostream>
#include <fstream>
#include <map>
#include <list>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <stack>

using namespace std;

class UserType {
public:
    string name;
    string address;
    string emailID;
    string password;
    string DOB;
    string ID;
    stack<string> myLifeEvents; //Keeping Life events in Stack

    UserType() {
        name = "Unknown";
        address = "Unknown";
        emailID = "Unknown";
        password = "Unknown";
        DOB = "Unknown";
        ID = "Unknown";
    }

    UserType(string n, string a, string e, string p, string d, string i) {
        name = n;
        address = a;
        emailID = e;
        password = p;
        DOB = d;
        ID = i;
    }

    //Overloading operators for comparing users.
    bool operator<(const UserType& other) const {
        return emailID < other.emailID;
    }

    bool operator==(const UserType& other) const {
        return emailID == other.emailID;
    }

    bool operator!=(const UserType& other) const {
        return !(*this == other);
    }

    void addLifeEvent(const string& event) {
        myLifeEvents.push(event);
    }

    void deleteLifeEvent() {
        if (!myLifeEvents.empty()) {
            myLifeEvents.pop();
        } else {
            cout << "No life events to delete." << endl;
        }
    }

    string getRecentLifeEvent() const {
        if (!myLifeEvents.empty()) {
            return myLifeEvents.top();
        } else {
            return "No life events available.";
        }
    }

    void showAllLifeEvents() const {
        if (myLifeEvents.empty()) {
            cout << "No life events available." << endl;
            return;
        }

        stack<string> tempStack = myLifeEvents;
        vector<string> events;

        while (!tempStack.empty()) {
            events.push_back(tempStack.top());
            tempStack.pop();
        }

        reverse(events.begin(), events.end());
        cout << "Life Events: " << endl;
        for (const string& event : events) {
            cout << event << endl;
        }
    }

    void saveLifeEvents(const string& fileName) const {
        ofstream outFile(fileName);
        stack<string> lifeEvents = myLifeEvents;
        vector<string> reversedEvents;

        while (!lifeEvents.empty()) {
            reversedEvents.push_back(lifeEvents.top());
            lifeEvents.pop();
        }
        reverse(reversedEvents.begin(), reversedEvents.end());
        for (const string& event : reversedEvents) {
            outFile << event << '\n';
        }
        outFile.close();
    }

    void loadLifeEvents(const string& fileName) {
        ifstream inFile(fileName);
        if (!inFile) {
            return;
        }

        string event;
        while (getline(inFile, event)) {
            myLifeEvents.push(event);
        }
        inFile.close();
    }
};

ostream& operator<<(ostream& os, const UserType& user) {
    os << user.name;
    return os;
}

template<typename T>
class Graph {
    map<T, list<T>> adjList;

public:
    void addEdge(T u, T v) {
        adjList[u].push_back(v);
        adjList[v].push_back(u);
    }

    void showFriends(T user) {
        if (adjList.find(user) == adjList.end()) {
            cout << "User not found in the graph." << endl;
            return;
        }

        cout << "Friends of " << user << " are: ";
        for (T friendUser : adjList[user]) {
            cout << friendUser << ", ";
        }
        cout << endl;
    }

    void showFriendsOfFriends(T user) {
        if (adjList.find(user) == adjList.end()) {
            cout << "User not found in the graph." << endl;
            return;
        }

        cout << "Friends of friends of " << user << " are: ";
        set<T> friendsOfFriends;
        for (T friendUser : adjList[user]) {
            for (T friendOfFriend : adjList[friendUser]) {
                if (friendOfFriend != user && adjList[user].end() == find(adjList[user].begin(), adjList[user].end(), friendOfFriend)) {
                    friendsOfFriends.insert(friendOfFriend);
                }
            }
        }

        for (T fof : friendsOfFriends) {
            cout << fof << ", ";
        }
        cout << endl;
    }

    void saveAdjList(const string& fileName) {
        ofstream outFile(fileName);
        for (const auto& pair : adjList) {
            outFile << pair.first.emailID << ":";
            for (const auto& friendUser : pair.second) {
                outFile << friendUser.emailID << ",";
            }
            outFile << endl;
        }
        outFile.close();
    }

    void loadAdjList(const string& fileName, const map<string, UserType>& users) {
        ifstream inFile(fileName);
        if (!inFile) {
            cout << "No existing friend data found. Starting fresh." << endl;
            return;
        }

        string line;
        while (getline(inFile, line)) {
            size_t pos = line.find(':');
            if (pos == string::npos) continue;

            string userEmail = line.substr(0, pos);
            string friendsStr = line.substr(pos + 1);

            if (users.find(userEmail) != users.end()) {
                UserType user = users.at(userEmail);
                size_t start = 0;
                size_t end = friendsStr.find(',');

                while (end != string::npos) {
                    string friendEmail = friendsStr.substr(start, end - start);
                    if (users.find(friendEmail) != users.end()) {
                        UserType friendUser = users.at(friendEmail);
                        adjList[user].push_back(friendUser);
                    }
                    start = end + 1;
                    end = friendsStr.find(',', start);
                }
            }
        }
        inFile.close();
    }
};

class SocialMediaNetwork {
    map<string, UserType> users;
    Graph<UserType> friendships;
    UserType* loggedInUser = nullptr;
    const string userFileName = "users.txt";
    const string friendsFileName = "friends.txt";
    const string lifeEventsFileName = "life_events_";

public:
    SocialMediaNetwork() {
        loadUsers();
        friendships.loadAdjList(friendsFileName, users);
    }

    ~SocialMediaNetwork() {
        saveUsers();
        friendships.saveAdjList(friendsFileName);
    }

    void registerUser() {
        string name, address, email, password, dob, id;
        int question;
        cin.ignore(); // Clear any previous input
        cout << "Enter ID: ";
        getline(cin, id);
        cout << "Enter name: ";
        getline(cin, name);
        cout << "Enter address: ";
        getline(cin, address);
        cout << "Enter email: ";
        getline(cin, email);
        cout << "Enter password: ";
        getline(cin, password);
        cout << "Enter DOB (YYYY-MM-DD): ";
        getline(cin, dob);

        if (users.find(email) != users.end()) {
            cout << "Email already registered!" << endl;
            return;
        }

        UserType newUser(name, address, email, password, dob, id);
        users[email] = newUser;
        saveUsers();

        cout << "Registration successful!\nGo to Profile?\n1. Yes\n2. No"<<endl;
        cin >> question;
        if(question == 1)
            Profile(id);
    }

    bool loginUser(bool& loggedIn) {
        string email, password;
        cin.ignore(); // Clear any previous input
        cout << "Enter email: ";
        getline(cin, email);
        cout << "Enter password: ";
        getline(cin, password);

        if (users.find(email) != users.end() && users[email].password == password) {
            loggedInUser = &users[email];
            cout << "Login successful! Welcome, " << loggedInUser->name << endl;
            loggedIn = true;
            loggedInUser->loadLifeEvents(lifeEventsFileName + loggedInUser->emailID + ".txt");
        } else {
            cout << "Invalid email or password!" << endl;
            loggedIn = false;
        }

        return loggedIn;
    }

    // Profile function to display user details based on ID
    void Profile(const string& id) const {
        bool userFound = false;
        for (const auto& pair : users) {
            const UserType& user = pair.second;
            if (user.ID == id) {
                userFound = true;
                cout << "User Profile:" << endl;
                cout << "Name: " << user.name << endl;
                cout << "Address: " << user.address << endl;
                cout << "Email: " << user.emailID << endl;
                cout << "DOB: " << user.DOB << endl;
                cout << "ID: " << user.ID << endl;
                cout << "Life Events: ";
                user.showAllLifeEvents();
                break;
            }
        }
        if (!userFound) {
            cout << "User not found with ID: " << id << endl;
        }
    }

    void addFriend() {
        if (!loggedInUser) {
            cout << "You need to log in first!" << endl;
            return;
        }

        string friendEmail;
        cin.ignore(); // Clear any previous input
        cout << "Enter friend's email: ";
        getline(cin, friendEmail);

        if (users.find(friendEmail) != users.end()) {
            UserType& friendUser = users[friendEmail];
            friendships.addEdge(*loggedInUser, friendUser);
            cout << "Friend added successfully!" << endl;
        } else {
            cout << "User not found!" << endl;
        }
    }

    void showMyFriends() {
        if (!loggedInUser) {
            cout << "You need to log in first!" << endl;
            return;
        }

        friendships.showFriends(*loggedInUser);
    }

    void showMyFriendsOfFriends() {
        if (!loggedInUser) {
            cout << "You need to log in first!" << endl;
            return;
        }

        friendships.showFriendsOfFriends(*loggedInUser);
    }

    void addLifeEvent() {
        if (!loggedInUser) {
            cout << "You need to log in first!" << endl;
            return;
        }

        string event;
        cin.ignore(); // Clear any previous input
        cout << "Enter life event: ";
        getline(cin, event);
        loggedInUser->addLifeEvent(event);
        loggedInUser->saveLifeEvents(lifeEventsFileName + loggedInUser->emailID + ".txt");
        cout << "Life event added successfully!" << endl;
    }

    void deleteLifeEvent() {
        if (!loggedInUser) {
            cout << "You need to log in first!" << endl;
            return;
        }

        loggedInUser->deleteLifeEvent();
        loggedInUser->saveLifeEvents(lifeEventsFileName + loggedInUser->emailID + ".txt");
    }

    void showMyLifeEvents() const {
        if (!loggedInUser) {
            cout << "You need to log in first!" << endl;
            return;
        }

        loggedInUser->showAllLifeEvents();
    }

    void saveUsers() const {
        ofstream outFile(userFileName);
        for (const auto& pair : users) {
            const UserType& user = pair.second;
            outFile << user.name << '\n' << user.address << '\n' << user.emailID << '\n' << user.password << '\n' << user.DOB << '\n';
        }
        outFile.close();
    }

    void loadUsers() {
        ifstream inFile(userFileName);
        if (!inFile) {
            return;
        }

        string name, address, email, password, dob, id;
        while (getline(inFile, name)) {
            getline(inFile, address);
            getline(inFile, email);
            getline(inFile, password);
            getline(inFile, dob);
            getline(inFile, id);
            users[email] = UserType(name, address, email, password, dob, id);
        }
        inFile.close();
    }
};

int main() {
    SocialMediaNetwork smn;
    bool isLoggedIn = false;

    while (true) {
        int choice;
        if (!isLoggedIn) {
            cout << "1. Register\n2. Login\n3. Exit\nChoose an option: ";
        } else {
            cout << "1. Add Friend\n2. Show Friends\n3. Show Friends of Friends\n4. Add Life Event\n5. Delete Life Event\n6. Show Life Events\n7. Logout\nChoose an option: ";
        }
        cin >> choice;

        if (!isLoggedIn) {
            switch (choice) {
            case 1:
                smn.registerUser();
                break;
            case 2:
                smn.loginUser(isLoggedIn);
                break;
            case 3:
                return 0;
            default:
                cout << "Invalid option! Please try again." << endl;
            }
        } else {
            switch (choice) {
            case 1:
                smn.addFriend();
                break;
            case 2:
                smn.showMyFriends();
                break;
            case 3:
                smn.showMyFriendsOfFriends();
                break;
            case 4:
                smn.addLifeEvent();
                break;
            case 5:
                smn.deleteLifeEvent();
                break;
            case 6:
                smn.showMyLifeEvents();
                break;
            case 7:
                isLoggedIn = false;
                break;
            default:
                cout << "Invalid option! Please try again." << endl;
            }
        }
    }

    return 0;
}
