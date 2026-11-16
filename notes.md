10.28
这是一个C++基础功能库，从benchmark可以看到同一功能由python和c++实现的对比
submit负责传入入局，具体实现如下：传入函数后通过bind捆绑由forward移动拷贝的数据形成数据集，由于package的包装不能拷贝之能赋值，所以显示赋值给智能指针来实现拷贝，通过get_future进入异步状态，由tasks_处正式加入线程池队列，之后正式执行线程时result才能得到具体结果
“能说出 submit 的五步链路，但 packaged_task 为什么不可拷贝、future 的共享状态是什么，还讲不清。明天用最小 demo 验证 broken_promise

10.29
yuandu@LAPTOP-7IJ5EP6P:~/cpp_base$ g++ -std=c++11 test.cpp -o test -pthread
./test
42
yuandu@LAPTOP-7IJ5EP6P:~/cpp_base$ g++ -std=c++11 test.cpp -o test -pthread
./test
terminate called after throwing an instance of 'std::future_error'
  what():  std::future_error: Broken promise
Aborted (core dumped)
拷贝构造要求内存分配空间并拷贝存入数据，移动构造借助右值引用不必重新分配内存将数据移动拷贝到数组中
move比copy慢，这应该是因为在处理较少量数据时python用copy比c++用move更快
✅ “跑通了 broken_promise，能说清 future 的共享状态由 packaged_task 写入。拷贝/移动的区别能讲，但漏了源对象置空和 noexcept。Move 慢的原因还没定位，怀疑 benchmark 构造不对称，明天读 benchmark_main.cpp 验证。”