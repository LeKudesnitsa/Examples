//Это курсовая работа 
#include <iostream>
#include <conio.h>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <windows.h>
#include <iomanip>

using namespace std;

struct Student
{
    char FirstName[31], SecondName[31], SurName[31];
    int marks[5];
    float salary;
    bool contract;//true - contract
    Student *link = NULL;
};

struct Group
{
    int number, NoS = 0;
    Student *first = NULL;
    Group *link = NULL;
};

void AddGroupConsole (Group *&g, int key);
void AddGroupFile (Group *&g, int key);
int AddStudentConsole (Student *&s, int num);
int AddStudentFile (Student *&s, int num, ifstream &inp);
void OutGroupConsole (Group *&g, int key);
void OutGroupFile (Group *&g, int key, ofstream &outp);
void EditGroup (Group *&g, int key);
void SearchStudent (Group *&g, Student *&found);
void DeleteStudent (Group *&g, Student *&s);
void DeleteGroup (Group *&prev, Group *&g, int key);
void InTask1 (Group *&g, int key);
void GroupSort(Group *&g, int key);
void InTask2 (Group *&g);
void InTask2_run (Group *&g, Group *&h);

int main ()
{
	//setlocale(LC_ALL, "rus");
    SetConsoleCP(1251);// установка кодовой страницы win-cp 1251 в поток ввода
    SetConsoleOutputCP(1251); // установка кодовой страницы win-cp 1251 в поток вывода
    //требуется шрифт Lucida Console

    Group *Flow = 0, *curr;//Flow -- корень потока
    int k, key;

    while (true)
    {
        system("cls");
        curr = Flow;
        k = -1;

        cout << "Список групп в потоке: ";
        if (Flow == 0)
            cout << "в потоке нет групп!";
        else
            while (true)
            {
                cout << curr->number << " ";
                if (curr->link == 0)
                    break;
                curr = curr->link;
            }

        cout << "\n1 -- добавить группу"
            << "\n2 -- вывести список группы"
            << "\n3 -- редактировать группу"
            << "\n4 -- выполнить ИЗ1"
            << "\n5 -- удалить группу"
            << "\n6 -- выполнить ИЗ2 (для всего потока)"
            << "\n0 -- закончить работу"
            << "\n===============================================================================" << endl;
        while ((k < 48) || (k > 54))
            k = _getch();
        if (Flow != 0)
        {
            if ((k > 49) && (k < 54))
            {
                cout << "Введите номер группы: ";
                cin >> key;
            }
        }
        else
            if ((k != 49) && (k != 54))
            {
                cout << "В потоке нет групп!" << endl;
                system("pause");
                continue;
            }

        switch(k)
        {
            case 49://1
            {
                cout << "Добавить группу из файла? (Y/N)\n";
                while (true)
                {
                    k = _getch();
                    if ((k == 121) || (k == 89) || (k == 141) || (k == 173))
                        {AddGroupFile(Flow, 0); break;}
                    if ((k == 110) || (k == 78) || (k == 146) || (k == 226))
                    {
                        cout << "Введите номер группы: ";
                        cin >> key;
                        AddGroupConsole(Flow, key);
                        break;
                    }
                }
                break;
            }
            case 50://2
            {
                cout << "Вывести группу в файл? (Y/N)\n";
                while (true)
                {
                    k = _getch();
                    if ((k == 121) || (k == 89) || (k == 141) || (k == 173))
                    {
                        ofstream outp("output.txt");
                        OutGroupFile (Flow, key, outp);
                        break;
                    }
                    if ((k == 110) || (k == 78) || (k == 146) || (k == 226))
                        {OutGroupConsole(Flow, key); break;}
                }
                break;
            }
            case 51://3
            {
                EditGroup (Flow, key);
                system("pause");
                break;
            }
            case 52://4
            {
                InTask1(Flow, key);
                break;
            }
            case 53://5
            {
                curr = 0;
                DeleteGroup(curr, Flow, key);
                break;
            }
            case 54://6
            {
                InTask2 (Flow);
                break;
            }
            case 48://0
            {
                cout << "\nВы уверены? Y/N" << endl;
                while (true)
                {
                    k = _getch();
                    if ((k == 121) || (k == 89) || (k == 141) || (k == 173))
                        exit(0);
                    if ((k == 110) || (k == 78) || (k == 146) || (k == 226))
                        break;
                }
            }
        }
        cout << endl;
        system("pause");
    }
	return 0;
}

