/* File : GF_ACTIONS.h
 * Author : dimos katsimardos
 * Created on April 1, 2017, 10:36 AM
 * Edited on April 5, 2017, 11:49 PM
 *
 * Header File with a GF(256) class for
 * adding | multiplying | power of two
 * Galois Field 256 elements (rs_power same as mul)
 * Also, 2 functions for identification if more errors
 * than algorithm correction capability have occurred.
 */
#ifndef GF_ACTIONS_H
#define GF_ACTIONS_H


template <class T>
class GF256 {
//private:
    //T num1;
    //T num2;
public:
    GF256() {}
    //GF256 (const T& a, const T& b) : num1(a), num2(b) {}
    ~GF256() {}
    T add(const T& a, const T& b);
    T mul(const T& a, const T& b);
    T rs_power(const T& base, const T& p);
    void ErrDet(T *beta, T ers, T mode);
    void Exception(T *beta, T ers);
    void foo(void) {}
};


template <class T>
T GF256<T>::add(const T& a, const T& b) {
    return (a+b)%255;
}

template <class T>
T GF256<T>::mul(const T& a, const T& b) {
    return (a*b)%255;
}

template <class T>
T GF256<T>::rs_power(const T& base, const T& p) {
    return (base*p)%255;
}

template <class T>
void GF256<T>::ErrDet(T *beta, T ers, T mode) {
    const int N=4;
    int grade[N] = {10, 7, 17, 13};
    int MaxErs[N] = {4, 2, 8, 6};
    for(int i=0;i<ers+1;i++) {
        if(beta[i]>grade[mode]-1) {
            cout << "More than " << MaxErs[mode] << " errors detected. Out of Algorithm capabilities. Aborting...\n";
            exit(1);
        }
    }
}

template <class T>
void GF256<T>::Exception(T *beta, T ers) {
    T sum=0;
    for(int i=0;i<ers+1;i++) {
        sum+=beta[i];
    }
    cout << (!sum ? "Only Error Detection. Errors out of bounds. Aborting...\n" : "\n");
    (!sum) ? exit(1) : foo();
}
#endif /* GF_ACTIONS.h */
