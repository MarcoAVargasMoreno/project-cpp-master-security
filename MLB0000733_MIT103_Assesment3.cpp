#include <iostream>
#include <algorithm> // for all_of()
#include <cctype> // for isdigit()
#include <string>
#include <vector>
#include <limits>
#include <fstream>
#include <stdexcept>
using namespace std;

//********************************************************************
// Declaration of the classes used by the program
//********************************************************************


// --------- Transaction Class
class Transaction{
// encapsulation method
    private:
    int id;
    string date;
    string product;
    string brand;    
    int quantity;
public:
    // constructor
    Transaction(int id_tr, string dt, string pd, string br, int q );
    
    int getId() const;
    string getDate() const;
    string getProduct() const;
    string getBrand() const;    
    int getQuantity() const;
    
    void display() const;
    
};


// --------- Inventory Class
class Inventory{
    private:
        vector<Transaction> txs;    // All transactions are stored in this vector
        int nextId =1;              // Unique ID for each transaction
        
        //private methods
        void selectionSortByDate();
        int binarySearchByDate(const string& date) const;
        //int findIndexById(int id) const;
        //int currentStock(const string& product) const;  
        int stockByBrand(const string& product, const string& brand) const;  
        
    public:
        void add();
        void dispatch();
        void report();
        void sortByDate();  // this method call the private method selectionSortByDate
        void searchMenu();    
        void reportTotals();
        void saveToFile(const string& filename);
        void loadFromFile(const string& filename);
};


//********************************************************************
// Functions Declaration 
//********************************************************************
void mainMenu(Inventory& inv, const string& filename); // Method To call the main Menu
int checkCharacter(string message); // Method that checks whether the user prompted a number or a character
string selectProduct(); // Method to choose main product 
int convertDate(const string& date); // method to convert date "DD/MM/YYYY" to YYYYMMDD
string toLower(const string& text);
string trim(const string& text);
string normalizeProduct(const string& product);

//********************************************************************
// main Function
// 
//********************************************************************

int main()
{
    string filename = "D:/workspaces_C++/workspace1/output_files/products.txt";
    Inventory inv; // the inventory is created
    mainMenu(inv, filename); // the inventory data is passed to the menu.
	
	return 0;
}


//*****************************************************************************
//  Constructor Implementation
//*****************************************************************************
Transaction::Transaction(int id_tr, string dt, string pd, string br, int q ){

    if(q==0){
        throw runtime_error("Quantity cannot be zero. \n");
    }

    // Brand Validation
    string cleanBrand = trim(br);
    if(cleanBrand.empty()){
        throw runtime_error("Brand cannot be empty. \n");
    }

    // Convert to lowercase
    cleanBrand =toLower(cleanBrand);

    // Date Validation
    string cleanDate = trim(dt);
        if(convertDate(cleanDate) == 0 ){
        throw runtime_error("Invalid date format. Use DD/MM/YYYY. from year 2000 to current year \n");
    }

    id = id_tr;
    date = cleanDate;
    product = pd;
    brand = cleanBrand;    
    quantity = q;
}

//*****************************************************************************
// Getter Methods Implementation  -- Transaction Class
//*****************************************************************************
int Transaction::getId() const{
    return id;        
}

string Transaction::getDate() const{
    return date;
}

string Transaction::getProduct() const{
    return product;
}

string Transaction::getBrand() const{
    return brand;
}

int Transaction::getQuantity() const{
    return quantity;
}

//*****************************************************************************
//  Display Method Implementation
//*****************************************************************************
void Transaction:: display() const{
    cout<<"ID  : " << id ;
    cout<<"| Date : " << date ;
    cout<<"| Product : " << product ;
    cout<<"| Brand : " << brand ;    
    cout<<"| Quantity : " << quantity << endl ;    
}

