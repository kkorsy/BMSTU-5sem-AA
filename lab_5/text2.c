#include "array.h"
#include <stdlib.h>
#include "rand_time.h"

#define N 1000
#define MEAN(t1, t2) ((t1 + t2) / 2)
#define EPS 1e-6

extern double t1_1, t1_2;
extern double t2_1, t2_2;
extern double t3_1, t3_2;
extern double t4_1, t4_2;

queue_arr *aq_create()
{
    queue_arr *aq = malloc(sizeof(queue_arr));
    if (aq != NULL)
    {
        aq->pin = -1;
        aq->pout = -1;
        aq->len = 0;
    }

    return aq;
}

void aq_destroy(queue_arr *aq)
{
    int tmp;

    while (!aq_is_empty(aq))
        aq_pop(aq, &tmp);
    
    free(aq);
}

int aq_pop(queue_arr *aq, int *x)
{
    if (aq_is_empty(aq))
        return 1;

    *x = aq->arr[aq->pin];
    if (aq->pin == aq->pout)
    {
        aq->pin = -1;
        aq->pout = -1;
    }
    else
        aq->pin = (aq->pin + 1) % N;

    aq->len--;

    return 0;
}

int aq_push(queue_arr *aq, int x)
{
    if (aq_is_full(aq))
        return 1;

    if (aq->pin == -1)
        aq->pin = 0;

    aq->pout = (aq->pout + 1) % N;
    aq->arr[aq->pout] = x;

    aq->len++;

    return 0;
}

int aq_is_full(queue_arr *aq)
{
    return ((aq->pin == aq->pout + 1) || (aq->pin == 0 && aq->pout == N - 1));
}

int aq_is_empty(queue_arr *aq)
{
    return (aq->pin == -1 || aq->len == 0);
}

void aq_model()
{
    unsigned long long int beg, end, sum = 0;
    int temp, printed_flag = 1;
    int i = 0;

    queue_arr *aq1 = aq_create();
    queue_arr *aq2 = aq_create();

    int in_req1 = 0, in_req2 = 0;
    int out_req1 = 0, out_req2 = 0;

    double t_come1 = get_time(t1_1, t1_2), t_come2 = get_time(t2_1, t2_2);
    double downtime = 0, t_work = get_time(0, 1), total_work = 0;

    int total_len1 = 0, total_len2 = 0;
    int work_req_type = 0;

    while (out_req1 < N)
    {
        beg = clock_get_time();
        i++;
        // пришла заявка
        if (t_work > t_come1 || t_work > t_come2)
        {
            // первая
            if (t_come1 <= t_come2)
            {
                if (aq_push(aq1, 1))
                {
                    printf("Массив переполнен\n");
                    break;
                }

                t_work -= t_come1;
                t_come2 -= t_come1;
                total_work += t_come1;

                end = clock_get_time();
                sum += end - beg;

                t_come1 = get_time(t1_1, t1_2);

                beg = clock_get_time();

                in_req1++;
            }
            // вторая
            else if (t_come2 < t_come1)
            {
                if (aq_push(aq2, 2))
                {
                    printf("Массив переполнен\n");
                    break;
                }

                t_work -= t_come2;
                t_come1 -= t_come2;
                total_work += t_come2;

                end = clock_get_time();
                sum += end - beg;

                t_come2 = get_time(t2_1, t2_2);

                beg = clock_get_time();

                in_req2++;
            }
        }
        // первая очередь не пустая и автомат уже отработал
        else if (!aq_is_empty(aq1))
        {
            aq_pop(aq1, &temp); // подаем первую заявку

            t_come1 -= t_work;
            t_come2 -= t_work;
            total_work += t_work;

            end = clock_get_time();
            sum += end - beg;

            t_work = get_time(t3_1, t3_2);

            beg = clock_get_time();
            
            if (work_req_type == 1)
                out_req1++;
            else if (work_req_type == 2)
                out_req2++;

            work_req_type = 1;
        }
        // вторая очередь не пустая и автомат отработал
        else if (!aq_is_empty(aq2))
        {
            aq_pop(aq2, &temp); // подаем вторую заявку

            t_come1 -= t_work;
            t_come2 -= t_work;
            total_work += t_work;

            end = clock_get_time();
            sum += end - beg;

            t_work = get_time(t4_1, t4_2);

            beg = clock_get_time();
            
            if (work_req_type == 1)
                out_req1++;
            else if (work_req_type == 2)
                out_req2++;

            work_req_type = 2;
        }
        // обе очереди пустые
        else
        {
            t_come1 -= t_work;
            t_come2 -= t_work;

            downtime += t_work;
        }
        total_len1 += aq1->len;
        total_len2 += aq2->len;

        end = clock_get_time();
        sum += end - beg;
        
        if (out_req1 % 100 == 0 && printed_flag)
        {
            printf("Текущая длина 1 очереди: %d\n", aq1->len);
            printf("Текущая длина 2 очереди: %d\n", aq2->len);
            printf("Средняя длина 1 очереди: %lf\n", (double)total_len1 / i);
            printf("Средняя длина 2 очереди: %lf\n", (double)total_len2 / i);
            printf("Среднее время пребывания заявки 1 типа в очереди: %lf\n", (double)total_len1 / i * MEAN(t1_1, t1_2));
            printf("Среднее время пребывания заявки 2 типа в очереди: %lf\n", (double)total_len2 / i * MEAN(t2_1, t2_2));
            printf("Количество вошедших заявок 1 типа: %d\n", in_req1);
            printf("Количество вошедших заявок 2 типа: %d\n", in_req2);
            printf("Количество вышедших заявок 1 типа: %d\n", out_req1);
            printf("Количество вышедших заявок 2 типа: %d\n\n", out_req2);

            printed_flag = 0;
        }

        if (out_req1 % 100 != 0)
            printed_flag = 1;
    }

    double time_expect = N * MEAN(t3_1, t3_2) + ((MEAN(t1_1, t1_2) - MEAN(t3_1, t3_2) > EPS) ? (MEAN(t1_1, t1_2) - MEAN(t3_1, t3_2)) * N : 0);

    printf("\nОжидаемое время моделирования: %lf е.в.\n", time_expect);
    printf("Общее время моделирования: %lf е.в.\n", total_work + downtime);
    printf("Погрешность: %.3lf %%\n\n", err_rate(time_expect, total_work + downtime) * 100);

    printf("Время простоя аппарата: %lf е.в.\n", downtime);
    printf("Количество вошедших в систему заявок 1 типа: %d\n", in_req1);
    printf("Количество вошедших в систему заявок 2 типа: %d\n", in_req2);
    printf("Количество вышедших из системы заявок 1 типа: %d\n", out_req1);
    printf("Количество вышедших из системы заявок 2 типа: %d\n\n", out_req2);

    printf("Затраты памяти: %zu\n", 2 * sizeof(queue_arr));
    printf("Реальное время моделирования: %lf мкс\n\n", sum / 10e3);

    aq_destroy(aq2);
    aq_destroy(aq1);
}
