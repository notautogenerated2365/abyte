This header defines a class called abyte which wraps an unsigned char
to provide additional functionality.

There are two main functional additions to an abyte: operator[] and operator().
operator[] takes an int argument from 0 to 7, and returns the bit at that index.
This can be used just like operator[] on an array, in that it can read or assign the value.
operator() takes two int parameters, a start index and an end index,
and returns an object similar to a std::vector that can be accessed or modified,
just like with operator[].

Example usage:

int main() {
    abyte x = 16;
    std::cout << x[4]; // 1
    x[4] = 0;
    std::cout << +x; // 0
    x(0, 4) = {1, 1, 1, 1}; // (startIndex (inclusive), endIndex (exclusive))
    std::cout << +x; // 15
}

The object returned by operator() is implicitly convertable to a std::vector<bool>,
but any changes made to the std::vector<bool> will not be reflected in the abyte.