#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "intarray.h"
#include "thread_safe_stack.h"
#include "tokenizer.h"

namespace py = pybind11;

PYBIND11_MODULE(cpp_core, m) {
    m.doc() = "C++ 高性能基础库 - IntArray + ThreadSafeStack + Tokenizer";

    // IntArray
    py::class_<IntArray>(m, "IntArray")
        .def(py::init<size_t>())
        .def("size", &IntArray::size)
        .def("__repr__", [](const IntArray& a) {
            return "<IntArray size=" + std::to_string(a.size()) + ">";
        });

    // ThreadSafeStack
    py::class_<ThreadSafeStack<int>>(m, "ThreadSafeStack")
        .def(py::init<>())
        .def("push", &ThreadSafeStack<int>::push)
        .def("pop", &ThreadSafeStack<int>::pop)
        .def("empty", &ThreadSafeStack<int>::empty)
        .def("__repr__", [](const ThreadSafeStack<int>& s) {
            return "<ThreadSafeStack empty=" + std::string(s.empty() ? "true" : "false") + ">";
        });

    // Tokenizer
    py::class_<Tokenizer>(m, "Tokenizer")
        .def(py::init<>())
        .def(py::init<const std::unordered_set<std::string>&>())
        .def("load_dict", &Tokenizer::load_dict)
        .def("tokenize", &Tokenizer::tokenize)
        .def("__repr__", [](const Tokenizer&) {
            return "<Tokenizer>";
        });
}