//*****************************************************************************
//  mainMenu Method Implementation
//*****************************************************************************
void mainMenu(Inventory& inv, const string& filename){
    int option = 0;
        
    do{    
    // This function displays the main menu, where the user can perform actions according to the selected option.   
    
        cout<<"==============================================\n";
        cout<<"=*** Beverage Inventory ***=. \n";
        cout<<"1 - Add Products. \n" ;
        cout<<"2 - Dispatch Products . \n";
        cout<<"3 - Generate Report. \n";
        cout<<"4 - Search for Date. \n";
        cout<<"5 - Sort by Date \n";    
        cout<<"6. Save Transactions to File\n";
        cout<<"7. Load Transactions from File\n";
        cout<<"8 - Exit Program. \n";        
        cout<<"============================================== \n";
        option = checkCharacter("Please, enter your selection \n");  //  the checkCharacter method is called
        
        switch (option){
            case 1:
                inv.add();  // Call the Add method from the Inventory class
                break;
                
            case 2:                
                inv.dispatch();  // Call the dispatch method from the Inventory class
                break;
                
            case 3:{
                cout<<"\n###################################################";
                cout<<"\n 1 - Show all transactions \n";
                cout<<"\n 2 - Show totals per product \n";
                cout<<"\n###################################################";
                
                int sub = checkCharacter("\n Please Select Your Report Option \n");
                if(sub ==1){
                    inv.report();  // Call the report method from the Inventory class
                }
                else if(sub ==2){
                    inv.reportTotals(); // Call the reportTotals method from the Inventory class
                }
                else{
                     cout << "Invalid report option.\n";
                }                                
                break;
            }           
     
            case 4:
                inv.searchMenu();   // Call the searchMenu method from the Inventory class             
                break;
                
            case 5: 
                inv.sortByDate();   // Call the sortByDate method from the Inventory class             
                break;
                
            case 6:
                inv.saveToFile(filename ); 
                break;
                
            case 7:
                inv.loadFromFile(filename);
                break;
                
            case 8:
                cout << "Have a goog Day \n";
                break;
                
            default:
                cout << "Invalid selection... Try again \n";
                break;
        }
    }while(option != 8);
}


int checkCharacter(string message){
    //......................................................................................
    /* This function checks whether the user entered a number or a character.
     * If the user enters a character, they receive an error message and are asked 
     * to provide a valid option according to the current menu. 
     */
    //......................................................................................
    string input;
    int number = 0;    
    while(true){
        cout<<message;
        cin>> input;
        // check wether all characters are digits
        bool isNumber = all_of(input.begin(), input.end(), ::isdigit );
        if(!isNumber){
            cout<<"Invalid option. Please enter a Number \n";
            continue;
        }
        // convert string datatype to numeric value
        number= stoi(input);
        break;
    }
    return number;    
}


//*****************************************************************************
//  selectProduct Method Implementation
//*****************************************************************************
string selectProduct(){
    cout<<"\n++++++++++++++++++++++++++++++++++++++\n";                          
    cout<< "\n Select Product \n";
    cout<< "------------------------------------- \n";                        
    cout<< "1 - Tequila \n";
    cout<< "2 - Mezcal \n";
    cout<< "3 - Kahlua \n";
    cout<< "4 - Pulque \n";
    cout<< "5 - Back to previous menu \n";
    cout<<"\n++++++++++++++++++++++++++++++++++++++\n";                                                  
    cout<< "Please enter your selection \n" ;
    int option = checkCharacter("Enter Product option: ");
    
    switch(option){
        case 1: return "Tequila";
        case 2: return "Mezcal";
        case 3: return "Kahlua";
        case 4: return "Pulque";
        case 5: return "return_back_menu";
        default:
            cout<<"Invalid Option, Please Try again. \n";
            return selectProduct(); // Recursive call to the same method if the user selects an invalid option
    }
};

//*****************************************************************************
//  Function toLower
//*****************************************************************************
string toLower(const string& text){
    /*  This function converts the entire text to lowercase.
        It uses a for loop to iterate through each character and convert it to lowercase.
    */
    string result = text;
    for(char &c :result){
        c=tolower(c);
    }
    return result;
};