void AddGroupConsole (Group *&g, int key)
{
    if ((g != 0) && (g->number == key))
        cout << "Такая группа уже есть!" << endl;
    else
        if (g == 0)
        {
            g = new Group;
            g->first = 0;
            g->link = 0;
            g->number = key;

            int k;
            cout << "Добавить студентов в список группы? Y/N" << endl;
            while (true)
            {
                k = _getch();
                if ((k == 121) || (k == 89) || (k == 141) || (k == 173))
                {
                    g->NoS = AddStudentConsole(g->first, 0);
                    GroupSort(g, key);
                    OutGroupConsole(g, key);
                }
                if ((k == 110) || (k == 78) || (k == 146) || (k == 226))
                    break;
            }

        }
        else AddGroupConsole(g->link, key);
}

void AddGroupFile (Group *&g, int key)
{
    int k, flag;
    Student *s;

    ifstream inp("input.txt");

    if (!inp.is_open())
    {
        cout << "Файла с входящими данными не существует!"
             << "\nСоздайте файл input.txt в директории с данной программой."
             << "\nПервое чиcло должно быть целым и указывать номер группы."
             << "\nЗа ним следуют данные студентов в виде ФИО, 5 оценок, стипендия"
             << "\nи признак бюджет/контракт (0/1). Данные отделяются знаком «пробел». "
             << "\nВ конце списка не должно быть переводов строки и иных данных, иначе"
             << "\nпрограмма отработает некорректно."
             << "\nЕсли группа с таким номером существует, то список студентов будет дополнен." << endl
             << "\nСоздать файл input.txt в текущей директории? Y/N" << endl;
            while (true)
            {
                k = _getch();
                if ((k == 121) || (k == 89) || (k == 141) || (k == 173))
                {
                    inp.close();
                    ofstream inp("input.txt");
                    inp << "номер группы\nФамилия Имя Отчество оценка оценка оценка оценка оценка стипендия признак\nФамилия Имя Отчество оценка оценка оценка оценка оценка стипендия признак";
                    cout << "\nНе забудьте заполнить его данными." << endl;
                    break;
                }
                if ((k == 110) || (k == 78) || (k == 146) || (k == 226))
                    break;
            }
    }
    else
    {
        inp >> key;

        if ((g != 0) && (g->number == key))
        {
            cout << "Такая группа уже есть! Список студентов будет дополнен из файла.\nПродлжить? (Y/N)" << endl;
            while (true)
            {
                k = _getch();
                if ((k == 121) || (k == 89) || (k == 141) || (k == 173))
                    {
                        g->NoS = AddStudentFile(g->first, 0, inp);
                        GroupSort(g, key);
                        OutGroupConsole(g, key);
                        break;
                    }
                if ((k == 110) || (k == 78) || (k == 146) || (k == 226))
                    break;
            }
        }
        else
            if (g == 0)
            {
                g = new Group;
                s = g->first = 0;
                g->link = 0;
                g->number = key;

                g->NoS = AddStudentFile(g->first, 0, inp);
                GroupSort(g, key);
                OutGroupConsole(g, key);
            }
            else AddGroupFile(g->link, key);
    }
    inp.close();
}

