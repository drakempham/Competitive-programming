f = [1, 2, 3]
for i in range(3, 85):
    f.append(f[i-1] + f[i-3])
print("f[40] =", f[40])
print("f[42] =", f[42])
