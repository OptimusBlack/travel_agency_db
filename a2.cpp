//Name: Lakhani Amsal Murad
//Uno.: 3035393729

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

struct promo
{
    string dest;
    string airline;
    string from;
    string to;
    int price;
    string ends;
    int quota;
    int searchmark;
    bool checked;
};

struct codes
{
    string code;
    string country;
};

struct bought
{
    string country;
    int amount;
    int num;
    bool checked;
};

int numlines (ifstream& file)
{
    string dummystring;
    int num=0;

    while (file.good())
    {
        getline(file, dummystring);
        num++;
    }

    num -= 2;

    file.clear();
    file.seekg(0, ios::beg);

    return num;
}

void readMainFile (ifstream& mainDBfile, promo mainDB[], int& nummain)
{
    string dummystring;
    char dummychar;
    getline (mainDBfile, dummystring);

    while (mainDBfile.good())
    {
        getline (mainDBfile, mainDB[nummain].dest, ',');
        getline (mainDBfile, mainDB[nummain].airline, ',');
        getline (mainDBfile, mainDB[nummain].from, ',');
        getline (mainDBfile, mainDB[nummain].to, ',');
        mainDBfile >> mainDB[nummain].price;
        mainDBfile >> dummychar;
        getline (mainDBfile, mainDB[nummain].ends, ',');
        mainDBfile >> mainDB[nummain].quota;
        getline (mainDBfile, dummystring);
        nummain++;
    }
}

void load_intermediates()
{
    ifstream intermediate_check;
    intermediate_check.open("intermediate.txt");
    if (intermediate_check.fail())
    {
        intermediate_check.close();
        ofstream intermediate_start;
        intermediate_start.open("intermediate.txt");
        intermediate_start << "(dest) (airline) (from) (to) (price) (ends) (quota)" << endl;
        intermediate_start.close();
    }

    else
    {
        intermediate_check.close();
    }

    ifstream bought_check;
    bought_check.open("bought.txt");

    if (bought_check.fail())
    {
        bought_check.close();
        ofstream bought_start;
        bought_start.open("bought.txt");
        bought_start << "(country)," << "(total amount) " << "(tickets bought)" << endl;
        bought_start.close();
    }

    else
    {
        bought_check.close();
    }
}

void active(ifstream& inter, promo mainDB[], promo activerecord[], int& numactive, int& nummain)
{
    int numinter=0;
    string dummystring;

    while (inter.good())
    {
        getline(inter, dummystring);
        numinter++;
    }

    numinter -= 2;
    promo temp[100];
    inter.clear();
    inter.seekg(0, ios::beg);

    getline(inter, dummystring);
    for (int i=0; i<numinter; i++)
    {
        inter >> temp[i].dest;
        inter >> temp[i].airline;
        inter >> temp[i].from;
        inter >> temp[i].to;
        inter >> temp[i].price;
        inter >> temp[i].ends;
        inter >> temp[i].quota;
        temp[i].checked = false;
    }

    promo counter;
    for (int i=0; i<nummain; i++)
    {
        counter = mainDB[i];
        for (int j=0; j<numinter; j++)
        {
            if ((mainDB[i].dest == temp[j].dest) && (mainDB[i].airline == temp[j].airline) && (mainDB[i].from == temp[j].from) && (mainDB[i].to == temp[j].to) && (mainDB[i].price == temp[j].price) && (mainDB[i].ends == temp[j].ends))
            {
                counter.quota = counter.quota + temp[j].quota;
                temp[j].checked = true;
            }
        }

        if (counter.quota > 0)
        {
            activerecord[numactive] = counter;
            numactive++;
        }
    }

    for (int i=0; i<numinter; i++)
    {
        if (!temp[i].checked)
        {
            counter = temp[i];
            for (int j=i+1; j<numinter; j++)
            {
                if ((temp[i].dest == temp[j].dest) && (temp[i].airline == temp[j].airline) && (temp[i].from == temp[j].from) && (temp[i].to == temp[j].to) && (temp[i].price == temp[j].price) && (temp[i].ends == temp[j].ends))
                {
                    counter.quota = counter.quota + temp[j].quota;
                    temp[j].checked = true;
                }
            }
            if (counter.quota > 0)
            {
                activerecord[numactive] = counter;
                numactive++;
            }
        }
    }
}

