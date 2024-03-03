#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MANTISSA_MAX 30
#define EXP_MAX 99999
#define STR_MAX MANTISSA_MAX + EXP_MAX + 7

#define OK 0
#define INPUT_ERROR 1
#define DATA_ERROR 2
#define ZERO_LEN 3
#define ZERO_RES 4
#define EXP_ERROR 5

struct number
{
    int sign_m;
    int mantissa[MANTISSA_MAX + 1];
    int sign_e;
    int exp;
};

int division(struct number divisible, struct number divisor, int len_1, int len_2);
void rounding(int *res);
int column(int *n1, int *n2, int *res);
void add_num(int *temp, int *n1, int *i, int len_n1);
int check_end(int *a);
void shift_l(int *a, int len);
void shift_r(int *a, int len);
void subtraction(int *n1, int *n2, int *res);
int cmp(int *n1, int *n2);
void num_print(struct number n);
int num_input(struct number *n, int *len_m);
int get_len(int *a);
void fill_array(int *a, int len_m);
int rm_0(int *a, int *len_m);
int count_symb(char *s, char symb);

int main(void)
{
    int cr, cr1 = OK, cr2 = OK, len_m1, len_m2;
    struct number divisible, divisor, zero;
    fill_array(divisible.mantissa, 0);
    fill_array(divisor.mantissa, 0);
    zero.sign_m = 0;
    fill_array(zero.mantissa, 0);
    zero.sign_e = 0;
    zero.exp = 0;
    setbuf(stdout, NULL);

    printf("Введите числа (по одному в строке) в любом из представленных ниже форматов\n");
    printf("Без точки: 123\n");
    printf("С точкой: .001, ±123.01, 1234.\n");
    printf("Экспоненциальная форма: ±123.123 E(e) ±456\n");
    printf("±------------------------------ e ±-----\n");
    cr1 = num_input(&divisible, &len_m1);
    cr2 = num_input(&divisor, &len_m2);

    if (!cr2 && !cr1)
    {
        if ((cr = division(divisible, divisor, len_m1, len_m2)) == ZERO_RES)
            printf("Результат деления - машинный нуль");
        else if (cr == EXP_ERROR)
            printf("Результат деления выходит за рамки допустимого диапазона");
    }
    else if (cr1 == INPUT_ERROR || cr2 == INPUT_ERROR)
        printf("Некорректный ввод\n");
    else if (cr1 == DATA_ERROR || cr2 == DATA_ERROR)
        printf("Не удалось выполнить деление введенных чисел\n");
    else if (cr2 == ZERO_LEN)
        printf("Второе число не может быть нулем\n");
    else if (cr1 == ZERO_LEN)
        num_print(zero);

    return cr1 || cr2;
}

int division(struct number divisible, struct number divisor, int len_1, int len_2)
{
    struct number result;
    int res[MANTISSA_MAX + 1] = { 0 }, cr;
    int one[MANTISSA_MAX + 1] = { 0 }, extra;
    one[MANTISSA_MAX] = 1;

    if (divisible.sign_m == 1 && divisor.sign_m == 1)
        result.sign_m = 0;
    else if (divisible.sign_m == 1 || divisor.sign_m == 1)
        result.sign_m = 1;
    else
        result.sign_m = 0;

    if ((cr = column(divisible.mantissa, divisor.mantissa, res)) == ZERO_RES)
        return ZERO_RES;

    rounding(res);
    memcpy(result.mantissa, res, sizeof(result.mantissa));

    extra = (cmp(divisor.mantissa, one) == 0 || (len_1 == len_2 && cmp(divisible.mantissa, divisor.mantissa) >= 0) || cr) ? 1 : 0;
    result.exp = divisible.exp - divisor.exp + extra;
    result.sign_e = (result.exp < 0) ? 1 : 0;

    if (result.exp > EXP_MAX || result.exp < -EXP_MAX)
        return EXP_ERROR;

    num_print(result);

    return 0;
}

void rounding(int *res)
{
    int plus = (res[MANTISSA_MAX] >= 5) ? 1 : 0;
    int temp;

    for (int i = MANTISSA_MAX - 1; i >= 0; i--)
    {
        temp = res[i] + plus;
        res[i] = temp % 10;
        plus = temp / 10;
    }
}

int column(int *n1, int *n2, int *res)
{
    int cr = 0;
    int temp[MANTISSA_MAX + 1] = { 0 };
    int i = 0, n, len_n1 = 0, count = 0;

    for (int a = 0; !len_n1 && a < MANTISSA_MAX; a++)
        if (n1[a] != 0)
            len_n1 = MANTISSA_MAX - a;

    if (!len_n1)
        len_n1 = MANTISSA_MAX;

    while (cmp(temp, n2) < 0 && count < MANTISSA_MAX + 2)
    {
        add_num(temp, n1, &i, len_n1);
        count++;
    }
    if (count == 1)
        cr = 1;

    for (int k = 0; k < MANTISSA_MAX + 1; k++)
    {
        if (cmp(temp, n2) < 0)
        {
            res[k] = 0;
            add_num(temp, n1, &i, len_n1);
            continue;
        }

        n = 0;
        while (cmp(temp, n2) >= 0)
        {
            subtraction(temp, n2, temp);
            n++;
        }
        res[k] = n;
        add_num(temp, n1, &i, len_n1);
    }

    return cr;
}

