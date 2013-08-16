#include <iostream>
#include <random>
#include <chrono>
#include <cstdlib>

namespace{
  class mtrand_t{
    std::mt19937 mt_;
    public:
    inline std::string name() const{ return "mt19937"; }
    inline double      zeroonerand() const { std::uniform_real_distribution<double> gen(0.0, 1.0); return gen(mt_);} 
    inline size_t      nrand(size_t n_rand) const { std::uniform_int_distribution<size_t> gen(0, n_rand-1); return gen(mt_);} 
    inline void        seed(int seed) { mt_.seed(seed); } 
  };
  class crand_t{
    public:
    inline std::string name() const{ return "std::rand"; }
    inline double      zeroonerand() const { return (((double) std::rand()-1)/((double)RAND_MAX)); } 
    inline size_t      nrand(size_t n_rand) const{ return n_rand * zeroonerand();}
    inline void        seed(int seed) { std::srand(seed); }
  };

  class zeroone_caller_t{
    public:
    template<class P>
    inline void call(const P randgen) const{ randgen->zeroonerand( ); }
  };
  class nrand_caller_t{
    size_t n_rand_;
    public:
    template<class P>
    inline void call(const P randgen) const{ randgen->nrand( n_rand_); }
    inline size_t n_rand()const{ return n_rand_;}
    inline void set_n_rand(size_t n_rand){ n_rand_ = n_rand; }
    nrand_caller_t(size_t n_rand): n_rand_(n_rand) {}
  };
  template<class T, class CALLER>
  void measure_time(T* randgen, const CALLER& caller, size_t n_iter, int seed){
    randgen->seed(seed);
    auto start = std::chrono::system_clock::now();
    for(size_t i = 0 ; i < n_iter ; ++i){  caller.call(randgen);  }
    auto d = std::chrono::system_clock::now() - start;
    std::cout << randgen->name() << ":\t" << std::chrono::duration<double>(d).count() << std::endl;
  }
}// namespace

int main(int argc, char* argv[]){
  size_t n_iter = 1000*1000*1000;
  size_t n_rand = 1000;
  zeroone_caller_t zeroone_caller;
  crand_t crand;
  measure_time(&crand, zeroone_caller, n_iter,  7);
  mtrand_t mtrand;
  measure_time(&mtrand, zeroone_caller, n_iter, 7);

  nrand_caller_t nrand_caller(n_rand);
  measure_time(&crand, nrand_caller, n_iter,  7);
  measure_time(&mtrand, nrand_caller, n_iter, 7);
}
