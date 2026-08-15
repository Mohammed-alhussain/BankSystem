#pragma warning(disable : 4996)
#include <iostream> 
#include <string> 
#include <iomanip> 
#include <vector> 
#include <fstream>
//#include <cstdlib> 

using namespace std;


const string ClientsFileName = "Client.txt";
const string UsersFileName = "User.txt";

struct stClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkToDelete = false;
};
struct stUser
{
    string Name;
    string Password;
    short Permissions;
    bool MarkToDelete = false;
};

enum enPermission
{
    pShowClientList = 1, pAddNowClient = 2,
    pDeleteClient = 4, pUpdateClient = 8,
    pFindClient = 16, pTransactions = 32,
    pManageUsers = 64, pAll = -1
};

stUser CurrentUser;
string MassigeDenied = "-----------------------------------\nAccess Denied,\nYou don't have Permission to do this,\nplease, contact your Admin.\n-----------------------------------\n";

void ShowMainMenu();
bool FindClientByAcountNumber(string AccountNumber, vector<stClient> vClients, stClient& Client);
vector<stClient> LoadClientDataRecoredFromFileToVector(string FileName);
void HeaderOfScreen(string Title);
stClient ConvertClientLineToRecored(string RecoredLine);
void ShowTransactionsMenuScreen();
void ShowManageUsersMenuScreen();
void SaveClientsDataToFile(string FileName, vector<stClient> vClients);
short GivePermissions();
stUser ConvertUserLineToRecored(string RecoredLine);
vector<stUser> LoadUserDataRecoredFromFileToVector(string FileName);



bool AccessTothisCase(enPermission Permission)
{
    if (CurrentUser.Permissions == enPermission::pAll)
        return true;
    
    if ((Permission & CurrentUser.Permissions) == Permission)
        return true;

    return false;
}



string JoinString(vector <string> vString, string Dlem)
{
    string Sentence = "";

    for (string i : vString)
    {
        Sentence += i + Dlem;
    }

    return Sentence.substr(0, Sentence.length() - Dlem.length());
}


//enum IsClientOrUser { ItIsClient = 1, ItIsUser = 2 };

bool IsClientExistByAccountNumber(string AccountNumber)
{
    fstream File;
    File.open(ClientsFileName, ios::in);//read Mode

    if (File.is_open())
    {
        string Line;
        stClient Client;

        while (getline(File, Line))
        {
            Client = ConvertClientLineToRecored(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                File.close();
                return true;
            }
        }
        File.close();
    }
    return false;
}
bool IsUserExistByUserName(string UserName)
{
    fstream File;
    File.open(UsersFileName, ios::in);//read Mode

    if (File.is_open())
    {
        string Line;
        stUser User;

        while (getline(File, Line))
        {
            User = ConvertUserLineToRecored(Line);
            if (User.Name == UserName)
            {
                File.close();
                return true;
            }
        }
        File.close();
    }
    return false;
}

stClient ReadClient()
{
    stClient Client;
    //stClient CAA;
    vector<stClient> vClients = LoadClientDataRecoredFromFileToVector(ClientsFileName);

    cout << "\nenter an Account Number: ";
    getline(cin >> ws, Client.AccountNumber);

    while (IsClientExistByAccountNumber(Client.AccountNumber))
    {
        cout << "Client With [" << Client.AccountNumber << "] already exist, Enter another Account Number: \n";
        getline(cin >> ws, Client.AccountNumber);
    }

    cout << "\nenter a Pin Code: ";
    getline(cin, Client.PinCode);
    cout << "\nenter a Name: ";
    getline(cin, Client.Name);
    cout << "\nenter a Phone Number: ";
    getline(cin, Client.Phone);
    cout << "\nenter an Account Balance: ";
    cin >> Client.AccountBalance;

    return Client;
}
stUser ReadUser()
{
    stUser User;
    //stClient CAA;
    vector<stUser> vUsers = LoadUserDataRecoredFromFileToVector(UsersFileName);

    cout << "\nenter an User Name: ";
    getline(cin >> ws, User.Name);

    while (IsUserExistByUserName(User.Name))
    {
        cout << "Client With [" << User.Name << "] already exist, Enter another Account Number: \n";
        getline(cin >> ws, User.Name);
    }

    cout << "\nenter a Password: ";
    getline(cin, User.Password);
    cout << "\nenter a permission: ";
     User.Permissions = GivePermissions();


    return User;
}


