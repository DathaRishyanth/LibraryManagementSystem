#include <iostream>
#include <map>
#include <vector>
#include <set>
using namespace std;

class Book
{
private:
    string isbn;
    int copiesAvailable;
    int totalCopies;

public:
    string title;
    string author;

    // constructor with default arguments
    Book(string title_ = "UnknownTitle", string author_ = "UnknownAuthor", string isbn_ = "ISBN", int copiesAvailable_ = 0, int totalCopies_ = 5)
        : isbn{isbn_}, copiesAvailable{copiesAvailable_}, totalCopies{totalCopies_}, title{title_}, author{author_} {}

    Book(Book &a, string newisbn)
        : isbn{newisbn}, copiesAvailable{a.copiesAvailable}, totalCopies{a.totalCopies}, title{a.title}, author{a.author} {}

    // Getters
    string getIsbn()
    {
        return isbn;
    }
    int getCopiesAvailable()
    {
        return copiesAvailable;
    }
    int getTotalCopies()
    {
        return totalCopies;
    }

    // Setters
    void updateCopies(int newcount)
    {
        if (copiesAvailable + newcount < 0 || totalCopies + newcount < 0)
        {
            cout << "Invalid request! Count becomes negative" << endl;
            return;
        }
        copiesAvailable = copiesAvailable + newcount;
        totalCopies = totalCopies + newcount;
    }

    void setIsbn(string newIsbn)
    {
        isbn = newIsbn;
    }

    // Utility Methods
    bool borrowBook()
    {
        if (copiesAvailable == 0)
        {
            cout << "Invalid request! Copy of book not available" << endl;
            return false;
        }
        copiesAvailable--;
        return true;
    }

    bool returnBook()
    {
        if (copiesAvailable == totalCopies)
        {
            cout << "Invalid request! Copy of book exceeds total copies" << endl;
            return false;
        }
        copiesAvailable++;
        return true;
    }

    // Method for printing details of the book
    void printDetails()
    {
        cout << title << " " << author << endl;
    }
};

class Member
{
private:
    string memberId;
    map<string, int> borrowedBooks;
    int borrowLimit;

public:
    string name;

    // Constructor with default arguments
    Member(string memberId_, string name_, int borrowLimit_ = 3)
        : memberId{memberId_}, borrowLimit{borrowLimit_}, name{name_} {}

    // getters
    string getMemberId()
    {
        return memberId;
    }

    int getNumOfCopies(string isbn)
    {
        return borrowedBooks[isbn];
    }

    int getBorrowLimit()
    {
        return borrowLimit;
    }

    int getNumOfBooks()
    {
        int cnt = 0;
        for (auto &book : borrowedBooks)
        {
            cnt += book.second;
        }
        return cnt;
    }

    // Utility Methods
    bool borrowBook(string isbn)
    {
        if (getNumOfBooks() == borrowLimit)
        {
            cout << "Invalid request! Borrow limit exceeded" << endl;
            return false;
        }
        borrowedBooks[isbn]++;
        return true;
    }

    bool returnBook(string isbn)
    {
        if (borrowedBooks[isbn] == 0)
        {
            cout << "Invalid request! Book not borrowed" << endl;
            return false;
        }
        borrowedBooks[isbn]--;
        return false;
    }

    // Method for printing details
    void printDetails()
    {
        for (auto &book : borrowedBooks)
        {
            string isbnOfBook = book.first;
            int numCopies = book.second;
            if (numCopies > 0)
                cout << memberId << " " << name << " " << isbnOfBook << " " << numCopies << endl;
        }
    }
};

class Library
{
private:
    vector<Book> books;
    vector<Member> members;
    set<string> existingIsbns;
    set<string> existingMemberIds;

public:
    // Utility Methods
    bool addBook(Book &book)
    {

        if (existingIsbns.count(book.getIsbn()))
        {
            cout << "Invalid request! Book with same isbn already exists" << endl;
            return false;
        }
        existingIsbns.insert(book.getIsbn());
        books.push_back(book);
        return true;
    }

    bool registerMember(Member &member)
    {
        if (existingMemberIds.count(member.getMemberId()))
        {
            cout << "Invalid request! Member with same id already exists" << endl;
            return false;
        }
        existingMemberIds.insert(member.getMemberId());
        members.push_back(member);
        return true;
    }

    bool borrowBook(string memberId, string isbn)
    {
        for (auto &book : books)
        {
            if (book.getIsbn() == isbn)
            {
                if (book.getCopiesAvailable() == 0)
                {
                    cout << "Invalid request! Copy of book not available" << endl;
                    return false;
                }
            }
        }
        for (auto &mem : members)
        {
            if (mem.getMemberId() == memberId)
            {
                if (mem.getNumOfBooks() == mem.getBorrowLimit())
                {
                    cout << "Invalid request! Borrow limit exceeded" << endl;
                    return false;
                }
            }
        }

        for (auto &mem : members)
        {
            if (mem.getMemberId() == memberId)
            {
                mem.borrowBook(isbn);
            }
        }

        for (auto &book : books)
        {
            if (book.getIsbn() == isbn)
            {
                book.borrowBook();
            }
        }

        return true;
    }

