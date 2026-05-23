import os
import matplotlib.pyplot as plt

# Base directory of this script (visuals.py)
BASE_DIR = os.path.dirname(os.path.abspath(__file__))

def full(path):
    return os.path.join(BASE_DIR, path)

# --------- 1) Reading helper functions ---------

def read_dependencies(path="dependencies.txt"):
    deps = []
    with open(full(path), "r", encoding="utf-8") as f:
        for line in f:
            parts = line.split()
            if len(parts) >= 4:
                Ii, Ij, reg, typ = parts[:4]
                deps.append((Ii, Ij, reg, typ))
    return deps

def read_int_matrix(path):
    rows = []
    with open(full(path), "r", encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            row = list(map(int, line.split()))
            rows.append(row)
    return rows

def read_order(path):
    with open(full(path), "r", encoding="utf-8") as f:
        text = f.read().strip()
        if not text:
            return []
        return list(map(int, text.split()))  # numbers from file

def read_speedup(path):
    with open(full(path), "r", encoding="utf-8") as f:
        line = f.read().strip()
    if not line:
        return 0.0, 0.0, 0.0
    a_str, b_str = line.split()[:2]
    a = float(a_str)
    b = float(b_str)
    ratio = a / b if b != 0 else float("inf")
    return a, b, ratio


# --------- 2) Stage name mapping ---------

stage_names = {
    -2: "",        # empty
    -1: "STALL",
     0: "IF",
     1: "ID",
     2: "EX",
     3: "MEM",
     4: "WB",
}

def int_table_to_str(table):
    return [[stage_names.get(x, "?") for x in row] for row in table]


# --------- 3) Plotting pipeline tables ---------

def plot_pipeline_table(str_table, row_labels, title, speed_info=None):
    """
    speed_info: tuple (a, b, ratio) or None
    """
    n_rows = len(str_table)
    if n_rows == 0:
        return
    n_cols = len(str_table[0])
    col_labels = [f"C{j+1}" for j in range(n_cols)]

    fig, ax = plt.subplots(figsize=(n_cols * 0.7, n_rows * 0.6 + 1.4))
    ax.axis("off")
    tbl = ax.table(
        cellText=str_table,
        rowLabels=row_labels,
        colLabels=col_labels,
        loc="center"
    )
    tbl.auto_set_font_size(False)
    tbl.set_fontsize(9)
    tbl.scale(1, 1.4)
    ax.set_title(title, pad=20)

    # write speedup under the table
    if speed_info is not None:
        a, b, r = speed_info
        text = f"Speedup = {a:.0f} / {b:.0f} = {r:.3f}"
        ax.text(
            0.5, -0.15, text,
            transform=ax.transAxes,
            ha="center", va="top"
        )

    plt.tight_layout()
    plt.show()


# --------- 4) Dependencies table figure ---------

def plot_dependencies_table(deps):
    if not deps:
        return
    col_labels = ["Dependent (Ii)", "Depends on (Ij)", "Register", "Type"]
    cell_text = [[Ii, Ij, reg, typ] for Ii, Ij, reg, typ in deps]
    row_labels = [f"D{k+1}" for k in range(len(deps))]

    fig, ax = plt.subplots(figsize=(8, 0.5 * len(deps) + 1))
    ax.axis("off")
    tbl = ax.table(
        cellText=cell_text,
        rowLabels=row_labels,
        colLabels=col_labels,
        loc="center"
    )
    tbl.auto_set_font_size(False)
    tbl.set_fontsize(9)
    tbl.scale(1, 1.2)
    ax.set_title("Instruction Dependencies", pad=20)
    plt.tight_layout()
    plt.show()


# --------- 5) Main ---------

if __name__ == "__main__":
    # 1) Read data from files written by main.exe
    deps = read_dependencies("dependencies.txt")
    stall_tab = read_int_matrix("stall_forwarding.txt")
    reord_tab = read_int_matrix("forwarding_reordering.txt")

    # order1.txt for stall_forwarding, order2.txt for forwarding_reordering
    order_stall = read_order("order1.txt")
    order_reord = read_order("order2.txt")

    # 2) Speedup files
    s1_a, s1_b, s1_r = read_speedup("speedUp1.txt")
    s2_a, s2_b, s2_r = read_speedup("speedUp2.txt")

    # 3) Print speedups nicely in console
    print("Speedup (stall + forwarding):")
    print(f"  {s1_a:.0f} / {s1_b:.0f} = {s1_r:.3f}")
    print("Speedup (forwarding + reordering):")
    print(f"  {s2_a:.0f} / {s2_b:.0f} = {s2_r:.3f}\n")

    # 4) Row labels (files already store logical numbers)
    if order_stall:
        stall_labels = [f"I{idx}" for idx in order_stall]
    else:
        stall_labels = [f"I{i+1}" for i in range(len(stall_tab))]

    if order_reord:
        reord_labels = [f"I{idx}" for idx in order_reord]
    else:
        reord_labels = [f"I{i+1}" for i in range(len(reord_tab))]

    # 5) Plot the two pipeline tables with speedup text under each
    plot_pipeline_table(
        int_table_to_str(stall_tab),
        stall_labels,
        "Stall + Forwarding",
        speed_info=(s1_a, s1_b, s1_r)
    )

    plot_pipeline_table(
        int_table_to_str(reord_tab),
        reord_labels,
        "Forwarding + Reordering",
        speed_info=(s2_a, s2_b, s2_r)
    )

    # 6) Plot dependencies in a separate window
    plot_dependencies_table(deps)