string ConvertClientRecoredToLine(stClient Client, string Dlem)
{
    string Info = "";
    Info += Client.AccountNumber + Dlem;
    Info += Client.PinCode + Dlem;
    Info += Client.Name + Dlem;
    Info += Client.Phone + Dlem;
    Info += to_string(Client.AccountBalance);

        return Info;
}
string ConvertUserRecoredToLine(stUser User, string Dlem)
{
    string Info = "";
    Info += User.Name + Dlem;
    Info += User.Password + Dlem;
    Info += to_string(User.Permissions);

    return Info;
}

vector<string> SplitString(string S1, string Dlem)
{
    short pos = 0;
    string sWord = "";
    vector<string> vWords;

    while ((pos = S1.find(Dlem)) != string::npos)
    {
        sWord = S1.substr(0, pos);

        if (sWord != "")
            vWords.push_back(sWord);

        S1.erase(0, pos + Dlem.length());

    }

    if (S1 != "")
        vWords.push_back(S1);

    return vWords;
}


void GoToMainMinue()
{
    cout << "\n\nPress any key to go to Main Menu...";
    system("pause>0");
    ShowMainMenu();
}
void GoToTransactionsMenu()
{
    cout << "\n\nPress any key to go to Transactions Menu...";
    system("pause>0");
    ShowTransactionsMenuScreen();
}
void GoToManageUsersMenu()
{
    cout << "\n\nPress any key to go to Manage User Menu...";
    system("pause>0");
    ShowManageUsersMenuScreen();
}



stClient ConvertClientLineToRecored(string RecoredLine)
{
    stClient Client;
    vector<string> vContentOfRecoredLine;

    vContentOfRecoredLine = SplitString(RecoredLine, "#//#");

    Client.AccountNumber = vContentOfRecoredLine[0];
    Client.PinCode = vContentOfRecoredLine[1];
    Client.Name = vContentOfRecoredLine[2];
    Client.Phone = vContentOfRecoredLine[3];
    Client.AccountBalance = stod(vContentOfRecoredLine[4]);

    return Client;

}
stUser ConvertUserLineToRecored(string RecoredLine)
{
    stUser User;
    vector<string> vContentOfRecoredLine;

    vContentOfRecoredLine = SplitString(RecoredLine, "#//#");

    User.Name = vContentOfRecoredLine[0];
    User.Password = vContentOfRecoredLine[1];
    User.Permissions = stoi(vContentOfRecoredLine[2]);

    return User;

}

void AddDataLineToFile(string FileName, string RecoredLine)
{
    fstream File;
    File.open(FileName, ios::out | ios::app);

    if (File.is_open())
    {
        File << RecoredLine << endl;
    }
    File.close();
}

void AddNewOneClient(string FileName)
{
    stClient Client = ReadClient();
    string RecoredLine = ConvertClientRecoredToLine(Client, "#//#");
    AddDataLineToFile(FileName, RecoredLine);
}
void AddNewOneUser(string FileName)
{
    stUser User = ReadUser();
    string RecoredLine = ConvertUserRecoredToLine(User, "#//#");
    AddDataLineToFile(FileName, RecoredLine);
}

void AddNewClients(string FileName) 
{
    //fstream fClient;
    stClient Client;
    //fClient.open(FileName, ios::out | ios::app);
    if (!AccessTothisCase(enPermission::pAddNowClient))
    {
        cout << MassigeDenied;
        return;
    }
    char Chice = 'y';
    do
    {
        // system("cls");
         //HeaderOfScreen("Add New Clients Screen");
        cout << "Adding new client:\n";
        AddNewOneClient(FileName);


        cout << "\nClient add Successfully, do you want to add more clients?(Y/N) ";
        cin >> Chice;
    } while (toupper(Chice) == 'Y');
    //fClient.close();
}
// add feature Permission User Function
void AddNewUsers(string FileName)
{
    //fstream fClient;
    stUser User;
    //fClient.open(FileName, ios::out | ios::app);

    char Chice = 'y';
    do
    {
        // system("cls");
         //HeaderOfScreen("Add New Clients Screen");
        cout << "Adding new User:\n";
        AddNewOneUser(FileName);


        cout << "\nUser add Successfully, do you want to add more clients?(Y/N) ";
        cin >> Chice;
    } while (toupper(Chice) == 'Y');
    //fClient.close();
}


