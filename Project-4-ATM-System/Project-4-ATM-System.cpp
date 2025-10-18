/****************************************************
 *              ATM Project in C++
 *        My Solution - Coded by Fekry ^_^
 ****************************************************/
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
using namespace std;

const string ClientsFileName = "Clients.txt";

void ShowATM_MainMenue();
void Login();
void ShowQuickWithdrawScreen();
void ShowNormalWithdrawScreen();
void ShowDepositScreen();

//========================================================
//                  STRUCTS & ENUMS
//========================================================

struct stClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance = 0.0;
    bool MarkForDelete = false;
};
enum enATM_MainMenueOptions
{
    eQuickWithdraw = 1, eNormalWithdraw = 2, eDeposite = 3, eCheckBalance = 4,
    eChangeMyPinCode = 5, eUpdateMyInformation = 6, eShowMyInformation = 7, eLogout = 8
};

// =====================================================
// GLOBAL (current logged Client)
// =====================================================
stClient CurrentClient;

//========================================================
//                  HELPER FUNCTIONS
//========================================================
vector<string> SplitString(string S1, string Delim)
{
    vector<string> vString;
    size_t pos = 0;
    string sWord = "";

    while ((pos = S1.find(Delim)) != string::npos)
    {
        sWord = S1.substr(0, pos);
        if (sWord != "")
        {
            vString.push_back(sWord);
        }
        S1.erase(0, pos + Delim.length());
    }
    if (S1 != "")
    {
        vString.push_back(S1);
    }
    return vString;
}

stClient ConvertLinetoRecord(string Line, string Seperator = "|")
{
    stClient Client;
    vector<string> vClientData = SplitString(Line, Seperator);

    if (vClientData.size() >= 5)
    {
        Client.AccountNumber = vClientData[0];
        Client.PinCode = vClientData[1];
        Client.Name = vClientData[2];
        Client.Phone = vClientData[3];
        try {
            Client.AccountBalance = stod(vClientData[4]);
        }
        catch (...) {
            Client.AccountBalance = 0.0;
        }
    }
    return Client;
}

string ConvertRecordToLine(stClient Client, string Seperator = "|")
{
    string stClientRecord = "";
    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}

vector<stClient> LoadCleintsDataFromFile(string FileName)
{
    vector<stClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open())
    {
        string Line;
        stClient Client;
        while (getline(MyFile, Line))
        {
            if (Line.empty()) continue;
            Client = ConvertLinetoRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}

void SaveCleintsDataToFile(string FileName, vector<stClient>& vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);
    string DataLine;
    if (MyFile.is_open())
    {
        for (stClient& C : vClients)
        {
            if (C.MarkForDelete == false)
            {
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
}

bool FindClientByAccountNumber(string AccountNumber, stClient& Client, vector<stClient>& vClients)
{
    for (stClient& C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, stClient& CurrentClient)
{
    vector<stClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    for (stClient C : vClients)
    {
        if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
        {
            CurrentClient = C;
            return true;
        }
    }
    return false;
}

bool LoadClientInfo(string AccountNumber, string PinCode)
{
    return FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClient);
}

void PrintClientCard(stClient& Client)
{
    cout << "--------------------------------------------\n";
    cout << "AccountNumber  : " << Client.AccountNumber << endl;
    cout << "PinCode        : " << Client.PinCode << endl;
    cout << "Name           : " << Client.Name << endl;
    cout << "Phone          : " << Client.Phone << endl;
    cout << "AccountBalance : " << Client.AccountBalance << endl;
    cout << "--------------------------------------------\n\n";
}

void ShowEndScreen()
{
    cout << "\n--------------------------------------\n";
    cout << "\tProgram Ends :-)\n";
    cout << "--------------------------------------\n\n";
}

//========================================================
//                  CORE ATM FUNCTIONS
//========================================================
bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector<stClient>& vClients)
{
    char Answer = 'n';
    cout << "\n\nAre you sure you want perfrom this transaction? y/n ^_^ ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        for (stClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveCleintsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully^_^ New balance is: " << C.AccountBalance << endl;
                return true;
            }
        }
        return false;
    }
    return false; 
}