bool enoughTickets (promo rec[], int numrec, promo ticket, int numtickets)
{
    for (int i=0; i<numrec; i++)
    {
        if ((rec[i].dest == ticket.dest) && (rec[i].airline == ticket.airline) && (rec[i].from == ticket.from) && (rec[i].to == ticket.to) && (rec[i].price == ticket.price))
        {
            if (rec[i].quota >= numtickets)
            {
                return true;
            }
        }
    }

    return false;
}

int indexPromo (promo rec[], int numrec, promo ticket)
{
    for (int i=0; i<numrec; i++)
    {
        if ((rec[i].dest == ticket.dest) && (rec[i].airline == ticket.airline) && (rec[i].from == ticket.from) && (rec[i].to == ticket.to) && (rec[i].price == ticket.price))
        {
            return i;
        }
    }
}

string codeToCountry (codes airport[], int numairport, string code)
{
    for (int i=0; i<numairport; i++)
    {
        if (airport[i].code == code)
        {
            return airport[i].country;
        }
    }
}

void displaySearch (promo searcharr[], int i)
{
    cout << "  <" << searcharr[i].searchmark << "> ";
    cout << searcharr[i].dest << "-" << searcharr[i].airline << "-" << searcharr[i].from << "-" << searcharr[i].to;
    cout << " HKD " << searcharr[i].price;
    cout << ", promotion expires " << searcharr[i].ends;
    cout << ", " << searcharr[i].quota << " quota" << endl;
}