void PrintClientCard(stClient Client)
{
    cout << "the following are the Client details:\n\n";
    cout << "___________________________________";
    cout << "\nAccount Number: " << Client.AccountNumber;
    cout << "\nPin Code:       " << Client.PinCode;
    cout << "\nName:           " << Client.Name;
    cout << "\nPhone:          " << Client.Phone;
    cout << "\nBalance:        " << Client.AccountBalance << endl;
    cout << "___________________________________";
}
void PrintUserCard(stUser User)
{
    cout << "the following are the Client details:\n\n";
    cout << "___________________________________";
    cout << "\nUser Number: " << User.Name << endl;
    cout << "\nPassword: " << User.Password << endl;
    cout << "\nPermissions: " << User.Permissions << endl;
    cout << "___________________________________";
}

void PrintOneDataClientInList(stClient Client)
{
    cout << "| " << left << setw(17) << Client.AccountNumber;
    cout << "| " << left << setw(11) << Client.PinCode;
    cout << "| " << left << setw(27) << Client.Name;
    cout << "| " << left << setw(11) << Client.Phone;
    cout << "| " << left << setw(11) << Client.AccountBalance << endl;
}
void PrintOneDataUserInList(stUser User)
{
    cout << "| " << left << setw(20) << User.Name;
    cout << "| " << left << setw(11) << User.Password;
    cout << "| " << left << setw(5) << User.Permissions << endl;

}

void PrintAllDataFromVectorOfClients(vector<stClient> vClients)
{
    cout << "                              Client List (" << vClients.size() << ") Client(s)\n";
    cout << "______________________________________________________________________________________\n";
    cout << "| " << left << setw(17) << "Account Number" << left << setw(13) << "| Pin Code";
    cout << left << setw(29) << "| Client Name" << left << setw(13) << "| Phone" << left << setw(13) << "| Balance " << "\n";
    cout << "______________________________________________________________________________________\n\n";
    for (stClient Client : vClients)
    {
        PrintOneDataClientInList(Client);
    }
    cout << "______________________________________________________________________________________\n\n";

}
void PrintAllDataFromVectorOfUsers(vector<stUser> vUsers)
{
    cout << "                              User List (" << vUsers.size() << ") User(s)\n";
    cout << "______________________________________________________________________________________\n";
    cout << "| " << left << setw(20) << "User Name" << left << setw(13) << "| Password";
    cout << left << setw(11) << "| Permissions" << "\n";
    cout << "______________________________________________________________________________________\n\n";
    for (stUser User : vUsers)
    {
        PrintOneDataUserInList(User);
    }
    cout << "______________________________________________________________________________________\n\n";

}

vector<stClient> LoadClientDataRecoredFromFileToVector(string FileName)
{
    vector<stClient> vClients;
    stClient Client;
    fstream File;

    File.open(FileName, ios::in);
    if (File.is_open())
    {
        string Line;
        while (getline(File, Line))
        {
            Client = ConvertClientLineToRecored(Line);
            vClients.push_back(Client);
        }
    }
    File.close();
    return vClients;
}
vector<stUser> LoadUserDataRecoredFromFileToVector(string FileName)
{
    vector<stUser> vUsers;
    stUser User;
    fstream File;

    File.open(FileName, ios::in);
    if (File.is_open())
    {
        string Line;
        while (getline(File, Line))
        {
            User = ConvertUserLineToRecored(Line);
            vUsers.push_back(User);
        }
    }
    File.close();
    return vUsers;
}

void ShowDataClients(string FileName)
{
    if (!AccessTothisCase(enPermission::pShowClientList))
    {
        cout << MassigeDenied;
        return;
    }
    vector<stClient> vFileClients = LoadClientDataRecoredFromFileToVector(FileName);
    PrintAllDataFromVectorOfClients(vFileClients);

}


bool FindUserByName(string UserName, vector<stUser> vUsers, stUser& User)
{
    //vector<stClient> vInfoFileClients = LoadRecoredDataFromFileToVector(FileName);

    for (stUser U : vUsers)
    {
        if (U.Name == UserName)
        {
            User = U;
            return true;
        }
    }
    return false;

}
bool FindClientByAcountNumber(string AccountNumber, vector<stClient> vClients, stClient& Client)
{
    //vector<stClient> vInfoFileClients = LoadRecoredDataFromFileToVector(FileName);

    for (stClient C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;

}

void FindClientByAcountNumberAndPrintTheResult(string AccountNumber, vector<stClient> vClients, stClient& Client)
{
    if (!AccessTothisCase(enPermission::pFindClient))
    {
        cout << MassigeDenied;
        return;
    }
    if (FindClientByAcountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);
    }
    else
    {
        cout << "Client with account number (" << AccountNumber << ") Not found";
    }
}
void FindUserByNameAndPrintTheResult(string Name, vector<stUser> vUsers, stUser& User)
{
    if (FindUserByName(Name, vUsers, User))
    {
        PrintUserCard(User);
    }
    else
    {
        cout << "User with Name (" << Name << ") Not found";
    }
}

