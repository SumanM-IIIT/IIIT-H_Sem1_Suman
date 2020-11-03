# SSD ASSIGNMENT - 3 (Part B)

- A new branch of the same **Github Repo** is used: https://github.com/SumanM-IIIT/SSD-Assignment-3-Part-A-/tree/branch_3b

### Input/Output Files:
- Kept all the input and output files inside the folder named **"files"**. 
- The output file for **Q2** is named as **output_q2.txt** and the output file for **Q3** is named as **output_q3.txt**, and both the files will be created / edited inside the folder named **"files"**.

### Q1:
- Taking the input json file as a command-line argument. For example: **python3 q1.py files/org.json**
- Taking the input from the user in a space separated manner in the following format:
    -  **<number_of_employees> <empid 1> <empid 2> ..... <empid n>**
    -  Example: **3 003 007 008**
- Output Format:
    - 1st line of output contains: **Common Leader: <leader emp id>**. 
    - Next subsequent n-lines contain: **Leader <leader emp id> is <k>-level(s) above employee <i-th emp-id>** where i <= no. of employees inputted.
- If one of the Employee is the **root** i.e. at **Level-0**, then technically NO common parent is available. So I am printing a message **"Common Leader NOT AVAILABLE"** in this case.

- **CHANGES w.r.t Assignment-3A:** 
    - Input format is changed since multiple employees can be given now **(Lines: 26 to 31)**.
    - Same functions are used as it is.
    - Calculating the common leader iteratively this time for multiple employees with a little bit of change in the previous logic **(Lines: 45 to 58)**.

### Q2:
- Taking inputs in the form of command-line arguments. 
    - Taking the input file path as 1st Command Line Argument.
    - Taking the date-format as 2nd Command Line Argument.
    - Example: **python3 q2.py files/date_calculator.txt mm/dd/yyyy**
- Any date library is NOT used.
- Output is written inh the **output_q2.txt** file as per the format given in the question.
- Following date formats (examples) are currently supported as input:
    - Existing Date Formats:
        - **dd/mm/yyyy**
        - **dd-mm-yyyy**
        - **dd.mm.yyyy**
    - Newly added Date Formats:
        - **mm/dd/yyyy**
        - **mm-dd-yyyy**
        - **mm.dd.yyyy**
- For the existing old date formats, two more date formats are supported in the input file which are (example): **13th Sept, 2020** and **13th September, 2020**. **BUT** the date-format to be given as command-line argument will be from the above mentioned 6 formats only. (This is just to check if the day is coming first or the month)
- For the newly added date formats, only the above 3 mentioned formats are supported.

- **CHANGES w.r.t Assignment-3A:**
    - The input format has changed (Two command-line arguments).
    - All the previous functions remain as it is.
    - A new function **date_format2()** is added for the newly added formats which is similar to the existing function **date_format()** with just one change in the functionality. It considers the month first and then the day **(Lines: 33-46)**.
    - A checking for the input date-format is done **(Lines: 78-86)**.

### Q3:
- All the employee files are kept in **"files/Employees"** and this path is taken as the Command-Line Argument. Example: **python3 q3.py files/Employees**
- Then a **single input** for **duration** in **hours (multiple of 0.5)** is taken from the user in float (examples: 0.5, 1, 2.5 etc).
- Output is written in the **output_q3.txt** file as per the format given in the question.
- If any one of the date is NOT same, then I am displaying **"DIFFERENT DATES !! Exiting.."** as the console message and exiting the program WITHOUT writing anything in the output file.
- If dates are same but NO free slots are available, then I am writing **"N/A"** in the common-slot position of the output file.
- The output format in the autput file remains same as given in the question.
- **CHANGES w.r.t Assignment-3A:**
   - All the previous functions remain same.
   - All the data fetching and formatting operations are done ITERATIVELY this time for multiple employees.
   - A new function **all_common_free_slots()** is added to cumulatively calculate all the common free-slots of **n-1** employees **(Lines: 70-110)**. 
   - Finally taking the cumulative common free-slots of **n-1** employees from the previous step and the free-slots of **n-th** employee to calculate the first common free-slot as per the given duration using the exising function **get_common_slot()**. **(Lines: 164-171)**