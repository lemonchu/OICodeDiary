import numpy as np
import time
from scipy.fft import fft, ifft

def compute_convolution(A, B):
    # 计算结果的长度
    n = len(A) + len(B) - 1

    # 找到大于或等于n的最小2的幂
    n_fft = 2**np.ceil(np.log2(n)).astype(int)

    # 对A和B进行FFT
    fft_A = fft(A, n=n_fft)
    fft_B = fft(B, n=n_fft)

    # 在频域中将两个FFT结果相乘
    fft_result = fft_A * fft_B

    # 进行逆FFT得到最终的卷积结果
    result = ifft(fft_result)

    # 取实部，并且只保留前n个结果
    result = result[:n].real

    return result

# 示例数组
A = np.random.randint(low=0, high=100, size=10000)
B = np.random.randint(low=0, high=100, size=10000)

# 测量时间
start_time = time.time()

# 计算卷积
convolution_result = compute_convolution(A, B)

# 记录结束时间
end_time = time.time()

# 输出结果
print("数组A的前几个值:", A[:10])
print("数组B的前几个值:", B[:10])
print("卷积结果的前几个值:", convolution_result[:10])

# 计算并打印运行时间（毫秒）
execution_time = (end_time - start_time) * 1000
print("Execution time: ", execution_time, "ms")