//It's Work
//bool IsSameInfoClient(stClient Client1, stClient Client2)
//{
//    if (Client1.AccounteNumber != Client2.AccounteNumber)
//        return false;
//
//    if (Client1.PinCode != Client2.PinCode)
//        return false;
//
//    if (Client1.Name != Client2.Name)
//        return false;
//
//    if (Client1.Phone != Client2.Phone)
//        return false;
//
//    if (Client1.AccountBalance != Client2.AccountBalance)
//        return false;
//
//    return true;
//}

bool MarkeClientToDeleteByAccountNumber(string AccountNumber, vector<stClient>& vClients)
{
    for (stClient& C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            C.MarkToDelete = true;
            return true;
        }
    }
    return false;
}
bool MarkeUserToDeleteByName(string UserName, vector<stUser>& vUsers)
{
    for (stUser& U : vUsers)
    {
        if (U.Name == UserName)
        {
            U.MarkToDelete = true;
            return true;
        }
    }
    return false;
}
    
void SaveClientsDataToFile(string FileName, vector<stClient> vClients)
{

    fstream File;
    File.open(FileName, ios::out);

    if (File.is_open())
    {

        for (stClient C : vClients)
        {
            if (C.MarkToDelete == false)
                File << ConvertClientRecoredToLine(C, "#//#") << endl;
        }

    }

}
void SaveUsersDataToFile(string FileName, vector<stUser> vUsers)
{
    fstream File;
    File.open(FileName, ios::out);

    if (File.is_open())
    {
        for (stUser U : vUsers)
        {
            if (U.MarkToDelete == false)
                File << ConvertUserRecoredToLine(U, "#//#") << endl;
        }
    }
}

void DeletClientByAccountNumber(string AccountNumber, vector<stClient>& vClients)
{



    stClient Client;
    vClients = LoadClientDataRecoredFromFileToVector(ClientsFileName);
    if (FindClientByAcountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);
        cout << "\nAre you sure you want to delete it?(y/n) ";
        char a = 'y';
        cin >> a;
        if (toupper(a) == 'Y')
        {
            MarkeClientToDeleteByAccountNumber(AccountNumber, vClients);
            SaveClientsDataToFile(ClientsFileName, vClients);
            cout << "\nClient Deleted successfully";
        }

    }
    else
        cout << "Client with account number (" << AccountNumber << ") Not found";

        vClients = LoadClientDataRecoredFromFileToVector(ClientsFileName);
}
void DeletUserByUserName(string Username, vector<stUser>& vUsers)
{
    stUser User;
    vUsers = LoadUserDataRecoredFromFileToVector(UsersFileName);
    if (FindUserByName(Username, vUsers, User))
    {
        if("Admin" == User.Name)
        {
            cout << "You can Not delete this user";
        }
        else
        {
            PrintUserCard(User);
            cout << "\nAre you sure you want to delete it?(y/n) ";
            char a = 'y';
            cin >> a;
            if (toupper(a) == 'Y')
            {
                MarkeUserToDeleteByName(Username, vUsers);
                SaveUsersDataToFile(UsersFileName, vUsers);
                cout << "\nClient Deleted successfully";
            }
        }

    }
    else
        cout << "Client with account number (" << Username << ") Not found";

    vUsers = LoadUserDataRecoredFromFileToVector(UsersFileName);
}

//It's Work
//stClient choseWhoseChange(stClient Client)
//{
//    cout << "\nWhat do you want to change? \n[1] Account Number\n";
//    cout << "[2] Pin Code\n[3] Name\n[4] Phone\n[5] Acount Balance\n";
//    cout << "[6] All Data\n";
//
//    short Input = 0;
//    cin >> Input;
//    if (Input == 1)
//    {
//        cout << "Please, enter a Account Number to change: ";
//        getline(cin >> ws, Client.AccounteNumber);
//        return Client;
//    }
//    else if (Input == 2)
//    {
//        cout << "Please, enter a Pin Code to change: ";
//        getline(cin >> ws, Client.PinCode);
//        return Client;
//    }
//    else if (Input == 3)
//    {
//        cout << "Please, enter a Name to change: ";
//        getline(cin >> ws, Client.Name);
//        return Client;
//    }
//    else if (Input == 4)
//    {
//        cout << "Please, enter a Phone Number to change: ";
//        getline(cin >> ws, Client.Phone);
//        return Client;
//    }
//    else if (Input == 5)
//    {
//        cout << "Please, enter a Account Balance to change: ";
//        cin >> Client.AccountBalance;
//        return Client;
//    }
//    else if (Input == 6)
//    {      
//        return ReadClient();
//    }
//}

