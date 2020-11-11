import ast
import sys
import os
from datetime import datetime, timedelta

def get_free_slots(emp_slot_list, start_time, end_time):
	free_slot = []
	for dur in emp_slot_list:
		times = dur.split('-')
		time_objs = [None] * len(times)
		for i in range(0, len(times)):
			tmp = times[i].strip()
			tmp = date_emp + tmp
			time_objs[i] = datetime.strptime(tmp, '%d/%m/%Y%I:%M%p')
		if(time_objs[0] > start_time):
			tmp2 = start_time.strftime('%I:%M%p') + ' - ' + time_objs[0].strftime('%I:%M%p')
			free_slot.append(tmp2)
		start_time = time_objs[1]	
		
	if start_time < end_time:
		free_slot.append(start_time.strftime('%I:%M%p') + ' - ' + end_time.strftime('%I:%M%p'))
	return free_slot

def timedelta_first_last(tmp1, tmp2):
	while(tmp1 < tmp2):
		tmp1 += timedelta(minutes = 30)
	while(tmp1 > tmp2):
		tmp2 += timedelta(minutes = 30)

def get_common_slot_util2(slot1, list2, delta, common_dict, f_list, curr_date):
	for slot2 in list2:
		if slot2[1] - slot2[0] < delta:
			continue	
		tmp1 = slot1[0]
		tmp2 = slot2[0]
		timedelta_first_last(tmp1, tmp2)
		if (tmp1 + delta) <= slot1[1] and (tmp2 + delta) <= slot2[1]:
			str_tmp = tmp1.strftime('%I:%M%p') + ' - ' + (tmp1 + delta).strftime('%I:%M%p')
			f_list.append(str_tmp)
			common_dict[curr_date] = f_list
			return common_dict

def get_common_slot_util(tmp_start, end_time, list1, list2, delta, curr_date, common_dict, f_list):
	while(tmp_start < end_time):
		for slot1 in list1:
			if slot1[1] - slot1[0] < delta:
				continue
			cd = get_common_slot_util2(slot1, list2, delta, common_dict, f_list, curr_date)
			if cd is not None:
				return cd
		tmp_start += timedelta(minutes = 30)
	f_list = ['N/A']
	common_dict[curr_date] = f_list
	return common_dict

def get_common_slot(curr_date, free_slot_emp1, free_slot_emp2, duration, start_time, end_time):
	str1 = str(float(duration))
	dur_list = str1.split('.')
	delta = timedelta(hours = int(dur_list[0]), minutes = int(dur_list[1]) * 6)
	t_mins = datetime.strptime(curr_date + '00:30', '%d/%m/%Y%H:%M')
	common_dict = {curr_date: 'N/A'}
	f_list = []
	list1 = []
	list2 = []
	for dur in free_slot_emp1:
		tmp = dur.split(' - ')
		time_objs = [None] * len(tmp)
		for i in range(0 ,len(tmp)):
			time_objs[i] = datetime.strptime(curr_date + tmp[i], '%d/%m/%Y%I:%M%p')
		list1.append(time_objs)
	for dur in free_slot_emp2:
		tmp = dur.split(' - ')
		time_objs = [None] * len(tmp)
		for i in range(0 ,len(tmp)):
			time_objs[i] = datetime.strptime(curr_date + tmp[i], '%d/%m/%Y%I:%M%p')
		list2.append(time_objs)
	str_tmp = 'N/A'
	tmp_start = start_time
	return get_common_slot_util(tmp_start, end_time, list1, list2, delta, curr_date, common_dict, f_list)

def all_common_free_slots_util2(tmp1, tmp2, tmp3, tmp4):
	while(tmp1 < tmp2):
		tmp1 += timedelta(minutes = 30)
	while(tmp1 > tmp2):
		tmp2 += timedelta(minutes = 30)
	while(tmp3 < tmp4):
		tmp4 -= timedelta(minutes = 30)
	while(tmp3 > tmp4):
		tmp3 -= timedelta(minutes = 30)