void add_num(int *temp, int *n1, int *i, int len_n1)
{
    shift_l(temp, MANTISSA_MAX + 1);
    if (*i <= len_n1)
        temp[MANTISSA_MAX] = n1[MANTISSA_MAX - len_n1 + *i];
    else
        temp[MANTISSA_MAX] = 0;
    (*i)++;
}

int check_end(int *a)
{
    for (int i = 0; i < MANTISSA_MAX; i++)
        if (a[i] != 0)
            return 0;
    return 1;
}

void shift_l(int *a, int len)
{
    for (int i = 0; i < len - 1; i++)
        a[i] = a[i + 1];
    a[len - 1] = 0;
}

void shift_r(int *a, int len)
{
    for (int i = len - 1; i > 0; i--)
        a[i] = a[i - 1];
    a[0] = 0;
}

void subtraction(int *n1, int *n2, int *res)
{
    int temp, minus = 0;
    for (int i = MANTISSA_MAX; i >= 0; i--)
    {
        temp = n1[i] - n2[i] - minus;
        if (temp < 0)
        {
            minus = 1;
            res[i] = temp + 10;
        }
        else
        {
            minus = 0;
            res[i] = temp;
        }
    }
}

int cmp(int *n1, int *n2)
{
    for (int i = 0; i < MANTISSA_MAX + 1; i++)
    {
        if (n1[i] > n2[i])
            return 1;
        if (n1[i] < n2[i])
            return -1;
    }

    return 0;
}

void num_print(struct number n)
{
    if (n.sign_m)
        printf("-");
    else
        printf("+");

    printf("0.");
    for (int i = 0; i < MANTISSA_MAX; i++)
        printf("%d", n.mantissa[i]);

    printf(" e ");
    if (!n.sign_e)
        printf("+");
    
    printf("%d", n.exp);
    printf("\n");
}

int num_input(struct number *n, int *len_m)
{
    char s[STR_MAX + 2];
    size_t len;

    if (!fgets(s, sizeof(s), stdin))
        return INPUT_ERROR;

    len = strlen(s);
    if (len && s[len - 1] == '\n')
    {
        s[len - 1] = '\0';
        len--;
    }

    if (!len || len > STR_MAX)
        return DATA_ERROR;

    int ind = 0, j = 0, i;
    if (s[ind] == '-')
    {
        n->sign_m = 1;
        ind++;
    }
    else
    {
        n->sign_m = 0;
        if (s[ind] == '+')
            ind++;
    }

    int temp, flag, m_exp = 0;
    char symb;

    if (count_symb(s, 'e') > 1 || count_symb(s, 'E') > 1 || count_symb(s, '.') > 1)
        return INPUT_ERROR;

    n->exp = 0;
    temp = (s[ind] == '0' || s[ind] == '.') ? 0 : 1;
    flag = (s[ind] == '0' || s[ind] == '.') ? 0 : 1;
    symb = (strchr(s, 'E') || strchr(s, 'e')) ? 'e' : '\0';

    if (s[ind] == '0')
        ind++;

    for (i = ind; tolower(s[i]) != symb && j < MANTISSA_MAX + 3; i++)
    {
        if (isdigit(s[i]))
        {
            if (flag)
                m_exp++;
            n->mantissa[j++] = s[i] - '0';
        }
        else if (s[i] == '.')
            flag = !flag;
        else if (isspace(s[i]) && tolower(s[i + 1]) == symb)
            continue;
        else
            return INPUT_ERROR;
    }

    if (j > MANTISSA_MAX + 1)
        return DATA_ERROR;

    *len_m = j;
    ind = ++i;

    if (isspace(s[ind]))
        ind++;
    
    n->sign_e = (s[ind] == '-') ? 1 : 0;

    if (check_end(n->mantissa))
        return ZERO_LEN;

    char *end;
    if (strchr(s, 'e') || strchr(s, 'E'))
    {
        if (temp)
            n->exp = strtol(s + ind, &end, 10) + *len_m;
        else
            n->exp = strtol(s + ind, &end, 10) - m_exp + *len_m;
    }
    else
    {
        if (temp)
            n->exp = strtol(s + ind, &end, 10) + *len_m;
        else
        {
            n->exp = strtol(s + ind, &end, 10) + 1;
            while (n->mantissa[0] == 0)
            {
                shift_l(n->mantissa, MANTISSA_MAX + 1);
                n->exp--;
            }
        }
    }

    if (*end != '\0')
        return INPUT_ERROR;

    for (int a = 0; a < MANTISSA_MAX + 1 - *len_m; a++)
        shift_r(n->mantissa, MANTISSA_MAX + 1);

    n->sign_e = (n->exp < 0) ? 1 : 0;

    return OK;
}

int count_symb(char *s, char symb)
{
    int c = 0;
    for (int i = 0; s[i] != '\0'; i++)
        if (s[i] == symb)
            c++;

    return c;
}

int get_len(int *a)
{
    int len = 0;
    for (int i = 0; i < MANTISSA_MAX; i++)
        if (a[i] == 0)
        {
            len = i;
            break;
        }
    return len;
}

void fill_array(int *a, int len_m)
{
    for (int i = 0; i < MANTISSA_MAX - len_m; i++)
        a[i] = 0;
}

int rm_0(int *a, int *len_m)
{
    while (a[0] == 0 && *len_m > 0)
    {
        for (int i = 0; i < *len_m; i++)
            a[i] = a[i + 1];
        (*len_m)--;
    }

    return *len_m; 
}