int AddStudentConsole (Student *&s, int num)
{
    int k;
    if(s == 0)
    {
        s = new Student;
        s->link = 0;
        num += 1;

        cout << "Фамилия: ";
        cin >> s->SurName;
        cout << "Имя: ";
        cin >> s->FirstName;
        cout << "Отчество: ";
        cin >> s->SecondName;
        cout << "Оценки [5]: ";
        for (int i = 0; i < 5; i++)
            cin >> s->marks[i];
        cout << "Стипедия: ";
        cin >> s->salary;
        cout << "Контракт?(Y/N)\n";
        while (true)
        {
            k = _getch();
            if ((k == 121) || (k == 89) || (k == 141) || (k == 173))
                {s->contract = true; break;}
            if ((k == 110) || (k == 78) || (k == 146) || (k == 226))
                {s->contract = false; break;}
        }

        cout << "Добавить ещё студента в список группы? Y/N" << endl;
        while (true)
        {
            k = _getch();
            if ((k == 121) || (k == 89) || (k == 141) || (k == 173))
                num = AddStudentConsole(s->link, num);
            if ((k == 110) || (k == 78) || (k == 146) || (k == 226))
                break;
        }
    }
    else AddStudentConsole(s->link, num);

    return num;
}

int AddStudentFile (Student *&s, int num, ifstream &inp)
{
    int k, flag;
    if(s == 0)
    {
        s = new Student;
        s->link = 0;
        num += 1;

        inp >> s->SurName;
        inp >> s->FirstName;
        inp >> s->SecondName;
        for (int i = 0; i < 5; i++)
            inp >> s->marks[i];
        inp >> s->salary;
        inp >> flag;
        if ( flag == 0)
            s->contract = false;
        else
            s->contract = true;

        if (!inp.eof())
            num = AddStudentFile(s->link, num, inp);
    }
    else AddStudentFile(s->link, num, inp);

    return num;
}

void OutGroupConsole (Group *&g, int key)
{
    Student *curr;

    if (g->number == key)
    {
        curr = g->first;
        cout << "В группе №"<< g->number << " " << g->NoS << " студента/ов" << endl;
        while (true)
        {
            if (curr == 0)
                break;

            cout << curr->SurName << " " << curr->FirstName << " " << curr->SecondName;
            cout << ": оценки: ";
            for (int i = 0; i < 5; i++)
                cout << " " << curr->marks[i];
            cout << ", стипендия " << curr->salary;
            if (curr->contract == true)
                cout << ", контрактник;" << endl;
            else
                cout << ", бюджетник;" << endl;

            curr = curr->link;
        }
    }
    else
        if (g->link != 0)
            OutGroupConsole(g->link, key);
        else
            cout << "Нет такой группы!" << endl;
}

void OutGroupFile (Group *&g, int key, ofstream &outp)
{
    Student *curr;

    if (g->number == key)
    {
        curr = g->first;
        outp << key << endl;
        outp << "В группе " << g->NoS << "студента/ов" << endl;
        while (true)
        {
            if (curr == 0)
                break;

            outp << curr->SurName << " " << curr->FirstName << " " << curr->SecondName;
            for (int i = 0; i < 5; i++)
                outp << " " << curr->marks[i];
            outp << " " << curr->salary;
            if (curr->contract == true)
                outp << " 1" << endl;
            else
                outp << " 0" << endl;

            curr = curr->link;
        }
    }
    else
        if (g->link != 0)
            OutGroupFile(g->link, key, outp);
        else
            cout << "Нет такой группы!" << endl;
}

void EditGroup (Group *&g, int key)
{

    if (g->number == key)
    {
        cout << "1 -- изменить номер группы\n2 -- добавить студента в группу\n3 -- удалить студента\n0 -- ничего не делать" << endl;
        int k;
        while ((k < 48) || (k > 51))
            k = _getch();
        switch(k)
        {
            case 49://1
            {
                cout << "Введите новый номер группы:";
                cin >> g->number;
                break;
            }
            case 50://2
            {
                g->NoS = AddStudentConsole (g->first, 0);
                GroupSort(g, key);
                OutGroupConsole(g, key);
                break;
            }
            case 51://3
            {
                if (g->NoS > 0)
                {
                    Student *DelKey;
                    bool flag;
                    SearchStudent(g, DelKey);
                    if (DelKey)
                        DeleteStudent (g, DelKey);
                }
                else
                    cout << "В группе нет студентов!" << endl;
                break;
            }
            case 48://0
            {
                break;
            }
        }
    }
    else EditGroup(g->link, key);
}

