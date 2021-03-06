#include <cstdint>
#include <memory>
#include "defs.hpp"

/*
struct ListNode {
	std::shared_ptr<ListNode> next;
	uint32_t data;
};
*/
//#define ADD_BACK
// Global List Head
std::shared_ptr<ListNode> head = nullptr;

/**
* add_node: Given a reference to a list head, create and add a new node,
*           containing the data element "value".
*/
void add_node(std::shared_ptr<ListNode>& head, uint32_t value)
{
#ifdef ADD_BACK
    auto tmp = head;
    if (tmp)
    {
        while (tmp->next)
        {
            tmp = tmp->next;
        }
        tmp->next = std::make_shared<ListNode>();
        tmp->next->data = value;
    }
    else
    {
        head = std::make_shared<ListNode>();
        head->data = value;
    }
#else
    //add front
    auto tmp = std::make_shared<ListNode>();
    tmp->data = value;
    tmp->next = head;
    head = tmp;
#endif
}

/**
* get_cout: Given a shared pointer to the head of a linked list, return
*           the number of nodes contained within the list.
*/
uint32_t get_count(std::shared_ptr<ListNode> head)
{
    auto count = 0;
    auto tmp = head;
    while(tmp)
    {
        count++;
        tmp = tmp->next;
    }
	return count;
}

/**
*  find_value: Given a shared pointer to the head of a linked list, search for the element
*              containing the data element "needle," and either return that element, or an
*			   empty shared_ptr if it cannot be found.
*/
std::shared_ptr<ListNode> find_value(std::shared_ptr<ListNode> head, uint32_t needle)
{
    auto tmp = head;
    while (tmp)
    {
        if (tmp->data == needle)
            return tmp;
        tmp = tmp->next;
    }
	return nullptr;
}


/**
* remove_node: Given a reference to a shared_ptr to the beginning of a list, remove the first element, and
*	           return it. If the list is empty, simply return an empty shared_ptr.
*/
std::shared_ptr<ListNode> remove_node(std::shared_ptr<ListNode>& head)
{
    auto tmp = head;
    head = head->next;
    tmp->next = nullptr;
    return tmp;
}


/***
* ListNode is an element in a linked list; each element contains a shared_ptr to next,
* or an "empty" shared pointer if there is no next element, and a "data" element, consisting
* of a uint32_t.
***/
int main(int argc, char** argv, char** envp)
{

	test_one();
	test_two();
	test_three();
	return 0;
}