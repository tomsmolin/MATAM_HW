#include <iostream>
#include <cstdio>
#include "date_wrap.h"
#include "exceptions.h"


static bool isDateValid(int day, int month);
static bool isNegativeNumber(int number);

using mtm::DateWrap;

DateWrap::DateWrap(int day, int month, int year): day_ptr(day), month_ptr(month), year_ptr(year)
{
    if(!isDateValid(day, month))
    {
        throw mtm::InvalidDate();
    }
    date = dateCreate(day, month, year);
    dateGet(date, &day_ptr, &month_ptr, &year_ptr);
}

DateWrap::DateWrap(const DateWrap& date_wrap): day_ptr(0), month_ptr(0), year_ptr(0)
{
    date = dateCopy(date_wrap.date);
    dateGet(date, &day_ptr, &month_ptr, &year_ptr);
}

DateWrap::~DateWrap() 
{ 
    dateDestroy(date);   
}

int DateWrap::day () const 
{ 
    return day_ptr; 
}

int DateWrap::month () const 
{ 
    return month_ptr; 
}

int DateWrap::year () const 
{ 
    return year_ptr;
}

std::ostream& mtm::operator<<(std::ostream& out, const DateWrap& date_object)
{
    out << date_object.day_ptr << '/' << date_object.month_ptr << '/' << date_object.year_ptr;
    return out;
}

bool DateWrap::operator==(const DateWrap& date_wrap_two) const 
{
    if (dateCompare(date, date_wrap_two.date) == 0)
    {
        return true;
    }
    return false;
}

bool DateWrap::operator!=(const DateWrap& date_wrap_two) const 
{
    if (dateCompare(date, date_wrap_two.date) != 0)
    {
        return true;
    }
    return false;
}

bool DateWrap::operator<(const DateWrap& date_wrap_two) const
{ 
    if (dateCompare(date, date_wrap_two.date) < 0)
    {
        return true;
    }
    return false;
}

bool DateWrap::operator<=(const DateWrap& date_wrap_two) const
{ 
    if (dateCompare(date, date_wrap_two.date) <= 0)
    {
        return true;
    }
    return false; 
}

bool DateWrap::operator>(const DateWrap& date_wrap_two) const
{ 
    if (dateCompare(date, date_wrap_two.date) > 0)
    {
        return true;
    }
    return false; 
}

bool DateWrap::operator>=(const DateWrap& date_wrap_two) const
{ 
    if (*this < date_wrap_two)
    {
        return false;
    }
    return true;
}

DateWrap DateWrap::operator++(int)
{
    DateWrap tmp(*this);
    dateTick(date); 
    dateGet(date, &day_ptr, &month_ptr, &year_ptr);
    return tmp;
}

DateWrap& DateWrap::operator+=(int days) 
{
    if(isNegativeNumber(days))
    {
        throw mtm::NegativeDays();
    }
    while (days > 0)
    {
        dateTick(date);
        days--;
    }
    dateGet(date, &day_ptr, &month_ptr, &year_ptr);
    return *this;
}

DateWrap mtm::operator+(DateWrap date_wrap, int days)
{
    if(isNegativeNumber(days))
    {
        throw mtm::NegativeDays();
    }
    date_wrap += days;
    return date_wrap;
}

DateWrap mtm::operator+(int days, DateWrap date_wrap)
{
    date_wrap += days;
    return date_wrap;
}

DateWrap& DateWrap::operator=(const DateWrap& date_wrap_to_copy) 
{ 
    if (this == &date_wrap_to_copy)
    {
        return *this;
    }

    dateDestroy(date);
    date = dateCopy(date_wrap_to_copy.date);
    dateGet(date, &day_ptr, &month_ptr, &year_ptr);
    return *this;

}


static bool isNegativeNumber(int days)
{
    if (days < 0)
    {
        return true;
    }
    return false;
}

static bool isDateValid(int day, int month)
{
    if (day < MIN_DAY || day > MAX_DAY)
    {
        return false;
    }
    if (month < MIN_MONTH || month > MAX_MONTH)
    {
        return false;
    }
    return true;
}