string ReadAccountNumber()
{
    string Input = "";
    cout << "enter an Account Number: ";
    cin >> Input;
    return Input;
}


stClient UpdateClinet(string AccountNumber)
{
    stClient ClinetUpdated;

    ClinetUpdated.AccountNumber = AccountNumber;

    cout << "\nenter a Pin Code: ";
    getline(cin >> ws, ClinetUpdated.PinCode);
    cout << "\nenter your Name: ";
    getline(cin, ClinetUpdated.Name);
    cout << "\nenter your Phone Number: ";
    getline(cin, ClinetUpdated.Phone);
    cout << "\nenter your Account Balance: ";
    cin >> ClinetUpdated.AccountBalance;

    return ClinetUpdated;
}
stUser UpdateUser(string Name)
{
    stUser UserUpdated;

    UserUpdated.Name = Name;

    cout << "\nenter a Password: ";
    getline(cin >> ws, UserUpdated.Password);
    cout << "\nenter your Name: ";
    cin >> UserUpdated.Permissions;



    return UserUpdated;
}

void UpdateClient(string AccountNumber, vector<stClient>& vClients)
{
    for (stClient& C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            C = UpdateClinet(AccountNumber);
            break;
        }

    }
}
void UpdateUser(string Name, vector<stUser>& vUsers)
{
    for (stUser& U : vUsers)
    {
        if (U.Name == Name)
        {
            U = UpdateUser(Name);
            break;
        }

    }
}

enum enPerformMainMenu
{
    ShowClinetList = 1, AddNewClient = 2,
    DeleteCilent = 3, UpdateClientInfo = 4,
    FindClient = 5, Transactions = 6, ManageUsers = 7, Logout = 8

};

bool UpdateClientByAccountNumber(string AccountNumber, vector<stClient>& vClients)
{
    if (!AccessTothisCase(enPermission::pUpdateClient))
    {
        cout << MassigeDenied;
        return false;
    }

    stClient Client;
    if (FindClientByAcountNumber(AccountNumber, vClients, Client))
    {
        PrintClientCard(Client);


            char Input = 'a';
        cout << "\nAre you sure do you want to Update this Client? y/n ";
        cin >> Input;
        if (Input == 'y' || Input == 'Y')
        {
            for (stClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = UpdateClinet(AccountNumber);
                    break;
                }

            }
            SaveClientsDataToFile(ClientsFileName, vClients);

            vClients = LoadClientDataRecoredFromFileToVector(ClientsFileName);
            return true;
        }
    }
    else
        cout << "Client with account number (" << AccountNumber << ") Not found";
    return false;

}
bool UpdateUserByName(string Name, vector<stUser>& vUsers)
{
    stUser User;
    if (FindUserByName(Name, vUsers, User))
    {
        PrintUserCard(User);

        char Input = 'a';
        cout << "Are you sure do you want to Update this Clinet? y/n ";
        cin >> Input;
        if (Input == 'y' || Input == 'Y')
        {
            for (stUser& U : vUsers)
            {
                if (U.Name == Name)
                {
                    U = UpdateUser(Name);
                    break;
                }

            }
            SaveUsersDataToFile(UsersFileName, vUsers);

            vUsers = LoadUserDataRecoredFromFileToVector(UsersFileName);
            return true;
        }
    }
    else
        cout << "User with Name (" << Name << ") Not found";
    return false;

}

void HeaderOfScreen(string Title)
{
    string Tabs = "\t\t";
    cout << Tabs + "=========================================\n";
    cout << Tabs + "\t" + Title << "\n";
    cout << Tabs + "=========================================\n";
    cout << "\n";
}

