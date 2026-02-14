Compares arrays of primes.\
Generates a new array for each set comparison.\
Filters the new arrays for primes.\
Stores the filtered arrays.\
Plots the filtered arrays.\
Manual mode: You decide. Slow with full control.\
Auto mode: Computer decides. Rapid with no control.\
The bin and parquet files get massive, make sure to have adequate storage.\
Requirements: pyarrow, parquet, pandas, matplotlib, tkinter.

1. Current AppImage:\
    https://sourceforge.net/projects/thedirector/

2.
    Download theDirector executable and parquet_plt.py\
    sudo apt update\
    sudo apt install -y -V ca-certificates lsb-release wget\
    wget https://packages.apache.org/artifactory/arrow/$(lsb_release --id --short | tr 'A-Z' 'a-z')/apache-arrow-apt-source-latest-$(lsb_release --codename --short).deb\
    sudo apt install -y -V ./apache-arrow-apt-source-latest-$(lsb_release --codename --short).deb\
    sudo apt update\
    sudo apt install libarrow-dev libparquet-dev qt6-base-dev python3-pip python3-tk\
    pip3 install matplotlib pyarrow parquet 

Has only been tested on Debian/Ubuntu distributions.
