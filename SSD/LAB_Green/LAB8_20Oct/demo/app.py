from flask import Flask, request

app = Flask(__name__)

@app.route('/')
def hello_world():
	return 'hellooo'

#Query Parameters
#GET example: /calculate?n=3&n=2&op=mul
#POST {"n": [3,5,2], "op": "add"}
@app.route('/calculate', methods = ['GET', 'POST'])
def calculate():
	# GET
	if request.method == 'GET':
		numbers = request.args.getlist('n')	#['3', '2']
		operator = request.args.get('op')

	# POST
	if request.method == 'POST':
		content = request.get_json()
		numbers = content['n']
		operator = content['op']

	numbers = [float(i) for i in numbers]

	operators = {
		"add": AddNums(numbers),
		"mul": MulNums(numbers)
	}
	sol = operators.get(operator)

	return {"sol": sol}


# utility functions
def MulNums(numList):
	sol = 1
	for i in numList:
		sol *= i
	return sol

def AddNums(numList):
	sol = 0
	for i in numList:
		sol += i
	return sol


if __name__ == "__main__":
	app.run(debug = True);