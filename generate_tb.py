# File paths
x_memcpy_file = "x_memcpy_operations.txt"
w_memcpy_file = "w_memcpy_operations.txt"
ref_memcpy_file = "ref_memcpy_operations.txt"

# Open files for writing
x_memcpy = open(x_memcpy_file, "w")
w_memcpy = open(w_memcpy_file, "w")
ref_memcpy = open(ref_memcpy_file, "w")

# Memcpy operations for x
for i in range(384):
    x_memcpy.write("memcpy(x[" + str(i) + "].X_c, X_c" + str(i + 1) + ", sizeof(X_c" + str(i + 1) + "));\n")

# Memcpy operations for w
for i in range(384):
    for j in range(384):
        w_memcpy.write("memcpy(w[" + str(i) + "].ith_filter[" + str(j) + "].W_c, filter" + str(i+1) + "_" + str(j+1) + ", sizeof(filter" + str(i) + "_" + str(j) + "));\n")

# Memcpy operations for ref
for i in range(384):
    ref_memcpy.write("memcpy(ref[" + str(i) + "].Z_out, zc" + str(i + 1) + ", sizeof(zc" + str(i + 1) + "));\n")

# Close files
x_memcpy.close()
w_memcpy.close()
ref_memcpy.close()
