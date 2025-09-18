import pandas as pd

sizes = [10, 100, 200, 400, 600, 800, 1000, 2000]   
ks = [2, 4, 8, 16, 32]                              
reps_v1 = 10                                        
reps_v23 = 10                                     

def read_times(filename):
    with open(filename, "r") as f:
        lines = f.readlines()
    return [float(line.split()[0]) for line in lines]

# ---------------- V1 ----------------
times_v1 = read_times("timesV1.doc")
assert len(times_v1) == len(sizes) * reps_v1, "V1 no cuadra"

df_v1 = pd.DataFrame({size: times_v1[i::len(sizes)] for i, size in enumerate(sizes)})
df_v1.loc["Promedio"] = df_v1.mean()


# ---------------- V2 / V3 ----------------
def build_block(filename):
    times = read_times(filename)
    expected = len(sizes) * len(ks) * reps_v23
    assert len(times) == expected, f"{filename} no cuadra con i,k,rep"

    index = 0
    blocks = []
    for k in ks:
        data = {}
        for i in sizes:
            colname = f"i={i}"
            data[colname] = times[index::len(sizes) * len(ks)]
            index += 1
        df = pd.DataFrame(data)
        df.loc["Promedio"] = df.mean()

        # Añadimos una columna para identificar el bloque de k
        df.insert(0, "k", k)
        blocks.append(df)

    # concatenamos todos los bloques uno debajo del otro
    return pd.concat(blocks, ignore_index=True)

df_v2 = build_block("timesV2.doc")
df_v3 = build_block("timesV3.doc")


# ---------------- Guardar en Excel ----------------
with pd.ExcelWriter("Resultados.xlsx", engine="openpyxl") as writer:
    df_v1.to_excel(writer, sheet_name="V1", index=False)
    df_v2.to_excel(writer, sheet_name="V2", index=False)
    df_v3.to_excel(writer, sheet_name="V3", index=False)

print("✅ Resultados guardados en Resultados.xlsx (V2 y V3 apilados por k en una sola hoja cada uno)")
