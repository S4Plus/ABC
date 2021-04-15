#include <CL/sycl.hpp>
#include "dpc_common.hpp"

using namespace std;
using namespace sycl;



// Matrix size constants.
constexpr int M = 1280;
constexpr int N = 1280;
constexpr int P = 1280;


template <typename T>
static T rand_uniform_01() {
  return T(std::rand()) / T(RAND_MAX);
}


template <typename T>
static std::vector<T> make_random_matrix(int m, int n) {
  std::vector<T> matrix(m * n);
  std::generate_n(matrix.data(), matrix.size(), rand_uniform_01<T>);
  return matrix;
}


int main() {

  

  // make random matrix
  std::cout << "make random matrix" << std::endl;
  auto A = make_random_matrix<float>(M, N);
  auto B = make_random_matrix<float>(N, P);
  auto C = std::vector<float>(M * P, 0);

  

  // build buffer for matrix
  std::cout << "build buffer for matrix" << std::endl;
  buffer buf_a{A.data(), range<2>{M, N}};
  buffer buf_b{B.data(), range<2>{N, P}};
  buffer buf_c{C.data(), range<2>{M, P}};

  queue q{property::queue::enable_profiling{}};

  cout << "Device: " << q.get_device().get_info<info::device::name>() << "\n";

  std::cout << "submit the task" << std::endl;
  event e =  q.submit([&](handler &cgh){
    auto a = buf_a.get_access<access::mode::read>(cgh);
    auto b = buf_b.get_access<access::mode::read>(cgh);
    auto c = buf_c.get_access<access::mode::write>(cgh);
    cgh.parallel_for(range<2>{M, P}, [=](id<2> index){
      size_t row = index[0];
      size_t col = index[1];
      float val = 0.0;
      for (size_t i = 0; i < N ; i++){
        val += a[row][i] * b[i][col];
      }
      c[row][col] = val;
    });
  });
  q.wait();
  double etime = e.get_profiling_info<info::event_profiling::command_end>() - 
    e.get_profiling_info<info::event_profiling::command_start>();
  int iteration = M*P;
  double msecPerMatrixMul = etime / iteration;
  double gflops = (2 * M* N) / (msecPerMatrixMul / 1000.f)* P * 1.0e-9f;
  printf("GEMM performance with DPAS : M = %d, P = %d, N = %d, compute time = %.3lf msec, Gflops = %.3lf\n",
                                         M, P, N, msecPerMatrixMul, gflops);


  std::cout << "Done" << std::endl;

  return 0; 
}