stClient ChangeClientRecord(string AccountNumber)
{
    stClient Client;
    Client.AccountNumber = AccountNumber;
    Client.PinCode = CurrentClient.PinCode;

    cout << "\nEnter Name ? ";
    getline(cin >> ws, Client.Name);

    cout << "\nEnter Phone ? ";
    getline(cin >> ws, Client.Phone);

    Client.AccountBalance = CurrentClient.AccountBalance;
    return Client;
}

void UpdateClientByAccountNumber(string AccountNumber, vector<stClient>& vClients)
{
    stClient Client;
    char Answer = 'n';
    if (FindClientByAccountNumber(AccountNumber, Client, vClients))
    {
        PrintClientCard(Client);
        cout << "\n\nAre you sure you want Update your Information? Y/N? ";
        cin >> Answer;
        if (Answer == 'Y' || Answer == 'y')
        {
            for (stClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    CurrentClient = C;
                    break;
                }
            }
            SaveCleintsDataToFile(ClientsFileName, vClients);
            cout << "\n\nClient Updated Successfully.\n";
        }
    }
    else
        cout << "\nClient with Account (" << AccountNumber << ") Not Found!\n";
}

//========================================================
//                  SCREEN FUNCTIONS
//========================================================
short ReadATM_MainMenueOption()
{
    short Choice = 0;
    cout << "Choose what do you want to do? [1 to 8]? ";
    cin >> Choice;
    return Choice;
}
short ReadQuickWithdrawOption()
{
    short Choice = 0;
    while (Choice < 1 || Choice > 9)
    {
        cout << "Choose what to do from [1] to [9] ? ";
        cin >> Choice;
    }
    return Choice;
}
int ReadWithdrawAmount()
{
    int Amount = 0;
    do
    {
        cout << "\nPlease enter an amount multiple of 5's ? ";
        cin >> Amount;
    } while (Amount % 5 != 0);
    return Amount;
}
double ReadDepositAmount()
{
    double Amount = 0;
    do
    {
        cout << "\nEnter a positive Deposit Amount ? ";
        cin >> Amount;
    } while (Amount < 0);
    return Amount;
}
short GetQuickWithdrawAmount(short QuickWithdrawOption)
{
    switch (QuickWithdrawOption)
    {
    case 1:return 20;
    case 2:return 50;
    case 3:return 100;
    case 4:return 200;
    case 5:return 400;
    case 6:return 600;
    case 7:return 800;
    case 8:return 1000;
    default:return 0;
    }

}
void PerformQuickWithdrawOption(short QuickWithdrawOption)
{
    if (QuickWithdrawOption == 9)
        return;

    short WithdrawBalance = GetQuickWithdrawAmount(QuickWithdrawOption);
    if (WithdrawBalance > CurrentClient.AccountBalance)
    {
        cout << "\n\nThe Amount Exceeds your balance, make another choice.\n";
        cout << "Press any key to continue...";
        system("pause>0");
        ShowQuickWithdrawScreen();
        return;
    }

    vector<stClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    bool found = false;
    for (stClient& C : vClients)
    {
        if (C.AccountNumber == CurrentClient.AccountNumber)
        {
            C.AccountBalance -= WithdrawBalance; // deduct from file-backed record
            found = true;
            break;
        }
    }

    if (found)
    {
        SaveCleintsDataToFile(ClientsFileName, vClients);

        // update the in-memory current client too
        CurrentClient.AccountBalance -= WithdrawBalance; // Deduct the withdrawn amount from the client's current balance in memory

        cout << "\n\nWithdraw successful. New balance: " << CurrentClient.AccountBalance << endl;
    }
    else
    {
        cout << "\n\nAccount not found. Operation failed.\n";
    }
}
void ShowQuickWithdrawScreen()
{
    system("cls");
    cout << "=============================================\n";
    cout << "\t\tQuick Withdraw\n";
    cout << "=============================================\n";
    cout << "\t[1] 20 \t\t[2] 50\n";
    cout << "\t[3] 100\t\t[4] 200\n";
    cout << "\t[5] 400\t\t[6] 600\n";
    cout << "\t[7] 800\t\t[8] 1000\n";
    cout << "\t[9] Exit\n";
    cout << "=============================================\n";
    cout << "Your Balance is " << CurrentClient.AccountBalance << endl;
    PerformQuickWithdrawOption(ReadQuickWithdrawOption());
}
void PerformNormalWithdrawOption()
{
    int WithdrawBalance = ReadWithdrawAmount();
    if (WithdrawBalance > CurrentClient.AccountBalance)
    {
        cout << "\n\nThe Amount Exceeds your balance, make another choice.\n";
        cout << "Press any key to continue...";
        system("pause>0");
        ShowNormalWithdrawScreen();
        return;
    }
    vector<stClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    bool found = false;
    for (stClient& C : vClients)
    {
        if (C.AccountNumber == CurrentClient.AccountNumber)
        {
            C.AccountBalance -= WithdrawBalance;
            found = true;
            break;
        }
    }

    if (found)
    {
        SaveCleintsDataToFile(ClientsFileName, vClients);
        CurrentClient.AccountBalance -= WithdrawBalance; // Deduct the withdrawn amount from the client's current balance in memory
        cout << "\n\nWithdraw successful. New balance: " << CurrentClient.AccountBalance << endl;
    }
    else
    {
        cout << "\n\nAccount not found. Operation failed.\n";
    }
}
void ShowNormalWithdrawScreen()
{
    system("cls");
    cout << "=====================================\n";
    cout << "\tNormal Withdraw Screen";
    cout << "\n=====================================\n";
    PerformNormalWithdrawOption();
}
void PerformDepositOption()
{
    double DepositeAmount = ReadDepositAmount();
    vector<stClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    bool found = false;
    for (stClient& C : vClients)
    {
        if (C.AccountNumber == CurrentClient.AccountNumber)
        {
            C.AccountBalance += DepositeAmount;
            found = true;
            break;
        }
    }

    if (found)
    {
        SaveCleintsDataToFile(ClientsFileName, vClients);
        CurrentClient.AccountBalance += DepositeAmount;
        cout << "\n\nDeposit successful. New balance: " << CurrentClient.AccountBalance << endl;
    }
    else
    {
        cout << "\n\nAccount not found. Deposit failed.\n";
    }
}
void ShowDepositScreen()
{
    system("cls");
    cout << "=====================================\n";
    cout << "\tDeposit Screen\n";
    cout << "=====================================\n";
    PerformDepositOption();
}
void ShowCheckBalanceScreen()
{
    cout << "=====================================\n";
    cout << "\tCheck Balance Screen";
    cout << "\n=====================================\n";
    cout << "Your Balance is " << CurrentClient.AccountBalance << endl << endl;
}
void ShowMyInformationScreen()
{
    cout << "========================================\n";
    cout << "\tShow My Information Screen";
    cout << "\n========================================\n\n";
    PrintClientCard(CurrentClient);
}
string ChangePinCode()
{
    string PinCode = "";
    char Answer;
    cout << "\nEnter The New PIN Code ? ";
    cin >> PinCode; 
    cout << "\nAre you sure you want to change PIN Code ? Y/N? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
    {
        CurrentClient.PinCode = PinCode;
        cout << "\n\nPIN Code changed Successfully!\n";
        return PinCode;
    }
    else
        return CurrentClient.PinCode;
}
void PerformChangePinCode()
{
    vector<stClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    for (stClient& C : vClients)
    {
        if (C.AccountNumber == CurrentClient.AccountNumber)
        {
            C.PinCode = ChangePinCode();
            break;
        }
    }
    SaveCleintsDataToFile(ClientsFileName, vClients);
}
void ShowChangePinCodeScreen()
{
    system("cls");
    cout << "========================================\n";
    cout << "\tChange PIN Code Screen";
    cout << "\n========================================\n\n";
    PerformChangePinCode();
}
void PerformUpdateInformation()
{
    vector <stClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    UpdateClientByAccountNumber(CurrentClient.AccountNumber, vClients);
}
void ShowUpdateInformationScreen()
{
    system("cls");
    cout << "========================================\n";
    cout << "\tUpdate My Information Screen";
    cout << "\n========================================\n\n";
    PerformUpdateInformation();
}

