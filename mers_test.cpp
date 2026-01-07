#include <iostream>
#include <fstream>
#include <string>
#include <gmpxx.h>
#include <csignal>
#include <sys/stat.h>
#include <iomanip>

const long p = 136279841;
const std::string checkpoint_file = "checkpoint.txt";
const int interval = 1000;
bool interrupted = false;
long current_i = 1;
mpz_class current_s = 4;

void save_progress(long p_val, long iteration, const mpz_class& s) {
    std::ofstream f(checkpoint_file);
    if (f.is_open()) {
        f << p_val << "\n" << iteration << "\n" << s.get_str(16) << std::endl;
        f.close();
    }
}

void signal_handler(int signum) {
    std::cout << "\nKeyboard Interrupt." << std::endl;
    save_progress(p, current_i, current_s);
    exit(signum);
}

bool file_exists(const std::string& name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

int main() {
    signal(SIGINT, signal_handler);
    mpz_class mers = (mpz_class(1) << p) - 1;
    long start_iteration = 1;

    if (file_exists(checkpoint_file)) {
        std::ifstream f(checkpoint_file);
        long check_p;
        std::string s_hex;
        
        if (f >> check_p >> start_iteration >> s_hex) {
            if (check_p == p) {
                std::cout << "Resuming from iteration " << start_iteration << "..." << std::endl;
                current_s = mpz_class(s_hex, 16);
                start_iteration++; 
            } else {
                std::cout << "Different checkpoint found.. New session started." << std::endl;
            }
        }
        f.close();
    }

    try {
        for (current_i = start_iteration; current_i < p - 1; ++current_i) {
            // llcheck()
            current_s = (current_s * current_s - 2) % mers;

            if (current_i % interval == 0) {
                save_progress(p, current_i, current_s);
                double progress = (double)current_i / (p - 2) * 100.0;
                std::cout << "Progress: " << current_i << "/" << (p - 2) 
                          << " (" << std::fixed << std::setprecision(2) << progress << "%)   \r" << std::flush;
            }
        }

        bool is_prime = (current_s == 0);
        std::cout << "\nConclusion: " << (is_prime ? "Prime" : "Composite") << std::endl;
        
        if (file_exists(checkpoint_file)) {
            remove(checkpoint_file.c_str());
        }

    } catch (const std::bad_alloc& e) {
        std::cerr << "\nMemory Error." << std::endl;
        save_progress(p, current_i, current_s);
        return 1;
    }

    return 0;
}
