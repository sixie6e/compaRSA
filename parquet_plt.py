import pandas as pd
import matplotlib.pyplot as plt
import pyarrow.parquet as pq

def cpp_plt(filename):
    table = pq.read_table(filename)
    df = table.to_pandas()
    
    print(f"File has {len(df.columns)} sets available.")

    try:
        setA = int(input("Set A: "))
        setB = int(input("Set B: "))
    except ValueError:
        print("Invalid.")
        return

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