//========================================================
//                  MAIN MENU & LOGIN
//========================================================
void GoBackToATM_MainMenue()
{
    cout << "\n\n\nPress any key to go back to ATM Main Menue... ";
    system("pause>0");
    ShowATM_MainMenue();
}
void PerformATM_MainMenueOption(enATM_MainMenueOptions ATM_MainMenueOption)
{
    switch (ATM_MainMenueOption)
    {
    case enATM_MainMenueOptions::eQuickWithdraw:
        system("cls");
        ShowQuickWithdrawScreen();
        GoBackToATM_MainMenue();
        break;
    case enATM_MainMenueOptions::eNormalWithdraw:
        system("cls");
        ShowNormalWithdrawScreen();
        GoBackToATM_MainMenue();
        break;
    case enATM_MainMenueOptions::eDeposite:
        system("cls");
        ShowDepositScreen();
        GoBackToATM_MainMenue();
        break;
    case enATM_MainMenueOptions::eCheckBalance:
        system("cls");
        ShowCheckBalanceScreen();
        GoBackToATM_MainMenue();
        break;
    case enATM_MainMenueOptions::eChangeMyPinCode:
        system("cls");
        ShowChangePinCodeScreen();
        GoBackToATM_MainMenue();
        break;
    case enATM_MainMenueOptions::eUpdateMyInformation:
        system("cls");
        ShowUpdateInformationScreen();
        GoBackToATM_MainMenue();
        break;
    case enATM_MainMenueOptions::eShowMyInformation:
        system("cls");
        ShowMyInformationScreen();
        GoBackToATM_MainMenue();
        break;
    case enATM_MainMenueOptions::eLogout:
        system("cls");
        Login();
        break;
    }
}
void ShowATM_MainMenue()
{
    system("cls");
    cout << "=============================================\n";
    cout << "\tATM Main Menue Screen\n";
    cout << "=============================================\n";
    cout << "\t[1] Quick Withdraw.\n";
    cout << "\t[2] Normal Withdraw.\n";
    cout << "\t[3] Deposite.\n";
    cout << "\t[4] Check Balance.\n";
    cout << "\t[5] Change My PIN Code.\n";
    cout << "\t[6] Update My Information.\n";
    cout << "\t[7] Show My Information.\n";
    cout << "\t[8] Logout.\n";
    cout << "=============================================\n";

    PerformATM_MainMenueOption(enATM_MainMenueOptions(ReadATM_MainMenueOption()));
}
void Login()
{
    bool LoginFailed = false;
    string AccountNumber, PinCode;

    do
    {
        system("cls");
        cout << "=============================================\n";
        cout << "\t\tLogin Screen\n";
        cout << "=============================================\n";

        if (LoginFailed)
            cout << "Invalid AccountNumber/PinCode!\n";

        cout << "Enter AccountNumber ? ";
        cin >> AccountNumber;

        cout << "Enter PinCode ? ";
        cin >> PinCode;

        LoginFailed = !LoadClientInfo(AccountNumber, PinCode);
    } while (LoginFailed);
    ShowATM_MainMenue();
}
int main()
{
    Login();
    system("pause>0");
    return 0;
}
