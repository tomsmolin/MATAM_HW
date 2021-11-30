#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "date.h"

#define MIN_DAY 1
#define MAX_DAY 30
#define MIN_MONTH 1
#define MAX_MONTH 12
#define FIRST_ARGUMENT_IS_BIGGER 1
#define SECOND_ARGUMENT_IS_BIGGER -1
/*-------------------------------------------------------------------------
    data_structures:
-------------------------------------------------------------------------*/
struct Date_t 
{
    int day;
    int month;
    int year;
};

/*-------------------------------------------------------------------------
    static function declaration:
-------------------------------------------------------------------------*/

static bool isDayValid(int day);

static bool isMonthNumberValid(int month);

/*Compare two integers and return the biggest.*/
static int isBigger(int argument_one, int argument_two);


/*-------------------------------------------------------------------------
    date's functions implementation:
-------------------------------------------------------------------------*/

Date dateCreate(int day, int month, int year) 
{
    if (!isDayValid(day) || !isMonthNumberValid(month)) {
        return NULL;
    }
    Date date = malloc(sizeof(*date));
    if (date == NULL) {
        return NULL;
    }
    date->day = day;
    date->month = month;
    date->year = year;
    return date;
}

void dateDestroy(Date date) 
{
    if (date == NULL) {
        return;
    }
    free(date);
}

bool dateGet(Date date, int *day, int *month, int *year) 
{
    if (date == NULL || day == NULL || month == NULL || year == NULL) {
        return false;
    }

    *day = date->day;
    *month = date->month;
    *year = date->year;

    return true;
}

void dateTick(Date date) 
{
    if (date == NULL) {
        return;
    }

    if (date->day == MAX_DAY) {
        date->day = MIN_DAY;
        int current_month = date->month;
        if (current_month == MAX_MONTH) {
            date->month = MIN_MONTH;
            (date->year)++;
            return;
        }
        date->month++;
        return;
    }

    (date->day)++;
}

Date dateCopy(Date date) 
{
    if (date == NULL) {
        return NULL;
    }
    return dateCreate(date->day, date->month, date->year);
}

int dateCompare(Date date_one, Date date_two) 
{
    if (date_one == NULL || date_two == NULL) {
        return 0;
    }

    int year_compare_result = isBigger(date_one->year, date_two->year);
    if (year_compare_result != 0) {
        return year_compare_result;
    }

    int month_compare_result = isBigger(date_one->month, date_two->month);
    if (month_compare_result != 0) {
        return month_compare_result;
    }

    int day_compare_result = isBigger(date_one->day, date_two->day);
    if (day_compare_result != 0) {
        return day_compare_result;
    }
    return 0;
}

/*-------------------------------------------------------------------------
    static functions implementation:
-------------------------------------------------------------------------*/
static bool isDayValid(int day) 
{
    return day >= MIN_DAY && day <= MAX_DAY;
}

static bool isMonthNumberValid(int month) 
{
    return month >= MIN_MONTH && month <= MAX_MONTH;
}

static int isBigger(int argument_one, int argument_two) 
{
    if (argument_one > argument_two) {
        return FIRST_ARGUMENT_IS_BIGGER;
    }
    if (argument_two > argument_one) {
        return SECOND_ARGUMENT_IS_BIGGER;
    }
    return 0;
}