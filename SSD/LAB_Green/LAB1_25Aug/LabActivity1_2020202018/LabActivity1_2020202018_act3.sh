#!/bin/bash
date +%m | cal > out1					#1. new file containing current month
date +%D >> out1					#2. appending today's date
cat out1						#3. display contents
tail -n 3 out1						#4. display last 3 lines
head -n 7 out1 | tail -n 5				#5. displaying lines 3 to 7 including both 3 and 7
head -n 7 out1 | tail -n 5 | wc -l			#6. number of lines for task 5
echo "This day is awesome." > out2			#7. new file with echo
cat out2 | wc -w					#8. number of words
echo "I am looking forward to the day." >> out2		#9. Append
cat out2 | wc -l					#10. number of lines
cat out1 | cut -d " " -f5				#11. 5th column
cat out1 | cut -d " " -f4-9				#12. columns 4 to 9 including both.
cat out1 | cut -d " " -f3-				#13. column 3 till end 
cat out2 | cut -d " " -f2,4				#14. 2nd and 4th words from every row

