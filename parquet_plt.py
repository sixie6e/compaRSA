import pandas as pd
import matplotlib.pyplot as plt
import pyarrow.parquet as pq

def cpp_plt(filename):
    table = pq.read_table(filename)
    df = table.to_pandas()
    plt.figure(figsize=(16, 10))

    for column in df.columns:
        data = pd.to_numeric(df[column], errors='coerce').dropna()

        if not data.empty:
            plt.plot(data.index, data.values, label=column,
                     marker='o', linestyle='none', markersize=2)

    plt.title("CPP_EXPORT")
    plt.xlabel("Index")
    plt.ylabel("Value")
    plt.yscale('log')
    plt.legend(loc='upper right', bbox_to_anchor=(1.15, 1))
    plt.grid(True, which="both", ls="-", alpha=0.5)
    plt.tight_layout()
    plt.show()
    
if __name__ == "__main__":
    cpp_plt("export.parquet")
