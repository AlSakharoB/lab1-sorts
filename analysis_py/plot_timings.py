import pandas as pd
import matplotlib.pyplot as plt
import os

# Убедимся, что папка для изображений существует
os.makedirs("../images", exist_ok=True)

# Чтение CSV
df = pd.read_csv("../data/timings.csv")
df.columns = df.columns.str.strip()

# Обычный график
plt.figure(figsize=(10, 6))
plt.plot(df["Size"], df["SelectionSort"], label="Selection Sort")
plt.plot(df["Size"], df["InsertionSort"], label="Insertion Sort")
plt.plot(df["Size"], df["QuickSort"], label="Quick Sort")
plt.plot(df["Size"], df["StdSort"], label="std::sort")
plt.xlabel("Размер массива")
plt.ylabel("Время (мс)")
plt.title("Сортировка — обычный масштаб")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("../images/sort_plot_linear.png", dpi=300)
plt.close()

# Логарифмический график
plt.figure(figsize=(10, 6))
plt.plot(df["Size"], df["SelectionSort"], label="Selection Sort")
plt.plot(df["Size"], df["InsertionSort"], label="Insertion Sort")
plt.plot(df["Size"], df["QuickSort"], label="Quick Sort")
plt.plot(df["Size"], df["StdSort"], label="std::sort")
plt.xlabel("Размер массива")
plt.ylabel("Время (мс)")
plt.title("Сортировка — логарифмическая шкала")
plt.yscale("log")
plt.legend()
plt.grid(True, which="both", linestyle="--", linewidth=0.5)
plt.tight_layout()
plt.savefig("../images/sort_plot_log.png", dpi=300)
plt.close()