void SearchStudent (Group *&g, Student *&found)
{
    Student *curr, *hcurr, *prev, SearchKey;
    Group *h;
    int i;

    h = new Group;
    h->number = g->number;
    curr = g->first;
    h->first = prev = hcurr = 0;

    cout << "Введите фамилию: ";
    cin >> SearchKey.SurName;
    while (true)
    {
        if (stricmp(curr->SurName, SearchKey.SurName) == 0)
        {
            hcurr = new Student;

            strcpy(hcurr->SurName, curr->SurName);
            strcpy(hcurr->FirstName, curr->FirstName);
            strcpy(hcurr->SecondName, curr->SecondName);
            hcurr->salary = curr->salary;
            hcurr->contract = curr->contract;
            for (i = 0; i < 5; i++)
                hcurr->marks[i] = curr->marks[i];
            h->NoS += 1;

            if (prev != 0)
            {
                prev->link = hcurr;
                prev = hcurr;
            }
            if (h->first == 0)
                prev = h->first = hcurr;
        }

        if(curr->link == 0)
            break;
        curr = curr->link;
    }

    if (h->NoS > 1)
    {
        curr = h->first;
        cout << "Введите имя: ";
        cin >> SearchKey.FirstName;

        while (true)
        {
            if (stricmp(curr->FirstName, SearchKey.FirstName) != 0)
                DeleteStudent(h, curr);
            if(curr->link == 0)
                break;
            curr = curr->link;
        }
    }

    if (h->NoS > 1)
    {
        curr = h->first;
        cout << "Введите отчество: ";
        cin >> SearchKey.SecondName;

        while (true)
        {
            if (stricmp(curr->SecondName, SearchKey.SecondName) != 0)
                DeleteStudent(h, curr);
            if(curr->link == 0)
                break;
            curr = curr->link;
        }
    }
    if (h->NoS > 1)
    {
        curr = h->first;
        cout << "Введите стипендию: ";
        cin >> SearchKey.salary;

        while (true)
        {
            if (curr->salary != SearchKey.salary)
                DeleteStudent(h, curr);
            if(curr->link == 0)
                break;
            curr = curr->link;
        }
    }
    if (h->NoS > 1)
    {
        curr = h->first;
        cout << "Введите признак: ";
        cin >> SearchKey.contract;

        while (true)
        {
            if (curr->contract != SearchKey.contract)
                DeleteStudent(h, curr);
            if(curr->link == 0)
                break;
            curr = curr->link;
        }
    }

    if (h->NoS == 1)
    {
        curr = g->first;
        while (true)
        {
            if (stricmp(h->first->SurName, curr->SurName) == 0)
                if(stricmp(h->first->FirstName, curr->FirstName) == 0)
                    if(stricmp(h->first->SecondName, curr->SecondName) == 0)
                        if(h->first->salary == curr->salary)
                            if(h->first->contract == curr->contract)
                                for (i = 0; i < 5; i++)
                                {
                                    if(h->first->marks[i] != curr->marks[i])
                                        break;
                                    if (i == 5)
                                        found = curr;
                                }
            if(curr->link == 0)
                break;
            curr = curr->link;
        }
    }
    else found = 0;
}

void DeleteStudent (Group *&g, Student *&s)
{
    Student *curr, *last, *prev;

    curr = last = prev = g->first;

    while (true)
    {
        if (curr->link == s)
            prev = curr;
        if (curr->link == 0)
        {
            last = curr;
            break;
        }
        curr = curr->link;
    }

    if (s == g->first)
        g->first = s->link;

    prev->link = s->link;

    if (s == last)
    {
        last = prev;
        last->link = 0;
    }

    g->NoS -= 1;
    delete s;
    s = prev;
}

void DeleteGroup (Group *&prev, Group *&g, int key)
{
    if (g->number == key)
    {
        Group *del;

        del = g;

        if (prev == 0)
            g = g->link;
        else
            prev->link = g->link;
        delete del;
    }
    else
        if (g->link == 0)
            cout << "Нет такой группы!" << endl;
        else
            DeleteGroup(g, g->link, key);
}

