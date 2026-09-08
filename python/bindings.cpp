#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "intarray.h"
#include "thread_safe_stack.h"

namespace py = pybind11;

PYBIND11_MODULE(cpp_core, m) {
    m.doc() = "C++ 高性能基础库 - IntArray + ThreadSafeStack 绑定";

    py::class_<IntArray>(m, "IntArray")
        .def(py::init<size_t>())
        .def("size", &IntArray::size)
        .def("data", &IntArray::data, py::return_value_policy::reference)
        .def("__repr__", [](const IntArray& a) {
            return "<IntArray size=" + std::to_string(a.size()) + ">";
        });

    py::class_<ThreadSafeStack<int>>(m, "ThreadSafeStack")
        .def(py::init<>())
        .def("push", &ThreadSafeStack<int>::push)
        .def("pop", &ThreadSafeStack<int>::pop)  // std::optional<int> 会被 pybind11 自动处理
        .def("empty", &ThreadSafeStack<int>::empty)
        .def("__repr__", [](const ThreadSafeStack<int>& s) {
            return "<ThreadSafeStack empty=" + std::string(s.empty() ? "true" : "false") + ">";
        });
}