void ShowClinetListScreen()
{
    //HeaderOfScreen("Show Client List Screen");
    ShowDataClients(ClientsFileName);
}
void AddNewClientsScreen()
{
    HeaderOfScreen("Add New Clients Screen"); // it's dublecated in line 332., so this Code is usless.
    AddNewClients(ClientsFileName);
}
void DeletClientScreen()
{
    HeaderOfScreen("Delet Client Screen");
    if (!AccessTothisCase(enPermission::pDeleteClient))
    {
        cout << MassigeDenied;
        return;
    }
    vector<stClient> vClients = LoadClientDataRecoredFromFileToVector(ClientsFileName);
    DeletClientByAccountNumber(ReadAccountNumber(), vClients);
}
void UpdateClientScreen()
{
    HeaderOfScreen("Update Client Screen");
    if (!AccessTothisCase(enPermission::pUpdateClient))
    {
        cout << MassigeDenied;
        return;
    }
    vector<stClient> vClients = LoadClientDataRecoredFromFileToVector(ClientsFileName);
    UpdateClientByAccountNumber(ReadAccountNumber(), vClients);
}
void FindClientScreen()
{
    stClient Clinet;
    HeaderOfScreen("Find Client Screen");
    if (!AccessTothisCase(enPermission::pFindClient))
    {
        cout << MassigeDenied;
        return;
    }
    vector<stClient> vClients = LoadClientDataRecoredFromFileToVector(ClientsFileName);
    FindClientByAcountNumberAndPrintTheResult(ReadAccountNumber(), vClients, Clinet);
}
void ExitScreen()
{
    system("cls");
    cout << "------------------------\n";
    cout << "     End Program :-)\n";
    cout << "------------------------\n";
}

void ChoseMainMenu(enPerformMainMenu NumMainMenu)
{

    switch (NumMainMenu)
    {
    case enPerformMainMenu::ShowClinetList:
            system("cls");
            ShowClinetListScreen();
            GoToMainMinue();
  

    case enPerformMainMenu::AddNewClient:

            system("cls");
            AddNewClientsScreen();
            GoToMainMinue();


    case enPerformMainMenu::DeleteCilent:

            system("cls");
            DeletClientScreen();
            GoToMainMinue();
   

    case enPerformMainMenu::UpdateClientInfo:

            system("cls");
            UpdateClientScreen();
            GoToMainMinue();
  
    case enPerformMainMenu::FindClient:
        
            system("cls");
            FindClientScreen();
            GoToMainMinue();
       

    case enPerformMainMenu::Transactions:
        
            ShowTransactionsMenuScreen();
            GoToMainMinue();
    

    case enPerformMainMenu::ManageUsers:

            ShowManageUsersMenuScreen();
            GoToMainMinue();
       
            
    case enPerformMainMenu::Logout:
        system("cls");
        //ExitScreen();
        break;
    default:
        cout << "your input is Wrong";
        break;
    }
}

short ReadChoseOfMeanMenu()
{
    short chose = 0;
    cout << "Choose what you want to do? [1 - 8]? ";
    cin >> chose;
    return chose;
}

enum enPerformTransaction
{
    eDeposit = 1, eWithdraw = 2,
    eTotalBalances = 3, eMainMenu = 4
};
enum enPerformManageUsers
{
    eListUsers = 1, eAddNewUsers = 2,
    eDeleteUser = 3, eUpdateUser = 4,
    eFindUser = 5, eeMainMenu = 6
};


bool DepositBalanceToClientByAccountNumber(string AccountNumber, short Amount, vector<stClient> vClients)
{
    char Input = 'a';
    cout << "are you sure do you want to continue? y/n ";
    cin >> Input;

    if (Input == 'Y' || Input == 'y')
    {
        stClient Client;
        for (stClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveClientsDataToFile(ClientsFileName, vClients);
                cout << "Done Successfully, your balance now is: " << C.AccountBalance;
                return true;
            }
        }

    }
    return false;
}

bool DepositScreen()
{
    HeaderOfScreen("Deposit Screen");

    vector<stClient> vClients = LoadClientDataRecoredFromFileToVector(ClientsFileName);
    stClient Client;
    string AccountNumber = ReadAccountNumber();



    while (!FindClientByAcountNumber(AccountNumber, vClients, Client))
    {
        cout << "Account Number with [" << AccountNumber << "] it's Not Exist\n";
        cout << "Please, enter another Account Number: ";
        cin >> AccountNumber;
    }

    PrintClientCard(Client);


    short Amount = 0;
    cout << "\nHow much do you want to Deposit ";
    cin >> Amount;
    DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);
    return true;

}

bool WithdrawScreen()
{
    //HeaderOfScreen("Deposit Screen");

    vector<stClient> vClients = LoadClientDataRecoredFromFileToVector(ClientsFileName);
    stClient Client;
    string AccountNumber = ReadAccountNumber();



    while (!FindClientByAcountNumber(AccountNumber, vClients, Client))
    {
        cout << "Account Number with [" << AccountNumber << "] it's Not Exist\n";
        cout << "Please, enter another Account Number: ";
        cin >> AccountNumber;
    }

    PrintClientCard(Client);


    short Amount = 0;
    cout << "\nHow much do you want to Withdraw ";
    cin >> Amount;

    while (Amount > Client.AccountBalance)
    {
        cout << "Amount with [" << Amount << "] it's over what do you have,\n";
        cout << "enter another Amount";
        cin >> Amount;
    }

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vClients);
    return true;

}

