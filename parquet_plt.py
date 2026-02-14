import pandas as pd
import matplotlib.pyplot as plt
import pyarrow.parquet as pq
from tkinter import simpledialog as sd

def cpp_plt(filename):
    table = pq.read_table(filename, thrift_string_size_limit=100*1024*1024, thrift_container_size_limit=100*1024*1024)
    df = table.to_pandas()

    setA = sd.askinteger("setA", f"File has {len(df.columns)} sets available. \n\nSet A: ")
    setB = sd.askinteger("setB", f"File has {len(df.columns)} sets available. \n\nSet B: ")

    selected_columns = df.columns[setA:setB]

    plt.figure(figsize=(16, 10))

    for column in selected_columns:
        data = pd.to_numeric(df[column], errors='coerce').dropna()

        if not data.empty:
            plt.plot(data.index, data.values, label=column,
                     marker='o', linestyle='solid', markersize=2)

    plt.title(".parquet_plot")
    plt.xlabel("Index")
    plt.ylabel("Value")
    plt.yscale('log')
    plt.legend(loc='best', bbox_to_anchor=(1.15, 1))
    plt.grid(True, which="both", ls="-", alpha=0.5)
    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    cpp_plt("export.parquet")