//*****************************************************************************
//  Function trim
//*****************************************************************************
string trim(const string& text){
    /*This function removes spaces at the beginning and at the end of a string.
       It finds the first non-space character from the left (start)
       and the last non-space character from the right (end),
       then returns the substring between them.
    */
   int start =0;
   int end = static_cast<int>(text.size()) -1;

   // Move start forward while there are spaces
   while(start <= end && isspace(static_cast<unsigned char>(text[start]))){
    start++;
   }

   // Move end backward while there are spaces
   while(end >= start && isspace(static_cast<unsigned char>(text[end]))){
    end--;
   }

   // If the string is all spaces, return an empty string
   if(start > end){
    return "";
   }

   // Return substring without leading/ trailing spaces
   return text.substr(start, end - start +1);
};


//*****************************************************************************
//  Inventory::add() Method Implementation
//*****************************************************************************
void Inventory::add(){
    cout<<".................................................\n";
    cout<< "\n You Are in the Add Products section:\n ";
    cout<<".................................................\n";    
    
    string product = selectProduct();  // call the method selectProduct
    if(product == "return_back_menu"){        
        cout << "Add operation cancelled. Returning to main menu.\n";
        return;
    }
    
    string date;
    cout<< "Enter date (DD/MM/YYYY)";
    cin>> date ;    
    //Clear the input buffer before reading a full line.
    cin.ignore(numeric_limits<streamsize>::max(), '\n' ) ;
    
    string brand;
    cout<<"Enter Brand: \n";
    getline(cin, brand);
    brand = toLower(trim(brand)); // call methods toLower and trim to eliminate a lowercase the brand 
    
    int qty = checkCharacter("Enter quantity:"); // call the method checkCharacter, to validate if is a number 
    if(qty <=0){
        cout<<"Quantity must be positive. Product not added. \n";
        return;
    }

    try
    {
        // Create and store the transaction
        Transaction t(nextId, date, product,brand, qty);
        txs.push_back(t);  // the information is stored in the txs vector
        nextId++; // Automatically increments the ID
        cout<<"Product added successfully.\n";
    }
    catch(const exception& e)
    {
        cout << "Error adding product: " << e.what() << '\n';
    }   
    
};

//*****************************************************************************
//  Inventory::dispatch() Method Implementation
//*****************************************************************************
void Inventory::dispatch(){
    cout<<".................................................\n";
    cout<< "\n You Are in the Dispatch Products section:\n ";
    cout<<".................................................\n";
    
    if(txs.empty()){
        cout<< "No transactions in the system yet. Add products before dispatching.\n";
        return;
    }
    
    string product = selectProduct(); // call the method selectProduct
    if(product == "return_back_menu"){        
        cout << "Add operation cancelled. Returning to main menu.\n";
        return;
    }
    
    string date;
    cout<< "Enter date (DD/MM/YYYY)";
    cin >> date;
    
    //Clear the input buffer before reading a full line.
    cin.ignore(numeric_limits<streamsize>::max(), '\n' ) ;
    
    string brand;
    cout<<"Enter Brand: \n";
    getline(cin, brand);        
    brand = toLower(trim(brand));  // call methods toLower and trim to eliminate a lowercase the brand 

    int qty = checkCharacter("Enter quantity to dispatch "); // call the method checkCharacter, to validate if is a number 
    if(qty <= 0){
        cout<<"Quantity must be positive. Product not dispatched";
        return;
    }
    
    // Calculate available stock for product and brand
    int available = stockByBrand(product, brand);  // call the method stockByBrand, to validate stock 
    
    if(available <=0){
        cout<< "There is no stock available for: \t" << product << " and " << brand << ". \n";
        cout<< "Dispatch cancelled.\n\n\n";
        return;
    }
    
    if(qty > available){
        cout<< "Not enough stock for" << product << " and " << brand << "available. \n";
        cout << "Available: " << available << ", Requested: " << qty << "\n";
        return;
    }

    try
    {
        // Create and dispatch the transaction  -- a negative value is used to indicate an output.
        Transaction t(nextId, date,product, brand, -qty);
        txs.push_back(t);
        nextId++;
        cout << "Product dispatched successfully.\n";
    }
    catch(const exception& e)
    {
        cout << "Error dispatching product:" << e.what() << '\n';
    }
    
};

