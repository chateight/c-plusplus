#include <iostream>
#include <vector>

//
// simple function template
class S
{
public:
    template <typename T> 
    T foo(T value)
    {
       return value;
    }
};

//
// class template
template <typename CT, typename DT, typename ET>
class ct{

public:
    CT val;
    DT val1;
    ET val2;

    void ct_f();
};

template <typename CT, typename DT, typename ET>
void ct<CT, DT, ET>::ct_f(){
    std::cout << val << " : " << val1<< std::endl;
}

template <>                     // explicit specialization for the class
class ct<void, void, void>{

public:

    void ct_f();
};

void ct<void, void, void>::ct_f(){

    std::cout << "class template \"void\" (explicit specialization)" << std::endl;
}

template <typename CT, typename ET>
class ct<CT, void, ET>{

public:
    CT val;
    ET val1;

    void ct_f(){

    std::cout << val << " : " << val1 << std::endl;
    }
};

//
// use virtual function in a class template
class ctv{

public:
    virtual void show(float fl){
        std::cout << "base : " << fl << std::endl;
    }

};

template <typename T>
class d_ctv : public ctv{

public:
    void show(T vbl) override{
        std::cout << "override : " << vbl << std::endl;
    }

};


//
// class template as a parameter of the function
template <typename T>
class SC{

public:
T val;
};

template <typename R>
void show(SC<R> ct){
    std::cout << "sqr = " << (ct.val)*(ct.val) << std::endl;
}

//
// forwarding reference (righthand ref & template)
template <typename T>
void foober(T& val){
    std::cout << " lefthand reference : " << val << std::endl;
}

template <typename T>
void foober(T&& val){
    std::cout << " righthand reference : " << val << std::endl;
}

//
// call by reference(with constant)
template < typename TR>
void footr(const TR& val){
    std::cout << val << std::endl;

}

//
// alias template for dependent name(1)
template <typename T>
using vec_ref = typename std::vector<T>::reference; // alias definition

template <typename T> 
//typename std::vector<T>::reference                // when alias is defined, typename definition is not required
vec_ref<T> func(std::vector<T>& v, std::size_t i){

    return v[i];
}

template <typename T>
typename std::size_t 
size(const std::vector<T>& v){

    return v.size();
}

//
// alias template for dependent name(2)
template <typename T>
class Alias
{
    T value;

public:
    using value_type = T;
    explicit Alias(T value) : value(value) {}

    T& get_value() { return value; }
};

template <typename T>
using A_value = typename Alias<T>::value_type;

template <typename T>
A_value<T>& get(Alias<T>& a)
{
    return a.get_value();
}

//
// variadic template
template <typename... Args>
void vt()
{
    std::cout << "no arg func is called\n" << std::endl;
}

template <typename Body>
void vt(Body head)
{
    std::cout << "one arg func is called : " << head << "\n" << std::endl;
}

template <typename Body>            // this accepts only one type argument
void vt(Body first, Body second )
{
    std::cout << "two args func is called : " << first << ", " << second << "\n" << std::endl;
}

template <typename Body1, typename Body2>   // this accepts different type arguments
void vt(Body1 first, Body2 second )
{
    std::cout << "two diffferent type of args func is called : " << first << ", " << second << "\n" << std::endl;
}

template <typename Head, typename... Body>
void vt(Head head, Body... body){
    std::cout << "argument head : " << head << "\t" << "size of the residual arguments : " << sizeof...(body) << "\n";
    vt(body...);        // if there are overload functions, "vt(body...)" calls these
}

//
// std::forward
template <typename... T>
void base_f(T&&... args){
    std::cout << "number of args : " << sizeof...(T) << std::endl;
}

template <typename... T>
void call_b(T&&... args){

    base_f(std::forward<T>(args)...);       // forward keeps lhs & rhs

}

//
// generic lambda
auto g_lambda = [](const auto& val)
{
    std::cout << " argument : " << val << std::endl;
};

//
// class template argument deduction
template <typename T>
class ded{

    T var;

public:
    ded(T var): var(var){};

    void show(){
        std::cout << var << std::endl;
    }
};


//
// using predicate to check the type traits
template <typename T>
void p_arg(T a){
    static_assert(std::is_integral<T>::value, " arg is not integer");
    std::cout << "arg : " << a << std::endl;

}


//
// main routine from here
int main()
{
    S s;

    std::cout << "----- template function -----" << std::endl;
    std::cout << " int : "<< s.foo<int>(5) << std::endl;
    std::cout << " float : " << s.foo<float>(53.238f) << std::endl;
    std::cout << " char : " << s.foo<char>('d') << std::endl;
    std::cout << " string : " << s.foo<std::string>("hello") << std::endl;

    std::cout << " string : " << s.foo("hello, using TYPE DEDUCTION ") << std::endl;

    std::cout << std::endl;
    std::cout << "----- template class -----" <<std::endl;
    ct<float, float, float> tc{52.3f, 12.5f, 45.2f};
    tc.ct_f();

    ct<std::string, std::string, std::string> tc_s{"bigger", "smaller", "middle"};
    tc_s.ct_f();

    ct<std::string, void, std::string> tsv{"partial specilization", "middle"};
    tsv.ct_f();

    ct<void, void, void> tc_v;
    tc_v.ct_f();

    std::cout << std::endl;
    std::cout << "----- using virtual function in a template class -----" <<std::endl;
    d_ctv<float> vin;
    vin.show(28);
    vin.ctv::show(21);
    static_cast<ctv&>(vin).show(27);        // call via virtual function( result is same as above )

    std::cout << std::endl;
    std::cout << "----- type deduction (class template as a parameter of the function) -----" <<std::endl;
    SC<int> ctp{10};
    show(ctp);

    std::cout << std::endl;
    std::cout << "----- forwarding reference (righthand ref & template) -----" <<std::endl;
    int fr = 10;

    foober(fr);
    foober(52);

    std::cout << std::endl;
    std::cout << "----- call by reference (with const) -----" <<std::endl;
    const int crf = 29;
    footr(crf);         // type deduction is applicable
    footr<int>(crf);    // same as above
    //footr<int&>(crf); -> error

    std::cout << std::endl;
    std::cout << "----- typename definition -> alias(1) -----" <<std::endl;

    std::vector<int> v = {0, 3, 5, 7};
    std::vector<int>::reference r = func(v, 1);
    std::cout << "vector element : " << r << std::endl;


    std::cout << std::endl;
    std::cout << "----- using alias to define the type explicitly(2) -----" <<std::endl;

    Alias<int> a{42};
    std::cout << "using alias : " << get(a) << std::endl;

    std::cout << std::endl;
    std::cout << "----- variadic template -----" <<std::endl;

    vt();
    vt<int>(0);                 // "<int>" is redundant
    vt<int>(1, 2, 3, 4, 5);
    vt("ab", "cd", "ef", "gh", 99); // type deduction is applicable

    std::cout << std::endl;
    std::cout << "----- forward all args -----" <<std::endl;
    call_b(1, 2, 3, 4, 5, 6);


    std::cout << std::endl;
    std::cout << "----- generic lambda -----" <<std::endl;
    g_lambda(7);
    g_lambda("string");

    std::cout << std::endl;
    std::cout << "----- class template argument deduction -----" <<std::endl;

    ded dd{"class parameter deduced"};
    dd.show();

    ded dd1{3.1415f};
    dd1.show();
 
    std::cout << std::endl;
    std::cout << "----- using predicate to check the arg type -----" <<std::endl;
    p_arg(10);
    //p_arg(5432.3f);   // compile error since accept only integer

 }
