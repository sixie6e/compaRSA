#include <iostream>
#include <vector>
#include <fstream>
#include <csignal>
#include <mkl.h>
#include <omp.h>
#include <iomanip>

const long P = 136279841;
const long N = 1L << 28; 
const std::string CHECKPOINT_FILE = "checkpoint.bin";

double* s_data = nullptr;
long current_iteration = 1;

void save_checkpoint(int signum) {
    std::cout << "\n[Interrupt] Saving state at iteration " << current_iteration << "..." << std::endl;
    
    std::ofstream f(CHECKPOINT_FILE, std::ios::binary);
    if (f.is_open()) {
        f.write(reinterpret_cast<char*>(&current_iteration), sizeof(long));
        f.write(reinterpret_cast<char*>(s_data), N * sizeof(double));
        f.close();
        std::cout << "State saved to " << CHECKPOINT_FILE << ". Exiting." << std::endl;
    }
    
    mkl_free(s_data);
    exit(signum);
}

void load_checkpoint() {
    std::ifstream f(CHECKPOINT_FILE, std::ios::binary);
    if (f.is_open()) {
        f.read(reinterpret_cast<char*>(&current_iteration), sizeof(long));
        f.read(reinterpret_cast<char*>(s_data), N * sizeof(double));
        f.close();
        std::cout << "Resuming from checkpoint at iteration " << current_iteration << std::endl;
    } else {
        std::fill(s_data, s_data + N, 0.0);
        s_data[0] = 4.0;
        std::cout << "No checkpoint found. New session started." << std::endl;
    }
}

int main() {
    std::signal(SIGINT, save_checkpoint);
    s_data = (double*)mkl_malloc(N * sizeof(double), 64);
    MKL_Complex16* fft_buffer = (MKL_Complex16*)mkl_malloc(N * sizeof(MKL_Complex16), 64);
    load_checkpoint();
    DFTI_DESCRIPTOR_HANDLE handle = NULL;
    DftiCreateDescriptor(&handle, DFTI_DOUBLE, DFTI_REAL, 1, N);
    DftiSetValue(handle, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
    mkl_set_num_threads(omp_get_max_threads());
    DftiCommitDescriptor(handle);

    for (; current_iteration < P - 1; ++current_iteration) {
        
        DftiComputeForward(handle, s_data, fft_buffer);

        #pragma omp parallel for
        for (long j = 0; j < N; j++) {
            double re = fft_buffer[j].real;
            double im = fft_buffer[j].imag;
            fft_buffer[j].real = re*re - im*im;
            fft_buffer[j].imag = 2*re*im;
        }

        DftiComputeBackward(handle, fft_buffer, s_data);
        // normalize_and_reduce(s_data, P); 
        if (current_iteration % 500 == 0) {
            double progress = (double)current_iteration / (P - 2) * 100.0;
            std::cout << "Iteration: " << current_iteration << " | " 
                      << std::fixed << std::setprecision(2) << progress << "%\r" << std::flush;
        }
    }

    std::cout << "\nComplete." << std::endl;
    
    DftiFreeDescriptor(&handle);
    mkl_free(s_data);
    mkl_free(fft_buffer);
    return 0;
}