void PrintOneDataClientInListTransaction(stClient Client)
{
    cout << "| " << left << setw(17) << Client.AccountNumber;
    cout << "| " << left << setw(27) << Client.Name;
    cout << "| " << left << setw(11) << Client.AccountBalance << endl;
}

void FanctionTotalBalances()
{
    stClient Client;
    vector<stClient> vClients = LoadClientDataRecoredFromFileToVector(ClientsFileName);
    double CollectBalances = 0;


    cout << "                              Client List (" << vClients.size() << ") Client(s)\n";
    cout << "______________________________________________________________________________________\n";
    cout << "| " << left << setw(17) << "Account Number";
    cout << left << setw(29) << "| Client Name" << left << setw(13) << "| Balance " << "\n";
    cout << "______________________________________________________________________________________\n\n";
    for (stClient Client : vClients)
    {
        PrintOneDataClientInListTransaction(Client);
        CollectBalances += Client.AccountBalance;
    }
    cout << "______________________________________________________________________________________\n\n";
    cout << "the total balance are: " << CollectBalances;



}
void ChoseTransaction(enPerformTransaction TypeOfTreansaction )
{
    switch(TypeOfTreansaction)
    {
    case enPerformTransaction::eDeposit:
        system("cls");
        DepositScreen();
        GoToTransactionsMenu();

    case enPerformTransaction::eWithdraw:
        system("cls");
        HeaderOfScreen("Withdraw Screen");
        WithdrawScreen();
        GoToTransactionsMenu();

    case enPerformTransaction::eTotalBalances:
        system("cls");
        HeaderOfScreen("Total Balance Screen");
        FanctionTotalBalances();
        GoToTransactionsMenu();

    case enPerformTransaction::eMainMenu:
        ShowMainMenu();

    default:
        cout << "your Input are Wrong";
        break;
    }
}

void ShowTransactionsMenuScreen()
{
    if (!AccessTothisCase(enPermission::pTransactions))
    {
        cout << MassigeDenied;
        return;
    }
    system("cls");
    string Tabs = "\t";
    cout << "========================================\n";
    cout << "\t\tTransactions Screen\n";
    cout << "========================================\n";
    cout << Tabs + "[1] Deposit.\n";
    cout << Tabs + "[2] Withdraw.\n";
    cout << Tabs + "[3] Total Balances.\n";
    cout << Tabs + "[4] Main Menu.\n";
    cout << "========================================\n";
    short a = 0;
    cout << "Choose what you want to do? [1 - 4]? ";
    cin >> a;

    ChoseTransaction((enPerformTransaction)a);
}

string ReadUserName()
{
    string a = "";
    cout << "Please, enter an User Name ";
    getline(cin >> ws, a);
    return a;
}

void ChoseManageUsersMenu(enPerformManageUsers NumManageUserMenu)
{
    switch (NumManageUserMenu)
    {
    case enPerformManageUsers::eListUsers:
        system("cls");
        {
            vector<stUser> vFileClients = LoadUserDataRecoredFromFileToVector(UsersFileName);
            PrintAllDataFromVectorOfUsers(vFileClients);
        }
        GoToManageUsersMenu();

    case enPerformManageUsers::eAddNewUsers:
        system("cls");
        HeaderOfScreen("Add New Users Screen"); // it's dublecated in line 332., so this Code is usless.
        AddNewUsers(UsersFileName);

        GoToManageUsersMenu();

    case enPerformManageUsers::eDeleteUser:
        system("cls");
        HeaderOfScreen("Delete User Screen");
        {
            vector<stUser> vUsers = LoadUserDataRecoredFromFileToVector(UsersFileName);

            DeletUserByUserName(ReadUserName(), vUsers);
        }
        GoToManageUsersMenu();

    case enPerformManageUsers::eUpdateUser:
        system("cls");
        {
            HeaderOfScreen("Update User Screen");
            vector<stUser> vUsers = LoadUserDataRecoredFromFileToVector(UsersFileName);
            UpdateUserByName(ReadUserName(), vUsers);
        }
        GoToManageUsersMenu();

    case enPerformManageUsers::eFindUser:
        system("cls");
        {
            stUser User;
            HeaderOfScreen("Find User Screen");
            vector<stUser> vUsers = LoadUserDataRecoredFromFileToVector(UsersFileName);
            string Input = "";
            cout << "enter an User Name: ";
            cin >> Input;
            FindUserByNameAndPrintTheResult(Input, vUsers, User);
        }
        GoToManageUsersMenu();

    case enPerformManageUsers::eeMainMenu:
        //system("cls");
        ShowMainMenu();
        break;

    }
}

