/**************************************************************
 *  Developer : Yamin Yee
 *  SU_ID     : 4005577
 *  Class     : CPSC 5005-01
 *  Project   : P1 Implementation
 *  File      : p1.cpp
 *  Date      : 4/14/2023
 ************************************************************/

#include <iostream>
#include <sstream>
#include <fstream>
#include "BookList.h"
#include "MemberList.h"
#include "RatingList.h"

using namespace std;

/**
 * Initial options to pick from when you launch the application
 * @return an integer between 1 - 4
 */
static int firstScreen();

/**
 * displays opening menu
 * and controls choices
 * @param b BookList object containing BookList
 * @param m MemberList object containing MemberList
 * @param r RatingList object containing RatingList
 */
static void startingDriver(BookList &b, MemberList &m, RatingList &r);

/**
 * Driver of the program handles a user to logged in
 * displays logged in menu and controls choices
 * @param b BookList object containing book data
 * @param m MemberList object containing member data
 * @param r RatingList object containing rating data
 * @param memberId the member iD of the logged in user
 * @param the choice made from the logged in menu
 */
static void signInDriver(BookList &b, MemberList &m, RatingList &r,
                         int memberId, int choice);

/**
 * Displays the menu for a logged in user lets them make a choice
 * of options
 * @param b BookList object containing book data
 * @param m MemberList object containing member data
 * @param r RatingList object containing rating data
 * @return integer choice made from the menu
 */
static int loggedInScreen(BookList &b, MemberList &m, RatingList &r);

/**
 * Method that asks a user for their member id and logs them in
 * @param m Member object containing member data
 * @return the member id of the logged in member
 */
static int memberLogin(MemberList &m);

/**
 * Logs a user out
 * @param m Member object containing member data
 * @param memberId of the user to be logged out
 */
static void memberLogout(MemberList &m, int memberId);

/**
 * Method than asks for info of a new member and adds them to the
 * member object
 * @param m Member object containing member data
 */
static void addMember(MemberList &m);

/**
 * Method than asks for info of a new book and adds it to the
 * BookList object
 * @param b BookList object containing book data
 */
static void addBook(BookList &b);

/**
 * Method that displays all of a user's book ratings
 * @param b BookList object containing book data
 * @param m MemberList object containing member data
 * @param r RatingList object containing rating data
 * @param memberId of the user who's ratings will be displayed
 */
static void displayUserRatings(BookList &b, MemberList &m,
                               RatingList &r, int memberId);

/**
 * Method to find a user's most similar member, display their name,
 * and display the most similar member's liked and really liked books
 * @param b BookList object containing book data
 * @param m MemberList object containing member data
 * @param r RatingList object containing rating data
 * @param memberId of the user who's recommendations are to be found
 */
static void displayRecommendations(BookList &b, MemberList &m,
                                   RatingList &r, int memberId);

/**
 * Method to allow a user to rate or re-rate a book
 * @param b BookList object containing book data
 * @param m MemberList object containing member data
 * @param r RatingList object containing rating data
 * @param memberId of the user who will rate a book
 */
static void rateBook(BookList &b, MemberList &m, RatingList &r, int memberId);

/**
 * Method that displays the application's goodbye message
 */
static void exitProgram();

/**
 * Function to load all books from a file and store them in a passed-in
 * BookList object
 * @param fileName - name of the file containing book info
 * @param book - address of a BookList object
 */
void loadBooks(const string &fileName, BookList &book) ;

/**
 * This function will load data from a member-rating file.
 * File needs to be formatted as such:
*     Member name
*     List of space separated ratings (i.e. 0 3 4 3 2 1)
 * @param fileName - name of the file containing member and rating data
 * @param member - address of a Member object
 * @param rating - address of a Rating object
 */
void loadMemberRatingData(const string &fileName, MemberList &member,
                          RatingList &rating);


/**
 * Main function of the program. gets the file from a user
 * @return 0 if the program runs with no errors
 */
int main() {
    BookList b;
    MemberList m;
    RatingList r(30);

    string bookFile;
    string ratingsFile;

    // get files from the user
    cout << "Please enter the name of a book file: ";
    cin >> bookFile;
    cout << "Please enter the name of a ratings file: ";
    cin >> ratingsFile;

    // Load books and rating file
    loadBooks(bookFile, b);
    loadMemberRatingData(ratingsFile, m, r);

    // Starts the program
    startingDriver(b, m, r);

    return 0;
}


void loadBooks(const string &fileName, BookList &book) {
    auto *currentBook = new string[4];
    int isbn = 1;

    string filepath = fileName;
    string bookLine;
    ifstream inFile;

    // Attempt to open file
    inFile.open(filepath);

    if (inFile.is_open()) {
        // Proceed while lines are being read from file
        while (getline(inFile, bookLine)) {
            istringstream ss(bookLine);

            // Add the isbn as the position in the list
            // The isbn being index + 1 is an implementation decision
            currentBook[0] = to_string(isbn++);

            // Add the author, title, and year to currentBook
            // in positions 1, 2, 3.
            int i = 0;
            string bookInfo;
            while (getline(ss, bookInfo, ',')) {
                currentBook[++i] = bookInfo;
            }

            // Add the book to the list the Book's list
            book.addBook(currentBook[0], currentBook[1],
                         currentBook[2], currentBook[3]);
        }
    } else {
        // File name was invalid, exit application
        cout << "ERROR: cannot open file";
        return;
    }

    // Perform cleanup
    inFile.close();
    delete[] currentBook;
}

