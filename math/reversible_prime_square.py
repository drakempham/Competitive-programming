import math


def list_primality(n):
    """Sieve of Eratosthenes to find all primes up to n"""
    result = [True] * (n + 1)
    result[0] = result[1] = False
    for i in range(int(math.sqrt(n)) + 1):
        if result[i]:
            for j in range(2 * i, len(result), i):
                result[j] = False
    return result


def is_not_palindrome(x):
    """Check if string x is not a palindrome"""
    return x != x[::-1]


def is_perfect_square(x):
    """Check if x is a perfect square"""
    sqrt_root = int(math.sqrt(x))
    return sqrt_root * sqrt_root == x


def compute(n, limit=5 * 10**7):
    is_prime = list_primality(limit)
    primes = [i for i, isprime in enumerate(is_prime) if isprime]

    values = []

    for x in primes:
        sq = x * x
        sq_str = str(sq)

        if is_not_palindrome(sq_str):
            # Reverse the square
            rev = int(sq_str[::-1])

            if is_perfect_square(rev) and is_prime[int(math.sqrt(rev))]:
                values.append(sq)

    # Get unique values, sort them, and take first n
    unique_values = sorted(set(values))
    first_n = unique_values[:n]

    return sum(first_n), first_n


result, first_50 = compute(50)
print(f"Sum of first 50 reversible prime squares: {result}")
print(f"First 50 reversible prime squares: {first_50}")
