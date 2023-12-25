// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <utility>
#include <type_traits>
#include <limits>
#include <vector>

#include<benchmark/benchmark.h>

// std::vector<signed char> v1 = make_vec<signed char>();
// std::vector<unsigned char> v1 = make_vec<unsigned char>();
// std::vector<short> v1 = make_vec<short>();
// std::vector<unsigned short> v1 = make_vec<unsigned short>();
// std::vector<int> v1 = make_vec<int>();
// std::vector<unsigned int> v1 = make_vec<unsigned int>();
// std::vector<long> v1 = make_vec<long>();
// std::vector<unsigned long> v1 = make_vec<unsigned long>();
// std::vector<long long> v1 = make_vec<long long>();
// std::vector<unsigned long long> v1 = make_vec<unsigned long long>();

// <-- new implimentaion of the cmp less -->
template <typename _Tp, typename _Up>
constexpr bool cmp_less_new(_Tp __t, _Up __u) noexcept {
  if constexpr (sizeof(_Tp) < sizeof(long long) && sizeof(_Up) < sizeof(long long))
    return static_cast<long long>(__t) < static_cast<long long>(__u);
  else if constexpr (is_signed_v<_Tp> == is_signed_v<_Up>)
    return __t < __u;
  else if constexpr (is_signed_v<_Tp>)
    return __t < 0 ? true : make_unsigned_t<_Tp>(__t) < __u;
  else
    return __u < 0 ? false : __t < make_unsigned_t<_Up>(__u);
}


// <-- function to create vector of each type -->
template<typename T>
std::vector<T> make_vec(){
    const T min_value = std::numeric_limits<T>::min();
    const T max_value = std::numeric_limits<T>::max();
    const T inc = max_value/100;
    vector<T> vec;
    for (T i = min_value; i < max_value; i += inc) {
        vec.push_back(i);
    }
    vec.push_back(max_value);
    return vec;
}


// <-- all the int types -->
std::tuple<
    unsigned long long, long long, unsigned long, long, unsigned int, int,
    unsigned short, short, unsigned char, signed char> types;



// <-- testing  -->
// <-- the test implimentation by the new cmp less -->
template<class... Ts, class UTuple>
void test_new(std::tuple<Ts ...>&,const UTuple& utuple){
    (test_impl_new<Ts>(utuple), ...);
}

template <class... Ts,class... Us>
void test_impl_new(std::tuple<Us ...>&){
    (test_cmp_less_new<T,Us>(), ...);
}

template <typename T, typename U>
void test_cmp_less_new() {
    std::vector<T> tvec = make_vec<T>;
    std::vector<U> uvec = make_vec<U>;

    for(int i = 0 ; i < T.size() && i < U.size() ; i++){
        cmp_less_new(tvec[i],uvec[i]);
        cmp_less_new(tvec[i],uvec[uvec.size() - i]);
    }
}

static void BM_cmp_less_new(benchmark::State& state){
    test_new(types,types);
}

// <-- implentation for the std cmp less -->
template<class... Ts, class UTuple>
void test_std(std::tuple<Ts ...>&,const UTuple& utuple){
    (test_impl_std<Ts>(utuple), ...);
}

template <class... Ts,class... Us>
void test_impl_std(std::tuple<Us ...>&){
    (test_cmp_less_std<T,Us>(), ...);
}

template <typename T, typename U>
void test_cmp_less_std() {
    std::vector<T> tvec = type_vec_of<T>;
    std::vector<U> uvec = type_vec_of<U>;

    for(int i = 0 ; i < T.size() && i < U.size() ; i++){
        std::cmp_less(tvec[i],uvec[i]);
        std::cmp_less(tvec[i],uvec[uvec.size() - i]);
    }

}

static void BM_cmp_less_std(benchmark::State& state){
    for(auto _ : state){
        benchmark::DoNotOptimize(test_std(types,types));
    }
}

BENCHMARK(BM_cmp_less_std);
BENCHMARK_MAIN();

// template <typename T>
// static void MyFuncBenchmark(benchmark::State& state) {
//     const T min_value = std::numeric_limits<T>::min();
//     const T max_value = std::numeric_limits<T>::max();
//     T inc = max_value/100;

//     vector<T> fwd,re;

//     for (auto _ : state) {
//         // Benchmark the myFunc function with input values from min to max
//         for (T i = min_value; i < max_value; ++inc) {
//             fwd.push_back(i);
//             //benchmark::DoNotOptimize(cmp_less_old(i)); // Prevent the compiler from optimizing away the function call
//         }
//         fwd.push_back(max_value);
        
