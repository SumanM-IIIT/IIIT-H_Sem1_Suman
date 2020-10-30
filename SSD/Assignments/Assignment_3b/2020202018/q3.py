import ast
import sys
from datetime import datetime, timedelta

def get_free_slots(emp_slot_list, start_time, end_time):
	free_slot = []
	for dur in emp_slot_list:
		times = dur.split('-')
		time_objs = [None] * len(times)
		for i in range(0, len(times)):
			tmp = times[i].strip()
			tmp = date_emp1 + tmp
			time_objs[i] = datetime.strptime(tmp, '%d/%m/%Y%I:%M%p')
		if(time_objs[0] > start_time):
			tmp2 = start_time.strftime('%I:%M%p') + ' - ' + time_objs[0].strftime('%I:%M%p')
			free_slot.append(tmp2)
		start_time = time_objs[1]	
		
	if start_time < end_time:
		free_slot.append(start_time.strftime('%I:%M%p') + ' - ' + end_time.strftime('%I:%M%p'))
	return free_slot

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
	while(tmp_start < end_time):
		#print(tmp_start)
		for slot1 in list1:
			if slot1[1] - slot1[0] < delta:
				continue
			for slot2 in list2:
				if slot2[1] - slot2[0] < delta:
					continue	
				tmp1 = slot1[0]
				tmp2 = slot2[0]
				while(tmp1 < tmp2):
					tmp1 += timedelta(minutes = 30)
				while(tmp1 > tmp2):
					tmp2 += timedelta(minutes = 30)
				
				if (tmp1 + delta) <= slot1[1] and (tmp2 + delta) <= slot2[1]:
					str_tmp = tmp1.strftime('%I:%M%p') + ' - ' + (tmp1 + delta).strftime('%I:%M%p')
					f_list.append(str_tmp)
					common_dict[curr_date] = f_list
					return str(common_dict)
		tmp_start += timedelta(minutes = 30)

	#f_list.append(str_tmp)
	f_list = ['N/A']
	common_dict[curr_date] = f_list
	return str(common_dict)


emp1_file = open(sys.argv[1], 'r') 
content = emp1_file.read()
emp1_dict = ast.literal_eval(content)

emp2_file = open(sys.argv[2], 'r') 
content = emp2_file.read()
emp2_dict = ast.literal_eval(content)

key1 = list(emp1_dict.keys())[0]
key2 = list(emp2_dict.keys())[0]
emp1_dict = emp1_dict[key1]
emp2_dict = emp2_dict[key2]

date_emp1 = list(emp1_dict.keys())[0]
date_emp2 = list(emp2_dict.keys())[0]
date1_list = date_emp1.split('/')
date2_list = date_emp2.split('/')
x = datetime(int(date1_list[2]), int(date1_list[1]), int(date1_list[0]))
y = datetime(int(date2_list[2]), int(date2_list[1]), int(date2_list[0]))

emp1_slot_list = emp1_dict[date_emp1]
emp2_slot_list = emp2_dict[date_emp2]

duration = input()
#dur_object = make_dur_object(date_emp1, duration)


if x == y:
	start_time = datetime.strptime(date1_list[0] + date1_list[1] + date1_list[2] + str(9), '%d%m%Y%H')
	end_time = datetime.strptime(date1_list[0] + date1_list[1] + date1_list[2] + str(17), '%d%m%Y%H')
	free_slot_emp1 = get_free_slots(emp1_slot_list, start_time, end_time)
	free_slot_emp2 = get_free_slots(emp2_slot_list, start_time, end_time)
	common_slot = get_common_slot(date_emp1, free_slot_emp1, free_slot_emp2, duration, start_time, end_time)
else:
	common_slot = str(['N/A (DIFFERENT DATES)'])
	free_slot_emp1 = ['N/A (DIFFERENT DATES)']
	free_slot_emp2 = ['N/A (DIFFERENT DATES)']

op_line = ['Available slots: \n']
op_line.append(key1 + ': ' + str(free_slot_emp1) + '\n')
op_line.append(key2 + ': ' + str(free_slot_emp2) + '\n\n')
op_line.append('Slot Duration: ' + duration + ' hour(s)\n')
op_line.append(common_slot)

op_file = open('files/output_q3.txt', 'w') 
op_file.writelines(op_line) 
op_file.close() 