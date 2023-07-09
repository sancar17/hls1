import numpy as np
import torch
import torch.nn.functional as F

# Set the seed for reproducibility
np.random.seed(0)
torch.manual_seed(0)

# Define the sizes
num_chnl_ip = 384
inputWidth = 16
KernelSize = 3
num_chnl_op = 384

# Generate random input, weights, and bias
X = np.random.rand(num_chnl_ip, inputWidth, inputWidth).astype(np.float32)
W = np.random.rand(num_chnl_op, num_chnl_ip, KernelSize, KernelSize).astype(np.float32)
b = np.random.rand(num_chnl_op).astype(np.float32)

# Convert numpy arrays to torch tensors
X_torch = torch.from_numpy(X).unsqueeze(0)  # Add extra dimension for batch size
W_torch = torch.from_numpy(W)
b_torch = torch.from_numpy(b)

# Compute output using PyTorch's conv2d function
Z_torch = F.conv2d(X_torch, W_torch, b_torch, stride=1, padding=0)
print(Z_torch)
Z = Z_torch.squeeze(0).numpy()  # Remove extra dimension and convert to numpy

print(Z)


# Function to write a 2D array in C++ format to a file
def write_array_2d(f, array, name):
    f.write(f"float {name}[{array.shape[0]}][{array.shape[1]}] = {{\n")
    for i in range(array.shape[0]):
        f.write("    {")
        for j in range(array.shape[1]):
            f.write(f"{array[i, j]:.4f}")
            if j < array.shape[1] - 1:
                f.write(", ")
        f.write("},\n")
    f.write("};\n\n")

# Write the input, weights, bias, and output in C++ format to a file
with open('arrays.txt', 'w') as f:
    for i in range(X.shape[0]):
        write_array_2d(f, X[i], f"X_c{i+1}")

    for i in range(W.shape[0]):
        for j in range(W.shape[1]):
            write_array_2d(f, W[i, j], f"filter{i+1}_{j+1}")

    write_array_2d(f, b[np.newaxis], "b")  # Add an extra dimension to b to make it 2D

    for i in range(Z.shape[0]):
        write_array_2d(f, Z[i], f"Z_out{i+1}")
