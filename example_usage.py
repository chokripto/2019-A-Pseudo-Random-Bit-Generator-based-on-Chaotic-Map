# example_usage.py

from prbg_nmgm import generate_sequence

if __name__ == "__main__":
    password = "ABIDINE"
    F = 1000  # 1000 bits
    sequence = generate_sequence(password, F)
    print(f"Generated {len(sequence)} bits")
    print(sequence[:100] + "..." if len(sequence) > 100 else sequence)
