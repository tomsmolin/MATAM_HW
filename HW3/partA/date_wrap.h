#ifndef HW3_DATE_WRAP_H
#define HW3_DATE_WRAP_H

#include <iostream>
#include <cstdio>
extern "C" {
#include "date.h"
}
#define MIN_DAY 1
#define MAX_DAY 30
#define MIN_MONTH 1
#define MAX_MONTH 12

namespace mtm
{
    class DateWrap
    {
        Date date;
        int day_ptr;
        int month_ptr;
        int year_ptr;

        public:
            DateWrap(int day, int month, int year);
            DateWrap(const DateWrap& date_wrap);
            ~DateWrap();
            int day () const;            
            int month () const;              
            int year () const;  
           
        
            friend std::ostream& operator<<(std::ostream& out, const DateWrap& date_object);

            bool operator==(const DateWrap& date_wrap_two) const;
            bool operator!=(const DateWrap& date_wrap_two) const;
            bool operator<(const DateWrap& date_wrap_two) const;
            bool operator<=(const DateWrap& date_wrap_two) const;
            bool operator>(const DateWrap& date_wrap_two) const;
            bool operator>=(const DateWrap& date_wrap_two) const;
            DateWrap operator++(int);
            DateWrap& operator+= (int days);
            DateWrap operator+();
            DateWrap& operator=(const DateWrap&);

    };
    std::ostream& operator<<(std::ostream& out, const DateWrap& date_object);
    DateWrap operator+(DateWrap date_wrap, int days);
    DateWrap operator+(int days, DateWrap date_wrap);

}

#endif //HW3_DATE_WRAP_H