//     }
// }

#include <utility>
#include <type_traits>
#include <limits>
#include <vector>

#include<benchmark/benchmark.h>

// std::vector<signed char> v1 = make_vec<signed char>();
// std::vector<unsigned char> v1 = make_vec<unsigned char>();
// std::vector<short> v1 = make_vec<short>();
// std::vector<unsigned short> v1 = make_vec<unsigned short>();
// std::vector<int> v1 = make_vec<int>();
// std::vector<unsigned int> v1 = make_vec<unsigned int>();
// std::vector<long> v1 = make_vec<long>();
// std::vector<unsigned long> v1 = make_vec<unsigned long>();
// std::vector<long long> v1 = make_vec<long long>();
// std::vector<unsigned long long> v1 = make_vec<unsigned long long>();

// <-- new implimentaion of the cmp less -->
template <typename _Tp, typename _Up>
constexpr bool cmp_less_new(_Tp __t, _Up __u) noexcept {
  if constexpr (sizeof(_Tp) < sizeof(long long) && sizeof(_Up) < sizeof(long long))
    return static_cast<long long>(__t) < static_cast<long long>(__u);
  else if constexpr (std::is_signed_v<_Tp> == std::is_signed_v<_Up>)
    return __t < __u;
  else if constexpr (std::is_signed_v<_Tp>)
    return __t < 0 ? true : std::make_unsigned_t<_Tp>(__t) < __u;
  else
    return __u < 0 ? false : __t < std::make_unsigned_t<_Up>(__u);
}


// <-- function to create vector of each type -->
template<typename T>
std::vector<T> make_vec(){
    const T min_value = std::numeric_limits<T>::min();
    const T max_value = std::numeric_limits<T>::max();
    const T inc = max_value/1000;
    std::vector<T> vec;
    for (T i = min_value; i < max_value; i += inc) {
        vec.push_back(i);
    }
    vec.push_back(max_value);
    return vec;
}


// <-- all the int types -->
std::tuple<
    unsigned long long, long long, unsigned long, long, unsigned int, int,
    unsigned short, short, unsigned char, signed char> types;



// <-- testing  -->
// <-- the test implimentation by the new cmp less -->

template <typename T, typename U>
void test_cmp_less_new() {
    std::vector<T> tvec = make_vec<T>();
    std::vector<U> uvec = make_vec<U>();

    for(int i = 0 ; i < tvec.size() && i < uvec.size() ; i++){
        cmp_less_new(tvec[i],uvec[i]);
        cmp_less_new(tvec[i],uvec[uvec.size() - i]);
    }
}

template <class Ts,class... Us>
void test_impl_new(std::tuple<Us ...>&){
    (test_cmp_less_new<Ts,Us>(), ...);
}

template<class... Ts, class UTuple>
void test_new(std::tuple<Ts ...>&,UTuple& utuple){
    (test_impl_new<Ts>(utuple), ...);
}

// <-- implentation for the std cmp less -->

template <typename T, typename U>
void test_cmp_less_std() {
    std::vector<T> tvec = make_vec<T>();
    std::vector<U> uvec = make_vec<U>();

    for(int i = 0 ; i < tvec.size() && i < uvec.size() ; i++){
        std::cmp_less(tvec[i],uvec[i]);
        std::cmp_less(tvec[i],uvec[uvec.size() - i]);
    }

}

template <class Ts,class... Us>
void test_impl_std(std::tuple<Us ...>&){
    (test_cmp_less_std<Ts,Us>(), ...);
}

template<class... Ts, class UTuple>
void test_std(std::tuple<Ts ...>&,UTuple& utuple){
    (test_impl_std<Ts>(utuple), ...);
}

static void BM_cmp_less_std(benchmark::State& state){
    for(auto _ : state){
        test_std(types,types);
    }
}
BENCHMARK(BM_cmp_less_std);


static void BM_cmp_less_new(benchmark::State& state){
    for(auto _ : state){
        test_new(types,types);
    }
}
BENCHMARK(BM_cmp_less_new);

/*
static void StringCreation(benchmark::State& state) {
  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    std::string created_string("hello");
    // Make sure the variable is not optimized away by compiler
    benchmark::DoNotOptimize(created_string);
  }
}
// Register the function as a benchmark
BENCHMARK(StringCreation);

static void StringCopy(benchmark::State& state) {
  // Code before the loop is not measured
  std::string x = "hello";
  for (auto _ : state) {
    std::string copy(x);
  }
}
BENCHMARK(StringCopy);
*/