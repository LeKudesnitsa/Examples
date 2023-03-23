//Это курсовая работа 

#include <string>
#include <conio.h>
#include <stdio.h>
#include <cstdlib>
#include <fstream>
#include <iostream>

#define ISNUMBER (str[i] == '1') || (str[i] == '2') || (str[i] == '3') || (str[i] == '4') ||(str[i] == '5') || (str[i] == '6') || (str[i] == '7') || (str[i] == '8') || (str[i] == '9') || (str[i] == '0') || (str[i] == '.')
#define ISOPERATOR (str[i] == '+') || (str[i] == '-') || (str[i] == '*') || (str[i] == '/')

using namespace std;

struct element
{
    bool control = false;
    char symb;
    double number;

    element *link = NULL;
};

int toRPN(string &str, element *&list_head);
void add_stack (char ch, element *&stack_head);
int calculate (element *&list_head);
int get_priority(char ch);
void delete_elements(element *&head);

void out (element *p); //used for debug
//not my functions:
double stod(const string &strInput); //converting string to double
double apow(const float &x, int y);  //helper function for stod

int main ()
{

    int      k, i;              //k for keyboard button value
    string   str;               //string for parse
    element  *list_head = NULL; //out string for reverse polish notation
    ifstream inp;               //input file

    setlocale(LC_ALL, "rus");

    while(true)
    {
        cout << "Нажмите \"C\", чтобы ввести формулу из консоли или \"F\", чтобы ввести формулу из файла.\nНажмите ESC, чтобы выйти." << endl;
        k = getch();
        if ((k == 99) || (k == 67) || (k == 145) || (k == 145)) //'c'
        {
            cout << "Введите формулу: ";
            getline(cin, str);

            switch (toRPN(str, list_head))
            {
                case 0:
                {
                    if( calculate (list_head) != 6)
                    {
                        cout << list_head->number << endl;
                        break;
                    }
                    else
                    {
                        cout << "Ошибка! В выражении присутствует неявное деление на ноль!" << endl;
                        break;
                    }
                }
                case 1: cout << "Ошибка! В выражении присутствует недопустимый символ!\nРазрешены только ()+-*/0123456789 и пробел." << endl; break;
                case 2: cout << "Ошибка! Проверьте правильность записи выражения!" << endl; break;
                case 3: cout << "Ошибка! Проверьте расстановку скобок в выражении!" << endl; break;
                case 5: cout << "Ошибка! В выражении присутствует деление на ноль!" << endl; break;

                default: break;
            }
            delete list_head;
            list_head = NULL;
            system("pause");
        }
        else if ((k == 70) || (k == 102) || (k == 128) || (k == 160)) // 'f'
        {
            inp.open("input.txt");

            if (!inp.is_open())
                cout << "Сначала создайте файл input.txt!" << endl;
            else
            {
                i = 1;
                while (!inp.eof())
                {
                    getline(inp, str);

                    switch (toRPN(str, list_head))
                    {
                        case 0:
                        {
                            if ( calculate (list_head) != 6)
                            {
                                cout << i << ": " << str << " = " << list_head->number << endl;
                                break;
                            }
                            else
                            {
                                cout << "Ошибка! В выражении " << i << ": " << str << " присутствует неявное деление на ноль!" << endl;
                                break;
                            }
                        }
                        case 1: cout << "Ошибка! В выражении " << i << ": " << str << " присутствует недопустимый символ!\nРазрешены только ()+-*/0123456789 и пробел." << endl; break;
                        case 2: cout << "Ошибка! Проверьте правильность расстановки знаков в выражении " << i << ": " << str << endl; break;
                        case 3: cout << "Ошибка! Проверьте расстановку скобок в выражении " << i << ": " << str << endl; break;
                        case 4: i--; break;
                        case 5: cout << "Ошибка! В выражении " << i << ": " << str  << " присутствует деление на ноль!" << endl; break;

                        default: break;
                    }
                    delete list_head;
                    list_head = NULL;
                    i++;
                }
            }
            inp.close();

            system("pause");
        }
        else if (k == 27) // esc
        {
            cout << "\nВы уверены? Y/N" << endl;
            while (true)
            {
                k = getch();
                if ((k == 121) || (k == 89) || (k == 141) || (k == 173))//Y
                    exit(0);
                if ((k == 110) || (k == 78)|| (k == 146) || (k == 226))//N
                    break;
            }
        }
        system("cls");
    }

    return 0;
}

