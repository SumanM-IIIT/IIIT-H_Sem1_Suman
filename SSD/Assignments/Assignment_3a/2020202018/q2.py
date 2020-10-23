month_dict = {'jan': 1, 'feb': 2, 'mar': 3, 'apr': 4, 'may': 5, 'jun': 6, 'jul': 7, 'aug': 8, 'sep': 9, 'oct': 10, 'nov': 11, 'dec': 12}
days_of_months = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]

def date_format(actual_d):
	date = None
	if '/' in actual_d:
		date = actual_d.split('/')
	elif '-' in actual_d:
		date = actual_d.split('-')
	elif '.' in actual_d:
		date = actual_d.split('.')
	elif ',' in actual_d:
		tmp_d = actual_d.split(',')
		yr = tmp_d[1].strip()

		tmp_d2 = tmp_d[0].split()
		dy = tmp_d2[0][0:-2]

		mn = month_dict[tmp_d2[1][0:3].lower()]
		date = []
		date.append(dy)
		date.append(mn)
		date.append(yr)

	#print (date)
	for i in range(0, len(date)):
		date[i] = int(date[i])
	return date

def add_leap_years(date):
	curr_yr = date[2]
	if date[1] <= 2:
		curr_yr -= 1
	count_leap = int(curr_yr // 4 - curr_yr // 100 + curr_yr // 400)
	return count_leap

def date_diff(date1, date2):
	days1 = date1[2] * 365 + date1[0]
	for i in range(0, date1[1] - 1):
		days1 += days_of_months[i]
	days1 += add_leap_years(date1)

	days2 = date2[2] * 365 + date2[0]
	for i in range(0, date2[1] - 1):
		days2 += days_of_months[i]
	days2 += add_leap_years(date2)

	return abs(days2 - days1)  


date_file = open('date_calculator.txt', 'r') 
dates = date_file.readlines()

actual_dates = []

for i in dates:
	actual_dates.append(i.strip().split(':')[1].strip())

#print(actual_dates)
date1 = date_format(actual_dates[0])
date2 = date_format(actual_dates[1])
print(date1)
print(date2)
diff = date_diff(date1, date2)
op_line = 'Date Difference: ' + str(diff) + ' Day(s)'

op_file = open('output.txt', 'w') 
op_file.writelines(op_line) 
op_file.close() 