string monthWordToNum (string word)
{
    string num;
    string mmWords[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    string mmNum[12] = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12"};

    for (int i=0; i<12; i++)
    {
        if (mmWords[i] == word)
        {
            return mmNum[i];
        }
    }
}

int main()
{
    promo mainDB[500], searcharr[500];
    codes airport[200];
    int nummain=0, numairport=0, numsearch=0;
    ifstream mainDBfile, airportCodeFile;

    for (int i=0; i<500; i++)
    {
        searcharr[i].searchmark = 0;
    }

    load_intermediates();

    airportCodeFile.open("airportcode.txt");
    while (airportCodeFile.good())
    {
        getline (airportCodeFile, airport[numairport].code, ',');
        getline (airportCodeFile, airport[numairport].country);
        numairport++;
    }
    airportCodeFile.close();

    mainDBfile.open("mainDB_big.csv");
    readMainFile (mainDBfile, mainDB, nummain);
    mainDBfile.close();

    string input;
    cin >> input;

    while (input != "end")
    {
        int numactive=0;
        promo activerec[500];
        ifstream readInter;
        readInter.open("intermediate.txt");
        active (readInter, mainDB, activerec, numactive, nummain);
        readInter.close();

        ofstream writeInter;
        writeInter.open("intermediate.txt", ios::app);

        ofstream writeBought;
        writeBought.open("bought.txt", ios::app);

        if (input == "all")
        {
            for (int i=0; i<numactive; i++)
            {
                cout << "[" << i+1 << "] ";
                cout << activerec[i].dest << "-" << activerec[i].airline << "-" << activerec[i].from << "-" << activerec[i].to;
                cout << " HKD " << activerec[i].price;
                cout << ", promotion expires " << activerec[i].ends;
                cout << ", " << activerec[i].quota << " quota" << endl;
            }
            cout << "total " << numactive << " records" << endl;
        }

        else if (input == "today")
        {
            string date;
            cin >> date;
            cout << "removing all records before " << date << endl;

            for (int i=0; i<numactive; i++)
            {
                if (activerec[i].ends < date)
                {
                    writeInter << activerec[i].dest << " " << activerec[i].airline << " " << activerec[i].from << " " << activerec[i].to << " " << activerec[i].price << " " << activerec[i].ends << " " << -activerec[i].quota << endl;
                }
            }
        }

        else if (input == "buy")
        {
            char dummy;
            cin >> dummy;

            promo ticket;
            int numtickets;

            if (dummy == '#')
            {
                int mark;
                cin >> mark;
                cin >> numtickets;

                if (numsearch == 0)
                {
                    cout << "wrong input";
                }

                else
                {
                    for (int i=0; i<numsearch; i++)
                    {
                        if (searcharr[i].searchmark == mark)
                        {
                            if (searcharr[i].quota >= numtickets)
                            {
                                ticket = searcharr[i];

                                writeInter << ticket.dest << " " << ticket.airline << " " << ticket.from << " " << ticket.to << " " << ticket.price << " " << ticket.ends << " " << -numtickets << endl;

                                string country;
                                country = codeToCountry(airport, numairport, ticket.dest);
                                int totalamount;
                                totalamount = ticket.price * numtickets * 0.9;
                                writeBought << country << "," << totalamount << " " << numtickets << endl;

                                cout << numtickets << " ticket(s) purchased" << endl;
                            }

                            else
                            {
                                cout << "not enough tickets";
                            }
                        }
                    }
                }
            }

            else
            {
                getline (cin, ticket.dest, '-');
                getline(cin, ticket.airline, '-');
                getline(cin, ticket.from, '-');
                getline(cin, ticket.to, '-');
                cin >> ticket.price;
                cin >> numtickets;

                ticket.dest = dummy + ticket.dest;

                if (enoughTickets(activerec, numactive, ticket, numtickets))
                {
                    int i;
                    i = indexPromo(activerec, numactive, ticket);
                    writeInter << activerec[i].dest << " " << activerec[i].airline << " " << activerec[i].from << " " << activerec[i].to << " " << activerec[i].price << " " << activerec[i].ends << " " << -numtickets << endl;

                    string country;
                    country = codeToCountry(airport, numairport, activerec[i].dest);
                    int totalamount;
                    totalamount = activerec[i].price * numtickets * 0.9;
                    writeBought << country << "," << totalamount << " " << numtickets << endl;

                    cout << numtickets << " ticket(s) purchased" << endl;
                }

                else
                {
                    cout << "not enough tickets" << endl;
                }
            }
        }

        else if (input == "most")
        {
            ifstream read_bought;
            int numbought;
            read_bought.open("bought.txt");
            numbought = numlines(read_bought);

            bought boughtarr[500];

            string dummystring;
            getline (read_bought, dummystring);
            for (int i=0; i<numbought; i++)
            {
                stringstream extract;
                getline(read_bought, boughtarr[i].country, ',');
                extract << boughtarr[i].country;
                extract >> boughtarr[i].country;
                read_bought >> boughtarr[i].amount;
                read_bought >> boughtarr[i].num;
                boughtarr[i].checked = false;
            }
            read_bought.close();

            int numcountry=0;
            bought country[500];

            for (int i=0; i<numbought; i++)
            {
                if (!boughtarr[i].checked)
                {
                    bought counter;
                    counter = boughtarr[i];

                    for (int j=i+1; j<numbought; j++)
                    {
                        if (boughtarr[i].country == boughtarr[j].country)
                        {
                            counter.amount += boughtarr[j].amount;
                            counter.num += boughtarr[j].num;
                            boughtarr[j].checked = true;
                        }
                    }

                    country[numcountry] = counter;
                    numcountry++;
                }
            }

            bought most;
            most.num=0;

            for (int i=0; i<numcountry; i++)
            {
                if (country[i].num > most.num)
                {
                    most = country[i];
                }
            }

            ofstream mostfile;
            mostfile.open("most.txt");

            stringstream extractcountry;

            extractcountry << most.country;
            extractcountry >> most.country;

            mostfile << most.country << endl;
            mostfile << "#" << most.num << endl;
            mostfile << "$" << most.amount << endl;

            mostfile.close();
        }

        else if (input == "search")
        {
            string line, searchtype, info;
            stringstream extract;

            getline(cin, line);

            extract << line;
            extract >> searchtype;

            if (numsearch != 0)
            {
                cout << "...please clear before searching" << endl;
            }

            else
            {
                if (searchtype == "dest")
                {
                    extract >> info;

                    for (int i=0; i<numactive; i++)
                    {
                        if (activerec[i].dest == info)
                        {
                            searcharr[numsearch] = activerec[i];
                            searcharr[numsearch].searchmark = numsearch + 1;
                            displaySearch(searcharr, numsearch);
                            numsearch++;
                        }
                    }
                }

                else if (searchtype == "date")
                {
                    extract >> info;

                    for (int i=0; i<numactive; i++)
                    {
                        if ((activerec[i].from <= info) && (activerec[i].to >= info))
                        {
                            searcharr[numsearch] = activerec[i];
                            searcharr[numsearch].searchmark = numsearch + 1;
                            displaySearch(searcharr, numsearch);
                            numsearch++;
                        }
                    }
                }
            }
        }

        else if (input == "prefer")
        {
            if (numsearch == 0)
            {
                cout << "perform search before this operation";
            }

            else
            {
                string dummy, pref;
                cin >> dummy;
                cin >> pref;

                int counter=1;

                for (int i=0; i<numsearch; i++)
                {
                    searcharr[i].searchmark = 0;

                    if (searcharr[i].airline == pref)
                    {
                        searcharr[i].searchmark = counter;
                        displaySearch(searcharr, i);
                        counter++;
                    }
                }
            }
        }

        else if (input == "no")
        {
            if (numsearch == 0)
            {
                cout << "perform search before this operation";
            }

            else
            {
                string dummy, npref;
                cin >> dummy;
                cin >> npref;

                int counter=1;

                for (int i=0; i<numsearch; i++)
                {
                    if (searcharr[i].searchmark >= 0)
                    searcharr[i].searchmark = 0;

                    if (searcharr[i].airline != npref && searcharr[i].searchmark >= 0)
                    {
                        searcharr[i].searchmark = counter;
                        displaySearch(searcharr, i);
                        counter++;
                    }
                    if (searcharr[i].airline == npref)
                        searcharr[i].searchmark = -1;
                }
            }
        }

        else if (input == "clear")
        {
            for (int i=0; i<numsearch; i++)
            {
                searcharr[i].searchmark = 0;
            }

            numsearch = 0;

            cout << "clearing all marked records" << endl;
        }

        else if (input == "add")
        {
            cout << "updating database" << endl;

            stringstream enddate;
            promo info;
            char dummychar;
            string dummystr, name, yy, mm, dd;
            cin >> dummystr;
            cin >> name;

            ifstream file;
            file.open(name.c_str());

            if (file.fail())
            {
                cout << "no such file exists" << endl;
            }

            else
            {
                getline (file, dummystr, ':');
                getline (file, info.airline);
                getline (file, dummystr, ':');
                file >> yy;
                file >> mm;
                file >> dd;
                enddate << setw(2) << setfill('0') << dd;
                enddate >> dd;
                info.ends = yy + monthWordToNum(mm) + dd;
                while (file.good())
                {
                    file >> dummychar;

                    if (dummychar == '#')
                    {
                        stringstream enddate2;
                        getline (file, dummystr, ':');
                        file >> yy;
                        file >> mm;
                        file >> dd;
                        enddate2 << setw(2) << setfill('0') << dd;
                        enddate2 >> dd;
                        info.ends = yy + monthWordToNum(mm) + dd;
                    }

                    else
                    {
                        stringstream extract, fromdate, todate;
                        getline (file, info.dest, ',');
                        info.dest = dummychar + info.dest;
                        file >> yy;
                        file >> mm;
                        file >> dd;
                        fromdate << setw(2) << setfill('0') << dd;
                        fromdate >> dd;
                        info.from = yy + monthWordToNum(mm) + dd;
                        file >> dummystr;
                        file >> yy;
                        file >> mm;
                        getline (file, dd, ',');
                        extract << dd;
                        extract >> dd;
                        todate << setw(2) << setfill('0') << dd;
                        todate >> dd;
                        info.to = yy + monthWordToNum(mm) + dd;
                        file >> dummystr;
                        file >> info.price;
                        file >> dummychar;
                        file >> dummystr;
                        file >> info.quota;

                        writeInter << info.dest << " " << info.airline << " " << info.from << " " << info.to << " " << info.price << " " << info.ends << " " << info.quota << endl;
                    }
                }
            }

            file.close();
        }

        writeInter.close();
        writeBought.close();

        cout << endl;
        cin >> input;
    }

    return 0;
}