int toRPN(string &str, element *&list_head)
{
    int     i           = 0;    //string iterator
    char    prev        = 0,    //previous symbol, 1 -- number, 2 -- operator
            status      = 0;    //return value; 0 -- OK, 1 -- wrong symbol, 2 -- wrong operators, 3 -- wrong brakes,
                                //4 -- empty line, 5 -- divide by zero, 6 -- implicit divide by zero
    bool    allowed     = false;//is current symbol allowed?
    string  str_num     = "";   //string of digits for converting to number
    element *tmp        = NULL,
            *stack_head = NULL;

    while(true)
    {
        allowed = false;

        if (ISNUMBER)
        {
            if (prev == 1) // если перед числом было другое, то ошибка записи
                {status = 2; break;}
            while(true)
            {
                str_num += str[i];
                allowed = true;
                i++;

                if(!(ISNUMBER))
                {
                    tmp = list_head;
                    list_head = new element;
                    list_head->link = tmp;

                    //list_head->number = atof(str_num.c_str());
                    list_head->number = stod(str_num);
                    str_num = "";
                    prev = 1;
                    allowed = false;
                    break;
                }
            }
        }
        if (str[i] == ' ') //пропускаем пробел
        {
            i++;
            continue;
        }
        if (ISOPERATOR)
        {
            allowed = true;

            if ((str[i-1] == '(') || (!list_head)) //проверка унарных операций:
                                                   //если перед оператором скобка или если выходная строка пуста,
                                                   //то проверяем знак
            {
                i++;                                //костыль, чтобы не плодить DEFINE ISNUMBER
                                                    //и выполнять меньше операций сложения/вычитания.
                if((str[i-1] == '-') && (ISNUMBER)) //если число отрицательное
                {
                    str_num += '-';
                    continue;
                }
                else if((str[i-1] == '+') && (ISNUMBER))
                    continue;
                    else {status = 2;  break;}
                i--;
            }
            while ((stack_head) && (get_priority(str[i]) <= get_priority(stack_head->symb)))
            //пока стэк не закончился и приоритет текущего оператора меньше
            //или равен приоритету оператора на вершине стека, переносим его в выходную строку
            {
                tmp = stack_head->link;
                stack_head->link = list_head;
                list_head = stack_head;
                stack_head = tmp;
            }

            if((prev == 2) || (str[i+1] == '\0')) //если 2 оператора подряд или текущий оператор последний в строке,
                                                  //то ошибка записи операторов
                {status = 2; break;}
            if ((str[i] == '/') && (str[i-1] == '0')) //проверка деления на 0
                {status = 5; break;}

            prev = 2;
            add_stack (str[i], stack_head);
        }
        if (str[i] == '(')
        {
            if (prev == 1) //если нет оператора или другой скобки перед текущей, то ошибка расстановки скобок
                {status = 3; break;}
            allowed = true;
            add_stack (str[i], stack_head);
        }
        if (str[i] == ')')
        {
            if (prev == 2) //если последний оператор не имеет правого операнда
                {status = 2; break;}
            allowed = true;

            while(true)
            {
                if (!stack_head) //если стэк закончился раньше, чем встречена открывающая скобка
                    {status = 3; break;}
                if (stack_head->symb != '(') //пока не встретим открывающую скобку,
                                             //переносим верхний элемент стэка в выходную строку
                {
                    tmp = stack_head->link;
                    stack_head->link = list_head;
                    list_head = stack_head;
                    stack_head = tmp;
                }
                else break;
            }
            if(stack_head) //удаляем открывающую скобку
            {
                tmp = stack_head->link;
                delete stack_head;
                stack_head = tmp;
            }
        }

        if (str[i] == '\0') //окончание разбора по достижении конца строки
            break;

        if (!allowed) //встречен недопустимый символ
            {status = 1; break;}

        i++;
    }

    if(!status)            //если при разборе не выявлено ошибок,
        while (stack_head) //то переносим все операторы из стэка в обратном порядке в выходную строку
        {
            if (stack_head->symb == '(') //если встречена открывающая скобка, то где-то нет закрывающей
                {status = 3; break;}

            tmp = stack_head->link;
            stack_head->link = list_head;
            list_head = stack_head;
            stack_head = tmp;
        }

    if((!status) && (!list_head)) //если пустая строка
        status = 4;

    if (status) //если при разборе выявлена ошибка, то очищаем стэк и выходную строку
    {
        delete_elements(list_head);
        delete_elements(stack_head);
    }
    return status;
}

