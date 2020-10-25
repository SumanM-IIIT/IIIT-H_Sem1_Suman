# SSD ASSIGNMENT - 3 (Part A)

### **Github Repo**: https://github.com/SumanM-IIIT/SSD-Assignment-3-Part-A-
### Input/Output Files:
- Kept all the input and output files inside the folder named **"files"**.
- Taking the **input files** in each question as **command-line argument(s)**.
    - For example:
       - **python3 q1.py files/org.json**
       - **python3 q2.py files/date_calculator.txt**
       - **python3 q3.py files/Employee1.txt files/Employee2.txt**
- The output file for **Q2** is named as **output_q2.txt** and the output file for **Q3** is named as **output_q3.txt**, and both the files will be created / edited inside the folder named **"files"**.

### Q1:
- Taken all the employee IDs as strings in the range **[001-999]** in the input json file.
- Converting the employee IDs from string to int before calculation.
- Output format consists of 3 lines (each line containing a single positive integer (i.e. Employee ID)):
    - 1st line: **Common Parent**. 
    - 2nd line: **Level of Employee-1 w.r.t. common parent**.
    - 3rd line: **Level of Employee-2 w.r.t. common parent**.
- If one of the Employee is the **root** i.e. at **Level-0**, then technically NO common parent is available. So I am printing a message **"Common Parent NOT AVAILABLE"** in this case.

### Q2:
- Any date library is NOT used.
- Following date formats (examples) are currently supported as input:
    - 13th September, 2020
    - 13th Sep, 2020
    - 13/09/2020
    - 13-09-2020
    - 13.09.2020

### Q3:
- **datetime** package is used.
- Only 1 employee record per input file is considered.
- Only 1 date per employee is currently considered.
- Output is written in the **output_q3.txt** file as per the format given in the example.
- If dates are NOT same, then I am displaying **"N/A (Different Dates)"** message everywhere in the output file.
- If dates are same but NO free slots are available, then I am displaying **"N/A"** in the common-slot position.