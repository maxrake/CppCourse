.. title:: Templates

:css: CSS/course.css

----

Templates
=========

Building programs that the compiler runs!
-----------------------------------------

.. image:: images/Templates.png

----

Templates
=========

Templates are how we achieve generic programming in C++. 

* Templates allow a straightforward way to represent a wide range of general concepts and simple ways to combine them. 
* The template mechanism allows a type or a value to be a parameter in the definition of a class, a function, or a type alias. 
* Every major standard-library abstraction is represented as a template

  * ostream, list, map, unique_ptr, function, ...

----

Defining a template
===================

As a function:

.. code:: C++

    template<typename T>
    T foo( T a, T b) {
        return a + b;
    }

----

As a class
==========

* A class generated from a template is an ordinary class
* There is no run-time mechanism or overhead

.. code:: C++

    template<typename T, int size>
    class Array {
        T[size] array_;
    public:
        ...
        T& operator[](int index){
          return array(index);
        }

    }

----

Instantiation of Templates
==========================

For classes we almost always supply template arguments.

.. code:: C++

    std::vector<int> a;
    std::map<int, double> b;
    std::string c; //typedef basic_string<char, char_traits<char>, allocator<char> > string;

For functions we sometimes supply template arguments.

.. code:: C++

    template<typename T> T to(const std::string& number) {
        std::stringstream ss(number);
        T num;
        ss >> num;
        return num;
    }

    template<typename T> T add(T num1, T num2){
        return num1 + num2;
    }

    auto num = to<int>("924");
    auto sum = add(1,2);
    auto sum = add<std::string>("924", "is a number"); //Even tho is isn't required we can still provide the arguments. 

----

Type Equivalence
================

It is important to understand how templates view types because for every new type use the compiler will generate a new version. 

* Aliases do not introduce a new type

.. code:: C++

    using uint = unsigned int;
    std::vector<uint> a;
    std::vector<unsigned int> b; //same type as a.

* Unsigned v. signed ARE different types. 
* The compiler can evaluate constexpr (constant expressions)
   
   * so ``Buffer<char, 20-10>`` will be the same type as ``Buffer<char, 10>``

* Types generated by a single template with different arguments are different types. 

----

Errors
======

Errors relating to template parameters cannot be detected until the template is used. 

.. code:: C++

    template<typename Cont, typename Elem>
    void push_back(Cont& container, const Elem& elem)
    {
        container.push_back(elem);
    }

    std::vector<int> vecInt;
    int num = 0;
    push_back(vecInt, 5); //FINE. 
    push_back(num, 5); //ERROR "left of .push_back must have class/struct/union"

----

Type Checking
=============

There is currently no way to implement requirements on template parameters.

.. code:: C++

    template <Container Cont, typename Elem>
        requires Equal_comparable<Cont::value_type, Elem>()
    int find_index(Cont& c, Elem e);

* This is the idea behind the concepts proposal that hasn't made it into the standard yet. 

----

static_assert
=============

static_assert allowing for better error messages. 

* static_assert is a compile time assert. 
* if false the message in the assert will appear as a compiler error

.. code:: 

    template<typename Cont, typename Elem>
    void push_back(Cont& container, const Elem& elem)
    {
        static_assert(std::is_class<Cont>::value, "Cont must be a class");
        container.push_back(elem);
    }

    ... 
    push_back(num, 5); //ERROR: Cont must be a class

----

Member templates
================

A template or non-template class can have templated member functions. 

.. code:: C++ 
    
    class foo {
        int count_ = 0;
    public:
        template<typename T> 
        void accumulate(T value) {
            count_ += value;
        }
    };

----

Overloading Function Templates
==============================

* overload resolution will be needed to deduce the proper function call
* The most specialize function will be called. 

.. code:: C++

    template<typename T> 
    T foo( const T& element1, const T& element2);

    template<typename T> 
    T foo( const T& element1, int elem2);

    template<typename T>
    std::vector<T> foo(const std::vector<T>& element1, const T& elem2);

    int foo(int elem1, int elem2);

    std::vector<FooBar> foobars;
    foo(1, 2); //int foo(int, int);
    foo(1.2, 2); //T foo(const T&, int);
    foo(foobars, FooBar()); //std::vector<T> foo(const std::vector<T>&, const T&);
    foo(2.3, 2LL); //T foo(const T&, const T&);
    foo('c', 1);

----

Function template deduction
===========================

.. code:: C++

    template<typename T> 
    T max(T, T);

    const int s = 7;

    void k(){
        max(1,2); //max<int>(1,2)
        max('a', 'b'); //max<char>('a', 'b')
        max(2.7, 4.9); //max<double>(2.7, 4.9)
        max(s, 7); //max<int>(int{s}, 7) (trivial conversion used)

        max('a', 1); //error: ambiguous: max<char, char>() or max<int, int>()?
        max(2.7, 4); //error: ambiguous: max<double, double>() or max<int, int>()?
    }

----

Argument Substitution Failure. 
==============================

.. code:: C++

    template<typename itr>
    typename itr::value_type mean(itr first, itr last) {
        typename itr::value_type tmp = 0;
        for(auto it = first; it != last; ++it)
            tmp += *it;
        return tmp / (std::distance(first, last));
    }

    int main()
    {
        std::vector<int> vecNums{ 1,2,3,4,5,6,7,8,9,10 };
        int arrayNums[] = { 1,2,3,4,5,6,7,8,9,10 };

        std::cout << "The mean of vecNums is " << mean(vecNums.begin(), vecNums.end()) << "\n";
        
        //int* doesn't have a member called value type. 
        std::cout << "the mean of arrayNums is " << mean(arrayNums, arrayNums + 10) << "\n"; 
    }

----

SFINAE
======

Substitution Failure Is Not An Error
------------------------------------

In the previous example the call to ``mean`` with pointers passed in failed because there is no such thing as a ``int*::value_type``. However, what if we defined another mean.

.. code:: C++

    template<typename T>
    T mean(T* first, T* last){
        T tmp = 0;
        for (auto ptr = first; ptr <= last; ++ptr)
            tmp += *ptr;
        return tmp / (last - first);
    }

This works even though the first definition of mean fails. That is because the language has a rule that states that **substitution failure** is not an error. It simply causes that template to be ignored; that is, the template does not contribute a specialization to the overload set. 

Without the SFINAE rule we would get compile-time errors even when error-free alternatives exist.

----

Concept-like
============

Concepts are a C++ feature that will be coming some time in the future that will allow us to be more granular in our allowed template parameters. Until they arrive we have to rely on template metaprograms to achieve the same effect. 