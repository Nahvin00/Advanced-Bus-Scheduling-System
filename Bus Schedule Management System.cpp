#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <conio.h>
#include <windows.h>
#include <mysql.h>
#include <thread>
#include <chrono>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <algorithm>
#include <iomanip>

using namespace std;
using std::flush;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;

int qstate;
MYSQL* conn;
MYSQL_ROW row;
MYSQL_RES* res;

string date_retrieval();
void slow_print(const string&, unsigned int);
void menus();
void log_check(int, int);
bool pwdEncryption(const string&, int, int, int);
bool pwdCheck(const string&, int, int);
int searchuser();
int removeuser();
int addadmin();
int updateadmin(int);
int admin();
int forgetlog();
int lecturer(int, int);
int addData(int, int);
int driver(int, int);
int login(int);
int forgot_password(int);
int newaccount(int);
int reportGen(int, int, int);

class db_response
{
public:
    static void ConnectionFunction()
    {
        conn = mysql_init(0);
        if (conn)
        {
            cout << "Database Connected" << endl;
            cout << "Press any key to continue..." << endl;
            system("cls");
        }
        else
            cout << "Failed To Connect!" << mysql_errno(conn) << endl;
        conn = mysql_real_connect(conn, "localhost", "root", "", "busschedule", 3306, NULL, 0);
        if (conn)
        {
            cout << "Database Connected To MySql" << conn << endl;
            cout << "Press any key to continue..." << endl;
            system("cls");
        }
        else
            cout << "Failed To Connect!" << mysql_errno(conn) << endl;
    }
};

string invalid_selection = "Invalid Selection. Please try again.\n";

int main()
{
    system("cls");
    system("title Bus Schedule Management System");
    system("color 0A");
    
    db_response::ConnectionFunction();
    srand(time(0));

    menus();
 
    return 0;
}

string date_retrieval()
{
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, 80, "%F / %X", timeinfo);
    return buffer;
}

void slow_print(const string& message, unsigned int millis_per_char)
{
    for (const char c : message)
    {
        cout << c << flush;
        sleep_for(milliseconds(millis_per_char));
    }
}

void log_check(int id, int choice)
{
    string strid = to_string(id);
    if (choice == 1)
    {
        string findbynum = "select lect_id from forgotpwd_log where status = 1 and lect_id = '"+strid+"'";
        const char* qnum = findbynum.c_str();
        qstate = mysql_query(conn, qnum);
    }
    else if (choice == 2)
    {
        string findbynum = "select driver_id from forgotpwd_log where status = 1 and driver_id = '" + strid + "'";
        const char* qnum = findbynum.c_str();
        qstate = mysql_query(conn, qnum);
    }
    res = mysql_store_result(conn);
    while ((row = mysql_fetch_row(res)))
    {
        cout << "This account exist in forgot password log. Upon successful log in the account will be removed from the log.\n";
        system("pause");
        system("cls");
        if (choice == 1)
        {
            string update_query = "update forgotpwd_log set status = 0 where lect_id ='" + strid + "'";
            const char* qu = update_query.c_str();
            qstate = mysql_query(conn, qu);
        }
        else
        {
            string update_query = "update forgotpwd_log set status = 0 where driver_id ='" + strid + "'";
            const char* qu = update_query.c_str();
            qstate = mysql_query(conn, qu);
        }
    }
}

