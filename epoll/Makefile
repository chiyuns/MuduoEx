.PHONY:clean all clean_a
src = $(wildcard ./*.c) #获取当前目录下的所有C源文件
targetsc = $(patsubst %.c, %, $(src)) #利用模式匹配将变量名逐个截断成指定形式 
objc = $(patsubst %.c, %.o, $(src)) #所有的依赖文件列表, 使用空格分隔
 
srcpp = $(wildcard ./*.cpp) #获取当前目录下的C++源文件
targetscpp = $(patsubst %.cpp, %, $(srcpp)) #利用模式匹配将变量名逐个截断成指定形式 
objcpp = $(patsubst %.cpp, %.o, $(srcpp)) #所有的依赖文件列表, 使用空格分隔
 
pp = g++
cc = gcc
 
CPPFLAGS = -I ./
CFLAGS = -g   
#也能使用多个变量,使此来区别链接哪些库！
 
#$(targetsc):%:%.o 表示我要告诉make你需要使用gcc来完成链接操作
 
ifeq ($(findstring .c, $(src)), .c)  
all:$(targetsc)
$(targetsc):%:%.o  
	$(cc)  $(CFLAGS) $< -o $@ 
$(objc):%.o:%.c
	$(cc) $(CPPFLAGS) -c $<
endif
 
#$(targetscpp):%:%.o 表示我要告诉make你需要使用g++来完成链接操作
ifeq ($(findstring .cpp, $(srcpp)), .cpp)
all:$(targetscpp)
$(targetscpp):%:%.o
	$(pp) $(CFLAGS) $< -o $@
$(objcpp):%.o:%.cpp
	$(pp) $(CPPFLAGS) -c $<
endif
 
clean:
	rm -rf $(objc) $(objcpp)
clean_a:
	rm -rf $(objc) $(objcpp)
	rm -rf $(targetsc) $(targetscpp)