void loadMemberRatingData(const string &fileName, MemberList &member,
                          RatingList &rating) {

    // numMembers will keep track of memberId's
    int numMembers = 0;
    int numBooks;
    string filepath = fileName;

    string name;
    string memberRatings;
    ifstream inFile;

    // Attempt to open file
    inFile.open(filepath);

    if (inFile.is_open()) {
        // Proceed while lines are being read from file
        while (getline(inFile, name)) {
            member.addMember(name);

            // After a name is read, next line is the member's book ratings
            if (getline(inFile, memberRatings)) {
                istringstream ss(memberRatings);
                numBooks = -1;

                // While there is data in the input stream
                while (ss) {
                    int currRating;
                    ss >> currRating;
                    // Assign the rating to the associated member's
                    // rating array
                    rating.addRating(numMembers, ++numBooks, currRating);
                }
            }
            numMembers++;
        }
    } else {
        // Filepath was invalid, exit application
        cout << "ERROR: cannot open file";
        return;
    }

    cout << "# of books: " << numBooks << std::endl;
    cout << "# of members: " << numMembers << std::endl;
    cout << endl;

    // Perform cleanup
    inFile.close();
}

int firstScreen() {
    cout << "************** MENU **************" << std::endl;
    cout << "* 1. Add a new member            *" << std::endl;
    cout << "* 2. Add a new book              *" << std::endl;
    cout << "* 3. Login                       *" << std::endl;
    cout << "* 4. Quit                        *" << std::endl;
    cout << "**********************************" << std::endl;
    cout << std::endl;

    // Get user choice
    int response;
    cout << "Enter a menu option: ";
    cin >> response;

    // Do not allow invalid choices
    while (response < 1 || response > 4) {
        cout << "That is not a valid selection. ";
        cout << "Please enter a menu option: ";
        cin >> response;
    }

    cout << endl;
    return response;
}


void startingDriver(BookList &b, MemberList &m, RatingList &r) {
    int memberId;
    int choice;
    switch (firstScreen()) {
        case 1:
            addMember(m);
            startingDriver(b, m, r);
            break;
        case 2:
            addBook(b);
            startingDriver(b, m, r);
            break;
        case 3:
            memberId = memberLogin(m);
            choice = loggedInScreen(b, m, r);
            signInDriver(b, m, r, memberId, choice);
            break;
        case 4:
            exitProgram();
            return;
    }
}

void signInDriver(BookList &b, MemberList &m, RatingList &r, int memberId, int choice) {
    switch (choice) {
        case 1:
            addMember(m);
            choice = loggedInScreen(b, m, r);
            signInDriver(b, m, r, memberId, choice);
            break;
        case 2:
            addBook(b);
            choice = loggedInScreen(b, m, r);
            signInDriver(b, m, r, memberId, choice);
            break;
        case 3:
            rateBook(b, m, r, memberId);
            choice = loggedInScreen(b, m, r);
            signInDriver(b, m, r, memberId, choice);
            break;
        case 4:
            displayUserRatings(b, m, r, memberId);
            choice = loggedInScreen(b, m, r);
            signInDriver(b, m, r, memberId, choice);
            break;
        case 5:
            displayRecommendations(b, m, r, memberId);
            choice = loggedInScreen(b, m, r);
            signInDriver(b, m, r, memberId, choice);
            break;
        default:
            memberLogout(m, memberId);
            startingDriver(b, m, r);
    }
}

int loggedInScreen(BookList &b, MemberList &m, RatingList &r) {
    cout << "************** MENU **************" << std::endl;
    cout << "* 1. Add a new member            *" << std::endl;
    cout << "* 2. Add a new book              *" << std::endl;
    cout << "* 3. Rate a book                 *" << std::endl;
    cout << "* 4. View ratings                *" << std::endl;
    cout << "* 5. See recommendations         *" << std::endl;
    cout << "* 6. Logout                      *" << std::endl;
    cout << "**********************************" << std::endl;
    cout << std::endl;

    // Get user choice
    int response;
    cout << "Enter a menu option: ";
    cin >> response;

    // Do not allow invalid choices
    while (response < 1 || response > 6) {
        cout << "That is not a valid selection. ";
        cout << "Please enter a menu option: ";
        cin >> response;
        cout << std::endl;
    }

    cout << endl;
    return response;
}

int memberLogin(MemberList &m) {
    int memberId;

    // Get users member Id
    cout << "Enter member account: ";
    cin >> memberId;
    // we used a 0 index code for our list of members
    memberId -=1;
    cout << endl;

    // Do not allow invalid member id
    while (memberId >= m.size()) {
        cout << "That is not a valid member Id. ";
        cout << "Please enter the correct id: ";
        cin >> memberId;
        cout << endl;
    }

    // Log the user in and notify
    m.login(memberId);
    cout << m.findName(memberId) << ", you are logged in!" << endl;
    cout << endl;

    return memberId;
}


