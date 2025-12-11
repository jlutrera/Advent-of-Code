def is_safe(report):
    """
    Check if a report is safe based on the criteria.
    :param report: List of integers representing levels in a report.
    :return: True if the report is safe, False otherwise.
    """
    differences = [report[i + 1] - report[i] for i in range(len(report) - 1)]
    
    # Check that all differences are between -3 and -1 (decreasing)
    # or between 1 and 3 (increasing), and not mixed.
    if all(-3 <= diff <= -1 for diff in differences):
        return True  # Decreasing and valid differences
    elif all(1 <= diff <= 3 for diff in differences):
        return True  # Increasing and valid differences
    
    return False

def count_safe_reports(file_path):
    """
    Count the number of safe reports in a file.
    :param file_path: Path to the file containing the data.
    :return: The count of safe reports.
    """
    safe_count = 0
    
    with open(file_path, 'r') as file:
        for line in file:
            report = list(map(int, line.strip().split()))
            if is_safe(report):
                safe_count += 1
    
    return safe_count

# Path to the file containing the data
file_path = "data.txt"  # Replace with your file's actual path

# Calculate the number of safe reports
safe_count = count_safe_reports(file_path)
print(f"Number of safe reports: {safe_count}")
