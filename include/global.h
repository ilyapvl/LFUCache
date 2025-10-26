#ifndef GLOBAL_H
#define GLOBAL_H

struct page
{
    int index;
    int sz;
    char* data;
};

int slow_get_page_int(int index)
{
    return index;
}

#endif // GLOBAL_H
