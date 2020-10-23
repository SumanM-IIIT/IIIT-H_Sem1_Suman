from flask import Flask, jsonify

app = Flask(__name__)
@app.route('/')
def home():
	return "<h1>Hello World </h1>"

@app.route('/json')
def json():
	return jsonify({'fn': 'Suman', 'ln': 'Mitra'})

if __name__ == '__main__':
	app.run(port = 5000)