bool isUserNameAndPasswordSame(string UserName, string Password, stUser& User)
{
    vector<stUser> vUsers = LoadUserDataRecoredFromFileToVector(UsersFileName);

    for (stUser U : vUsers)
    {
        if (U.Name == UserName && U.Password == Password)
        {
            User = U;
            return true;
        }
    }
    return false;
}

bool LoadUserInfo(string UserName, string Password)
{
    if (isUserNameAndPasswordSame(UserName, Password, CurrentUser))
        return true;
    else
        return false;
}

void Login()
{

    string UserName = "";
    string Password = "";
    bool LoadFale= false;
    stUser User;

    system("cls");


    do
    {

        HeaderOfScreen("Login Screen");

        cout << "\nPlease enter an User Name: ";
        cin >> UserName;
        cout << "\nPlease enter a Password: ";
        cin >> Password;



        system("cls");

        if(LoadFale== true)
        {
            cout << "Invalidate in UserName/Password";
        }

        LoadFale =!LoadUserInfo(UserName, Password);

    } while (LoadFale);

}

short GivePermissions()
{

        short SumPermission = 0;
        char Input = 'j';

        cout << "Do you want to give full access? y/n ";
        cin >> Input;

        if (Input == 'Y' || Input == 'y')
        {
            return -1;
        }

        cout << "Show Client List? y/n? ";
        cin >> Input;
        if (Input == 'Y' || Input == 'y')
            SumPermission = SumPermission + enPermission::pShowClientList;

        cout << "Add New Clients? y/n? ";
        cin >> Input;
        if (Input == 'Y' || Input == 'y')
            SumPermission = SumPermission + enPermission::pAddNowClient;

        cout << "Delete Clients? y/n? ";
        cin >> Input;
        if (Input == 'Y' || Input == 'y')
            SumPermission = SumPermission + enPermission::pDeleteClient;

        cout << "Update Client? y/n? ";
        cin >> Input;
        if (Input == 'Y' || Input == 'y')
            SumPermission = SumPermission + enPermission::pUpdateClient;

        cout << "Find Client? y/n? ";
        cin >> Input;
        if (Input == 'Y' || Input == 'y')
            SumPermission = SumPermission + enPermission::pFindClient;

        cout << "Transactions? y/n? ";
        cin >> Input;
        if (Input == 'Y' || Input == 'y')
            SumPermission = SumPermission + enPermission::pTransactions;

        cout << "Manage Users? y/n? ";
        cin >> Input;
        if (Input == 'Y' || Input == 'y')
            SumPermission = SumPermission + enPermission::pManageUsers;

    
    

        return SumPermission;
}

void ShowManageUsersMenuScreen()
{
    if (!AccessTothisCase(enPermission::pManageUsers))
    {
        cout << MassigeDenied;
        return;
    }

    system("cls");
    string Tabs = "\t";
    cout << "========================================\n";
    cout << "\t\tManage Users Menu Screen\n";
    cout << "========================================\n";
    cout << Tabs + "[1] List Users.\n";
    cout << Tabs + "[2] Add New User.\n";
    cout << Tabs + "[3] Delete User.\n";
    cout << Tabs + "[4] Update User.\n";
    cout << Tabs + "[5] Find User.\n";
    cout << Tabs + "[6] Main Menu.\n";
    cout << "========================================\n";
    short a = 0;
    cout << "Choose what you want to do? [1 - 6]? ";
    cin >> a;

    ChoseManageUsersMenu((enPerformManageUsers)a);
}

void ShowMainMenu()
{
    system("cls");
    string Tabs = "\t";
    cout << "========================================\n";
    cout << "\t\tMainMenu\n";
    cout << "========================================\n";
    cout << Tabs + "[1] Show Client List.\n";
    cout << Tabs + "[2] Add New Clients.\n";
    cout << Tabs + "[3] Delete Client.\n";
    cout << Tabs + "[4] Update Client Info.\n";
    cout << Tabs + "[5] Find Client.\n";
    cout << Tabs + "[6] Transactions.\n";
    cout << Tabs + "[7] Manage Users.\n";
    cout << Tabs + "[8] Logout.\n";
    cout << "========================================\n";




    ChoseMainMenu((enPerformMainMenu)ReadChoseOfMeanMenu());
}

int main()
{

    while(true)
    {
        Login();
        ShowMainMenu();
    }

}