void add_stack (char ch, element *&stack_head)
{
    element *prev;

    prev = stack_head;
    stack_head = new element;
    stack_head->link = prev;
    prev = stack_head;

    stack_head->control = true;
    stack_head->symb = ch;
}

int calculate (element *&list_head)
{
    element *tmp;
    int result = 0;

    if ((list_head->control) && (!list_head->link->control) && (!list_head->link->link->control))
    //если после текущего оператора есть операнды, то вычисляем результат операции
    {

        list_head->control = false;
        switch (list_head->symb)
        {
            case '+':
                list_head->number = list_head->link->link->number + list_head->link->number;
                break;
            case '-':
                list_head->number = list_head->link->link->number - list_head->link->number;
                break;
            case '*':
                list_head->number = list_head->link->link->number * list_head->link->number;
                break;
            case '/':
            {
                if(list_head->link->number == 0)//неявное деление на 0
                {
                    delete_elements(list_head);
                    return 6;
                }
                else
                    list_head->number = list_head->link->link->number / list_head->link->number;
                break;
            }
        }

        //удаляем операнды, т.к. результат операции записываетс на место оператора
        tmp = list_head->link->link->link;
        delete list_head->link->link;
        delete list_head->link;
        list_head->link = tmp;
    }
    else
        if ((result == 0) && (list_head->link) && (list_head->link->link->link)) //если нет деления на ноль,
            result = calculate (list_head->link);                                //текущий элемент не в конце списка
                                                                                 //и после него есть как минимум три элемента,
                                                                                 //то продолжаем вычисления

    while((result == 0) && (list_head->control)) //пока нет деления на ноль и текущий элемент является оператором,
        result = calculate (list_head);          //продолжаем вычисления

    return result;
}

int get_priority(char ch)
{
    switch (ch)
    {
        case '(': return 0; break;
        case ')': return 0; break;
        case '+': return 1; break;
        case '-': return 1; break;
        case '*': return 2; break;
        case '/': return 2; break;
        break;
    }
}

void delete_elements(element *&head)
{
    element *tmp;

    while (head)
    {
        tmp = head->link;
        delete head;
        head = tmp;
    }
}

void out (element *p)
{
    if (p == 0)
        return;
    if (p->control)
        cout << p->symb;
    else
        cout << p->number << " ";
    out(p->link);
}


