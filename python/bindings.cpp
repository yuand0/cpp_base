#include <pybind11/pybind11.h>
#include "intarray.h"

namespace py = pybind11;

PYBIND11_MODULE(cpp_core, m) {
    m.doc() = "C++ 高性能基础库 - IntArray 绑定";

    py::class_<IntArray>(m, "IntArray")
        .def(py::init<size_t>(), "构造一个 IntArray")
        .def("size", &IntArray::size, "返回数组大小")
        .def("data", &IntArray::data, py::return_value_policy::reference, "返回数据指针（只读）")
        .def("__repr__", [](const IntArray& a) {
            return "<IntArray size=" + std::to_string(a.size()) + ">";
        });
}