bool pwdEncryption(const string& pwd, int id, int choice, int type)
{
    int enc_code=0,max_enc_num;
    string strid = to_string(id), enc_num, test="                                      ";
    if (type == 1)
    {
        string findbynum = "select max(encryption_number) from encryption";
        const char* qnum = findbynum.c_str();
        qstate = mysql_query(conn, qnum);
        res = mysql_store_result(conn);
        while ((row = mysql_fetch_row(res)))
        {
            max_enc_num = stoi(row[0]);
        }
        if (choice == 1)
        {
            string insert_query = "insert into encryption (encryption_number, lect_id, encryption_code) values ('" + to_string(max_enc_num+1) + "','" + to_string(id) + "','" + to_string(rand() % 10) + "')";
            const char* q = insert_query.c_str();
            qstate = mysql_query(conn, q);
        }
        else if (choice == 2)
        {
            string insert_query = "insert into encryption (encryption_number, driver_id, encryption_code) values ('" + to_string(max_enc_num + 1) + "','" + to_string(id) + "','" + to_string(rand() % 10) + "')";
            const char* q = insert_query.c_str();
            qstate = mysql_query(conn, q);
        }
        else if (choice == 3)
        {
            string insert_query = "insert into encryption (encryption_number, admin_id, encryption_code) values ('" + to_string(max_enc_num + 1) + "','" + to_string(id) + "','" + to_string(rand() % 10) + "')";
            const char* q = insert_query.c_str();
            qstate = mysql_query(conn, q);
        }
    }
    if (choice == 1)
    {
        string findbyid = "select encryption_number, encryption_code from encryption where lect_id = '" + strid + "'";
        const char* qid = findbyid.c_str();
        qstate = mysql_query(conn, qid);
    }
    else if (choice == 2)
    {
        string findbyid = "select encryption_number, encryption_code from encryption where driver_id = '" + strid + "'";
        const char* qid = findbyid.c_str();
        qstate = mysql_query(conn, qid);
    }
    else if (choice == 3)
    {
        string findbyid = "select encryption_number, encryption_code from encryption where admin_id = '" + strid + "'";
        const char* qid = findbyid.c_str();
        qstate = mysql_query(conn, qid);
    }
    res = mysql_store_result(conn);
    while ((row = mysql_fetch_row(res)))
    {
        enc_num = row[0];
        enc_code = stoi(row[1]);
    }
    test.resize(pwd.length() * 2);
    for (int i = 0;i < pwd.length() * 2;i++)
    {
        int x = i / 2;
        if (i == 0)
        {
            test[i] = pwd[i];
            test[i] += enc_code;

        }
        else if (i % 2 == 0)
        {
            test[i] = pwd[x];
            test[i] += enc_code;
        }
        else
        {
            test[i] = ' ' + (rand() % 94);
        }
    }
    if (type == 1)
    {
        string insert_query = "insert into passwords (encryption_number, p_password) values ('" + enc_num + "','" + test + "')";
        const char* q = insert_query.c_str();
        qstate = mysql_query(conn, q);
    }
    else if(type==2)
    {
        string update_query = "update passwords set p_password = '" + test + "' where encryption_number ='" + enc_num + "'";
        const char* qu = update_query.c_str();
        qstate = mysql_query(conn, qu);
    }
    if (!qstate)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool pwdCheck(const string& pwd, int id, int choice)
{
    string strid = to_string(id),pwdori,test;
    int code=0;
    if (choice == 1)
    {
        string findbyid = "select e.encryption_code, p.p_password from lecturer l, encryption e, passwords p where l.lect_id=e.lect_id and e.encryption_number=p.encryption_number and e.lect_id = '" + strid + "'";
        const char* qid = findbyid.c_str();
        qstate = mysql_query(conn, qid);
    }
    else if (choice == 2)
    {
        string findbyid = "select e.encryption_code, p.p_password from busdriver d, encryption e, passwords p where d.driver_id=e.driver_id and e.encryption_number=p.encryption_number and e.driver_id = '" + strid + "'";
        const char* qid = findbyid.c_str();
        qstate = mysql_query(conn, qid);
    }
    else if (choice == 3)
    {
        string findbyid = "select e.encryption_code, p.p_password from admin a, encryption e, passwords p where a.admin_id=e.admin_id and e.encryption_number=p.encryption_number and e.admin_id = '" + strid + "'";
        const char* qid = findbyid.c_str();
        qstate = mysql_query(conn, qid);
    }
    res = mysql_store_result(conn);
    while ((row = mysql_fetch_row(res)))
    {
        code = stoi(row[0]);
        pwdori=row[1];
    }
    test.resize(pwdori.length() / 2);
    for (int i = 0;i < (pwdori.length() / 2);i++)
    {
        int x = i * 2;

        if (i == 0)
        {
            test[i] = pwdori[i];
        }
        else
        {
            test[i] = pwdori[x];
        }
        test[i] -= code;
    }
    if (test == pwd)
    {
        system("cls");
        cout << "Login Successful.\n";
        system("pause");
        return true;
    }
    else
        return false;
    
}

int login(int choice) 
{
id:
    int i = 0, option,id,forgot_temp,login_temp;
    system("cls");
    if (choice == 1)
    {
        cout << "Path> Main menu/Login/Lecturer/\n\n";
        cout << "\nPlease enter your lecturer ID:\n:";
    }
    else
    {
        cout << "Path> Main menu/Login/Bus driver/\n\n";
        cout << "\nPlease enter your driver ID:\n:";
    }
    cin >> id;
    if (id > 99999)
    {
        system("cls");
        cout << "Id is too long.";
    d:
        cout << "\n\nPlease choose:\n\n1. Retry ID\n2. Back\n3. Main menu\n4. Exit\n\n:";
        cin >> option;
        switch (option)
        {
        case 1:
            goto id;
            break;
        case 2:
            return 3;
            break;
        case 3:
            return 1;
            break;
        case 4:
            return 2;
            break;
        default:
            system("cls");
            cout << invalid_selection;
            goto d;
            break;
        }
    }
    cout << "\n\n\n";
pass:
    char pwd[20], temp;
    cout << "Please enter your password:\n:";
    for (i = 0;;)
    {
        temp = _getch();
        if ((temp >= 'a' && temp <= 'z') || (temp >= 'A' && temp <= 'Z') || (temp >= '0' && temp <= '9'))
        {
            pwd[i] = temp;
            ++i;
            cout << "*";
        }
        if (temp == '\b' && i >= 1)
        {
            cout << "\b \b";
            --i;
        }
        if (temp == '\r')
        {
            pwd[i] = '\0';
            break;
        } 
    }
    string pwdori = pwd;
    system("cls");
    if (choice == 1)
    {
        cout << "Path> Main menu/Login/Lecturer/\n\n";
        cout << "\nLecturer ID:" << id << endl;
    }
    else
    {
        cout << "Path> Main menu/Login/Bus driver/\n\n";
        cout << "\nDriver ID:" << id << endl;
    }
    if (i < 7) 
    {
        cout << "Minimum 8 characters needed.";
    c:
        cout << "\n\nPlease choose:\n\n1. Retry password\n2. Forgot password\n3. Back\n4. Main menu\n5. Exit\n\n:";
        cin >> option;
        switch (option)
        {
        case 1:
            system("cls");
            goto pass;
            break;
        case 2:
            forgot_temp = forgot_password(choice);
            switch (forgot_temp)
            {
            case 1:
                return 1;
                break;
            case 2:
                return 2;
                break;
            }
            break;
        case 3:
            return 3;
            break;
        case 4:
            return 1;
            break;
        case 5:
            return 2;
            break;
        default:
            cout << invalid_selection;
            goto c;
            break;
        }
    }
    else if (pwdCheck(pwdori,id,choice)==1)
    {
        if (choice == 1)
        {
            login_temp=lecturer(id, 0);
        }
        else
        {
            login_temp=driver(id, 0);
        }
        if (login_temp == 3)
        {
            goto id;
        }
        else if (login_temp == 4)
        {
            choice = 1;
            goto id;
        }
        else if (login_temp == 5)
        {
            choice = 2;
            goto id;
        }
        else
        { 
            return login_temp;
        }
    }
    else
    {
        cout << "The ID or password is incorrect.";
    b:

        cout << "\n\nPlease choose:\n\n1. Retry\n2. Forgot password\n3. Back\n4. Main menu\n5. Exit\n\n:";
        cin >> option;

        switch (option)
        {
        case 1:
            system("cls");
            goto id;
            break;
        case 2:
            forgot_temp = forgot_password(choice);
            return forgot_temp;
            break;
        case 3:
            return 3;
            break;
        case 4:
            return 1;
            break;
        case 5:
            return 2;
            break;
        default:
            cout << invalid_selection;
            goto b;
            break;
        }
    }
    return 1;
}

int forgot_password(int choice)
{
start:
    string name, temp, findbyid, insert_query;
    int id,option,checker=0;
    char conf;
    system("cls");
    if (choice == 1)
    {
        cout << "Path> Main menu/Login/Lecturer/Forgot password/\n\n";
        cout << "\nPlease re-enter your lecturer id:\n:";
    }
    else
    {
        cout << "Path> Main menu/Login/Bus driver/Forgot password/\n\n";
        cout << "\nPlease re-enter your driver id:\n:";
    }
    cin >> id;
    if (id > 99999)
    {
        system("cls");
        cout << "Id is too long.";
    d:
        cout << "\n\nPlease choose:\n\n1. Retry ID\n2. Main menu\n3. Exit\n\n:";
        cin >> option;
        switch (option)
        {
        case 1:
            goto start;
            break;
        case 2:
            return 1;
            break;
        case 3:
            return 2;
            break;
        default:
            system("cls");
            cout << "Invalid Selection. Please try again.\n";
            goto d;
            break;
        }
    }
    system("cls");
    temp = to_string(id);
    if(choice==1)
        findbyid = "select * from lecturer where lect_id = '" + temp + "'";
    else
        findbyid = "select * from busdriver where driver_id = '" + temp + "'";
    const char* qid = findbyid.c_str();
    qstate = mysql_query(conn, qid);
    res = mysql_store_result(conn);
    while ((row = mysql_fetch_row(res)))
    {
    b:
        checker = 1;
        cout << "Please confirm your name. Is your name " << row[1] << " ? (Y/N)\n:";
        cin >> conf;
        if (conf == 'y' || conf == 'Y')
        {
            if (choice == 1)
            {
                insert_query = "insert into forgotpwd_log (log_no,lect_id,driver_id,date,status,admin_id) values (NULL,'" + temp + "',NULL,'" + date_retrieval() + "',1,NULL)";
            }
            else
            {
                insert_query = "insert into forgotpwd_log (log_no,lect_id,driver_id,date,status,admin_id) values (NULL,NULL,'" + temp + "','" + date_retrieval() + "',1,NULL)";
            }
            const char* q = insert_query.c_str();
            qstate = mysql_query(conn, q);
            if (!qstate)
            {
                cout << "\nSuccessful. System administor will contact you within a week time.\n";
            }
            else
            {
                cout << "\nQuery Execution Problem!" << mysql_errno(conn);
            }
            string findbyid = "select * from admin";
            const char* qid = findbyid.c_str();
            qstate = mysql_query(conn, qid);
            res = mysql_store_result(conn);
            while ((row = mysql_fetch_row(res)))
            {
                cout << "\n\nFor further assisstance, please contact:\n" << row[1] << " (" << row[2] << "/" << row[3] << ")\n\n";
                break;
            }
            a:cout << "Please choose:\n\n1. Main menu\n2. Exit\n\n:";
            cin >> option;
            switch (option)
            {
            case 1:
                return 1;
                break;
            case 2:
                return 2;
                break;
            default:
                system("cls");
                cout << "Invalid Selection. Please try again.\n";
                goto a;
            }
        }
        else if (conf == 'n' || conf == 'N')
        {
        c:
            cout << "Please choose:\n\n1. Main menu\n2. Exit\n\n:";
            cin >> option;
            switch (option)
            {
            case 1:
                return 1;
                break;
            case 2:
                return 2;
                break;
            default:
                system("cls");
                cout << "Invalid Selection. Please try again.\n";
                goto c;
            }
        }
        else
        {
            system("cls");
            cout << "\nInvalid character. Try again.\n" << endl;
            goto b;
        }
    }
    if (checker==0)
    {
        cout << "The id does not exist.\n\n";
    menu:
        cout << "Please choose:\n\n1. Retry\n2. Main menu\n3. Exit\n\n:";
        cin >> option;
        switch (option)
        {
        case 1:
            goto start;
            break;
        case 2:
            return 1;
            break;
        case 3:
            return 2;
            break;
        default:
            cout << "Invalid Selection. Please try again.\n";
            system("cls");
            goto menu;
            break;
        }
    }
}

int searchuser()
{
main:
    int option = 0, turn=1, test1=0, test2=0, test3=0, entry=1;
    string temp;
    system("cls");
    cout << "Path> Admin menu/Search user/\n\n";
    cout << "Searh user by:\n\n1. ID\n2. Name\n3. IC no.\n4. Faculty (Only for lecturers)\n5. Phone number\n6. Email\n7. Admin menu\n8. Main menu\n9. Exit\n\n:";
    cin >> option;
    system("cls");
    cout << "Path> Admin menu/Search user/\n\n";
swth:
    switch (option)
    {
    case 1:
    {
        if (turn == 1)
        {
            cout << "Please enter ID to search: ";
            cin >> temp;
            string findbyid = "select * from lecturer where lect_id = '" + temp + "'";
            const char* qid = findbyid.c_str();
            qstate = mysql_query(conn, qid);
        }
        else if (turn == 2)
        {
            string findbyid = "select * from busdriver where driver_id = '" + temp + "'";
            const char* qid = findbyid.c_str();
            qstate = mysql_query(conn, qid);
        }
        else
        {
            string findbyid = "select * from admin where admin_id = '" + temp + "'";
            const char* qid = findbyid.c_str();
            qstate = mysql_query(conn, qid);
        }
    }
        break;
    case 2:
    {
        if (turn == 1)
        {
            cout << "Please enter name to search: ";
            cin >> temp;
            string findbyname = "select * from lecturer where lect_name like '" + temp + "%'";
            const char* qname = findbyname.c_str();
            qstate = mysql_query(conn, qname);
        }
        else if (turn==2)
        {
            string findbyname = "select * from busdriver where driver_name like '" + temp + "%'";
            const char* qname = findbyname.c_str();
            qstate = mysql_query(conn, qname);
        }
        else
        {
            string findbyname = "select * from admin where admin_name like '" + temp + "%'";
            const char* qname = findbyname.c_str();
            qstate = mysql_query(conn, qname);
        }
    }
        break;
    case 3:
    {
        if (turn == 1)
        {
            cout << "Please enter IC no. to search: ";
            cin >> temp;
            string findbyic = "select * from lecturer where lect_ic like '" + temp + "%'";
            const char* qic = findbyic.c_str();
            qstate = mysql_query(conn, qic);
        }
        else if (turn == 2)
        {
            string findbyic = "select * from busdriver where driver_ic like '" + temp + "%'";
            const char* qic = findbyic.c_str();
            qstate = mysql_query(conn, qic);
        }
    }
        break;
    case 4:
    {
        if (turn == 1)
        {
            cout << "Please enter faculty to search: ";
            cin >> temp;
            string findbyfac = "select * from lecturer where lect_faculty like '" + temp + "%'";
            const char* qfac = findbyfac.c_str();
            qstate = mysql_query(conn, qfac);
        }
    }
        break;
    case 5:
    {
        if (turn == 1)
        {
            cout << "Please enter phone number to search: ";
            cin >> temp;
            string findbypn = "select * from lecturer where lect_phone like '" + temp + "%'";
            const char* qpn = findbypn.c_str();
            qstate = mysql_query(conn, qpn);
        }
        else if (turn == 2)
        {
            string findbypn = "select * from busdriver where driver_phone like '" + temp + "%'";
            const char* qpn = findbypn.c_str();
            qstate = mysql_query(conn, qpn);
        }
        else
        {
            string findbypn = "select * from admin where admin_phone like '" + temp + "%'";
            const char* qpn = findbypn.c_str();
            qstate = mysql_query(conn, qpn);
        }
    }
        break;
    case 6:
    {
        if (turn == 1)
        {
            cout << "Please enter email to search: ";
            cin >> temp;
            string findbymail = "select * from lecturer where lect_email like '" + temp + "%'";
            const char* qmail = findbymail.c_str();
            qstate = mysql_query(conn, qmail);
        }
        else if (turn == 2)
        {
            string findbymail = "select * from busdriver where driver_email like '" + temp + "%'";
            const char* qmail = findbymail.c_str();
            qstate = mysql_query(conn, qmail);
        }
        else
        {
            string findbymail = "select * from admin where admin_email like '" + temp + "%'";
            const char* qmail = findbymail.c_str();
            qstate = mysql_query(conn, qmail);
        }
    }
        break;
    case 7:
        return 3;
        break;
    case 8:
        return 1;
        break;
    case 9:
        return 2;
        break;
    default:
        system("cls");
        cout << invalid_selection;
        system("pause");
        goto main;
        break;
    }
    if (turn==1)
    {
        res = mysql_store_result(conn);
        while ((row = mysql_fetch_row(res)))
        {
            test1 = 1;
            cout << "\nResult: " << entry << "\nID: " << row[0] << "\nName: " << row[1] << "\nIC no: " << row[2] << "\nUser type = Lecturer" << "\nFaculty: " << row[3] << "\nPhone number: " << row[4] << "\nEmail: " << row[5] << "\n\n";
            entry++;
        }
        turn = 2;
        goto swth;
    }
    if (turn==2)
    {
        res = mysql_store_result(conn);
        while ((row = mysql_fetch_row(res)))
        {
            test2 = 1;
            cout << "\nResult: " << entry << "\nID: " << row[0] << "\nName: " << row[1] << "\nIC no: " << row[2] << "\nUser type = Bus driver" << "\nPhone number: " << row[3] << "\nEmail: " << row[4] << "\n\n";
            entry++;
        }
        turn = 3;
        goto swth;
    }
    if (turn==3)
    {
        res = mysql_store_result(conn);
        while ((row = mysql_fetch_row(res)))
        {
            test3 = 1;
            cout << "\nResult: " << entry << "\nID: " << row[0] << "\nName: " << row[1] << "\nUser type = Admin" << "\nPhone number: " << row[2] << "\nEmail: " << row[3] << "\n\n";
            entry++;
        }
    }
    if (test1 == 0 && test2 == 0 && test3 == 0)
    {
        cout << "\n\nNo data matched.\n\n";
    }
menu:
    cout << "\Please choose:\n1. Search user again\n2. Admin menu\n3. Main menu\n4. Exit\n\n:";
    cin >> option;
    switch (option)
    {
    case 1:
        goto main;
        break;
    case 2:
        return 3;
        break;
    case 3:
        return 1;
        break;
    case 4:
        return 2;
        break;
    default:
        system("cls");
        cout << invalid_selection;
        system("pause");
        goto menu;
        break;

    }
}

int removeuser()
{
    int option;
    char test;
    string temp;
menu: system("cls");
    int checker = 0;
    cout << "Path> Admin menu/Remove user/\n\n";
    cout << "Please select the type of user to remove:\n\n1. Lecturer\n2. Bus driver\n3. Admin\n4. Admin menu\n5. Main menu\n6. Exit\n\n:";
    cin >> option;
    switch (option)
    {
    case 1:
    {
    lectmain: system("cls");
        cout << "Path> Admin menu/Remove user/Lecturer/\n\n";
        cout << "Please enter the Lecturer ID of the account to be removed:\n:";
        cin >> temp;
        string findbyid = "select * from lecturer where lect_id = '" + temp + "'";
        const char* qid = findbyid.c_str();
        qstate = mysql_query(conn, qid);
        res = mysql_store_result(conn);
        while ((row = mysql_fetch_row(res)))
        {
        exit:
            checker = 1;
            system("cls");
            cout << "Are you sure to remove the lecturer account with the ID " << temp << " ? (Y/N): ";
            cin >> test;
            if (test == 'y' || test == 'Y')
            {
                string delete_query1 = "delete from password where encryption_number = (select encryption_number from encryption where lect_id = '" + temp + "')";
                const char* qd1 = delete_query1.c_str();
                qstate = mysql_query(conn, qd1);
                string delete_query2 = "delete from encryption where lect_id = '" + temp + "'";
                const char* qd2 = delete_query2.c_str();
                qstate = mysql_query(conn, qd2);
                string delete_query3 = "delete from forgotpwd_log where lect_id = '" + temp + "'";
                const char* qd3 = delete_query3.c_str();
                qstate = mysql_query(conn, qd3);
                string delete_query4 = "delete from data_table where lect_id = '" + temp + "'";
                const char* qd4 = delete_query4.c_str();
                qstate = mysql_query(conn, qd4);
                string delete_query = "delete from lecturer where lect_id = '" + temp + "'";
                const char* qd = delete_query.c_str();
                qstate = mysql_query(conn, qd);
            }
            else if (test == 'n' || test == 'N')
            {
            exit1: system("cls");
                cout << "The account has not been removed. Please choose:\n\n1. Retry\n2. Admin menu\n\n:";
                cin >> option;
                if (option == 1)
                    goto lectmain;
                else if (option == 2)
                    return 3;
                else
                {
                    cout << invalid_selection;
                    system("pause");
                    goto exit1;
                }
            }
            else
            {
                cout << "\nInvalid character. Try again.\n" << endl;
                system("pause");
                goto exit;
            }
        }
        if(checker==0)
        {
        lectno:
            system("cls");
            cout << "The Lecturer ID does not exist. Please choose:\n\n1. Retry\n2. Admin menu\n3. Main menu\n4. Exit\n\n:";
            cin >> option;
            switch (option)
            {
            case 1:
                goto lectmain;
                break;
            case 2:
                return 3;
                break;
            case 3:
                return 1;
                break;
            case 4:
                return 2;
                break;
            default:
                cout << invalid_selection;
                system("pause");
                goto lectno;
                break;
            }
        }
    }
        break;
    case 2:
    {
    busmain: system("cls");
        cout << "Path> Admin menu/Remove user/Bus driver/\n\n";
        cout << "Please enter the Driver ID of the account to be removed:\n:";
        cin >> temp;
        string findbyid = "select * from busdriver where driver_id = '" + temp + "'";
        const char* qid = findbyid.c_str();
        qstate = mysql_query(conn, qid);
        res = mysql_store_result(conn);
        while ((row = mysql_fetch_row(res)))
        {
        busexit:
            checker = 1;
            system("cls");
            cout << "Are you sure to remove the bus driver account with the ID " << temp << " ? (Y/N): ";
            cin >> test;
            if (test == 'y' || test == 'Y')
            {
                string delete_query1 = "delete from password where encryption_number = (select encryption_number from encryption where driver_id = '" + temp + "')";
                const char* qd1 = delete_query1.c_str();
                qstate = mysql_query(conn, qd1);
                string delete_query2 = "delete from encryption where driver_id = '" + temp + "'";
                const char* qd2 = delete_query2.c_str();
                qstate = mysql_query(conn, qd2);
                string delete_query3 = "delete from forgotpwd_log where driver_id = '" + temp + "'";
                const char* qd3 = delete_query3.c_str();
                qstate = mysql_query(conn, qd3);
                string delete_query = "delete from busdriver where driver_id = '" + temp + "'";
                const char* qd = delete_query.c_str();
                qstate = mysql_query(conn, qd);
            }
            else if (test == 'n' || test == 'N')
            {
            busexit1: system("cls");
                cout << "The account has not been removed. Please choose:\n\n1. Retry\n2. Admin menu\n\n:";
                cin >> option;
                if (option == 1)
                    goto busmain;
                else if (option == 2)
                    return 3;
                else
                {
                    cout << invalid_selection;
                    system("pause");
                    goto busexit1;
                }
            }
            else
            {
                cout << "\nInvalid character. Try again.\n" << endl;
                system("pause");
                goto busexit;
            }
        }
        if (checker == 0)
        {
        busno:
            system("cls");
            cout << "The Driver ID does not exist. Please choose:\n\n1. Retry\n2. Admin menu\n3. Main menu\n4. Exit\n\n:";
            cin >> option;
            switch (option)
            {
            case 1:
                goto busmain;
                break;
            case 2:
                return 3;
                break;
            case 3:
                return 1;
                break;
            case 4:
                return 2;
                break;
            default:
                cout << invalid_selection;
                system("pause");
                goto busno;
                break;
            }
        }
    }
        break;
    case 3:
    {
    admmain: system("cls");
        cout << "Path> Admin menu/Remove user/Admin/\n\n";
        cout << "Please enter the Admin ID of the account to be removed:\n:";
        cin >> temp;
        string findbyid = "select * from admin where admin_id = '" + temp + "'";
        const char* qid = findbyid.c_str();
        qstate = mysql_query(conn, qid);
        res = mysql_store_result(conn);
        while ((row = mysql_fetch_row(res)))
        {
        admexit:
            checker = 1;
            system("cls");
            cout << "Are you sure to remove the admin account with the ID " << temp << " ? (Y/N): ";
            cin >> test;
            if (test == 'y' || test == 'Y')
            {
                string delete_query1 = "delete from password where encryption_number = (select encryption_number from encryption where admin_id = '" + temp + "')";
                const char* qd1 = delete_query1.c_str();
                qstate = mysql_query(conn, qd1);
                string delete_query2 = "delete from encryption where admin_id = '" + temp + "'";
                const char* qd2 = delete_query2.c_str();
                qstate = mysql_query(conn, qd2);
                string delete_query = "delete from admin where admin_id = '" + temp + "'";
                const char* qd = delete_query.c_str();
                qstate = mysql_query(conn, qd);
            }
            else if (test == 'n' || test == 'N')
            {
            admexit1: system("cls");
                cout << "The account has not been removed. Please choose:\n\n1. Retry\n2. Admin menu\n\n:";
                cin >> option;
                if (option == 1)
                    goto admmain;
                else if (option == 2)
                    return 3;
                else
                {
                    cout << invalid_selection;
                    system("pause");
                    goto admexit1;
                }
            }
            else
            {
                cout << "\nInvalid character. Try again.\n" << endl;
                system("pause");
                goto admexit;
            }
        }
        if (checker == 0)
        {
        admno:
            system("cls");
            cout << "The Admin ID does not exist. Please choose:\n\n1. Retry\n2. Admin menu\n3. Main menu\n4. Exit\n\n:";
            cin >> option;
            switch (option)
            {
            case 1:
                goto admmain;
                break;
            case 2:
                return 3;
                break;
            case 3:
                return 1;
                break;
            case 4:
                return 2;
                break;
            default:
                cout << invalid_selection;
                system("pause");
                goto admno;
                break;
            }
        }
    }
        break;
    case 4:
        return 3;
        break;
    case 5:
        return 1;
        break;
    case 6:
        return 2;
        break;
    default:
        cout << invalid_selection;
        system("pause");
        goto menu;
        break;
    }
menu2: system("cls");
    if (qstate)
        cout << "Failed to remove the account. ";
    else
        cout << "The account has been successfully removed. ";
    cout << "Please choose:\n\n1. Remove another account\n2. Admin menu\n3. Main menu\n4. Exit\n\n:";
    cin >> option;
    switch (option)
    {
    case 1:
        goto menu;
        break;
    case 2:
        return 3;
        break;
    case 3:
        return 1;
        break;
    case 4:
        return 2;
        break;
    default:
        cout << invalid_selection;
        system("pause");
        goto menu2;
        break;
    }
}

int addadmin()
{
main:
    int option;
    string temp1,temp2,temp3,temp4;
    system("cls");
    cout << "Path> Admin menu/Add admin/\n\n";
    cout << "Plases enter new Admin ID:\n:";
    cin >> temp1;
    string findbyid = "select * from admin where admin_id = '" + temp1 + "'";
    const char* qid = findbyid.c_str();
    qstate = mysql_query(conn, qid);
    res = mysql_store_result(conn);
    while ((row = mysql_fetch_row(res)))
    {
        cout << "\n The admin ID already exist. Please try again.\n";
        system("pause");
        goto main;
    }
    cout << "\nPlases enter the name:\n:";
    cin >> temp2;
    cout << "\nPlases enter the phone number:\n:";
    cin >> temp3;
    cout << "\nPlases enter the email:\n:";
    cin >> temp4;
    string insert_query = "insert into admin (admin_id, admin_name, admin_phone, admin_email) values ('" + temp1 + "','" + temp2 + "','" + temp3 + "','" + temp4 + "')";
    const char* q = insert_query.c_str();
    qstate = mysql_query(conn, q);
    system("cls");
    if (!qstate)
    {
        pwdEncryption("abcd1234", stoi(temp1), 3, 1);
        cout << "The account has been sucessfully created.\nThe default password is abcd1234. The password can be updated under update personal information option.\n\n";
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
    }
e: cout << "Please choose:\n1. Add another admin\n2. Admin menu\n3. Main menu\n4. Exit\n\n:";
    cin >> option;
    switch (option)
    {
    case 1:
        goto main;
        break;
    case 2:
        return 3;
        break;
    case 3:
        return 1;
        break;
    case 4:
        return 2;
        break;
    default:
        system("cls");
        cout << invalid_selection;
        goto e;
        break;
    }
}

int updateadmin(int id)
{
main:
    int option, turn;
    char conf, pwd[20], temp1;
    string temp, pwd1, pwd2,temp_id;
    temp_id = to_string(id);
    string findbyid = "select * from admin where admin_id = '" + temp_id + "'";
    const char* qid = findbyid.c_str();
    qstate = mysql_query(conn, qid);
    res = mysql_store_result(conn);
    while ((row = mysql_fetch_row(res)))
    {
        system("cls");
        cout << "Path> Admin menu/Update personal information/\n\n";
        cout << "Please choose attributes to update:\n\n1. Name\n2. Phone number\n3. Email\n4. Password\n5. Admin menu\n6. Main menu\n7. Exit\n\n:";
        cin >> option;
        system("cls");
        switch (option)
        {
        case 1:
        name2:
            cout << "Path> Admin menu/Update personal information/\n\n";
            cout << "Current name: "<<row[1]<<"\nPlease enter your name or 'X' to go back:\n:";
            cin >> temp;
            if (temp == "x" || temp == "X")
            {
                system("cls");
                goto main;
            }
            else
            {
            name3:
                cout << "\nPlease confirm to update (Y/N).\n:";
                cin >> conf;
                if (conf == 'Y' || conf == 'y')
                {
                    cout << endl;
                    string update_query = "update admin set admin_name = '" + temp + "' where admin_id = '" + temp_id + "'";
                    const char* qu = update_query.c_str();
                    qstate = mysql_query(conn, qu);
                name4:
                    if(!qstate)
                        cout << "Your name has been updated to " << temp;
                    else
                        cout << "Failed To Update!" << mysql_errno(conn);
                    cout << "\n\nPlease choose:\n\n1. Back\n2. Admin menu\n3. Main menu\n4. Exit\n\n:";
                    cin >> option;
                    system("cls");
                    switch (option)
                    {
                    case 1:
                        goto main;
                        break;
                    case 2:
                        return 3;
                        break;
                    case 3:
                        return 1;
                        break;
                    case 4:
                        return 2;
                        break;
                    default:
                        cout << invalid_selection;
                        goto name4;
                        break;
                    }
                }
                else if (conf == 'N' || conf == 'n')
                {
                    system("cls");
                    goto name2;
                }
                else
                {
                    cout << invalid_selection;
                    goto name3;
                }
            }
            break;
        case 2:
        pn2:
            cout << "Path> Admin menu/Update personal information/\n\n";
            cout << "Current phone number: " << row[2] << "Please enter your phone number or 'X' to go back:\n:";
            cin >> temp;
            if (temp == "x" || temp == "X")
            {
                system("cls");
                goto main;
            }
            else
            {
            pn3:
                cout << "\nPlease confirm to update (Y/N).\n:";
                cin >> conf;
                if (conf == 'Y' || conf == 'y')
                {
                    cout << endl;
                    string update_query = "update admin set admin_phone = '" + temp + "' where admin_id = '" + temp_id + "'";
                    const char* qu = update_query.c_str();
                    qstate = mysql_query(conn, qu);
                pn4:
                    if(!qstate)
                        cout << "Your phone number has been updated to " << temp;
                    else
                        cout << "Failed To Update!" << mysql_errno(conn);
                    cout << "\n\nPlease choose:\n\n1. Back\n2. Admin menu\n3. Main menu\n4. Exit\n\n:";
                    cin >> option;
                    system("cls");
                    switch (option)
                    {
                    case 1:
                        goto main;
                        break;
                    case 2:
                        return 3;
                        break;
                    case 3:
                        return 1;
                        break;
                    case 4:
                        return 2;
                        break;
                    default:
                        cout << invalid_selection;
                        goto pn4;
                        break;
                    }
                }
                else if (conf == 'N' || conf == 'n')
                {
                    system("cls");
                    goto pn2;
                }
                else
                {
                    cout << invalid_selection;
                    goto pn3;
                }
            }
            break;
        case 3:
        mail2:
            cout << "Path> Admin menu/Update personal information/\n\n";
            cout << "Current email: " << row[3] << "Please enter your email or 'X' to go back:\n:";
            cin >> temp;
            if (temp == "x" || temp == "X")
            {
                system("cls");
                goto main;
            }
            else
            {
            mail3:
                cout << "\nPlease confirm to update (Y/N).\n:";
                cin >> conf;
                if (conf == 'Y' || conf == 'y')
                {
                    cout << endl;
                    string update_query = "update admin set admin_email = '" + temp + "' where admin_id = '" + temp_id + "'";
                    const char* qu = update_query.c_str();
                    qstate = mysql_query(conn, qu);
                mail4:
                    if(!qstate)
                        cout << "Your email has been updated to " << temp;
                    else
                        cout << "Failed To Update!" << mysql_errno(conn);
                    cout << "\n\nPlease choose:\n\n1. Back\n2. Admin menu\n3. Main menu\n4. Exit\n\n:";
                    cin >> option;
                    system("cls");
                    switch (option)
                    {
                    case 1:
                        goto main;
                        break;
                    case 2:
                        return 3;
                        break;
                    case 3:
                        return 1;
                        break;
                    case 4:
                        return 2;
                        break;
                    default:
                        cout << invalid_selection;
                        goto mail4;
                        break;
                    }
                }
                else if (conf == 'N' || conf == 'n')
                {
                    system("cls");
                    goto mail2;
                }
                else
                {
                    cout << invalid_selection;
                    goto mail3;
                }
            }
            break;
        case 4:
        pwd2:
            turn = 1;
            system("cls");
            cout << "Path> Admin menu/Update personal information/\n\n";
            cout << "Please enter your new password (only alphanumeric) or 'X' to go back:\n:";
        passwd:
            for (int i = 0;;)
            {
                temp1 = _getch();
                if ((temp1 >= 'a' && temp1 <= 'z') || (temp1 >= 'A' && temp1 <= 'Z') || (temp1 >= '0' && temp1 <= '9'))
                {
                    pwd[i] = temp1;
                    ++i;
                    cout << "*";
                }
                if (temp1 == '\b' && i >= 1)
                {
                    cout << "\b \b";
                    --i;
                }
                if (temp1 == '\r')
                {
                    pwd[i] = '\0';
                    break;
                }
            }
            if (turn == 1)
                pwd1 = pwd;
            else
            {
                pwd2 = pwd;
                goto pwd5;
            }
            if (pwd1 == "x" || pwd1 == "X")
            {
                system("cls");
                goto main;
            }
            else
            {
                cout << "\n\n\nPlease re-enter your new password:\n:";
                turn = 2;
                goto passwd;
            pwd5:
                if (pwd1.compare(pwd2) == 0)
                {
                    cout << endl;
                    pwdEncryption(pwd, id, 3, 2);
                    system("cls");
                    cout << "Your password has been successfully updated. Please login again. \n";
                    system("pause");
                    return 1;
                }
                else
                {
                pwdxmatch:
                    cout << "\n\nPassword does not match. Please choose:\n\n1. Retry\n2. Back\n3. Admin menu\n4. Main menu\n5. Exit\n\n:";
                    cin >> option;
                    system("cls");
                    switch (option)
                    {
                    case 1:
                        goto pwd2;
                        break;
                    case 2:
                        goto main;
                        break;
                    case 3:
                        return 3;
                        break;
                    case 4:
                        return 1;
                        break;
                    case 5:
                        return 2;
                        break;
                    default:
                        cout << invalid_selection;
                        goto pwdxmatch;
                        break;
                    }
                }
            }
            break;
        case 5:
            return 3;
            break;
        case 6:
            return 1;
            break;
        case 7:
            return 2;
            break;
        default:
            cout << invalid_selection;
            goto main;
            break;
        }
    }
    return 3;
}

int forgetlog()
{
    menu: 
        system("cls");
        int option,total_res=0,curr_res=1;
        string id,findbyid = "select * from forgotpwd_log where status = 1";
        const char* qid = findbyid.c_str();
        qstate = mysql_query(conn, qid);
        res = mysql_store_result(conn);
        while ((row = mysql_fetch_row(res)))
        {
            total_res++;
        }
        findbyid = "select f.lect_id, l.lect_name, l.lect_phone, l.lect_email from forgotpwd_log f, lecturer l where f.lect_id=l.lect_id and f.status=1";
        qid = findbyid.c_str();
        qstate = mysql_query(conn, qid);
        res = mysql_store_result(conn);
        while ((row = mysql_fetch_row(res)))
        {
            id = row[0];
            cout << "Path> Admin menu/Proccess forget password log/\n\n";
            cout << "Log no: " << curr_res << "\tRemaining log: " << --total_res;
            cout << "\nID: " << id << "\nName: " << row[1] << "\nJob: Lecturer\nPhone number: " << row[2] << "\nEmail: " << row[3];
            cout << "\n\nPlease choose:\n\n1. Process log (Reset pasword to abcd1234)\n2. Admin menu\n3. Main menu\n4. Exit\n\n:";
            cin >> option;
            switch (option)
            {
            case 1:
            {
                curr_res++;
                pwdEncryption("abcd1234", stoi(id), 1, 2);
                string update_query = "update forgotpwd_log set status = 0 where lect_id ='" + id + "'";
                const char* qu = update_query.c_str();
                qstate = mysql_query(conn, qu);
                cout << "\nThe password for ID " << id << " has been sucessfully resetted.\n";
                system("pause");
            }
                break;
            case 2:
                return 3;
                break;
            case 3:
                return 1;
                break;
            case 4:
                return 2;
                break;
            default:
                cout << invalid_selection;
                system("pause");
                goto menu;
                break;
            }
            system("cls");
        }
        findbyid = "select f.driver_id, d.driver_name, d.driver_phone, d.driver_email from forgotpwd_log f, busdriver d where f.driver_id=d.driver_id and f.status=1";
        qid = findbyid.c_str();
        qstate = mysql_query(conn, qid);
        res = mysql_store_result(conn);
        while ((row = mysql_fetch_row(res)))
        {
            id = row[0];
            cout << "Path> Admin menu/Proccess forget password log/\n\n";
            cout << "Log no: " << curr_res << "\tRemaining log: " << --total_res;
            cout << "\nID: " << id << "\nName: " << row[1] << "\nJob: Bus driver\nPhone number: " << row[2] << "\nEmail: " << row[3];
            cout << "\n\nPlease choose:\n\n1. Process log (Reset pasword to abc123)\n2. Admin menu\n3. Main menu\n4. Exit\n\n:";
            cin >> option;
            switch (option)
            {
            case 1:
            {
                curr_res++;
                pwdEncryption("abcd1234", stoi(id), 2, 2);
                string update_query = "update forgotpwd_log set status = 0 where driver_id ='" + id + "'";
                const char* qu = update_query.c_str();
                qstate = mysql_query(conn, qu);
                cout << "\nThe password for ID " << id << " has been sucessfully resetted.\n";
                system("pause");
            }
                break;
            case 2:
                return 3;
                break;
            case 3:
                return 1;
                break;
            case 4:
                return 2;
                break;
            default:
                cout << invalid_selection;
                system("pause");
                goto menu;
                break;
            }
            system("cls");
        }
        cout << "There is no forget password log. Returning to admin menu...\n";
        system("pause");
        return 3;
}

int admin()
{
    int option, test=0, id, ids, choice;
    string temp1, admintitle = "Switching to admin mode...\n", exitadmin = "Exiting admin mode...\n", return_main = "Returning to main menu...\n";
    system("cls");
    char pwd[20], temp;
    cout << "Please enter your Admin ID:\n:";
    cin >> id;
    string findbyid = "select * from admin where admin_id = '" + to_string(id) + "'";
    const char* qid = findbyid.c_str();
    qstate = mysql_query(conn, qid);
    res = mysql_store_result(conn);
    while ((row = mysql_fetch_row(res)))
    {
        test = 2;
    }
    if (test!=2)
    {
        system("cls");
        slow_print(return_main, 50);
        return 1;
    }
    cout << "\n\nPlease enter your admin password:\n:";
    for (int i = 0;;)
    {
        temp = _getch();
        if ((temp >= 'a' && temp <= 'z') || (temp >= 'A' && temp <= 'Z') || (temp >= '0' && temp <= '9'))
        {
            pwd[i] = temp;
            ++i;
            cout << "*";
        }
        if (temp == '\b' && i >= 1)
        {
            cout << "\b \b";
            --i;
        }
        if (temp == '\r')
        {
            pwd[i] = '\0';
            break;
        }
    }
    if (pwdCheck(pwd, id, 3) == 1)
    {
        system("cls");
        system("color 04");
        slow_print(admintitle, 50);
    adminmenu: system("cls");
        cout << "ADMIN MENU\n\n";
        cout << "Please choose:\n\n1.  Search user\n2.  Remove user\n3.  User override\n4.  Add admin\n5.  Reset password for other admin\n6.  Update personal information\n7.  Process forgot password log\n8.  Update week\n9.  Update create new account code\n10. Update admin login code\n11. Generate report\n12. Main menu\n13. Exit\n\n:";
        cin >> option;
        switch (option)
        {
        case 1:
            test = searchuser();
            switch (test)
            {
            case 1:
                return 1;
                break;
            case 2:
                return 2;
                break;
            case 3:
                goto adminmenu;
                break;
            }
            break;
        case 2:
            test = removeuser();
            switch (test)
            {
            case 1:
                return 1;
                break;
            case 2:
                return 2;
                break;
            case 3:
                goto adminmenu;
                break;
            }
            break;
        case 3:
        {
        menu: system("cls");
            cout << "Path> Admin menu/User override/\n\n";
            cout << "Please select the type of user to override:\n\n1. Lecturer\n2. Bus driver\n3. Admin menu\n4. Main menu\n5. Exit\n\n:";
            cin >> option;
        menu1:
            system("cls");
            switch (option)
            {
            case 1:
                choice = 1;
                cout << "Path> Admin menu/User override/Lecturer/\n\n";
                cout << "\nPlease enter your lecturer ID:\n:";
                break;
            case 2:
                choice = 2;
                cout << "Path> Admin menu/User override/Bus driver/\n\n";
                cout << "\nPlease enter your driver ID:\n:";
                break;
            case 3:
                goto adminmenu;
                break;
            case 4:
                return 1;
                break;
            case 5:
                return 2;
                break;
            default:
                system("cls");
                cout << invalid_selection;
                system("pause");
                goto menu;
                break;
            }
            cin >> ids;
            if (choice == 1)
            {
                string findbyid = "select * from lecturer where lect_id = '" + to_string(ids) + "'";
                const char* qid = findbyid.c_str();
                qstate = mysql_query(conn, qid);
            }
            else if (choice == 2)
            {
                string findbyid = "select * from busdriver where driver_id = '" + to_string(ids) + "'";
                const char* qid = findbyid.c_str();
                qstate = mysql_query(conn, qid);
            }
            res = mysql_store_result(conn);
            while ((row = mysql_fetch_row(res)))
            {
                test = 1;
            }
            if (test != 1)
            {
                system("cls");
                cout << "Id does not exist.";
            d:
                cout << "\n\nPlease choose:\n\n1. Retry ID\n2. Admin menu\n3. Exit\n\n:";
                cin >> option;
                switch (option)
                {
                case 1:
                    goto menu1;
                    break;
                case 2:
                    goto adminmenu;
                    break;
                case 3:
                    return 2;
                    break;
                default:
                    system("cls");
                    cout << invalid_selection;
                    goto d;
                    break;
                }
            }
            else
            {
                if (choice == 1)
                    test = lecturer(ids, 1);
                else if (choice == 2)
                    test = driver(ids, 1);
                switch (test)
                {
                case 1:
                    goto adminmenu;
                    break;
                case 2:
                    return 2;
                    break;
                }
            }
        }
        break;
        case 4:
            test = addadmin();
            switch (test)
            {
            case 1:
                return 1;
                break;
            case 2:
                return 2;
                break;
            case 3:
                goto adminmenu;
                break;
            }
            break;
        case 5:
        {
        case5main:
            system("cls");
            cout << "Path> Admin menu/Reset password for other admin/\n\n";
            cout << "Please enter Admin ID to reset password:\n:";
            cin >> temp1;
            string findbyid = "select * from admin where admin_id = '" + temp1 + "'";
            const char* qid = findbyid.c_str();
            qstate = mysql_query(conn, qid);
            res = mysql_store_result(conn);
            while ((row = mysql_fetch_row(res)))
            {
                goto case5;
            }
            cout << "\nThe admin ID does not exist. Please try again.\n";
            system("pause");
            goto case5main;
        case5:
            cout << "\n\nAre you sure to reset the password(abcd1234) for admin account with ID " << temp1 << " ?(Y/N): ";
            cin >> temp;
            if (temp == 'y' || temp == 'Y')
            {
                if (pwdEncryption("abcd1234", stoi(temp1), 3, 2))
                {
                    system("cls");
                    cout << "The password for admin account with ID " << temp1 << " has been sucessfully resetted to (abcd1234).\n";
                }
                else
                {
                    system("cls");
                    cout << "The password cannot be resetted.\n";
                }
                system("pause");
                goto adminmenu;
            }
            else if (temp == 'n' || temp == 'N')
            {
                goto adminmenu;
            }
            else
            {
                cout << "\nInvalid character. Try again.\n" << endl;
                system("pause");
                system("cls");
                goto case5;
            }
        }
        break;
        case 6:
            test = updateadmin(id);
            switch (test)
            {
            case 1:
                return 1;
                break;
            case 2:
                return 2;
                break;
            case 3:
                goto adminmenu;
                break;
            }
            break;
        case 7:
            test = forgetlog();
            switch (test)
            {
            case 1:
                return 1;
                break;
            case 2:
                return 2;
                break;
            case 3:
                goto adminmenu;
                break;
            }
            break;
        case 8:
        {
        case4: system("cls");
            int c_week = 0, lect_entry = 0;
            string strid;
            string findweek = "select current_week from additional_info";
            const char* qweek = findweek.c_str();
            qstate = mysql_query(conn, qweek);
            res = mysql_store_result(conn);
            while ((row = mysql_fetch_row(res)))
            {
                c_week = stoi(row[0]);
            }
            cout << "Path> Admin menu/Update week/\n\n";
            cout << "Current week: " << c_week << "\n\nPlease choose:\n\n1. Update current week\n2. Admin menu\n3. Main menu\n4. Exit\n\n:";
            cin >> option;
            switch (option)
            {
            case 1:
            {
                system("cls");
                int count = 1;
                string findlect = "select lect_id from data_table where data_week = '"+to_string(c_week)+"' group by lect_id having count(data_number)<5";
                const char* qlect = findlect.c_str();
                qstate = mysql_query(conn, qlect);
                res = mysql_store_result(conn);
                while ((row = mysql_fetch_row(res)))
                {
                    if (count == 1)
                        cout << "Lecturers that has incomplete data:\nLecturer ID:\n";
                    cout << row[0]<<endl;
                    count = 2;
                }
                if (count == 2)
                {
                    cout << "\n\nPlease notify the lecturers to update the data in order to update the week. Week can only be updated once all lecturers updated their data.\n";
                    system("pause");
                    goto adminmenu;
                }
                cout << "Path> Admin menu/Update week/\n\n";
            case4_1: cout << "Are you sure you want to update current week to " << c_week + 1 << " ?(Y/N): ";
                cin >> temp;
                if (temp == 'y' || temp == 'Y')
                {
                    system("cls");
                    string update_query = "update additional_info set current_week = '" + to_string(c_week + 1) + "'";
                    const char* qu = update_query.c_str();
                    qstate = mysql_query(conn, qu);
                    if (!qstate)
                        cout << "The current week has been successfully updated to " << c_week + 1 << ".\n";
                    else
                        cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
                    system("pause");
                    goto adminmenu;
                }
                else if (temp == 'n' || temp == 'N')
                {
                    goto adminmenu;
                }
                else
                {
                    cout << "\nInvalid character. Try again.\n" << endl;
                    system("pause");
                    goto case4_1;
                }
            }

            break;
            case 2:
                goto adminmenu;
                break;
            case 3:
                return 1;
                break;
            case 4:
                return 2;
                break;
            default:
                cout << "Invalid Selection. Please try again.\n";
                system("pause");
                goto case4;
                break;
            }
        }
        break;
        case 9:
        {
        case8:system("cls");
            cout << "Path> Admin menu/Update create new account code/\n\n";
            string findbyid = "select * from additional_info";
            const char* qid = findbyid.c_str();
            qstate = mysql_query(conn, qid);
            res = mysql_store_result(conn);
            while ((row = mysql_fetch_row(res)))
            {
                cout << "Lecturer code: " << row[1] << "\nBus driver code: " << row[2] << "\n\nPlease choose:\n\n1. Update lecturer code\n2. Update bus driver code\n3. Admin menu\n4. Main menu\n5. Exit\n\n:";
            }
            cin >> option;
            switch (option)
            {
            case 1:
            {
                system("cls");
                cout << "Path> Admin menu/Update create new account code/Update lecturer code/\n\nPlease enter the new lecturer code:\n:";
                cin >> temp1;
                string update_query = "update additional_info set lect_code = '" + temp1 + "'";
                const char* qu = update_query.c_str();
                qstate = mysql_query(conn, qu);
                if (!qstate)
                    cout << "\n\nThe lecturer code has been sucessfully updated to " << temp1;
                else
                    cout << "\n\nFailed To Update!" << mysql_errno(conn);
            }
            break;
            case 2:
            {
                system("cls");
                cout << "Path> Admin menu/Update create new account code/Update bus driver code/\n\nPlease enter the new driver code:\n:";
                cin >> temp1;
                string update_query = "update additional_info set driver_code = '" + temp1 + "'";
                const char* qu = update_query.c_str();
                qstate = mysql_query(conn, qu);
                if (!qstate)
                    cout << "\n\nThe driver code has been sucessfully updated to " << temp1;
                else
                    cout << "\n\nFailed To Update!" << mysql_errno(conn);
            }
            break;
            case 3:
                goto adminmenu;
                break;
            case 4:
                return 1;
                break;
            case 5:
                return 2;
                break;
            default:
                system("cls");
                cout << invalid_selection;
                system("pause");
                goto case8;
                break;
            }
        case8_1:cout << "\nPlease choose:\n1. Back\n2. Admin menu\n3. Main menu\n4. Exit\n\n:";
            cin >> option;
            switch (option)
            {
            case 1:
                goto case8;
                break;
            case 2:
                goto adminmenu;
                break;
            case 3:
                return 1;
                break;
            case 4:
                return 2;
                break;
            default:
                system("cls");
                cout << invalid_selection;
                system("pause");
                system("cls");
                goto case8_1;
                break;
            }
        }
        break;
        case 10:
        {
            system("cls");
            cout << "Path> Admin menu/Update admin login code/\n\n";
            string findbyid = "select admin_code from additional_info";
            const char* qid = findbyid.c_str();
            qstate = mysql_query(conn, qid);
            res = mysql_store_result(conn);
            while ((row = mysql_fetch_row(res)))
            {
                cout << "Current admin login code: " << row[0];
            }
            cout << "\nEnter new admin login code (make sure not 1, 2, or 3):\n:";
            cin >> ids;
            string update_query = "update additional_info set admin_code = '" + to_string(ids) + "'";
            const char* qu = update_query.c_str();
            qstate = mysql_query(conn, qu);
            if (!qstate)
            {
                cout << "\nSucessfully updated. Please login again.\n";
                system("pause");
                system("cls");
                slow_print(exitadmin, 50);
                slow_print(return_main, 50);
                return 1;
            }

            else
            {
                cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
                system("pause");
                goto adminmenu;
            }
        }
        break;
        case 11:
        {
        rptopt:
            system("cls");
            cout << "Path> Admin menu/Generate report/\n\n";
            cout << "Please enter the week to generate report:\n:";
            cin >> option;
            test = reportGen(1, option, id);
            switch (test)
            {
            case 1:
                goto adminmenu;
                break;
            case 2:
                goto rptopt;
                break;
            }
        }
            break;
        case 12:
            system("cls");
            slow_print(exitadmin, 50);
            slow_print(return_main, 50);
            return 1;
            break;
        case 13:
            system("cls");
            slow_print(exitadmin, 50);
            return 2;
            break;
        default:
            system("cls");
            cout << "Invalid Selection. Please try again.\n";
            goto adminmenu;
            break;
        }
    }
    else
    {
        system("cls");
        slow_print(return_main, 50);
        return 1;
    }
    return 1;
}

int lecturer(int id, int adm)
{
    int option, turn, ids=id, test, adms=adm;
    char conf, pwd[20], temp1;
    string temp, pwd1, pwd2, temp_id=to_string(id);
    system("cls");
    log_check(ids, 1);
main:
    if (adm == 0)
    {
        cout << "Path> Main menu/Login/Lecturer/\n\n";
        cout << "Please choose:\n\n1. Update personal information\n2. Add data to schedule\n3. Access individual report\n4. Main menu\n5. Exit\n\n:";
    }  
    else
    {
        cout << "Path> Admin menu/User override/Lecturer/\n\n";
        cout << "Please choose:\n\n1. Update personal information\n2. Add data to schedule\n3. Access individual report\n4. Admin menu\n5. Exit\n\n:";
    }
    cin >> option;
    switch (option)
    {
    case 1:
    {
    main1:
        system("cls");
        string findbyid = "select * from lecturer where lect_id = '" + temp_id + "'";
        const char* qid = findbyid.c_str();
        qstate = mysql_query(conn, qid);
        res = mysql_store_result(conn);
        while ((row = mysql_fetch_row(res)))
        {
            if (adm == 0)
            {
                cout << "Path> Main menu/Login/Lecturer/Update personal information/\n\n";
                cout << "Please choose attributes to update:\n\n1. Name\n2. Ic no.\n3. Faculty\n4. Phone number\n5. Email\n6. Password\n7. Back\n8. Main menu\n9. Exit\n\n:";
            }
            else
            {
                cout << "Path> Admin menu/User override/Lecturer/Update personal information/\n\n";
                cout << "Please choose attributes to update:\n\n1. Name\n2. Ic no.\n3. Faculty\n4. Phone number\n5. Email\n6. Password\n7. Back\n8. Admin menu\n9. Exit\n\n:";
            }
            cin >> option;
            system("cls");
            switch (option)
            {
            case 1:
            name2:
                if (adm == 0)
                    cout << "Path> Main menu/Login/Lecturer/Update personal information/\n\n";
                else
                    cout << "Path> Admin menu/User override/Lecturer/Update personal information/\n\n";
                cout << "Your current name: " << row[1];
                cout << "\nPlease update your name or 'X' to go back:\n:";
                cin >> temp;
                if (temp == "x" || temp == "X")
                {
                    system("cls");
                    goto main1;
                }
                else
                {
                name3:
                    cout << "\nPlease confirm to update (Y/N).\n:";
                    cin >> conf;
                    if (conf == 'Y' || conf == 'y')
                    {
                        cout << endl;
                        string update_query = "update lecturer set lect_name = '" + temp + "' where lect_id = '" + temp_id + "'";
                        const char* qu = update_query.c_str();
                        qstate = mysql_query(conn, qu);
                    name4:
                        if (!qstate)
                            cout << "Your name has been updated to " << temp;
                        else
                            cout << "Failed To Update!" << mysql_errno(conn);
                        if (adm == 0)
                            cout << "\n\nPlease choose:\n\n1. Back\n2. Main menu\n3. Exit\n\n:";
                        else
                            cout << "\n\nPlease choose:\n\n1. Back\n2. Admin menu\n3. Exit\n\n:";
                        cin >> option;
                        system("cls");
                        switch (option)
                        {
                        case 1:
                            goto main1;
                            break;
                        case 2:
                            return 1;
                            break;
                        case 3:
                            return 2;
                            break;
                        default:
                            cout << invalid_selection;
                            goto name4;
                            break;
                        }
                    }
                    else if (conf == 'N' || conf == 'n')
                    {
                        system("cls");
                        goto name2;
                    }
                    else
                    {
                        cout << invalid_selection;
                        goto name3;
                    }
                }
                break;
            case 2:
            ic2:
                if (adm == 0)
                    cout << "Path> Main menu/Login/Lecturer/Update personal information/\n\n";
                else
                    cout << "Path> Admin menu/User override/Lecturer/Update personal information/\n\n";
                cout << "Your current IC no: " << row[2];
                cout << "\nPlease update your IC no. or 'X' to go back:\n:";
                cin >> temp;
                if (temp == "x" || temp == "X")
                {
                    goto main1;
                }
                else
                {
                ic3:
                    cout << "\nPlease confirm to update (Y/N).\n:";
                    cin >> conf;
                    if (conf == 'Y' || conf == 'y')
                    {
                        cout << endl;
                        string update_query = "update lecturer set lect_ic = '" + temp + "' where lect_id = '" + temp_id + "'";
                        const char* qu = update_query.c_str();
                        qstate = mysql_query(conn, qu);
                    ic4:
                        if (!qstate)
                            cout << "Your IC no. has been updated to " << temp;
                        else
                            cout << "Failed To Update!" << mysql_errno(conn);
                        if (adm == 0)
                            cout << "\n\nPlease choose:\n\n1. Back\n2. Main menu\n3. Exit\n\n:";
                        else
                            cout << "\n\nPlease choose:\n\n1. Back\n2. Admin menu\n3. Exit\n\n:";
                        cin >> option;
                        system("cls");
                        switch (option)
                        {
                        case 1:
                            goto main1;
                            break;
                        case 2:
                            return 1;
                            break;
                        case 3:
                            return 2;
                            break;
                        default:
                            cout << invalid_selection;
                            goto ic4;
                            break;
                        }
                    }
                    else if (conf == 'N' || conf == 'n')
                    {
                        system("cls");
                        goto ic2;
                    }
                    else
                    {
                        cout << invalid_selection;
                        goto ic3;
                    }
                }
                break;
            case 3:
            fac2:
                if (adm == 0)
                    cout << "Path> Main menu/Login/Lecturer/Update personal information/\n\n";
                else
                    cout << "Path> Admin menu/User override/Lecturer/Update personal information/\n\n";
                cout << "Your current faculty: " << row[3];
                cout << "\nPlease update your faculty or 'X' to go back:\n:";
                cin >> temp;
                if (temp == "x" || temp == "X")
                {
                    system("cls");
                    goto main1;
                }
                else
                {
                fac3:
                    cout << "\nPlease confirm to update (Y/N).\n:";
                    cin >> conf;
                    if (conf == 'Y' || conf == 'y')
                    {
                        cout << endl;
                        string update_query = "update lecturer set lect_faculty = '" + temp + "' where lect_id = '" + temp_id + "'";
                        const char* qu = update_query.c_str();
                        qstate = mysql_query(conn, qu);
                    fac4:
                        if (!qstate)
                            cout << "Your faculty has been updated to " << temp;
                        else
                            cout << "Failed To Update!" << mysql_errno(conn);
                        if (adm == 0)
                            cout << "\n\nPlease choose:\n\n1. Back\n2. Main menu\n3. Exit\n\n:";
                        else
                            cout << "\n\nPlease choose:\n\n1. Back\n2. Admin menu\n3. Exit\n\n:";
                        cin >> option;
                        system("cls");
                        switch (option)
                        {
                        case 1:
                            goto main1;
                            break;
                        case 2:
                            return 1;
                            break;
                        case 3:
                            return 2;
                            break;
                        default:
                            cout << invalid_selection;
                            goto fac4;
                            break;
                        }
                    }
                    else if (conf == 'N' || conf == 'n')
                    {
                        system("cls");
                        goto fac2;
                    }
                    else
                    {
                        cout << invalid_selection;
                        goto fac3;
                    }
                }
                break;
            case 4:
            pn2:
                if (adm == 0)
                    cout << "Path> Main menu/Login/Lecturer/Update personal information/\n\n";
                else
                    cout << "Path> Admin menu/User override/Lecturer/Update personal information/\n\n";
                cout << "Your current phone number: " << row[4];
                cout << "\nPlease update your phone number or 'X' to go back:\n:";
                cin >> temp;
                if (temp == "x" || temp == "X")
                {
                    system("cls");
                    goto main1;
                }
                else
                {
                pn3:
                    cout << "\nPlease confirm to update (Y/N).\n:";
                    cin >> conf;
                    if (conf == 'Y' || conf == 'y')
                    {
                        cout << endl;
                        string update_query = "update lecturer set lect_phone = '" + temp + "' where lect_id = '" + temp_id + "'";
                        const char* qu = update_query.c_str();
                        qstate = mysql_query(conn, qu);
                    pn4:
                        if (!qstate)
                            cout << "Your phone number has been updated to " << temp;
                        else
                            cout << "Failed To Update!" << mysql_errno(conn);
                        if (adm == 0)
                            cout << "\n\nPlease choose:\n\n1. Back\n2. Main menu\n3. Exit\n\n:";
                        else
                            cout << "\n\nPlease choose:\n\n1. Back\n2. Admin menu\n3. Exit\n\n:";
                        cin >> option;
                        system("cls");
                        switch (option)
                        {
                        case 1:
                            goto main1;
                            break;
                        case 2:
                            return 1;
                            break;
                        case 3:
                            return 2;
                            break;
                        default:
                            cout << invalid_selection;
                            goto pn4;
                            break;
                        }
                    }
                    else if (conf == 'N' || conf == 'n')
                    {
                        system("cls");
                        goto pn2;
                    }
                    else
                    {
                        cout << invalid_selection;
                        goto pn3;
                    }
                }
                break;
            case 5:
            mail2:
                if (adm == 0)
                    cout << "Path> Main menu/Login/Lecturer/Update personal information/\n\n";
                else
                    cout << "Path> Admin menu/User override/Lecturer/Update personal information/\n\n";
                cout << "Your current email: " << row[5];
                cout << "\nPlease update your email or 'X' to go back:\n:";
                cin >> temp;
                if (temp == "x" || temp == "X")
                {
                    system("cls");
                    goto main1;
                }
                else
                {
                mail3:
                    cout << "\nPlease confirm to update (Y/N).\n:";
                    cin >> conf;
                    if (conf == 'Y' || conf == 'y')
                    {
                        cout << endl;
                        string update_query = "update lecturer set lect_email = '" + temp + "' where lect_id = '" + temp_id + "'";
                        const char* qu = update_query.c_str();
                        qstate = mysql_query(conn, qu);
                    mail4:
                        if (!qstate)
                            cout << "Your email has been updated to " << temp;
                        else
                            cout << "Failed To Update!" << mysql_errno(conn);
                        if (adm == 0)
                            cout << "\n\nPlease choose:\n\n1. Back\n2. Main menu\n3. Exit\n\n:";
                        else
                            cout << "\n\nPlease choose:\n\n1. Back\n2. Admin menu\n3. Exit\n\n:";
                        cin >> option;
                        system("cls");
                        switch (option)
                        {
                        case 1:
                            goto main1;
                            break;
                        case 2:
                            return 1;
                            break;
                        case 3:
                            return 2;
                            break;
                        default:
                            cout << invalid_selection;
                            goto mail4;
                            break;
                        }
                    }
                    else if (conf == 'N' || conf == 'n')
                    {
                        system("cls");
                        goto mail2;
                    }
                    else
                    {
                        cout << invalid_selection;
                        goto mail3;
                    }
                }
                break;
            case 6:
            pwd2:
                turn = 1;
                system("cls");
                if (adm == 0)
                    cout << "Path> Main menu/Login/Lecturer/Update personal information/\n\n";
                else
                    cout << "Path> Admin menu/User override/Lecturer/Update personal information/\n\n";
                cout << "Please enter your new password (only alphanumeric) or 'X' to go back:\n:";
            passwd:
                for (int i = 0;;)
                {
                    temp1 = _getch();
                    if ((temp1 >= 'a' && temp1 <= 'z') || (temp1 >= 'A' && temp1 <= 'Z') || (temp1 >= '0' && temp1 <= '9'))
                    {
                        pwd[i] = temp1;
                        ++i;
                        cout << "*";
                    }
                    if (temp1 == '\b' && i >= 1)
                    {
                        cout << "\b \b";
                        --i;
                    }
                    if (temp1 == '\r')
                    {
                        pwd[i] = '\0';
                        break;
                    }
                }
                if (turn == 1)
                    pwd1 = pwd;
                else
                {
                    pwd2 = pwd;
                    goto pwd5;
                }
                if (pwd1 == "x" || pwd1 == "X")
                {
                    system("cls");
                    goto main1;
                }
                else
                {
                    cout << "\n\n\nPlease re-enter your new password:\n:";
                    turn = 2;
                    goto passwd;
                pwd5:
                    if (pwd1.compare(pwd2) == 0)
                    {
                        cout << endl;
                        pwdEncryption(pwd, ids, 1, 2);
                    pwd4:
                        system("cls");
                        cout << "Your password has been successfully updated";
                        if (adm == 0)
                        {
                            cout << "\nPlease login again.\n";
                            system("pause");
                            return 4;
                        }
                        else
                            cout << "\nPlease choose:\n\n1. Back\n2. Admin menu\n3. Exit\n\n:";
                        cin >> option;
                        system("cls");
                        switch (option)
                        {
                        case 1:
                            goto main1;
                            break;
                        case 2:
                            return 1;
                            break;
                        case 3:
                            return 2;
                            break;
                        default:
                            cout << invalid_selection;
                            goto pwd4;
                            break;
                        }
                    }
                    else
                    {
                    pwdxmatch:
                        if (adm == 0)
                            cout << "\n\nPassword does not match. Please choose:\n\n1. Retry\n2. Back\n3. Main menu\n4. Exit\n\n:";
                        else
                            cout << "\n\nPassword does not match. Please choose:\n\n1. Retry\n2. Back\n3. Admin menu\n4. Exit\n\n:";
                        cin >> option;
                        system("cls");
                        switch (option)
                        {
                        case 1:
                            goto pwd2;
                            break;
                        case 2:
                            goto main1;
                            break;
                        case 3:
                            return 1;
                            break;
                        case 4:
                            return 2;
                            break;
                        default:
                            cout << invalid_selection;
                            goto pwdxmatch;
                            break;
                        }
                    }
                }
                break;
            case 7:
                system("cls");
                goto main;
                break;
            case 8:
                return 1;
                break;
            case 9:
                return 2;
                break;
            default:
                cout << invalid_selection;
                goto main1;
                break;
            }
        }
    }
        break;
    case 2:
        test=addData(ids, adms);
        system("cls");
        goto main;
        break;
    case 3:
    {
    case3lec:
        system("cls");
        string menupath;
        if (adm == 0)
            menupath="Path> Main menu/Login/Lecturer/Access individual report/\n\n";
        else
            menupath="Path> Admin menu/User override/Lecturer/Access individual report/\n\n";
        cout << menupath << "Enter week to generate report:\n:";
        cin >> test;
        string findbyid = "select current_week from additional_info";
        const char* qid = findbyid.c_str();
        qstate = mysql_query(conn, qid);
        res = mysql_store_result(conn);
        while ((row = mysql_fetch_row(res)))
        {
            option = stoi(row[0]);
        }
        if (test <= option)
        {
            cout << menupath << "Report for week " << test << ":\n\n";
            cout << "DAY/TIME|" << setw(8) << "10AM|" << setw(11) << "11AM|" << setw(11) << "12PM|" << setw(11) << "1PM|" << setw(11) << "4PM|" << setw(11) << "6PM\n" << "=========================================================================\n";
            string findbyid = "select data_day, data_10am, data_11am, data_12pm, data_1pm, data_4pm, data_6pm from data_table where data_week = '"+to_string(test)+"' and lect_id = '"+temp_id+"'";
            const char* qid = findbyid.c_str();
            qstate = mysql_query(conn, qid);
            res = mysql_store_result(conn);
            while ((row = mysql_fetch_row(res)))
            {
                cout << row[0] <<"     |"<< setw(7) << row[1] << "|" << setw(10) << row[2] << "|" << setw(10) << row[3] << "|" << setw(10) << row[4] << "|" << setw(10) << row[5] << "|" << setw(10) << row[6] << endl;
            }
            cout << "\n\nReport date/time: " << date_retrieval() << "\n\n";
        rpt1:
            cout<<"Please choose:\n\n1. Download report\n2. Back\n\n:";
            cin >> option;
            system("cls");
            switch (option)
            {
            case 1:
            {
                ifstream checkFile;
                checkFile.open("Individual_Weekly_Report.txt");
                if (!checkFile)
                {
                    ofstream outputFile("Individual_Weekly_Report.txt", ios::out);
                    outputFile << "Individual Weekly Report\nLecturer ID: "<<temp_id<<"\nWeek: "<<test << "\n\nDAY/TIME|" << setw(8) << "10AM|" << setw(11) << "11AM|" << setw(11) << "12PM|" << setw(11) << "1PM|" << setw(11) << "4PM|" << setw(11) << "6PM\n" << "=========================================================================\n";
                    string findbyid = "select data_day, data_10am, data_11am, data_12pm, data_1pm, data_4pm, data_6pm from data_table where data_week = '" + to_string(test) + "' and lect_id = '" + temp_id + "'";
                    const char* qid = findbyid.c_str();
                    qstate = mysql_query(conn, qid);
                    res = mysql_store_result(conn);
                    while ((row = mysql_fetch_row(res)))
                    {
                        outputFile << row[0] << "     |" << setw(7) << row[1] << "|" << setw(10) << row[2] << "|" << setw(10) << row[3] << "|" << setw(10) << row[4] << "|" << setw(10) << row[5] << "|" << setw(10) << row[6] << endl;
                    }
                    outputFile << "\n\nReport date/time: " << date_retrieval();
                    cout << "Report successfully downloaded. Please check your project directory.\n";
                    system("pause");
                    system("cls");
                    goto main;
                }
                else
                {
                    cout << "File named Individual_Weekly_Report.txt already exist in project directory. Please try to move or delete the file before downloading the report again.\n";
                    system("pause");
                    system("cls");
                    goto rpt1;
                }
            }
                break;
            case 2:
                goto main;
                break;
            default:
                cout << invalid_selection;
                system("pause");
                system("cls");
                goto rpt1;
                break;
            }
        }
        else
        {
            cout << "\nInvalid week. Please try again.\n";
            system("pause");
            goto case3lec;
        }
    }
        break;
    case 4:
        return 1;
        break;
    case 5:
        return 2;
        break;
    default:
        cout << invalid_selection;
        system("pause");
        system("cls");
        goto main;
        break;
    }
    return 1;
}

int addData(int id, int adm)
{
    int option, c_week=0, ids=id, data_num=0, type=0, err_check=0;
    char conf;
    string temp1, temp2, temp3, temp4, temp5, temp6;
    system("cls");
    if (adm == 0)
        cout << "Path> Main menu/Login/Lecturer/Add data to schedule/\n\n";
    else
        cout << "Path> Admin menu/User override/Lecturer/Add data to schedule/\n\n";
    string findweek = "select current_week from additional_info";
    const char* qweek = findweek.c_str();
    qstate = mysql_query(conn, qweek);
    res = mysql_store_result(conn);
    while ((row = mysql_fetch_row(res)))
    {
        c_week = stoi(row[0]);
    }
    cout << "Current week: " << c_week;
    string findbyid = "select count(data_number) from data_table where lect_id = '" + to_string(ids) + "' and data_week = '"+to_string(c_week)+"'";
    const char* qid = findbyid.c_str();
    qstate = mysql_query(conn, qid);
    res = mysql_store_result(conn);
    while ((row = mysql_fetch_row(res)))
    {
        data_num = stoi(row[0]);
    }
    if (data_num == 5)
    {
    exist:
        cout << "\nThe data already exist for current week. Do you want to update new data? (Y/N): ";
        cin >> conf;
        if (conf == 'y' || conf == 'Y')
        {
            type = 1;
            string delete_query = "delete from data_table where lect_id = '" + to_string(ids) + "' and data_week = '" + to_string(c_week) + "'";
            const char* qd = delete_query.c_str();
            qstate = mysql_query(conn, qd);
            data_num = 0;
        }
        else if (conf == 'n' || conf == 'N')
        {
            system("cls");
            return 3;
        }
        else
        {
            cout << invalid_selection;
            system("pause");
            system("cls");
            goto exist;
        }
    }
    else if (data_num > 0 && data_num < 5)
    {
    incomp:
        cout << "\nThe data is incomplete for current week. Please choose:\n\n1. Continue updating\n2. Add new data\n3. Back\n\n: ";
        cin >> option;
        switch (option)
        {
        case 1:
            break;
        case 2:
        {
            string delete_query = "delete from data_table where lect_id = '" + to_string(ids) + "' and data_week = '" + to_string(c_week) + "'";
            const char* qd = delete_query.c_str();
            qstate = mysql_query(conn, qd);
            data_num = 0;
        }
            break;
        case 3:
            system("cls");
            return 3;
            break;
        default:
            cout << invalid_selection;
            system("pause");
            system("cls");
            goto incomp;
            break;
        }
    }
updt:
    system("cls");
    if (adm == 0)
        cout << "Path> Main menu/Login/Lecturer/Add data to schedule/\n\n";
    else
        cout << "Path> Admin menu/User override/Lecturer/Add data to schedule/\n\n";
    cout << "\nPlease choose how you want to update the data?\n\n1. Manual\n2. Using file\n3. Back\n\n:";
    cin >> option;
    switch (option)
    {
    case 1:
    {
        system("cls");
        if (adm == 0)
            cout << "Path> Main menu/Login/Lecturer/Add data to schedule/Manual/\n\n";
        else
            cout << "Path> Admin menu/User override/Lecturer/Add data to schedule/Manual/\n\n";
        if (data_num < 1)
        {
            //mon
            cout << "Enter number of students having last class on MONDAY at:\n11am: ";
            cin >> temp1;
            cout << "1pm: ";
            cin >> temp2;
            cout << "4pm: ";
            cin >> temp3;
            cout << "6pm: ";
            cin >> temp4;
            string insert_query = "insert into data_table (data_week, data_day, lect_id, data_10am, data_11am, data_12pm, data_1pm, data_4pm, data_6pm) values ('" + to_string(c_week) + "','MON','" + to_string(ids) + "','0','" + temp1 + "','0','" + temp2 + "','" + temp3 + "','" + temp4 + "')";
            const char* q = insert_query.c_str();
            qstate = mysql_query(conn, q);
            if (qstate)
                err_check = 1;
        }
        if (data_num < 2)
        {
            //tue
            cout << "Enter number of students having last class on TUESDAY at:\n11am: ";
            cin >> temp1;
            cout << "1pm: ";
            cin >> temp2;
            cout << "4pm: ";
            cin >> temp3;
            cout << "6pm: ";
            cin >> temp4;
            string insert_query = "insert into data_table (data_week, data_day, lect_id, data_10am, data_11am, data_12pm, data_1pm, data_4pm, data_6pm) values ('" + to_string(c_week) + "','TUE','" + to_string(ids) + "','0','" + temp1 + "','0','" + temp2 + "','" + temp3 + "','" + temp4 + "')";
            const char* q = insert_query.c_str();
            qstate = mysql_query(conn, q);
            if (qstate)
                err_check = 1;

        }
        if (data_num < 3)
        {
            //wed
            cout << "Enter number of students having last class on WEDNESDAY at:\n11am: ";
            cin >> temp1;
            cout << "1pm: ";
            cin >> temp2;
            cout << "4pm: ";
            cin >> temp3;
            cout << "6pm: ";
            cin >> temp4;
            string insert_query = "insert into data_table (data_week, data_day, lect_id, data_10am, data_11am, data_12pm, data_1pm, data_4pm, data_6pm) values ('" + to_string(c_week) + "','WED','" + to_string(ids) + "','0','" + temp1 + "','0','" + temp2 + "','" + temp3 + "','" + temp4 + "')";
            const char* q = insert_query.c_str();
            qstate = mysql_query(conn, q);
            if (qstate)
                err_check = 1;
        }
        if (data_num < 4)
        {
            //thu
            cout << "Enter number of students having last class on THURSDAY at:\n11am: ";
            cin >> temp1;
            cout << "1pm: ";
            cin >> temp2;
            cout << "4pm: ";
            cin >> temp3;
            cout << "6pm: ";
            cin >> temp4;
            string insert_query = "insert into data_table (data_week, data_day, lect_id, data_10am, data_11am, data_12pm, data_1pm, data_4pm, data_6pm) values ('" + to_string(c_week) + "','THU','" + to_string(ids) + "','0','" + temp1 + "','0','" + temp2 + "','" + temp3 + "','" + temp4 + "')";
            const char* q = insert_query.c_str();
            qstate = mysql_query(conn, q);
            if (qstate)
                err_check = 1;
        }
        if (data_num < 5)
        {
            //fri
            cout << "Enter number of students having last class on FRIDAY at:\n10am: ";
            cin >> temp1;
            cout << "12pm: ";
            cin >> temp2;
            string insert_query = "insert into data_table (data_week, data_day, lect_id, data_10am, data_11am, data_12pm, data_1pm, data_4pm, data_6pm) values ('" + to_string(c_week) + "','FRI','" + to_string(ids) + "','" + temp1 + "','0','" + temp2 + "','0','0','0')";
            const char* q = insert_query.c_str();
            qstate = mysql_query(conn, q);
            if (qstate)
                err_check = 1;
        }
        system("cls");
        if (adm == 0)
            cout << "Path> Main menu/Login/Lecturer/Add data to schedule/Manual/\n\n";
        else
            cout << "Path> Admin menu/User override/Lecturer/Add data to schedule/Manual/\n\n";
        if (err_check == 0)
            cout << "The data has been successfully updated.\n";
        else
            cout<< "Query Execution Problem!" << mysql_errno(conn) << endl;
        cout << "Returning to lecturer menu..\n";
        system("pause");
        return 3;
    }
        break;
    case 2:
    {
        system("cls");
        if (adm == 0)
            cout << "Path> Main menu/Login/Lecturer/Add data to schedule/Manual/\n\n";
        else
            cout << "Path> Admin menu/User override/Lecturer/Add data to schedule/Manual/\n\n";
        ifstream checkFile;
        checkFile.open("BusScheduleData.txt");
        if (!checkFile)
        {
            ofstream myFile("BusScheduleData.txt");
            myFile << "MON_10AM MON_11AM MON_12PM MON_1PM MON_4PM MON_6PM\nTUE_10AM TUE_11AM TUE_12PM TUE_1PM TUE_4PM TUE_6PM\nWED_10AM WED_11AM WED_12PM WED_1PM WED_4PM WED_6PM\nTHU_10AM THU_11AM THU_12PM THU_1PM THU_4PM THU_6PM\nFRI_10AM FRI_11AM FRI_12PM FRI_1PM FRI_4PM FRI_6PM\n";
            myFile.close();
            cout << "A new file named BusScheduleData.txt has been created. ";
        }
        cout << "Please update your data inside the BusScheduleData.txt file located inside project directory.\n\n*Note: For new users the column represents day (MON,TUE,WED,THU,FRI) and row represents time (10AM,11AM,12PM,1PM,4PM,6PM)";
        cout << "\n\nOnce data is updated and saved, ";
        system("pause");
        ifstream inputFile("BusScheduleData.txt", ios::in);
        int count = 1;
        while (inputFile >> temp1 >> temp2 >> temp3 >> temp4 >> temp5 >> temp6)
        {
            if (count == 1 && data_num == 0)
            {
                string insert_query = "insert into data_table (data_week, data_day, lect_id, data_10am, data_11am, data_12pm, data_1pm, data_4pm, data_6pm) values ('" + to_string(c_week) + "','MON','" + to_string(ids) + "','0','" + temp2 + "','0','" + temp4 + "','" + temp5 + "','" + temp6 + "')";
                const char* q = insert_query.c_str();
                qstate = mysql_query(conn, q);
            }
            else if (count == 2 && data_num <=1)
            {
                string insert_query = "insert into data_table (data_week, data_day, lect_id, data_10am, data_11am, data_12pm, data_1pm, data_4pm, data_6pm) values ('" + to_string(c_week) + "','TUE','" + to_string(ids) + "','0','" + temp2 + "','0','" + temp4 + "','" + temp5 + "','" + temp6 + "')";
                const char* q = insert_query.c_str();
                qstate = mysql_query(conn, q);
            }
            else if (count == 3 && data_num <= 2)
            {
                string insert_query = "insert into data_table (data_week, data_day, lect_id, data_10am, data_11am, data_12pm, data_1pm, data_4pm, data_6pm) values ('" + to_string(c_week) + "','WED','" + to_string(ids) + "','0','" + temp2 + "','0','" + temp4 + "','" + temp5 + "','" + temp6 + "')";
                const char* q = insert_query.c_str();
                qstate = mysql_query(conn, q);
            }
            else if (count == 4 && data_num <= 3)
            {
                string insert_query = "insert into data_table (data_week, data_day, lect_id, data_10am, data_11am, data_12pm, data_1pm, data_4pm, data_6pm) values ('" + to_string(c_week) + "','THU','" + to_string(ids) + "','0','" + temp2 + "','0','" + temp4 + "','" + temp5 + "','" + temp6 + "')";
                const char* q = insert_query.c_str();
                qstate = mysql_query(conn, q);
            }
            else if (count == 5 && data_num <= 4)
            {
                string insert_query = "insert into data_table (data_week, data_day, lect_id, data_10am, data_11am, data_12pm, data_1pm, data_4pm, data_6pm) values ('" + to_string(c_week) + "','FRI','" + to_string(ids) + "','" + temp1 + "','0','" + temp3 + "','0','0','0')";
                const char* q = insert_query.c_str();
                qstate = mysql_query(conn, q);
            }
            if (qstate)
                err_check = 1;
            count++;
        }
        if (err_check == 0)
            cout << "\n\nThe data has been successfully updated.\n";
        else
            cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
        cout << "Returning to lecturer menu..\n";
        system("pause");
        return 3;
    }
        break;
    case 3:
        return 3;
        break;
    default:
        cout << invalid_selection;
        system("pause");
        goto updt;
        break;
    }
    
}

int driver(int id, int adm)
{
    int option, turn, ids=id, test;
    char conf, pwd[20], temp1;
    string temp, pwd1, pwd2, temp_id=to_string(id);
    system("cls");
    log_check(ids, 2);
main:
    if (adm == 0)
    {
        cout << "Path> Main menu/Login/Bus driver/\n\n";
        cout << "Please choose:\n\n1. Update personal information\n2. Access report\n3. Main menu\n4. Exit\n\n:";
    }
    else
    {
        cout << "Path> Admin menu/User override/Bus driver/\n\n";
        cout << "Please choose:\n\n1. Update personal information\n2. Access report\n3. Admin menu\n4. Exit\n\n:";
    }
    cin >> option;
    switch (option)
    {
    case 1:
    {
    main1:
        system("cls");
        string findbyid = "select * from busdriver where driver_id = '" + temp_id + "'";
        const char* qid = findbyid.c_str();
        qstate = mysql_query(conn, qid);
        res = mysql_store_result(conn);
        while ((row = mysql_fetch_row(res)))
        {
            if (adm == 0)
            {
                cout << "Path> Main menu/Login/Bus driver/Update personal information/\n\n";
                cout << "Please choose attributes to update:\n\n1. Name\n2. Ic no.\n3. Phone number\n4. Email\n5. Password\n6. Back\n7. Main menu\n8. Exit\n\n:";
            }
            else
            {
                cout << "Path> Admin menu/User override/Bus driver/Update personal information/\n\n";
                cout << "Please choose attributes to update:\n\n1. Name\n2. Ic no.\n3. Phone number\n4. Email\n5. Password\n6. Back\n7. Admin menu\n8. Exit\n\n:";
            }
            cin >> option;
            system("cls");
            switch (option)
            {
            case 1:
            name2:
                if (adm == 0)
                    cout << "Path> Main menu/Login/Bus driver/Update personal information/\n\n";
                else
                    cout << "Path> Admin menu/User override/Bus driver/Update personal information/\n\n";
                cout << "Current name: " << row[1] << "\nPlease enter your name or 'X' to go back:\n:";
                cin >> temp;
                if (temp == "x" || temp == "X")
                {
                    system("cls");
                    goto main1;
                }
                else
                {
                name3:
                    cout << "\nPlease confirm to update (Y/N).\n:";
                    cin >> conf;
                    if (conf == 'Y' || conf == 'y')
                    {
                        cout << endl;
                        string update_query = "update busdriver set driver_name = '" + temp + "' where driver_id = '" + temp_id + "'";
                        const char* qu = update_query.c_str();
                        qstate = mysql_query(conn, qu);
                    name4:
                        if (!qstate)
                            cout << "Your name has been updated to " << temp;
                        else
                            cout << "Failed To Update!" << mysql_errno(conn);
                        if (adm == 0)
                            cout << "\n\nPlease choose:\n\n1. Back\n2. Main menu\n3. Exit\n\n:";
                        else
                            cout << "\n\nPlease choose:\n\n1. Back\n2. Admin menu\n3. Exit\n\n:";
                        cin >> option;
                        system("cls");
                        switch (option)
                        {
                        case 1:
                            goto main1;
                            break;
                        case 2:
                            return 1;
                            break;
                        case 3:
                            return 2;
                            break;
                        default:
                            cout << invalid_selection;
                            goto name4;
                            break;
                        }
                    }
                    else if (conf == 'N' || conf == 'n')
                    {
                        system("cls");
                        goto name2;
                    }
                    else
                    {
                        cout << invalid_selection;
                        goto name3;
                    }
                }
                break;
            case 2:
            ic2:
                if (adm == 0)
                    cout << "Path> Main menu/Login/Bus driver/Update personal information/\n\n";
                else
                    cout << "Path> Admin menu/User override/Bus driver/Update personal information/\n\n";
                cout << "Current IC no: " << row[2] << "\nPlease enter your IC no. or 'X' to go back:\n:";
                cin >> temp;
                if (temp == "x" || temp == "X")
                {
                    system("cls");
                    goto main1;
                }
                else
                {
                ic3:
                    cout << "\nPlease confirm to update (Y/N).\n:";
                    cin >> conf;
                    if (conf == 'Y' || conf == 'y')
                    {
                        cout << endl;
                        string update_query = "update busdriver set driver_ic = '" + temp + "' where driver_id = '" + temp_id + "'";
                        const char* qu = update_query.c_str();
                        qstate = mysql_query(conn, qu);
                    ic4:
                        if (!qstate)
                            cout << "Your IC no. has been updated to " << temp;
                        else
                            cout << "Failed To Update!" << mysql_errno(conn);
                        if (adm == 0)
                            cout << "\n\nPlease choose:\n\n1. Back\n2. Main menu\n3. Exit\n\n:";
                        else
                            cout << "\n\nPlease choose:\n\n1. Back\n2. Admin menu\n3. Exit\n\n:";
                        cin >> option;
                        system("cls");
                        switch (option)
                        {
                        case 1:
                            goto main1;
                            break;
                        case 2:
                            return 1;
                            break;
                        case 3:
                            return 2;
                            break;
                        default:
                            cout << invalid_selection;
                            goto ic4;
                            break;
                        }
                    }
                    else if (conf == 'N' || conf == 'n')
                    {
                        system("cls");
                        goto ic2;
                    }
                    else
                    {
                        cout << invalid_selection;
                        goto ic3;
                    }
                }
                break;
            case 3:
            pn2:
                if (adm == 0)
                    cout << "Path> Main menu/Login/Bus driver/Update personal information/\n\n";
                else
                    cout << "Path> Admin menu/User override/Bus driver/Update personal information/\n\n";
                cout << "Current phone number: " << row[3] << "\nPlease enter your phone number or 'X' to go back:\n:";
                cin >> temp;
                if (temp == "x" || temp == "X")
                {
                    system("cls");
                    goto main1;
                }
                else
                {
                pn3:
                    cout << "\nPlease confirm to update (Y/N).\n:";
                    cin >> conf;
                    if (conf == 'Y' || conf == 'y')
                    {
                        cout << endl;
                        string update_query = "update busdriver set driver_phone = '" + temp + "' where driver_id = '" + temp_id + "'";
                        const char* qu = update_query.c_str();
                        qstate = mysql_query(conn, qu);
                    pn4:
                        if (!qstate)
                            cout << "Your phone number has been updated to " << temp;
                        else
                            cout << "Failed To Update!" << mysql_errno(conn);
                        if (adm == 0)
                            cout << "\n\nPlease choose:\n\n1. Back\n2. Main menu\n3. Exit\n\n:";
                        else
                            cout << "\n\nPlease choose:\n\n1. Back\n2. Admin menu\n3. Exit\n\n:";
                        cin >> option;
                        system("cls");
                        switch (option)
                        {
                        case 1:
                            goto main1;
                            break;
                        case 2:
                            return 1;
                            break;
                        case 3:
                            return 2;
                            break;
                        default:
                            cout << invalid_selection;
                            goto pn4;
                            break;
                        }
                    }
                    else if (conf == 'N' || conf == 'n')
                    {
                        system("cls");
                        goto pn2;
                    }
                    else
                    {
                        cout << invalid_selection;
                        goto pn3;
                    }
                }
                break;
            case 4:
            mail2:
                if (adm == 0)
                    cout << "Path> Main menu/Login/Bus driver/Update personal information/\n\n";
                else
                    cout << "Path> Admin menu/User override/Bus driver/Update personal information/\n\n";
                cout << "Current email: " << row[4] << "\nPlease enter your email or 'X' to go back:\n:";
                cin >> temp;
                if (temp == "x" || temp == "X")
                {
                    system("cls");
                    goto main1;
                }
                else
                {
                mail3:
                    cout << "\nPlease confirm to update (Y/N).\n:";
                    cin >> conf;
                    if (conf == 'Y' || conf == 'y')
                    {
                        cout << endl;
                        string update_query = "update busdriver set driver_email = '" + temp + "' where driver_id = '" + temp_id + "'";
                        const char* qu = update_query.c_str();
                        qstate = mysql_query(conn, qu);
                    mail4:
                        if (!qstate)
                            cout << "Your email has been updated to " << temp;
                        else
                            cout << "Failed To Update!" << mysql_errno(conn);
                        if (adm == 0)
                            cout << "\n\nPlease choose:\n\n1. Back\n2. Main menu\n3. Exit\n\n:";
                        else
                            cout << "\n\nPlease choose:\n\n1. Back\n2. Admin menu\n3. Exit\n\n:";
                        cin >> option;
                        system("cls");
                        switch (option)
                        {
                        case 1:
                            goto main1;
                            break;
                        case 2:
                            return 1;
                            break;
                        case 3:
                            return 2;
                            break;
                        default:
                            cout << invalid_selection;
                            goto mail4;
                            break;
                        }
                    }
                    else if (conf == 'N' || conf == 'n')
                    {
                        system("cls");
                        goto mail2;
                    }
                    else
                    {
                        cout << invalid_selection;
                        goto mail3;
                    }
                }
                break;
            case 5:
            pwd2:
                turn = 1;
                system("cls");
                if (adm == 0)
                    cout << "Path> Main menu/Login/Bus driver/Update personal information/\n\n";
                else
                    cout << "Path> Admin menu/User override/Bus driver/Update personal information/\n\n";
                cout << "Please enter your new password (only alphanumeric) or 'X' to go back:\n:";
            passwd:
                for (int i = 0;;)
                {
                    temp1 = _getch();
                    if ((temp1 >= 'a' && temp1 <= 'z') || (temp1 >= 'A' && temp1 <= 'Z') || (temp1 >= '0' && temp1 <= '9'))
                    {
                        pwd[i] = temp1;
                        ++i;
                        cout << "*";
                    }
                    if (temp1 == '\b' && i >= 1)
                    {
                        cout << "\b \b";
                        --i;
                    }
                    if (temp1 == '\r')
                    {
                        pwd[i] = '\0';
                        break;
                    }
                }
                if (turn == 1)
                    pwd1 = pwd;
                else
                {
                    pwd2 = pwd;
                    goto pwd5;
                }
                if (pwd1 == "x" || pwd1 == "X")
                {
                    system("cls");
                    goto main1;
                }
                else
                {
                    cout << "\n\n\nPlease re-enter your new password:\n:";
                    turn = 2;
                    goto passwd;
                pwd5:
                    if (pwd1.compare(pwd2) == 0)
                    {
                        cout << endl;
                        pwdEncryption(pwd, ids, 2, 2);
                    pwd4:
                        system("cls");
                        cout << "Your password has been successfully updated";
                        if (adm == 0)
                        {
                            cout << "\nPlease login again.\n";
                            system("pause");
                            return 5;
                        }
                        else
                            cout << "\nPlease choose:\n\n1. Back\n2. Admin menu\n3. Exit\n\n:";
                        cin >> option;
                        system("cls");
                        switch (option)
                        {
                        case 1:
                            goto main1;
                            break;
                        case 2:
                            return 1;
                            break;
                        case 3:
                            return 2;
                            break;
                        default:
                            cout << invalid_selection;
                            goto pwd4;
                            break;
                        }
                    }
                    else
                    {
                    pwdxmatch:
                        if (adm == 0)
                            cout << "\n\nPassword does not match. Please choose:\n\n1. Retry\n2. Back\n3. Main menu\n4. Exit\n\n:";
                        else
                            cout << "\n\nPassword does not match. Please choose:\n\n1. Retry\n2. Back\n3. Admin menu\n4. Exit\n\n:";
                        cin >> option;
                        system("cls");
                        switch (option)
                        {
                        case 1:
                            goto pwd2;
                            break;
                        case 2:
                            goto main1;
                            break;
                        case 3:
                            return 1;
                            break;
                        case 4:
                            return 2;
                            break;
                        default:
                            cout << invalid_selection;
                            goto pwdxmatch;
                            break;
                        }
                    }
                }
                break;
            case 6:
                system("cls");
                goto main;
                break;
            case 7:
                return 1;
                break;
            case 8:
                return 2;
                break;
            default:
                cout << invalid_selection;
                goto main1;
                break;
            }
        }
    }
        break;
    case 2:
    rptopt:
        system("cls");
        if (adm == 0)
            cout << "Path> Main menu/Login/Bus driver/Access individual report/\n\n";
        else
            cout << "Path> Admin menu/User override/Bus driver/Access individual report/\n\n";
        cout << "Enter week to generate report:\n:";
        cin >> option;
        test = reportGen(2, option, ids);
        switch (test)
        {
        case 1:
            system("cls");
            goto main;
            break;
        case 2:
            goto rptopt;
            break;
        }
        break;
    case 3:
        return 1;
        break;
    case 4:
        return 2;
        break;
    default:
        cout << invalid_selection;
        goto main;
        system("pause");
        system("cls");
        break;
    }
    return 1;
}

int newaccount(int choice)
{
    int option, code, lectcode=0, drivercode=0;
    string temp1, temp2, temp3, temp4, temp5, temp6;
    string find = "select lect_code, driver_code from additional_info";
    const char* qr = find.c_str();
    qstate = mysql_query(conn, qr);
    res = mysql_store_result(conn);
    while ((row = mysql_fetch_row(res)))
    {
        lectcode = stoi(row[0]);
        drivercode = stoi(row[1]);
    }
    if (choice == 1)
    {
        a: system("cls");
        cout << "Path> Main menu/Create new account/Lecturer/\n\n";
        cout << "\nPlease enter the lecturer code to create a new lecturer account:\n:";
        cin >> code;
        if (code == lectcode)
        {
            cout << "\nCode matched. ";
            system("pause");
            system("cls");
            cout << "Path> Main menu/Create new account/Lecturer/\n\n";
            cout << "Plases enter your Lecturer ID:\n:";
        input1:
            cin >> temp1;
            string findbyid = "select lect_id from lecturer where lect_id = '"+temp1+"'";
            const char* qid = findbyid.c_str();
            qstate = mysql_query(conn, qid);
            res = mysql_store_result(conn);
            while ((row = mysql_fetch_row(res)))
            {
                system("cls");
                cout << "The lecturer ID already exist. Please enter a different Lecturer ID:\n:";
                goto input1;
            }
            cout << "\nPlease enter your name:\n:";
            cin >> temp2;
            cout << "\nPlease enter your IC num:\n:";
            cin >> temp3;
            cout << "\nPlease enter your faculty:\n:";
            cin >> temp4;
            std::transform(temp4.begin(), temp4.end(), temp4.begin(), ::toupper);
            cout << "\nPlease enter your phone number:\n:";
            cin >> temp5;
            cout << "\nPlease enter your email:\n:";
            cin >> temp6;
            string insert_query = "insert into lecturer (lect_id, lect_name, lect_ic, lect_faculty, lect_phone, lect_email) values ('" + temp1 + "','" + temp2 + "','" + temp3 + "','" + temp4 + "','" + temp5 + "','" + temp6 + "')";
            const char* q = insert_query.c_str();
            qstate = mysql_query(conn, q);
            pwdEncryption("abcd1234", stoi(temp1), 1, 1);
        }
        else
        {
            system("cls");
            b: cout << "Path> Main menu/Create new account/Lecturer/\n\n";
            cout << "The code does not match. Please choose:\n\n1. Retry\n2. Main menu\n3. Exit\n\n:";
            cin >> option;
            switch (option)
            {
            case 1:
                goto a;
                break;
            case 2:
                return 1;
                break;
            case 3:
                return 2;
                break;
            default:
                cout << invalid_selection;
                goto b;
                break;
            }
        }
    }
    else if (choice == 2)
    {
    c: system("cls");
        cout << "Path> Main menu/Create new account/Bus driver/\n\n";
        cout << "\nPlease enter the bus driver code to create a new bus driver account:\n:";
        cin >> code;
        if (code == drivercode)
        {
            cout << "\nCode matched. ";
            system("pause");
            system("cls");
            cout << "Path> Main menu/Create new account/Bus driver/\n\n";
            cout << "Plases enter your Driver ID:\n:";
        input2:
            cin >> temp1;
            string findbyid = "select driver_id from busdriver where driver_id = '" + temp1 + "'";
            const char* qid = findbyid.c_str();
            qstate = mysql_query(conn, qid);
            res = mysql_store_result(conn);
            while ((row = mysql_fetch_row(res)))
            {
                system("cls");
                cout << "The bus driver ID already exist. Please enter a different Bus driver ID:\n:";
                goto input2;
            }
            cout << "\nPlease enter your name:\n:";
            cin >> temp2;
            cout << "\nPleases enter your IC num:\n:";
            cin >> temp3;
            cout << "\nPlease enter your phone number:\n:";
            cin >> temp4;
            cout << "\nPlease enter your email:\n:";
            cin >> temp5;
            string insert_query = "insert into busdriver (driver_id, driver_name, driver_ic, driver_phone, driver_email) values ('" + temp1 + "','" + temp2 + "','" + temp3 + "','" + temp4 + "','" + temp5 + "')";
            const char* q = insert_query.c_str();
            qstate = mysql_query(conn, q);
            pwdEncryption("abcd1234", stoi(temp1), 2, 1);
        }
        else
        {
            system("cls");
        d: cout << "Path> Main menu/Create new account/Bus driver/\n\n";
            cout << "The code does not match. Please choose:\n\n1. Retry\n2. Main menu\n3. Exit\n\n:";
            cin >> option;
            switch (option)
            {
            case 1:
                goto c;
                break;
            case 2:
                return 1;
                break;
            case 3:
                return 2;
                break;
            default:
                cout << invalid_selection;
                goto d;
                break;
            }
        }
    }
    system("cls");
e:
    if (!qstate)
    {
        cout << "The account has been sucessfully created.\nThe default password is abcd1234. The password can be updated under update personal information option.\n\n";
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
    }
    cout << "1. Login to account\n2. Main menu\n3. Exit\n\n:";
    cin >> option;
    switch (option)
    {
    case 1:
        login(choice);
        break;
    case 2:
        return 1;
        break;
    case 3:
        return 2;
        break;
    default:
        cout << invalid_selection;
        goto e;
        break;
    }
    return 1;
}

int reportGen(int type, int week, int id)
{
    int c_week = 0, option, am10=0, am11=0, pm12=0, pm1=0, pm4=0, pm6=0, am10bus=0, am11bus=0, pm12bus=0, pm1bus=0, pm4bus=0, pm6bus=0, count=1 ;
    string findbyid = "select current_week from additional_info";
    const char* qid = findbyid.c_str();
    qstate = mysql_query(conn, qid);
    res = mysql_store_result(conn);
    while ((row = mysql_fetch_row(res)))
    {
        c_week = stoi(row[0]);
    }
    if (week <= c_week)
    {
        if (type == 1)
        {
            cout << "Total number of students for week " << week << ":\n\n";
            cout << "DAY/TIME|" << setw(8) << "10AM|" << setw(11) << "11AM|" << setw(11) << "12PM|" << setw(11) << "1PM|" << setw(11) << "4PM|" << setw(11) << "6PM\n" << "=========================================================================\n";
            string findbyid = "select data_day, sum(data_10am), sum(data_11am), sum(data_12pm), sum(data_1pm), sum(data_4pm), sum(data_6pm) from data_table where data_week = '"+to_string(week)+"' group by data_day order by data_number";
            const char* qid = findbyid.c_str();
            qstate = mysql_query(conn, qid);
            res = mysql_store_result(conn);
            while ((row = mysql_fetch_row(res)))
            {
                cout << row[0] << "     |" << setw(7) << row[1] << "|" << setw(10) << row[2] << "|" << setw(10) << row[3] << "|" << setw(10) << row[4] << "|" << setw(10) << row[5] << "|" << setw(10) << row[6] << endl;
            }
            cout << "\n\nReport date/time: " << date_retrieval() << "\n\n";
        rpt1:
            cout << "Please choose:\n\n1. Download report\n2. Back\n\n:";
            cin >> option;
            system("cls");
            switch (option)
            {
            case 1:
            {
                ifstream checkFile;
                checkFile.open("Admin_Weekly_Report.txt");
                if (!checkFile)
                {
                    ofstream outputFile("Admin_Weekly_Report.txt", ios::out);
                    outputFile << "Admin ID: " << id << "\Total number of students for Week " << week << ":\n\nDAY/TIME|" << setw(8) << "10AM|" << setw(11) << "11AM|" << setw(11) << "12PM|" << setw(11) << "1PM|" << setw(11) << "4PM|" << setw(11) << "6PM\n" << "=========================================================================\n";
                    string findbyid = "select data_day, sum(data_10am), sum(data_11am), sum(data_12pm), sum(data_1pm), sum(data_4pm), sum(data_6pm) from data_table where data_week = '" + to_string(week) + "' group by data_day order by data_number";
                    const char* qid = findbyid.c_str();
                    qstate = mysql_query(conn, qid);
                    res = mysql_store_result(conn);
                    while ((row = mysql_fetch_row(res)))
                    {
                        outputFile << row[0] << "     |" << setw(7) << row[1] << "|" << setw(10) << row[2] << "|" << setw(10) << row[3] << "|" << setw(10) << row[4] << "|" << setw(10) << row[5] << "|" << setw(10) << row[6] << endl;
                    }
                    outputFile << "\n\nReport date/time: " << date_retrieval();
                    cout << "Report successfully downloaded (Admin_Weekly_Report.txt). Please check your project directory.\n";
                    system("pause");
                    system("cls");
                    return 1;
                }
                else
                {
                    cout << "File named Admin_Weekly_Report.txt already exist in project directory. Please try to move or delete the file before downloading the report again.\n";
                    system("pause");
                    system("cls");
                    goto rpt1;
                }
            }
            break;
            case 2:
                return 1;
                break;
            default:
                cout << invalid_selection;
                system("pause");
                system("cls");
                goto rpt1;
                break;
            }
        }
        else if (type == 2 || type ==3)
        {
            if(type==2)
                cout << "Scheduled buses for week " << week << ":\n\n";
            else
                cout << "Scheduled buses for week " << week << ":\n\n";
            cout << "DAY/TIME|" << setw(8) << "10AM|" << setw(11) << "11AM|" << setw(11) << "12PM|" << setw(11) << "1PM|" << setw(11) << "4PM|" << setw(11) << "6PM\n" << "=========================================================================\n";
            string findbyid = "select data_day, sum(data_10am), sum(data_11am), sum(data_12pm), sum(data_1pm), sum(data_4pm), sum(data_6pm) from data_table where data_week = '" + to_string(week) + "' group by data_day order by data_number";
            const char* qid = findbyid.c_str();
            qstate = mysql_query(conn, qid);
            res = mysql_store_result(conn);
            while ((row = mysql_fetch_row(res)))
            {
                am10 = 0, am11 = 0, pm12 = 0, pm1 = 0, pm4 = 0, pm6 = 0, am10bus = 0, am11bus = 0, pm12bus = 0, pm1bus = 0, pm4bus = 0, pm6bus = 0;
                if (count != 5)
                {
                    am11 = stoi(row[2]);
                    pm1 = stoi(row[4]);
                    pm4 = stoi(row[5]);
                    pm6 = stoi(row[6]);
                    while (am11 > 40)
                    {
                        am11 -= 40;
                        am11bus++;
                    }
                    pm1 += am11;
                    while (pm1 > 40)
                    {
                        pm1 -= 40;
                        pm1bus++;
                    }
                    pm4 += pm1;
                    while (pm4 > 40)
                    {
                        pm4 -= 40;
                        pm4bus++;
                    }
                    pm6 += pm4;
                    while (pm6 > 0)
                    {
                        pm6 -= 40;
                        pm6bus++;
                    }
                    am10bus = 0;
                    pm12bus = 0;
                }
                else
                {
                    am10 = stoi(row[1]);
                    pm12 = stoi(row[3]);
                    while (am10 > 40)
                    {
                        am10 -= 40;
                        am10bus++;
                    }
                    pm12 += am10;
                    while (pm12 > 0)
                    {
                        pm12 -= 40;
                        pm12bus++;
                    }
                    am11bus = 0;
                    pm1bus = 0;
                    pm4bus = 0;
                    pm6bus = 0;
                }
                cout << row[0] << "     |" << setw(7) << am10bus << "|" << setw(10) << am11bus << "|" << setw(10) << pm12bus << "|" << setw(10) << pm1bus << "|" << setw(10) << pm4bus << "|" << setw(10) << pm6bus << endl;
                count++;
            }
            if(type==2)
                cout << "\n\nReport date/time: " << date_retrieval() << "\n\n";
            else
                cout << "\n\nSchedule retrieved date/time: " << date_retrieval() << "\n\n";
            
        rpt2:
            if(type==2)
                cout << "Please choose:\n\n1. Download report\n2. Back\n\n:";
            else
                cout << "Please choose:\n\n1. Download schedule\n2. Back\n\n:";
            cin >> option;
            system("cls");
            switch (option)
            {
            case 1:
            {
                ifstream checkFile;
                if(type==2)
                    checkFile.open("BusDriver_Weekly_Report.txt");
                else
                    checkFile.open("Bus_Schedule.txt");
                if (!checkFile)
                {
                    if (type == 2)
                    {
                        std::fstream bd;
                        bd.open("BusDriver_Weekly_Report.txt", fstream::app);
                        bd << "Bus Driver ID: " << id << "\nScheduled buses for week " << week << ":\n\nDAY/TIME|" << setw(8) << "10AM|" << setw(11) << "11AM|" << setw(11) << "12PM|" << setw(11) << "1PM|" << setw(11) << "4PM|" << setw(11) << "6PM\n" << "=========================================================================\n";
                    }
                    else
                    {
                        std::fstream sc;
                        sc.open("Bus_Schedule.txt", fstream::app);
                        sc << "Scheduled buses for Week " << week << ":\n\nDAY/TIME|" << setw(8) << "10AM|" << setw(11) << "11AM|" << setw(11) << "12PM|" << setw(11) << "1PM|" << setw(11) << "4PM|" << setw(11) << "6PM\n" << "=========================================================================\n";
                    }
                    string findbyid = "select data_day, sum(data_10am), sum(data_11am), sum(data_12pm), sum(data_1pm), sum(data_4pm), sum(data_6pm) from data_table where data_week = '" + to_string(week) + "' group by data_day order by data_number";
                    const char* qid = findbyid.c_str();
                    qstate = mysql_query(conn, qid);
                    res = mysql_store_result(conn);
                    while ((row = mysql_fetch_row(res)))
                    {
                        am10 = 0, am11 = 0, pm12 = 0, pm1 = 0, pm4 = 0, pm6 = 0, am10bus = 0, am11bus = 0, pm12bus = 0, pm1bus = 0, pm4bus = 0, pm6bus = 0;
                        if (count != 5)
                        {
                            am11 = stoi(row[2]);
                            pm1 = stoi(row[4]);
                            pm4 = stoi(row[5]);
                            pm6 = stoi(row[6]);
                            while (am11 > 40)
                            {
                                am11 -= 40;
                                am11bus++;
                            }
                            pm1 += am11;
                            while (pm1 > 40)
                            {
                                pm1 -= 40;
                                pm1bus++;
                            }
                            pm4 += pm1;
                            while (pm4 > 40)
                            {
                                pm4 -= 40;
                                pm4bus++;
                            }
                            pm6 += pm4;
                            while (pm6 > 0)
                            {
                                pm6 -= 40;
                                pm6bus++;
                            }
                            am10bus = 0;
                            pm12bus = 0;
                        }
                        else
                        {
                            am10 = stoi(row[1]);
                            pm12 = stoi(row[3]);
                            while (am10 > 40)
                            {
                                am10 -= 40;
                                am10bus++;
                            }
                            pm12 += am10;
                            while (pm12 > 0)
                            {
                                pm12 -= 40;
                                pm12bus++;
                            }
                            am11bus = 0;
                            pm1bus = 0;
                            pm4bus = 0;
                            pm6bus = 0;
                        }
                        if (type == 2)
                        {
                            std::fstream bd;
                            bd.open("BusDriver_Weekly_Report.txt", fstream::app);
                            bd << row[0] << "     |" << setw(7) << am10bus << "|" << setw(10) << am11bus << "|" << setw(10) << pm12bus << "|" << setw(10) << pm1bus << "|" << setw(10) << pm4bus << "|" << setw(10) << pm6bus << endl;
                        }
                        else
                        {
                            std::fstream sc;
                            sc.open("Bus_Schedule.txt", fstream::app);
                            sc << row[0] << "     |" << setw(7) << am10bus << "|" << setw(10) << am11bus << "|" << setw(10) << pm12bus << "|" << setw(10) << pm1bus << "|" << setw(10) << pm4bus << "|" << setw(10) << pm6bus << endl;
                        }
                        count++;
                    }
                    if (type == 2)
                    {
                        std::fstream bd;
                        bd.open("BusDriver_Weekly_Report.txt", fstream::app);
                        bd << "\n\nReport date/time: " << date_retrieval();
                        cout << "Report successfully downloaded (BusDriver_Weekly_Report.txt). Please check your project directory.\n";
                    }
                    else
                    {
                        std::fstream sc;
                        sc.open("Bus_Schedule.txt", fstream::app);
                        sc << "\n\nSchedule retrieved date/time: " << date_retrieval();
                        cout << "Report successfully downloaded (Bus_Schedule.txt). Please check your project directory.\n";
                    }
                    system("pause");
                    system("cls");
                    return 1;
                }
                else
                {
                    if(type==2)
                        cout << "File named BusDriver_Weekly_Report.txt already exist in project directory. Please try to move or delete the file before downloading the report again.\n";
                    else
                        cout << "File named Bus_Schedule.txt already exist in project directory. Please try to move or delete the file before downloading the report again.\n";
                    system("pause");
                    system("cls");
                    goto rpt2;
                }
            }
            break;
            case 2:
                return 1;
                break;
            default:
                cout << invalid_selection;
                system("pause");
                system("cls");
                goto rpt2;
                break;
            }
        }
    }
    else
    {
        cout << "\nInvalid week. Please try again.\n";
        system("pause");
        return 2;
    }
    return 2;
}

void menus()
{
main:
    int option, temp = 0,test=0;
    char exit_option;
    string pwd, adminpwd = "password", return_main = "Returning to main menu...\n",
        message = "BUS SCHEDULE MANAGEMENT SYSTEM\n\n\n";
    string findbyid = "select admin_code from additional_info";
    const char* qid = findbyid.c_str();
    qstate = mysql_query(conn, qid);
    res = mysql_store_result(conn);
    while ((row = mysql_fetch_row(res)))
    {
        test = stoi(row[0]);
    }
    system("cls");
    slow_print(message, 30);
    cout << "Main menu:\n\n";
main1:
    cout << "Please choose:\n\n1. Login\n2. Create new account\n3. Access bus schedule\n4. Exit\n\n:";
    cin >> option;
    if (option == test)
    {
        temp = admin();
        system("color 0A");
        if (temp == 1)
            goto main;
        else if (temp == 2)
            goto exit;
    }
    switch (option)
    {
    case 1:
        goto a;
        break;
    case 2:
        goto b;
        break;
    case 3:
    {
        system("cls");
        cout << "Path> Main menu/Access Bus Schedule/\n\n";
        string findweek = "select current_week from additional_info";
        const char* qweek = findweek.c_str();
        qstate = mysql_query(conn, qweek);
        res = mysql_store_result(conn);
        while ((row = mysql_fetch_row(res)))
        {
            test = stoi(row[0]);
        }
        test = reportGen(3, test - 1, 0);
        goto main;
    }
        break;
    case 4:
        goto exit;
        break;
    default:
        system("cls");
        cout << invalid_selection;
        system("pause");
        goto main1;
        break;
    }

a:
    system("cls");
    cout << "Path> Main menu/Login/\n\n";
    cout << "Please choose:\n\nLogin as:\n\n1. Lecturer\n2. Bus Driver\n3. Main menu\n4. Exit\n\n:";
    cin >> option;

    switch (option)
    {
    case 1:
        temp=login(1);
        break;
    case 2:
        temp=login(2);
        break;
    case 3:
        goto main;
        break;
    case 4:
        goto exit;
        break;
    default:
        cout <<"\n\n"<< invalid_selection;
        system("pause");
        goto a;
        break;
    }
    switch (temp)
    {
    case 1:
        goto main;
        break;
    case 2:
        goto exit;
        break;
    case 3:
        goto a;
        break;
    }

b:
    system("cls");
    cout << "Path> Main menu/Create new account/\n\n";
    cout << "Please choose:\n\nCreate account as:\n\n1. Lecturer\n2. Bus Driver\n3. Back\n4. Exit\n\n:";
    cin >> option;

    switch (option)
    {
    case 1:
        temp = newaccount(1);
        break;
    case 2:
        temp = newaccount(2);
        break;
    case 3:
        goto main;
        break;
    case 4:
        goto exit;
        break;
    default:
        cout << invalid_selection;
        goto b;
        break;
    }
    switch (temp)
    {
    case 1:
        goto main;
        break;
    case 2:
        goto exit;
        break;
    case 3:
        goto b;
        break;
    }


exit:
    system("cls");
    cout << "Program terminating. Are you sure? (Y/N): ";
    cin >> exit_option;
    if (exit_option == 'y' || exit_option == 'Y') 
    {
        exit(0);
    }
    else if (exit_option == 'n' || exit_option == 'N') 
    {
        system("cls");
        slow_print(return_main, 50);
        goto main;
    }
    else 
    {
        cout << "\nInvalid character. Try again.\n" << endl;
        system("pause");
        goto exit;
    }

}
