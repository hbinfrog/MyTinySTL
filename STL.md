### 一些忘记的语法知识

#### static（C++）

- 被 static 修饰的变量属于类变量，可以通过类名.变量名直接引用，而不需要new出一个类来。类方法同理

- 被 static 修饰的变量、被 static 修饰的方法统一属于类的静态资源，是类实例之间共享的，**换言之，一处变、处处变。静态成员是属于整个类的而不是某个对象**，静态成员变量只存储一份供所有对象共用。所以在所有对象中都可以共享它

  ```c++
  class Point  
  {  
  public:   
      void init()  
      {    
      }  
      static void output()  
      {  
      }  
  };  
  void main()  
  {  
      //Point::init(); //报错 
      Point::output();  
  }
  ```

  

#### new

- placement new 调用构造函数：在一个分配好的空间，调用构造函数，创建一个类

  语法：new (T1*) T1(T2 &)

  ```c++
  void construct(T1* p, const T2& value){
    new (p) T1 (value);//开辟一个p的数据，将value强行转换为T1
  }
  
  
  class A{
    int a;
  };
  int buf[sizeof(A)];//在栈上分配一个数组
  A *obj = new (buf) A();//在这个数组构造一个A的对象，这里调用了默认构造函数
  ```

#### 类型转换

- static_cast是一个强制类型转换操作符。强制类型转换，也称为显式转换。编译器隐式执行的任何类型转换都是通过static_cast来实现

  ```c++
  T2 value;
  T1 a = static_cast<T1>(value);//将value转换为T1型，并且用a来接收
  ```

- dynamic_cast

- const_cast: const_cast < type-id > ( expression ) 主要是用来去掉const属性，当然也可以加上const属性。

  ```c++
  const T* a;
  T* b = const_cast<T *>(a);//const T* ->T*
  ```

  

- reinterpret_cast

### 变参数模板

c++11中新增加了一项内容，叫做变参数模板，所谓变参数模板，顾名思义就是参数个数和类型都可能发生变化的模板

#### 模板形参包

```c++
template<int ... data>
```

类型选择一个固定的类型，data其实是一个可修改的参数名，注意：**这个固定的类型是有限制的，标准c++规定，只能为整型、指针和引用。**

一个例子

```c++
#include <iostream>
using namespace std;

//这里加一个空模板函数是为了编译可以通过，否则编译期间调用printAmt<int>(int&)就会找不到可匹配的函数
//模板参数第一个类型实际上是用不到的，但是这里必须要加上，否则就是调用printAmt<>(int&)，模板实参为空，但是模板形参列表是不能为空的
template<class type>
void printAmt(int &iSumAge)
{
    return;
}

template<class type, int age0, int ... age>
void printAmt(int &iSumAge)
{
    iSumAge += age0;
    //这里sizeof ... (age)是计算形参包里的形参个数，返回类型是std::size_t，后续同理
    if ( (sizeof ... (age)) > 0 )
    {
        //这里的age...其实就是语法中的一种包展开，这个后续会具体说明
        printAmt<type, age...>(iSumAge);
    }
}

int main()
{
    int sumAge = 0;
    printAmt<int,1,2,3,4,5,7,6,8>(sumAge);
    cout << "the sum of age is " << sumAge << endl;
    return 0;
}
```

#### 类型模板形参包

```c++
template<typename... _Args>
```

### noexcept



### explicit

在c++中explicit关键字只能用来修饰构造函数。使用explicit可以禁止编译器自动调用拷贝初始化，还可以禁止编译器对拷贝函数的参数进行隐式转换。

```c++
class A{
	public:
	explicit A(int x){
		cout<<"我被用了"<<endl;
	}
};


int main(){
  A a1(10);
  A a2 = 10;//错误，
}
```

### 右值引用

左值是指表达式结束后依然存在的持久化对象，右值是指表达式结束时就不再存在的临时对象。右值不具有名，如12，"123"等

- 右值引用，用以引用一个右值，可以延长右值的生命期

```c++
int&& a = 123;
int&& b = std::move(a);
int&& c = a;//error, a is lvalue 

int i = 0;
int&& j = i++;
int&& k = ++i;//error: cannot bind ‘int’ lvalue to ‘int&&’
int& m = i++;//error: invalid initialization of non-const reference of type ‘int&’ from an rvalue of type ‘int’
int& n = ++i;
```

#### std::move()

```c++
T b = std::move(a);
//移动之后，b指向a的资源，a资源为空
```

源代码

```c++
constexpr typename std::remove_reference<_Tp>::type&& move(_Tp&& __t) noexcept
{ return static_cast<typename std::remove_reference<_Tp>::type&&>(__t); }
//move并没有”移动“什么内容，只是将传入的值转换为右值。
```



### allocator





### 进度

#### 2022.04.01

暂时未完成，后续需要补充的部分：

construct.h第52行

allocator.h第73行

type_traits.h

## 问题

vector.h重载等号运算符时，传入参数为右值引用的vector时，前两句话似乎没有作用（可能是int型比较特殊）

vector.h重载等号运算符时，传入参数为左值引用时，所见的数据是一样的，但是在[end_, cap_)	拷贝有一点bug；以及为什么const vector& rhs使用rhs的参数时会报错呢？

vector.h中关于emplace_back和push_back的区别，以及与insert的差别？
