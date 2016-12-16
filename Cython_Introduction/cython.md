#Cython 基础 详解
##Cython helloworld 介绍
* 首先创建 helloworld.pyx:   
```python
cdef extern from"stdio.h":
    extern int printf(const char *format, ...)  
def SayHello():
    printf("hello,world\n")
```  
* 如何编译：（在helloworld 目录下新建一个setup.py文件）
```python
from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize
setup(
  name = 'helloworld',
  ext_modules=cythonize([
    Extension("helloworld", ["helloworld.pyx"]),
    ]),
)
```
* 编译命令：
``` bash
python setup.py build
```
* 实际效果
``` bash
>>>import helloworld 
>>>helloworld.SayHello() 
hello,world
```
##Cython 类型
###类型定义  
* 定义一个C变量
``` python
    cdef int an[10]
    cdef int n = 123
    cdef int *pn = &n
    printf("%d \n",pn[0])
```
不同的地方就是在声明的最前面要加上cdef,另外，末尾不用加分号;
* 定义python变量
```python
b = [1,2,3]
a = 'hello,world'
```
与python无区别
###类型转换
```python
cdef float a= 123.456
cdef int b
b = <int>a
```
**在Cython里用<>替代了()来进行类型转换**<br>
注意问题：  
**以Cython里不能用类似*ptr这样代码来对指针变量进行取值，而必须用ptr[0]这样的形式**<br>
##Cython 函数编写
* python 方法（add.py）
```python
import math
import time
def big_circle(lon1, lat1, lon2, lat2):
    radius = 3965 
    x = math.pi / 180.0
    a = (90.0 - lat1) * (x)
    b = (90.0 - lat2) * (x)
    theta = (lon2 - lon1) * (x)
    c = math.acos(math.cos(a) * math.cos(b) +
        math.sin(a) * math.sin(b)*math.cos(theta))
    return c*radius
lon1, lat1, lon2, lat2 = -72.345, 34.323, -61.823, 54.826
st = time.time()
for i in range(5000000):
    result = big_circle(lon1, lat1, lon2, lat2)
print time.time() - st
```
* cython 代码(add.pyx)
``` python
    float cosf(float theta)  
    float sinf(float theta)  
    float acosf(float theta)
cimport cython
import time
def big_circle(float lon1,float lat1,float lon2,float lat2):  
    cdef float radius = 3956.0  
    cdef float pi = 3.14159265  
    cdef float x = pi/180.0  
    cdef float a,b,theta,c  
    a = (90.0-lat1)*(x)  
    b = (90.0-lat2)*(x)  
    theta = (lon2-lon1)*(x)  
    c = acosf((cosf(a)*cosf(b)) + (sinf(a)*sinf(b)*cosf(theta)))   
    return radius*c  
def test_add():
    lon1, lat1, lon2, lat2 = -72.345, 34.323, -61.823, 54.826
    st = time.time()
    for i in range(5000000):
        result = big_circle(lon1, lat1, lon2, lat2)
    print time.time() - st
    
 ```
 **注意这里只讨论python 调c的情况**<br>
###Cython 结构体，枚举等其他类型<br>
####结构体(1)
 ```
 cdef struct AB:
     int a
     int b
 
  def StructTest():
   cdef AB ab
   ab.a = 1
   ab.b = 2
   return ab
   ```
**注意：**  
**Cython里没有->的操作符，用"."替代"->"**  
**Cython里不能用*来对指针变量取值，用[0]替代**
####结构体(2)
 ```python
 cdef struct AB:
    int a
    int b
    or
 ctypedef struct AB:
    int a
    int b
def StructTest():
    cdef AB ab
    cdef AB *pAB = &ab
    pAB.a = 1
    pAB.b = 2
    return pAB[0]
    ```
  **说明：**
  **Cython里结构体的定义比较像C++的语法，即在声明一个结构体变量时不用在结构体名前再加上struct关键字**<br>
  **在C,C++代码里，返回一个结构体变量时，会把结构体转成Python的dict对象**
####枚举
  ```python
  cdef enum MyEnum:
    a
    b = 2
    c = 5
    or
    ctypedef enum MyEnum:
    a
    b = 2
    c = 5
  ```
####Cython里调用外部定义的函数和结构体、枚举
  ```python
  cdef extern from "image.h":
    ctypedef struct image:
        unsigned int height; 
        unsigned int width;      
  ```
*注意：*
**定义的结构体以及枚举类型可集中写在一个头文件内，后缀为.pyd**
###Cython 中的类(用法同函数)
  
  
    