def all_common_free_slots_util(list1, list2):
	f_list = []
	for slot1 in list1:
		for slot2 in list2:
			tmp1 = slot1[0]
			tmp2 = slot2[0]
			tmp3 = slot1[1]
			tmp4 = slot2[1]
			all_common_free_slots_util2(tmp1, tmp2, tmp3, tmp4)
			if tmp3 - tmp1 >= timedelta(minutes = 30):
				str_tmp = tmp1.strftime('%I:%M%p') + ' - ' + tmp3.strftime('%I:%M%p')
				f_list.append(str_tmp)
				break
	return f_list

def all_common_free_slots(curr_date, free_slot_emp1, free_slot_emp2, duration, start_time, end_time):
	str1 = str(float(duration))
	dur_list = str1.split('.')
	delta = timedelta(hours = int(dur_list[0]), minutes = int(dur_list[1]) * 6)
	t_mins = datetime.strptime(curr_date + '00:30', '%d/%m/%Y%H:%M')
	f_list = []
	list1 = []
	list2 = []
	for dur in free_slot_emp1:
		tmp = dur.split(' - ')
		time_objs = [None] * len(tmp)
		for i in range(0 ,len(tmp)):
			time_objs[i] = datetime.strptime(curr_date + tmp[i], '%d/%m/%Y%I:%M%p')
		list1.append(time_objs)
	for dur in free_slot_emp2:
		tmp = dur.split(' - ')
		time_objs = [None] * len(tmp)
		for i in range(0 ,len(tmp)):
			time_objs[i] = datetime.strptime(curr_date + tmp[i], '%d/%m/%Y%I:%M%p')
		list2.append(time_objs)
	str_tmp = 'N/A'
	tmp_start = start_time
	return all_common_free_slots_util(list1, list2)

emp_in_dir_list = []
emp_dict_list = []
key_list = []
for file in os.listdir(sys.argv[1]):
    emp_in_dir_list.append(os.path.join(sys.argv[1], file))

for file in emp_in_dir_list:
	emp1_file = open(file, 'r') 
	content = emp1_file.read()
	emp_dict_list.append(ast.literal_eval(content))

for dicti in emp_dict_list:
	key_list.append(list(dicti.keys())[0])

for i in range(0, len(key_list)):
	emp_dict_list[i] = emp_dict_list[i][key_list[i]]

emp_date_list = []
all_dates = []
for e in emp_dict_list:
	emp_date_list.append(list(e.keys())[0])

date_emp = emp_date_list[0]
for d in emp_date_list:
	tmp = d.split('/')
	all_dates.append(tmp)

exact_dates = []
for date in all_dates:
	exact_dates.append(datetime(int(date[2]), int(date[1]), int(date[0])))

for i in range(1, len(exact_dates)):
	if exact_dates[i - 1] != exact_dates[i]:
		print("DIFFERENT DATES !! Exiting..")
		exit()

all_emp_slot_list = []
for i in range(0, len(emp_date_list)):
	all_emp_slot_list.append(emp_dict_list[i][emp_date_list[i]])

duration = input()
free_slot_list = []
op_line = ['Available slots: \n']
for i in range(0, len(all_emp_slot_list)):
	start_time = datetime.strptime(all_dates[i][0] + all_dates[i][1] + all_dates[i][2] + str(9), '%d%m%Y%H')
	end_time = datetime.strptime(all_dates[i][0] + all_dates[i][1] + all_dates[i][2] + str(17), '%d%m%Y%H')
	free_slot_list.append(get_free_slots(all_emp_slot_list[i], start_time, end_time))

for i in range(0, len(free_slot_list)):
	op_line.append(key_list[i] + ': ' + str(free_slot_list[i]) + '\n')
op_line.append('\nSlot Duration: ' + duration + ' hour(s)\n')

dem = free_slot_list[0]
for i in range(1, len(free_slot_list) - 1):
	dem = all_common_free_slots(date_emp, dem, free_slot_list[i], duration, start_time, end_time)

if dem is not None:
	common_slot = get_common_slot(date_emp, dem, free_slot_list[-1], duration, start_time, end_time)[date_emp]
else:
	common_slot = ['N/A'] 

op_line.append('{' + date_emp + ': ' + str(common_slot) + '}')
op_file = open('files/output_q3.txt', 'w') 
op_file.writelines(op_line) 
op_file.close() 