void memberLogout(MemberList &m, int memberId) {
    // Log user out
    m.logout(memberId);
    cout << endl;
}

void addMember(MemberList &m) {

    // Get new member name
    string newName;
    cout << "Enter the name of the new member :";
    cin >> newName;

    // Add member to Member object
    int memberId = m.addMember(newName);

    // Notify user than new member has been added
    cout << m.findName(memberId);
    cout << " (member Id: " << memberId;
    cout << ") was added." << endl;
    cout << endl;
}

void addBook(BookList &b) {
    string author;
    string title;
    string year;

    // set isbn
    string isbn = to_string(b.size() + 1);

    // Remove the hanging newline character
    cin.get();

    // Get info about new book
    cout << "Enter the author of the new book: ";
    getline(cin, author);

    cout << "Enter the title of the new book: ";
    getline(cin, title);

    cout << "Enter the year (or range of years) of the new book: ";
    getline(cin, year);
    cout << endl;

    // Add book to Book object
    b.addBook(isbn, author, title, year);

    // Newest book will always have a bookId(index) of one less than the size
    // Always the last item in the bookArrray
    cout << b.getBook(b.size() - 1);
    cout << " was added." << endl;
    cout << endl;
}

void displayUserRatings(BookList &b, MemberList &m, RatingList &r, int memberId) {
    // Displays the user ratings
    cout << m.findName(memberId) << "'s ratings..." << endl;

    for (int i = 0; i < b.size(); ++i) {
        cout << b.getBook(i);
        cout << " => rating: " << r.getRating(memberId, i);
        cout << endl;
    }
    cout << endl;
}

void displayRecommendations(BookList &b, MemberList &m, RatingList &r, int memberId) {

    // Get most similar member
    int mostSimilar = r.similar(memberId);

    // Notify user of their most similar member
    cout << "You have similar taste in books as ";
    cout << m.findName(mostSimilar) << "!" << endl;
    cout << endl;

    // Display mostSimilar's books with rating of 5
    cout << "Here are the books they really liked:" << endl;
    for (int i = 0; i < b.size(); ++i) {
        if (r.getRating(mostSimilar, i) == 5) {
            cout << b.getBook(i);
            cout << endl;
        }
    }
    cout << endl;

    // Display most Similar's books with rating of 3
    cout << "And here are the books they liked:" << endl;
    for (int i = 0; i < b.size(); ++i) {
        if (r.getRating(mostSimilar, i) == 3) {
            cout << b.getBook(i);
            cout << endl;
        }
    }
    cout << endl;
}

void rateBook(BookList &b, MemberList &m, RatingList &r, int memberId) {

    // Get isbn
    int isbn;
    cout << "Enter the ISBN for the book you'd like to rate: ";
    cin >> isbn;

    // Isbn == (index in bookArray) + 1
    while (isbn < 1 || isbn > b.size()) {
        cout << "That is an invalid isbn.  Please enter an ISBN: ";
        cin >> isbn;
    }
    std::cout << "\n";

    // Get the book id from the isbn
    int bookId = b.findISBN(to_string(isbn));

    // Check if the book has already been rated
    if (r.getRating(memberId, bookId) != 0) {
        cout << "Your current rating for ";
        cout << b.getBook(bookId);
        cout << " => rating: " << r.getRating(memberId, bookId) << endl;

        cout << "Would you like to re-rate this book (y/n)? ";
        string reRate;
        cin >> reRate;

        // Require y or n response
        while (reRate != "y" && reRate != "n") {
            cout << "Please enter (y/n): ";
            cin >> reRate;
        }

        // Exit if response is 'n'
        if (reRate == "n") {
            cout << endl;
            return;
        }
    }

    // Let user rate the book
    int rating;
    cout << "Enter your rating: ";
    cin >> rating;
    cout << endl;

    // Require valid rating
    while (
            rating != -5 &&
            rating != -3 &&
            rating != 3 &&
            rating != 5 &&
            rating != 1
            ) {
        cout << "    ____ ______________________" << endl;
        cout << "   | -5 | Hated it!            |" << endl;
        cout << "   | -3 | Didn't like it       |" << endl;
        cout << "   |  1 | neither hot nor cold |" << endl;
        cout << "   |  3 | Liked it!            |" << endl;
        cout << "   |  5 | Really liked it!     |" << endl;
        cout << "    ---------------------------" << endl;
        cout << "Please enter a valid rating: " << endl;
        cin >> rating;
        cout << endl;
    }

    // Display the user's book and rating
    r.addRating(memberId, bookId, rating);
    cout << "Your new rating for ";
    cout << b.getBook(bookId);
    cout << " => rating: " << rating << endl;

    cout << endl;
}

void exitProgram() {
    cout << "Thank you for using the Book Recommendation Program!"
              << endl;
}