void InTask1 (Group *&g, int key)
{
    if (g->number == key)
    {
        Group *h;
        Student *curr, *hcurr, *prev;
        int i;
        float average = 0, c_average;
        bool sorted = false, swapped = false;

        h = new Group;
        h->number = g->number;
        curr = g->first;
        h->first = prev = hcurr = 0;

        while (true)
        {
            for (i = 0; i < 5; i++)
                average += curr->marks[i];
            if (curr->link == 0)
                break;
            curr = curr->link;
        }
        average /= (g->NoS*5);

        curr = g->first;
        while (true)
        {
            c_average = 0;
            for (i = 0; i < 5; i++)
                c_average += curr->marks[i];
            c_average /= 5;

            if ((curr->contract == 1) && (c_average < average))
            {
                hcurr = new Student;

                strcpy(hcurr->SurName, curr->SurName);
                strcpy(hcurr->FirstName, curr->FirstName);
                strcpy(hcurr->SecondName, curr->SecondName);
                hcurr->salary = curr->salary;
                hcurr->contract = curr->contract;
                for (i = 0; i < 5; i++)
                    hcurr->marks[i] = curr->marks[i];
                h->NoS += 1;

                if (prev != 0)
                {
                    prev->link = hcurr;
                    prev = hcurr;
                }
                if (h->first == 0)
                    prev = h->first = hcurr;
            }
            if (curr->link == 0)
                break;
            curr = curr->link;
        }
        if (h->NoS == 0)
            cout << "В группе нет людей, удовлетворяющим данным условиям!";
        else
            if (h->NoS > 0)
                OutGroupConsole(h, key);
    }
    else
        if (g->link == 0)
            cout << "Нет такой группы!" << endl;
        else
            InTask1(g->link, key);
}

void GroupSort(Group *&g, int key)
{
    if (g->number == key)
    {
        if (g->NoS <= 1)
            return;

        Student *curr, *prev = 0, *min_prev = 0, *min_last, *s_min;
        bool new_min = false, continue_check = false, first = true;
        int i;

        s_min = curr = min_last = g->first;
        new_min = true;
        while (new_min)
        {
            min_prev = min_last;
            if (!first)
                s_min = curr = min_last->link;
            new_min = false;

            while(true)
            {
                if (s_min->link == 0)
                    break;
                i = 0;
                continue_check = false;
                while (true)
                {
                    if ((s_min->SurName[i] == '\0') && (curr->SurName[i] == '\0'))
                        {
                            continue_check = true;
                            break;
                        }
                    if (curr->SurName[i] == '\0')
                    {
                        if (s_min->SurName[i] == '\0')
                            continue_check = true;
                        else
                        {
                            min_prev = prev;
                            s_min = curr;
                            new_min = true;
                        }
                        break;
                    }
                    if (curr->SurName[i] > s_min->SurName[i])
                        break;
                    if (curr->SurName[i] < s_min->SurName[i])
                    {
                        min_prev = prev;
                        s_min = curr;
                        new_min = true;
                        break;
                    }
                    if (curr->SurName[i] <= s_min->SurName[i])
                        i++;
                }

                i = 0;
                if (continue_check)
                {
                    continue_check = false;
                    while (true)
                    {
                        continue_check = false;
                        if ((s_min->FirstName[i] == '\0') && (curr->FirstName[i] == '\0'))
                        {
                            continue_check = true;
                            break;
                        }
                        if (curr->FirstName[i] == '\0')
                        {
                            if (s_min->FirstName[i] == '\0')
                                continue_check = true;
                            else
                            {
                                min_prev = prev;
                                s_min = curr;
                                new_min = true;
                            }
                            break;
                        }
                        if (curr->FirstName[i] > s_min->FirstName[i])
                            break;
                        if (curr->FirstName[i] < s_min->FirstName[i])
                        {
                            min_prev = prev;
                            s_min = curr;
                            new_min = true;
                            break;
                        }
                        if (curr->FirstName[i] <= s_min->FirstName[i])
                            i++;
                    }
                }

                i = 0;
                if (continue_check)
                {
                    continue_check = false;
                    while (true)
                    {
                        continue_check = false;
                        if ((s_min->SecondName[i] == '\0') && (curr->SecondName[i] == '\0'))
                        {
                            new_min = true;
                            break;
                        }
                        if (curr->SecondName[i] == '\0')
                        {
                            if (s_min->SecondName[i] == '\0')
                                new_min = true;
                            else
                            {
                                min_prev = prev;
                                s_min = curr;
                                new_min = true;
                            }
                            break;
                        }
                        if (curr->SecondName[i] > s_min->SecondName[i])
                            break;
                        if (curr->SecondName[i] < s_min->SecondName[i])
                        {
                            min_prev = prev;
                            s_min = curr;
                            new_min = true;
                            break;
                        }
                        if (curr->SecondName[i] <= s_min->SecondName[i])
                            i++;
                    }
                }

                if (curr->link == 0)
                    break;
                prev = curr;
                curr = curr->link;

            }
            if (new_min)
            {
                if (min_prev)
                    min_prev->link = s_min->link;

                if ((s_min != g->first) && (first == true))
                {
                    s_min->link = g->first;
                    g->first = s_min;
                    s_min->link = min_last;
                    first = false;
                }
                else
                    if((s_min == g->first) && (first == true))
                        first = false;
                    else
                    {
                        s_min->link = min_last->link;
                        min_last->link = s_min;
                    }
                min_last = s_min;
            }
        }
    }
    else
        if (g->link != 0)
            GroupSort(g->link, key);
}

