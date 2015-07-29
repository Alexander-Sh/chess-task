#ifndef UTILS_H
#define UTILS_H


namespace Utils {
    template<typename T>
    T abs(const T& val) {
        return val > 0 ? val : val * -1;
    }
}

#endif // UTILS_H