//*****************************************************************************
//  Inventory::Report() Method Implementation
//*****************************************************************************
void Inventory::report(){
    cout<<".................................................\n";
    cout<< "\n You Are in the Generate Report section: \n ";
    cout<<".................................................\n";
    
    if(txs.empty()){
        cout<<"No transactions recorded yet.\n";
        return;
    }
    
    cout<<"Total Transactions: " << txs.size() << "\n\n";
    
    for(size_t i =0; i < txs.size(); i++ ){
        cout<<"Transaction # " << (i+1) << "-> ";
        txs[i].display(); 
    }
}

//*****************************************************************************
//  Inventory::reportTotals() Method Implementation
//*****************************************************************************
void Inventory::reportTotals(){
    cout<<".................................................\n";
    cout<< "\n Totals per product (net quantity): \n ";
    cout<<".................................................\n";
    
    if(txs.empty() ){
        cout << "No transactions recorded yet.\n";
        return;
    }
    
    int totalTequila =0;
    int totalMezcal=0;
    int totaKahlua=0;
    int totalPulque=0;
    
    // Loop through all transactions
    for(size_t i=0; i<txs.size(); i++ ){
        string product = txs[i].getProduct();
        int qty = txs[i].getQuantity();
        
        if(product== "Tequila"){
            totalTequila += qty;
        }
        else if(product=="Mezcal"){
            totalMezcal += qty;
        }
        else if(product=="Kahlua"){
            totaKahlua += qty;
        }
        else if(product=="Pulque"){
            totalPulque += qty;
        }
    }
    
    cout<<"Tequila : " << totalTequila << endl;
    cout<<"Mezcal : " << totalMezcal << endl;
    cout<<"Kahlua : " << totaKahlua << endl;
    cout<<"Pulque : " << totalPulque << endl;    
};

//*****************************************************************************
//  Inventory::selectionSortByDate() Method Implementation
//*****************************************************************************
void Inventory:: selectionSortByDate(){
    if (txs.size() <2 ){
        cout << "Not enough transactions to sort.\n";
        return;
    }
    // Sort from oldest to newest
    for( size_t i=0; i< txs.size() - 1; i++ ){
        size_t minIndex = i;
        int minKey = convertDate(txs[minIndex].getDate());
        
        for(size_t j = i+1; j < txs.size(); ++j ){
            int key = convertDate(txs[j].getDate()) ;
            
            if(key < minKey){
                minKey = key;
                minIndex = j;
            }
        }
        
        if(minIndex != i){
            // Swap transactions
            Transaction temp = txs[i];
            txs[i] = txs[minIndex];
            txs[minIndex] = temp;
        }        
    }     
    cout << "Transactions sorted by date (oldest to newest).\n";
};

//*****************************************************************************
//  Inventory::sortByDate() Method Implementation
//*****************************************************************************
void Inventory::sortByDate(){
    if(txs.empty()){
        cout<<"No transactions to sort.\n";
        return;
    }    
    selectionSortByDate(); // call the selectionSortByDate private method
}

//*****************************************************************************
//  Inventory::binarySearchByDate() Method Implementation
//*****************************************************************************
int Inventory:: binarySearchByDate(const string& date) const{
    if(txs.empty()){
        return -1; //
    }
    int targetKey = (convertDate(date));
    if(targetKey ==0){
        return-1; // invalid date format according to our convertDate() function
    }
    
    int low=0;
    int high=static_cast<int>(txs.size()) -1 ;
    
    // Iterative binary search algorithm
    while(low <= high){
        int mid = (low + high) /2;
        int midkey = convertDate( txs[mid].getDate() );
        
        if(midkey == targetKey){
            return mid;  // a transaction with that date was found.
        }
        else if(midkey < targetKey){
            low = mid + 1; // search the right half of the array
        }
        else {
            high = mid - 1; // search the left half of the array
        }   
    }
    // If the loop ends, the date was not found.
    return -1;
};