void InTask2 (Group *&g)
{

    if(g != 0)
    {
        Group *h, *curr, *prev = 0, *max_prev = 0, *max_last, *h_max;
        bool new_max = false,  first = true;
        h = new Group;

        InTask2_run(g, h);

        h_max = curr = max_last = h;
        new_max = true;
        while (new_max)
        {
            max_prev = max_last;
            if (!first)
                h_max = curr = max_last->link;
            new_max = false;

            while(true)
            {
                if (h_max->link == 0)
                    break;

                if (curr->first->salary > h_max->first->salary)
                {
                    max_prev = prev;
                    h_max = curr;
                    new_max = true;
                }
                if (curr->link == 0)
                    break;
                prev = curr;
                curr = curr->link;
            }
            if (new_max)
            {
                if (max_prev)
                    max_prev->link = h_max->link;

                if ((h_max != g) && (first == true))
                {
                    h_max->link = h;
                    h = h_max;
                    h_max->link = max_last;
                    first = false;
                }
                else
                    if((h_max == h) && (first == true))
                        first = false;
                    else
                    {
                        h_max->link = max_last->link;
                        max_last->link = h_max;
                    }
                max_last = h_max;
            }
        }

        cout << "| номер  |   кол-во    |   сумма   |" << endl
             << "| группы | бюджетников | стипендий |" << endl
             << "|--------|-------------|-----------|" << endl;

        curr = h;
        while (true)
        {
            cout << "|" << setw(6) << curr->number << "  |"<< setw(7) << curr->NoS << "      |" << setw(10) << curr->first->salary << " |"<< endl;
            if (curr->link == 0)
                break;
            curr = curr->link;
        }
        cout << "|--------|-------------|-----------|" << endl << endl;
    }
    else
        cout << "В потоке нет групп!" << endl;
}

void InTask2_run (Group *&g, Group *&h)
{
    Student *curr;

    if (h == 0)
        h = new Group;

    h->first = new Student;
    h->number = g->number;

    curr = g->first;
    while (true)
    {
        if (curr->contract == 0)
        {
            h->NoS++;
            h->first->salary += curr->salary;
        }
        if (curr->link == 0)
            break;
        curr = curr->link;
    }
    if (g->link != 0)
        InTask2_run(g->link, h->link);
}
