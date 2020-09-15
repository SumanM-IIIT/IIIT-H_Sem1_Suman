# SSD ASSIGNMENT - 1

### Q1:
  - Used **touch** command to create multiple files at once.
  - Used **find** with **mv** to rename multiple files at once.
- Used **ls -laSr** to list the content in long list format sorted in ascending order where; **'-l'** for long list format, **'-a'** for all files, **'-S'** for sort by file size (largest first) and **'-r'** for reverse sort (i.e. increasing order of file size).
- Used **find** command with **-maxdepth 2** to display full paths of each files/folders upto two levels of depth.
- Used **readlink -f** to display the full path.

### Q2:
- Used **compgen -c** to get the list of all the available commands.
- Sorted the scrambled input word given by user.
- In loop, sorted each command from the list one by one, and compared it with the sorted input word. 

### Q3:
- Used **history 10** command to get the list of last 10 commands and a logic using **awk** to calculate and print each command alongwith its count.
- Sorted according to the 2nd column (i.e. the counts) in decreasing order using **sort -k2 -nr**, where **'-k2'** for 2nd column, **'-n'** for numeric sort and **'-r'** for reverse (descending) order.

### Q4:
- Used **sed** multiple times in pipeline to format the output as per the need.
- Kept a single pair of bracket intentionally. The answer is displayed inside those brackets, as it is shown in the examples provided with the question.

### Q5.
- Converted the user given input string into a lowercase string using **tr '[:upper:]' '[:lower:]'**.
- Compared each character from front and end of the string one by one to check for palindrome.

### Q6.
- Took the first command line argument using **$1**.
- Shift the rest of the command line arguments by one left using **shift 1** i.e. made **$2** as **$1**, **$3** as **$2** etc.
- Then accessed the rest of the command line arguments using **$@**.
- Used **bc** calculator for calculating the exponential values.

### Q7.
- Used **ps a -o pid** command to get all the current process IDs and stored them in **pid.txt** (saved locally) after formatting it according to the need using **cut**.
- Displayed the **N** smallest process IDs using **head** command. If **N** is greater than the total no. of processes, then considered *N = total no. of processes*.
- Used **awk '$1=$1'** to remove the trailing extra spaces while displaying the PIDs.
- Intentionally used **ps ax** instead of **ps -r** to store the running processes as **-r** only shows the single process which is currently running i.e. **ps -r** itself.

### Q8.
- Directly executed the crontab file using **crontab** command and checked the exit status to confirm the correctness of the crontab format.

### Q9.
- Validated if the input contains only numbers and/or spaces using **regex**.
- Validated if the length of the input >= 1.
- Stored all the digits as an array and applied **Luhn algorithm** to check validity of the input.

### Q10.
- Used **bc** calculator for all the calculations.
- Used **scale=4** in the *bc* calculator to get precision 4 for the decimal numbers.
