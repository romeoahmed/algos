def karatsuba(x: int, y: int) -> int:
    """Return the exact product of two nonnegative integers."""
    if x == 0 or y == 0:
        return 0
    bits = max(x.bit_length(), y.bit_length())
    if bits <= 8:
        return x * y
    half = bits // 2
    mask = (1 << half) - 1
    a, b = x >> half, x & mask
    c, d = y >> half, y & mask
    high = karatsuba(a, c)
    low = karatsuba(b, d)
    cross = karatsuba(a + b, c + d) - high - low
    return (high << (2 * half)) + (cross << half) + low