    bool returnBook(string memberId, string isbn)
    {
        for (auto &book : books)
        {
            if (book.getIsbn() == isbn)
            {
                if (book.getCopiesAvailable() == book.getTotalCopies())
                {
                    cout << "Invalid request! Copy of book exceeds total copies" << endl;
                    return false;
                }
            }
        }

        for (auto &mem : members)
        {
            if (mem.getMemberId() == memberId)
            {
                if (mem.getNumOfCopies(isbn) == 0)
                {
                    cout << "Invalid request! Book not borrowed" << endl;
                    return false;
                }
            }
        }

        for (auto &mem : members)
        {
            if (mem.getMemberId() == memberId)
            {
                mem.returnBook(isbn);
            }
        }

        for (auto &book : books)
        {
            if (book.getIsbn() == isbn)
            {
                book.returnBook();
            }
        }

        return true;
    }

    void updateCopiesCount(string isbn, int newCount)
    {
        for (auto &book : books)
        {
            if (book.getIsbn() == isbn)
            {
                book.updateCopies(newCount);
            }
        }
    }

    void addCopy(string oldIsbn, string newIsbn)
    {
        if (existingIsbns.count(newIsbn))
        {
            cout << "Invalid request! Book with same isbn already exists" << endl;
            return;
        }

        for (auto &book : books)
        {
            if (book.getIsbn() == oldIsbn)
            {
                Book newBook(book, newIsbn);
                addBook(newBook);
                return; // Exit once the copy is added
            }
        }

        cout << "Invalid request! Original ISBN not found" << endl; // If oldIsbn is not in the library
    }

    void printBook(string isbn)
    {
        for (auto &book : books)
        {
            if (book.getIsbn() == isbn)
            {
                book.printDetails();
            }
        }
    }

    void printMember(string memberId)
    {
        for (auto &member : members)
        {
            if (member.getMemberId() == memberId)
            {
                member.printDetails();
            }
        }
    }

    void printLibrary()
    {
        for (auto &book : books)
        {
            cout << book.title << " " << book.author << " " << book.getCopiesAvailable() << endl;
        }
        for (auto &member : members)
        {
            cout << member.getMemberId() << " " << member.name << endl;
        }
    }
};

int main()
{

    string s;
    Library lib;
    while (true)
    {
        cin >> s;
        if (s == "Done")
            break;
        if (s == "Book")
        {
            string temp;
            cin >> temp;
            // cout << temp << endl;
            if (temp == "None")
            {
                Book book;
                lib.addBook(book);
            }
            else if (temp == "ExistingBook")
            {
                string oldIsbn, newIsbn;
                cin >> oldIsbn >> newIsbn;
                lib.addCopy(oldIsbn, newIsbn);
            }
            else
            {
                string title = temp, author, isbn;
                int copiesAvailable, totalCopies;
                cin >> author >> isbn >> copiesAvailable >> totalCopies;

                // Create and add the book
                Book book(title, author, isbn, copiesAvailable, totalCopies);
                lib.addBook(book);
            }
        }

        else if (s == "UpdateCopiesCount")
        {
            string isbn;
            int newCount;
            cin >> isbn >> newCount;
            lib.updateCopiesCount(isbn, newCount);
        }
        else if (s == "Member")
        {
            string temp;
            cin >> temp;
            if (temp == "NoBorrowLimit")
            {
                string memberId, name;
                cin >> memberId >> name;
                Member member(memberId, name);
                lib.registerMember(member);
            }
            else
            {
                string memberId, name;
                int borrowLimit;
                memberId = temp;
                cin >> name >> borrowLimit;
                Member member(memberId, name, borrowLimit);
                lib.registerMember(member);
            }
        }
        else if (s == "Borrow")
        {
            string memberId, isbn;
            cin >> memberId >> isbn;
            lib.borrowBook(memberId, isbn);
        }
        else if (s == "Return")
        {
            string memberId, isbn;
            cin >> memberId >> isbn;
            lib.returnBook(memberId, isbn);
        }
        else if (s == "PrintBook")
        {
            string isbn;
            cin >> isbn;
            lib.printBook(isbn);
        }
        else if (s == "PrintMember")
        {
            string memberId;
            cin >> memberId;
            lib.printMember(memberId);
        }
        else if (s == "PrintLibrary")
        {
            lib.printLibrary();
        }
    }
}
