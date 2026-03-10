import subprocess
import time
import csv
import shutil
import psutil
from pathlib import Path
from datetime import datetime

# Configuration
SCANNER_PATH = Path(__file__).parent.parent / "cmake-build-debug" / "MemoryLeakDetector"
OUTPUT_DIR = Path(__file__).parent / "results"
TEST_DIR = Path(__file__).parent / "tmp"
MAX_FILES = 1000       # Max number of files to test
STEP = 50            # Increase file count by this much each run
LINES_PER_FILE = 500  # Keep file size uniform

def create_dummy_files(num_files: int, directory: Path):
    """Creates a specific number of uniform C files."""
    if directory.exists():
        shutil.rmtree(directory)

    directory.mkdir(parents=True, exist_ok=True)
    dummy_code = "void test_func() {\n    int *ptr = malloc(sizeof(int));\n}\n" * LINES_PER_FILE

    for i in range(num_files):
        file_path = directory / f"test_{i}.c"
        file_path.write_text(dummy_code)

def main():
    results = []

    print(f"Starting benchmark for {SCANNER_PATH}...")
    print("-" * 70)

    for num_files in range(STEP, MAX_FILES + 1, STEP):
        create_dummy_files(num_files, TEST_DIR)

        start_time = time.perf_counter()

        # --- NEW: Launch process without blocking ---
        process = subprocess.Popen(
            [str(SCANNER_PATH), "-r", str(TEST_DIR)],
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL
        )

        max_memory_bytes = 0

        # --- NEW: Poll memory usage while it runs ---
        try:
            ps_process = psutil.Process(process.pid)
            # While the process is still running (poll() is None)
            while process.poll() is None:
                mem_info = ps_process.memory_info()
                if mem_info.rss > max_memory_bytes:
                    max_memory_bytes = mem_info.rss
                time.sleep(0.005)  # Poll every 5 milliseconds
        except psutil.NoSuchProcess:
            # The process finished so fast it disappeared before we could check it
            pass

        process.wait() # Ensure the C tool is completely finished
        end_time = time.perf_counter()

        duration = end_time - start_time
        total_loc = num_files * LINES_PER_FILE

        # Convert bytes to Megabytes for easier reading
        max_memory_mb = max_memory_bytes / (1024 * 1024)

        results.append((num_files, total_loc, duration, max_memory_mb))
        print(f"Scanned {num_files:4} files ({total_loc:6} LOC) | Time: {duration:.4f}s | Peak RAM: {max_memory_mb:.2f} MB")

    if TEST_DIR.exists():
        shutil.rmtree(TEST_DIR)

    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    timestamp = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
    csv_filename = OUTPUT_DIR / f"benchmark_{timestamp}.csv"

    with csv_filename.open("w", newline="") as f:
        writer = csv.writer(f)
        # --- NEW: Added Memory column to CSV ---
        writer.writerow(["Number of Files", "Lines of Code", "Time (Seconds)", "Peak Memory (MB)"])
        writer.writerows(results)

    print("-" * 70)
    print(f"Done! Results saved to:\n{csv_filename.resolve()}")

if __name__ == "__main__":
    main()