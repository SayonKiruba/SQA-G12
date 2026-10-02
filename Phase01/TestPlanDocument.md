CSCI 3060U: Software Quality Assurance  
Lab Assignment — Automated Testing Framework & Test Plan Document  
Course: Software Quality Assurance
Phase: Phase 01 — Black Box Testing Automation
1. Test Directory Structure  
   The black box test suite tests are organized in subfolders based on features. Each test case is stored in its own folder with its respective input, expected output and expected transaction files.
   Phase01/  
   ├── data/                            # System state data files  
   │   ├── available-games              # Global games registry  
   │   └── current-users                # Active user accounts registry  
   ├── tests/                           # Black-box test cases grouped by feature  
   │   ├── login/                       # Feature: User Authentication  
   │   │   ├── TC01_Valid_Login/        # Individual Test Case  
   │   │   │   ├── input.txt            # Test input commands  
   │   │   │   ├── output.txt           # Expected standard output  
   │   │   │   └── transaction.txt      # Expected daily transaction file  
   │   │   ├── TC02_Login_Before_Other_Transaction/  
   │   │   └── TC03_Double_Login_Rejected/  
   │   ├── logout/                      # Feature: Session Logout  
   │   ├── create/                      # Feature: Create User Account  
   │   ├── delete/                      # Feature: Delete User Account  
   │   ├── sell/                        # Feature: List Game for Sale  
   │   ├── buy/                         # Feature: Purchase Game  
   │   ├── refund/                      # Feature: Issue Credit Refund  
   │   ├── add-credit/                  # Feature: Add Credit to Account  
   │   └── transaction/                 # Feature: Transaction Record Formatting  
   └── MasterTable.md  
   │  
   └── TestPlan.md  
   │  
   └── README.md
2. Test Execution Scripts  
   Tests will be executed using an automated Bash shell script (run_tests.sh).  
   This will iterate through every test case directory under tests/, redirect input from input.txt into the compiled application binary, and give the location of the available games and current users file to the program. The application's standard terminal output is put into actual_output.txt, and its daily transaction record is put into actual_trans.txt.  
   The script compares actual results vs expected results using the diff utility:
   output.txt vs. actual_output.txt
   transaction.txt vs. actual_trans.txt
3. Output Organization & Regression Tracking
   Timestamped Execution Runs: Every test execution stores outputs to a folder under test-results/Run_<timestamp>/ so previous run results are never overwritten.
   Failure Logging: If a test fails, the execution script logs the difference between expected and actual output in diff_output.txt or diff_trans.txt.
   Regression Comparison: A summary.log records the pass/fail status of all test cases. Comparing summary.log across test runs can help detect regression issues introduced by new code changes.