//not my functions
//taken from
//https://www.daniweb.com/programming/software-development/code/217315/c-function-stod-string-to-double-convert-a-string-to-a-double
double stod(const string &strInput)
{
    /* STOD: String to double  */
    /* Written by Mathias Van Malderen */
    double dbl_one = 0;
    double dbl_two = 0;
    double dbl_final = 0;
    int strlen;
    bool dec_pt = false;
    int nums_before_dec = 0;
    int nums_after_dec = 0;
    strlen = strInput.length();
    /* Check whether the string can be transformed into a number */
    if(strInput[0] == '0' && strInput[1] == '0')
    {
        // invalid number !
        return 0;
    }
    for(int i = 0; i < strlen; i++)
    {
        if(strInput[i] == '0' || strInput[i] == '1' || strInput[i] == '2' || strInput[i] == '3' || strInput[i] == '4' || strInput[i] == '5' || strInput[i] == '6' || strInput[i] == '7' || strInput[i] == '8' || strInput[i] == '9')
        {
            // valid number
        } else if(strInput[i] == '.'){
            if(dec_pt)
            {
                // there was already a decimal point counted
                // invalid number !
                return 0;
            } else {
                dec_pt = true; // increment by one
            }
        } else {
            // invalid number !
            return 0;
        }
    }
    /* Convert the number */
    // STEP 1: Calculate the amount of numbers before/after the decimal point (if there's one)
    if(dec_pt) // if there's a decimal point in the number
    {
        for(int i = 0; i < strlen; i++)
        {
            if(strInput[i+1] != '.')
            {
                nums_before_dec++;
            } else {
                nums_before_dec++;
                break;
            }
        }
        nums_after_dec = strlen-nums_before_dec;
        nums_after_dec -= 1;
    } else {
        // This piece of code was added later as a BUGFIX !
        // Now the STOD-function is working 100% perfect !!!
        nums_after_dec = 0;
        nums_before_dec = strlen;
    }

    // STEP 2: Convert the string to a real number
    for(int i = 0; i < nums_before_dec; i++)
    {
        switch(strInput[i])
        {
            case '0':
                dbl_one += 0 * apow(10, (nums_before_dec - i));
                break;
            case '1':
                dbl_one += 1 * apow(10, (nums_before_dec - i));
                break;
            case '2':
                dbl_one += 2 * apow(10, (nums_before_dec - i));
                break;
            case '3':
                dbl_one += 3 * apow(10, (nums_before_dec - i));
                break;
            case '4':
                dbl_one += 4 * apow(10, (nums_before_dec - i));
                break;
            case '5':
                dbl_one += 5 * apow(10, (nums_before_dec - i));
                break;
            case '6':
                dbl_one += 6 * apow(10, (nums_before_dec - i));
                break;
            case '7':
                dbl_one += 7 * apow(10, (nums_before_dec - i));
                break;
            case '8':
                dbl_one += 8 * apow(10, (nums_before_dec - i));
                break;
            case '9':
                dbl_one += 9 * apow(10, (nums_before_dec - i));
                break;
            default:
                // invalid number !
                return 0;
        }
    }
    dbl_one = dbl_one / 10; // little fix
    for(int i = 0; i < nums_after_dec; i++)
    {
        switch(strInput[i + nums_before_dec + 1])
        {
            case '0':
                dbl_two += (0 / apow(10, i+1));
                break;
            case '1':
                dbl_two += (1 / apow(10, i+1));
                break;
            case '2':
                dbl_two += (2 / apow(10, i+1));
                break;
            case '3':
                dbl_two += (3 / apow(10, i+1));
                break;
            case '4':
                dbl_two += (4 / apow(10, i+1));
                break;
            case '5':
                dbl_two += (5 / apow(10, i+1));
                break;
            case '6':
                dbl_two += (6 / apow(10, i+1));
                break;
            case '7':
                dbl_two += (7 / apow(10, i+1));
                break;
            case '8':
                dbl_two += (8 / apow(10, i+1));
                break;
            case '9':
                dbl_two += (9 / apow(10, i+1));
                break;
            default:
                // invalid number !
                return 0;
        }
    }
    // STEP 3: Return the converted string as a double:
    dbl_final = dbl_one + dbl_two;
    return dbl_final;
}
/* This function 'apow' raises x to the power of y, it's a dependency of 'stod' */
double apow(const float &x, int y)
{
    double result = 1;
    if(y == 0)
        return result;
    if(y < 0)
    {
        y = -y;
        for(int i = 0; i < y; i++)
            result = result * x;
        return 1/result;
    }
    for(int i = 0; i < y; i++)
        result = result * x;
    return result;
}


