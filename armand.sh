code c_security_analyzer.sh

#!/bin/bash
mkdir -p reports
echo "Running Static Security Analysis for C Code..."

# Flawfinder Scan (C/C++)
echo "Running Flawfinder..."
flawfinder --html . > reports/flawfinder_report.html

# Cppcheck Scan (C/C++)
echo "Running Cppcheck..."
cppcheck --enable=all --xml . 2> reports/cppcheck_report.xml

# Semgrep Scan for C
echo "Running Semgrep..."
semgrep scan --config=auto --lang=c > reports/semgrep_report.txt

echo "Security analysis completed! Reports saved in 'reports/' directory."

./c_security_analyzer.sh