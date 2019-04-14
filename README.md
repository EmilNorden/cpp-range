# cpp-range

cpp-range is my attempt at implementing some of the extension methods from C# in C++ (Take, Where, Skip, etc).

The goal is for the functions to be:
* Generic (Work on any container type)
* Lazy (should do minimal amount of work until being iterated)
* Stateless (No internal state, which means that when the source container updates, the result of iteration also changes)

Some functions (GroupBy etc) proved to be very difficult (impossible?) due to the difference in design of iterators in C# vs C++.