//*****************************************************************************
//  Inventory::searchMenu() Method Implementation
//*****************************************************************************
void Inventory::searchMenu(){
    cout<<".................................................\n";
    cout << "\n--- Search Transaction by Date ---\n";
    cout<<".................................................\n";
    
    if (txs.empty()){
        cout<<"No transactions to search.\n"; 
        return;
    }
    
    cout << "Remember to use format DD/MM/YYYY.\n";
    
    string date;
    cout <<"Enter date to search: \n" ;
    cin>> date;    
    
    // We make sure that the transactions are sorted by date
    sortByDate();
    
    int index = binarySearchByDate(date); // call the method binarySearchByDate
    
    if(index == -1){
        cout<< "No transaction found with that date.\n" ;
    }
    else{
        cout<< "Transaction found at position " << (index + 1) << ":\n" ;
        txs[index].display();
    }        
};

//*****************************************************************************
//  Inventory::stockByBrand() Method Implementation
//*****************************************************************************
int Inventory::stockByBrand(const string& product, const string& brand) const{
    // method that obtains the current inventory by product and brand 
    int total = 0;
    
    for(const auto& t : txs){
        if(t.getProduct() == product && t.getBrand() == brand ){
            total += t.getQuantity(); 
        }
    }
    
    return total;
};

//*****************************************************************************
//  Inventory::saveToFile() Method Implementation
//*****************************************************************************
void Inventory::saveToFile(const string& filename ){
    ofstream out(filename);
    if(!out){
        cout << "Error opening file for writing.\n";
        return;
    }
    
    for(const auto& t : txs){
        out << t.getId()        << "\n"
            << t.getDate()      << "\n"
            << t.getProduct()   << "\n"
            << t.getBrand()     << "\n"
            << t.getQuantity()    << "\n";        
    }
    
    out.close();
    cout << "Transactions saved to file.\n";
};

//*****************************************************************************
//  Inventory::loadFromFile() Method Implementation
//*****************************************************************************

void Inventory::loadFromFile(const string& filename){
    ifstream in(filename);
    if(!in){
        cout<<"Error opening file for reading.\n";
        return;
    }
    
    txs.clear();  //We clean what was there before
    
    while(true){
        int id;
        string date;
        string product;
        string brand;
        int qty;
        
        // 1) We read the id; if it fails, we exit the loop
    if(!(in >> id)) break;
    in.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the line break
    
    // 2) Read the remaining fields, each on a separate line
    if(!getline(in, date))      break;
    if(!getline(in, product))   break;
    if(!getline(in, brand))     break;
    if(!(in >> qty))            break;
    in.ignore(numeric_limits<streamsize>::max(), '\n' ); // Clear the line break
    
    // 3) Rebuild the transaction and store it in the vector
    Transaction t(id, date, product, brand, qty);
    txs.push_back(t);
    }
    
    in.close();
    cout<< "Transactions loaded from file.\n";
    
}


//*****************************************************************************
//  convertDate Method Implementation
//*****************************************************************************
int convertDate(const string& date){
    // expected date input format: "DD/MM/YYYY"
    if( date.size() !=10 || date[2] != '/' || date[5] != '/' ){
        return 0;
    }    
    // Extract the day, month, and year into their corresponding variables.
    string dd = date.substr(0,2);
    string mm = date.substr(3,2);
    string yyyy = date.substr(6,4);    
    // Convert dd, mm, and yyyy to numeric values.
    int day = stoi(dd);
    int month = stoi(mm);
    int year = stoi(yyyy);
    // Day Validation
    if(day < 1 || day > 31 ){
        return 0;
    }
    // Month Validation
    if(month < 1 || month > 12){
        return 0;
    }
    // Year Validation
    if(year < 2000 || year > 2025){
        return 0;
    }
    // Format the date as YYYYMMDD
    int key = (year * 10000) + (month * 100) + day;    
    return key;    
};
